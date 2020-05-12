/*
 * Unyaz0
 *
 * Decompress yaz0 files
 */

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>

static std::uint32_t swap(std::uint32_t v)
{
    return (v << 24) | ((v << 8) & 0x00ff0000) | ((v >> 8) & 0x0000ff00) | (v >> 24);
}

static void usage()
{
    std::puts("usage: unyaz0 <dest> <src>");
}

static void decompress(std::FILE *dst, std::FILE *src)
{
    std::uint8_t *data;
    std::uint32_t dataSize;

    std::uint8_t *out;
    std::uint32_t outSize;

    std::fseek(src, 0, SEEK_END);
    dataSize = (std::uint32_t)std::ftell(src) - 0x10;
    std::fseek(src, 4, SEEK_SET);
    std::fread(&outSize, 4, 1, src);
    outSize = swap(outSize);
    std::fseek(src, 0x10, SEEK_SET);
    data = new std::uint8_t[dataSize];
    std::fread(data, dataSize, 1, src);

    out = new std::uint8_t[outSize];
    unsigned inCursor{};
    unsigned outCursor{};
    std::uint8_t group;
    int len;
    int rrr;

    while (inCursor < dataSize)
    {
        group = data[inCursor++];
        for (int b = 0; b < 8; ++b)
        {
            if (inCursor >= dataSize)
                break;

            if (group & (1 << (7 - b)))
            {
                /* Direct data */
                out[outCursor++] = data[inCursor++];
            }
            else
            {
                if (data[inCursor] & 0xf0)
                {
                    len = (data[inCursor] >> 4) + 2;
                    rrr = (((data[inCursor] & 0xf) << 8) | (data[inCursor + 1])) + 1;
                    inCursor += 2;
                }
                else
                {
                    rrr = (((data[inCursor] & 0xf) << 8) | (data[inCursor + 1])) + 1;
                    len = (data[inCursor + 2]) + 0x12;
                    inCursor += 3;
                }
                for (int i = 0; i < len; ++i)
                {
                    out[outCursor] = out[outCursor - rrr];
                    outCursor++;
                }
            }
        }
    }

    std::fwrite(out, outSize, 1, dst);

    delete[] data;
    delete[] out;
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

    decompress(dst, src);

    std::fclose(src);
    std::fclose(dst);

    return 0;
}
