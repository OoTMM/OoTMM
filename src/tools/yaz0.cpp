/*
 * Yaz0
 *
 * Compress yaz0 files
 */

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>

enum class RunType
{
    None = 0,
    Data = 1,
    Ref = 2
};

struct Run
{
    RunType type;
    union {
        std::uint8_t data;
        struct
        {
            std::uint16_t offset;
            std::uint16_t len;
        };
    };

    Run() : type(RunType::None) {}

    static Run makeData(std::uint8_t data)
    {
        Run r;

        r.type = RunType::Data;
        r.data = data;

        return r;
    }

    static Run makeRef(std::uint16_t offset, std::uint8_t len)
    {
        Run r;

        r.type = RunType::Ref;
        r.offset = offset;
        r.len = len;

        return r;
    }
};

struct Chunk
{
    Run r[8];
    std::uint32_t len;
    std::uint8_t group;

    Chunk() : len{}, group{} {}
};

struct Compressor
{
    std::FILE *src;
    std::FILE *dst;
    char *data;
    std::uint32_t dataSize;
    std::uint32_t inCursor;
};

static int cost(const Run &run)
{
    switch (run.type)
    {
    case RunType::None:
        return 1000;
    case RunType::Data:
        return 0;
    case RunType::Ref:
        return ((run.len >= 0x12) ? 3 : 2) - run.len;
    }
    return 0;
}

static int cost(const Chunk &ch)
{
    int acc{};

    for (int i = 0; i < 8; ++i)
    {
        acc += cost(ch.r[i]);
    }

    return acc;
}

static Chunk best(const Chunk &a, const Chunk &b)
{
    return cost(a) <= cost(b) ? a : b;
}

static Run
best(const Run &a, const Run &b)
{
    return (cost(a) <= cost(b)) ? a : b;
}

static std::uint32_t length(const Run &r)
{
    switch (r.type)
    {
    case RunType::None:
        return 0;
    case RunType::Data:
        return 1;
    case RunType::Ref:
        return r.len;
    }
    return 0;
}

static std::uint8_t swap(std::uint8_t v)
{
    return v;
}

static std::uint16_t swap(std::uint16_t v)
{
    return (v << 8) | (v >> 8);
}

static std::uint32_t swap(std::uint32_t v)
{
    return (v << 24) | ((v << 8) & 0x00ff0000) | ((v >> 8) & 0x0000ff00) | (v >> 24);
}

static void usage()
{
    std::puts("usage: yaz0 <dest> <src>");
}

static bool makeMatchPattern(Run &run, Compressor &compressor, int len)
{
    const char *pattern = compressor.data + compressor.inCursor;
    int cursorBase = (int)compressor.inCursor - 0x1000;
    int cursorMax = (int)compressor.inCursor - len;

    if (compressor.inCursor + len > compressor.dataSize)
        return false;

    if (cursorBase < 0)
        cursorBase = 0;

    if (cursorMax < 0)
        return false;

    for (int i = cursorMax; i >= cursorBase; --i)
    {
        if (std::memcmp(compressor.data + i, pattern, len) == 0)
        {
            run = Run::makeRef(compressor.inCursor - i, len);
            return true;
        }
    }
    return false;
}

static void refineMatch(Compressor &compressor, Run &run)
{
    if (run.type != RunType::Ref)
        return;

    if (run.offset != run.len)
        return;

    int initialLen = run.len;

    for (;;)
    {
        if (run.len == 0x111)
            return;
        if (compressor.inCursor + run.len == compressor.dataSize)
            return;
        if (compressor.data[compressor.inCursor + run.len] != compressor.data[compressor.inCursor + (run.len % initialLen)])
            return;
        run.len++;
    }
}

static Run makeMatch(Compressor &compressor)
{
    Run run;
    Run bestRun;

    for (int i = 1; i <= 0x111; ++i)
    {
        if (!makeMatchPattern(run, compressor, i))
            break;
        refineMatch(compressor, run);
        bestRun = best(bestRun, run);
    }

    return bestRun;
}

static Run makeRun(Compressor &compressor, bool dataOnly)
{
    Run r1;
    Run r2;

    if (compressor.inCursor == compressor.dataSize)
        return Run();
    /* Get the basic data run */
    r1 = Run::makeData(compressor.data[compressor.inCursor]);
    if (!dataOnly)
        r2 = makeMatch(compressor);

    return best(r1, r2);
}

static void writeRun(Compressor &c, const Run &r)
{
    std::uint8_t tmp8;
    std::uint16_t tmp16;

    switch (r.type)
    {
    case RunType::None:
        break;
    case RunType::Data:
        std::fwrite(&r.data, 1, 1, c.dst);
        break;
    case RunType::Ref:
        tmp16 = (r.offset - 1);
        if (r.len <= 0x11)
        {
            tmp16 |= (std::uint16_t(r.len - 2) << 12);
            tmp16 = swap(tmp16);
            std::fwrite(&tmp16, 2, 1, c.dst);
        }
        else
        {
            tmp16 = swap(tmp16);
            std::fwrite(&tmp16, 2, 1, c.dst);
            tmp8 = (r.len - 0x12);
            std::fwrite(&tmp8, 1, 1, c.dst);
        }
        break;
    }
}

static Chunk makeChunkCandidate(Compressor &c, std::uint8_t mask)
{
    Chunk ch;
    std::uint32_t cursor;
    Run run;

    cursor = c.inCursor;
    for (int i = 0; i < 8; ++i)
    {
        run = makeRun(c, mask & (1 << (7 - i)));
        if (run.type == RunType::Data)
        {
            ch.group |= (1 << (7 - i));
        }
        ch.r[i] = run;
        c.inCursor += length(run);
        ch.len += length(run);
    }

    c.inCursor = cursor;
    return ch;
}

static void makeChunk(Compressor &c)
{
    Chunk ch;

    ch = makeChunkCandidate(c, 0x00);
    ch = best(ch, makeChunkCandidate(c, 0x01));
    ch = best(ch, makeChunkCandidate(c, 0x02));
    ch = best(ch, makeChunkCandidate(c, 0x04));
    ch = best(ch, makeChunkCandidate(c, 0x08));
    ch = best(ch, makeChunkCandidate(c, 0x10));
    ch = best(ch, makeChunkCandidate(c, 0x20));
    ch = best(ch, makeChunkCandidate(c, 0x40));
    ch = best(ch, makeChunkCandidate(c, 0x80));

    std::fwrite(&ch.group, 1, 1, c.dst);
    for (int i = 0; i < 8; ++i)
    {
        writeRun(c, ch.r[i]);
    }
    c.inCursor += ch.len;
}

static void writeHeader(Compressor &c)
{
    std::uint32_t tmp;

    std::fwrite("Yaz0", 4, 1, c.dst);
    tmp = swap(c.dataSize);
    std::fwrite(&tmp, 4, 1, c.dst);
    tmp = 0;
    std::fwrite(&tmp, 4, 1, c.dst);
    std::fwrite(&tmp, 4, 1, c.dst);
}

static void compress(std::FILE *dst, std::FILE *src)
{
    Compressor compressor{};

    compressor.src = src;
    compressor.dst = dst;

    std::fseek(src, 0, SEEK_END);
    compressor.dataSize = (std::uint32_t)std::ftell(src);
    std::fseek(src, 0, SEEK_SET);
    compressor.data = new char[compressor.dataSize];
    std::fread(compressor.data, compressor.dataSize, 1, src);

    writeHeader(compressor);

    while (compressor.inCursor < compressor.dataSize)
        makeChunk(compressor);

    delete[] compressor.data;
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        usage();
        return 0;
    }

    else if (argc != 3)
    {
        usage();
        return 1;
    }

    std::FILE *dst;
    std::FILE *src;

    src = std::fopen(argv[2], "rb");
    if (!src)
    {
        std::printf("Could not read %s\n", argv[2]);
        return 1;
    }

    dst = std::fopen(argv[1], "wb");
    if (!dst)
    {
        std::printf("Could not write %s\n", argv[1]);
        std::fclose(src);
        return 1;
    }

    compress(dst, src);

    std::fclose(src);
    std::fclose(dst);

    return 0;
}
