#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <elf.h>

#define SE_TEXT     0
#define SE_DATA     1
#define SE_RODATA   2
#define SE_BSS      3

static const char* sSectionNames[] = {
    ".text",
    ".data",
    ".rodata",
    ".bss",
};

static const char* sSectionRelNames[] = {
    ".rel.text",
    ".rel.data",
    ".rel.rodata",
};

typedef struct
{
    char*       data;
    uint32_t    size;
}
StateSection;

typedef struct
{
    FILE*           in;
    FILE*           out;
    Elf32_Ehdr      ehdr;
    char*           shstrtab;
    uint32_t        shstrtabSize;
    uint32_t        meta[4];
    char*           sectionData[4];
    uint32_t        sectionStart[4];
    uint32_t        sectionSize[4];
    uint32_t*       relocs;
    uint32_t        relocsCount;
    uint32_t        relocsCapacity;
}
State;

static uint32_t eswap32(uint32_t val)
{
    return ((val & 0x000000FF) << 24) |
           ((val & 0x0000FF00) << 8) |
           ((val & 0x00FF0000) >> 8) |
           ((val & 0xFF000000) >> 24);
}

static uint16_t eswap16(uint16_t val)
{
    return ((val & 0x00FF) << 8) |
           ((val & 0xFF00) >> 8);
}

static uint32_t round16(uint32_t val)
{
    return (val + 15) & ~15;
}

static int locateSection(State* state, Elf32_Shdr* shdr, const char* name)
{
    Elf32_Shdr tshdr;
    int ret;

    for (int i = 0; i < state->ehdr.e_shnum; ++i)
    {
        /* Read */
        fseek(state->in, state->ehdr.e_shoff + i * sizeof(Elf32_Shdr), SEEK_SET);
        fread(&tshdr, sizeof(Elf32_Shdr), 1, state->in);

        /* Convert endian */
        tshdr.sh_name = eswap32(tshdr.sh_name);
        tshdr.sh_type = eswap32(tshdr.sh_type);
        tshdr.sh_flags = eswap32(tshdr.sh_flags);
        tshdr.sh_addr = eswap32(tshdr.sh_addr);
        tshdr.sh_offset = eswap32(tshdr.sh_offset);
        tshdr.sh_size = eswap32(tshdr.sh_size);
        tshdr.sh_link = eswap32(tshdr.sh_link);
        tshdr.sh_info = eswap32(tshdr.sh_info);
        tshdr.sh_addralign = eswap32(tshdr.sh_addralign);
        tshdr.sh_entsize = eswap32(tshdr.sh_entsize);

        if (strcmp(state->shstrtab + tshdr.sh_name, name) == 0)
        {
            memcpy(shdr, &tshdr, sizeof(Elf32_Shdr));
            return 0;
        }
    }

    return 1;
}

static int loadSectionStringTable(State* state)
{
    Elf32_Shdr shdr;
    int ret;
    char* shstrtab;

    /* Read the strtab shdr */
    if (fseek(state->in, state->ehdr.e_shoff + state->ehdr.e_shstrndx * sizeof(Elf32_Shdr), SEEK_SET) != 0)
    {
        fprintf(stderr, "Error: Failed to seek to shstrtab shdr\n");
        return 1;
    }

    ret = fread(&shdr, sizeof(Elf32_Shdr), 1, state->in);
    if (ret != 1)
    {
        fprintf(stderr, "Error: Failed to read shstrtab shdr: %d\n", ret);
        return 1;
    }
    shdr.sh_size = eswap32(shdr.sh_size);
    shdr.sh_offset = eswap32(shdr.sh_offset);

    /* Read the shstrtab */
    shstrtab = malloc(shdr.sh_size + 1);
    if (shstrtab == NULL)
    {
        fprintf(stderr, "Error: Failed to allocate memory for shstrtab\n");
        return 1;
    }
    shstrtab[shdr.sh_size] = 0;
    if (fseek(state->in, shdr.sh_offset, SEEK_SET) != 0)
    {
        free(shstrtab);
        fprintf(stderr, "Error: Failed to seek to shstrtab\n");
        return 1;
    }
    if (fread(shstrtab, shdr.sh_size, 1, state->in) != 1)
    {
        free(shstrtab);
        fprintf(stderr, "Error: Failed to read shstrtab\n");
        return 1;
    }

    state->shstrtab = shstrtab;
    state->shstrtabSize = shdr.sh_size;

    return 0;
}

static int loadElfHeader(State* state)
{
    Elf32_Ehdr ehdr;

    /* Read ELF header */
    if (fread(&ehdr, sizeof(Elf32_Ehdr), 1, state->in) != 1)
    {
        fprintf(stderr, "Error: Failed to read ELF header\n");
        return 1;
    }

    if (ehdr.e_ident[EI_MAG0] != ELFMAG0 ||
        ehdr.e_ident[EI_MAG1] != ELFMAG1 ||
        ehdr.e_ident[EI_MAG2] != ELFMAG2 ||
        ehdr.e_ident[EI_MAG3] != ELFMAG3)
    {
        fprintf(stderr, "Error: Not an ELF file\n");
        return 1;
    }

    /* Swap */
    ehdr.e_shoff = eswap32(ehdr.e_shoff);
    ehdr.e_shentsize = eswap16(ehdr.e_shentsize);
    ehdr.e_shnum = eswap16(ehdr.e_shnum);
    ehdr.e_shstrndx = eswap16(ehdr.e_shstrndx);

    memcpy(&state->ehdr, &ehdr, sizeof(Elf32_Ehdr));

    return 0;
}

static int setupFiles(State* st, const char* pathIn, const char* pathOut)
{
    FILE* in;
    FILE* out;

    in = fopen(pathIn, "rb");
    if (in == NULL)
    {
        fprintf(stderr, "Error: Failed to open %s\n", pathIn);
        return 1;
    }

    out = fopen(pathOut, "wb");
    if (out == NULL)
    {
        fprintf(stderr, "Error: Failed to open %s\n", pathOut);
        fclose(in);
        return 1;
    }

    st->in = in;
    st->out = out;

    return 0;
}

static int loadMeta(State* state)
{
    Elf32_Shdr shdr;
    uint32_t meta[2];

    if (locateSection(state, &shdr, ".meta"))
    {
        fprintf(stderr, "Error: Failed to locate .meta section\n");
        return 1;
    }

    /* Read */
    fseek(state->in, shdr.sh_offset, SEEK_SET);
    fread(state->meta, sizeof(uint32_t) * 2, 1, state->in);

    return 0;
}

static int loadSection(State* state, int secId)
{
    Elf32_Shdr shdr;
    const char* secName;
    char* data;
    uint32_t size;

    /* Find the section */
    secName = sSectionNames[secId];
    if (locateSection(state, &shdr, secName))
        return 0;
    size = round16(shdr.sh_size);

    if (secId != SE_BSS)
    {
        /* Alloc */
        data = malloc(size);
        if (data == NULL)
        {
            fprintf(stderr, "Error: Failed to allocate memory for section %s\n", secName);
            return 1;
        }
        memset(data, 0, size);

        /* Read */
        fseek(state->in, shdr.sh_offset, SEEK_SET);
        fread(data, shdr.sh_size, 1, state->in);
    }
    else
    {
        data = NULL;
    }

    /* Store */
    state->sectionData[secId] = data;
    state->sectionStart[secId] = shdr.sh_addr;
    state->sectionSize[secId] = size;

    return 0;
}

static int addReloc(State* state, uint32_t reloc)
{
    uint32_t newCapacity;
    uint32_t* newData;

    if (state->relocsCapacity == state->relocsCount)
    {
        newCapacity = state->relocsCapacity * 2;
        if (newCapacity == 0)
            newCapacity = 16;
        newData = realloc(state->relocs, newCapacity * sizeof(uint32_t));
        if (newData == NULL)
        {
            fprintf(stderr, "Error: Failed to reallocate memory for relocs\n");
            return 1;
        }
        state->relocs = newData;
        state->relocsCapacity = newCapacity;
    }

    state->relocs[state->relocsCount++] = reloc;
    return 0;
}

static int emitReloc(State* state, int secId, int type, uint32_t addr, uint32_t off)
{
    uint32_t reloc;

    if (addr < 0x80d00000 || addr >= 0x81000000)
    {
        //fprintf(stderr, "Warn: Address %08X is out of range\n", addr);
        return 0;
    }

    reloc = (((secId + 1) << 30) | (type << 24) | (off & 0x00ffffff));
    //printf("N64 Reloc: 0x%08x\n", reloc);
    return addReloc(state, eswap32(reloc));
}

static int loadRelocs(State* state, int secId)
{
    Elf32_Shdr shdr;
    Elf32_Rel rel;
    uint16_t hi16;
    uint32_t hi16off;
    int hi16pending;
    uint32_t off;
    uint32_t data;
    uint32_t addr;
    int count;
    int type;
    int reg;

    /* Find the section */
    if (locateSection(state, &shdr, sSectionRelNames[secId]))
        return 0;

    fseek(state->in, shdr.sh_offset, SEEK_SET);
    count = shdr.sh_size / sizeof(Elf32_Rel);
    for (int i = 0; i < count; ++i)
    {
        /* Load the reloc */
        fread(&rel, sizeof(Elf32_Rel), 1, state->in);
        rel.r_info = eswap32(rel.r_info);
        rel.r_offset = eswap32(rel.r_offset);

        /* Fetch the data */
        off = rel.r_offset - state->sectionStart[secId];
        data = eswap32(*(uint32_t*)(state->sectionData[secId] + off));

        type = ELF32_R_TYPE(rel.r_info);
        switch (type)
        {
        case R_MIPS_HI16:
            hi16 = data & 0xffff;
            hi16pending = 1;
            hi16off = off;
            addr = 0;
            break;
        case R_MIPS_LO16:
            reg = (data >> 0x15) & 0x1f;
            addr = ((uint32_t)hi16 << 16) + ((int16_t)(data & 0xffff));
            if (hi16pending)
            {
                hi16pending = 0;
                emitReloc(state, secId, R_MIPS_HI16, addr, hi16off);
            }
            emitReloc(state, secId, R_MIPS_LO16, addr, off);
            break;
        case R_MIPS_26:
            addr = ((data & 0x03ffffff) << 2 | (state->sectionStart[secId] & 0xf0000000));
            emitReloc(state, secId, R_MIPS_26, addr, off);
            break;
        case R_MIPS_32:
            addr = data;
            emitReloc(state, secId, R_MIPS_32, addr, off);
            break;
        default:
            fprintf(stderr, "Error: Unknown reloc type %d\n", type);
            return 1;
        }
    }

    return 0;
}

static int emit(State* state)
{
    uint32_t vstart;
    uint32_t vend;
    uint32_t relocHeader[5];
    uint32_t relocHeaderSize;
    uint32_t z;

    /* Compute vsize */
    vstart = state->sectionStart[0];
    vend = vstart;
    for (int i = 0; i < 4; ++i)
        vend += state->sectionSize[i];
    vend += round16((state->relocsCount + 6) * 4);
    state->meta[2] = eswap32(vstart);
    state->meta[3] = eswap32(vend);

    /* Output meta */
    fwrite(state->meta, sizeof(state->meta), 1, state->out);

    /* Output sections */
    for (int i = 0; i < 4; ++i)
    {
        relocHeader[i] = eswap32(state->sectionSize[i]);
        if (state->sectionData[i])
            fwrite(state->sectionData[i], state->sectionSize[i], 1, state->out);
    }
    relocHeader[4] = eswap32(state->relocsCount);

    /* Output reloc header */
    fwrite(relocHeader, sizeof(relocHeader), 1, state->out);

    /* Output relocs */
    fwrite(state->relocs, sizeof(uint32_t) * state->relocsCount, 1, state->out);

    /* Compute offset and pad */
    z = 0;
    relocHeaderSize = sizeof(relocHeader) + sizeof(uint32_t) * state->relocsCount;
    while (relocHeaderSize % 16 != 12)
    {
        fwrite(&z, 4, 1, state->out);
        relocHeaderSize += 4;
    }
    relocHeaderSize += 4;
    relocHeaderSize = eswap32(relocHeaderSize);
    fwrite(&relocHeaderSize, 4, 1, state->out);

    return 0;
}

static int run(const char* pathIn, const char* pathOut)
{
    State state;
    int ret;

    memset(&state, 0, sizeof(State));
    if (setupFiles(&state, pathIn, pathOut)) return 1;
    if (loadElfHeader(&state)) return 1;
    if (loadSectionStringTable(&state)) return 1;
    if (loadMeta(&state)) return 1;

    for (int i = 0; i < 4; ++i)
        if (loadSection(&state, i)) return 1;

    for (int i = 0; i < 3; ++i)
        if (loadRelocs(&state, i)) return 1;

    if (emit(&state)) return 1;

    fclose(state.in);
    fclose(state.out);

    return 0;
}

int main(int argc, char** argv)
{
    State state;

    if (argc != 3)
        return 1;

    return run(argv[1], argv[2]);
}
