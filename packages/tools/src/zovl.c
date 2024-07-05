#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <elf.h>

#define SE_TEXT     1
#define SE_DATA     2
#define SE_RODATA   3
#define SE_BSS      4

typedef struct
{
    char*       data;
    uint32_t    size;
}
StateSection;

typedef struct
{
    StateSection text;
    StateSection data;
    StateSection rodata;
    uint32_t     bssSize;
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

static int loadElfFile(State* st, FILE* elf)
{
    Elf32_Ehdr ehdr;
    Elf32_Shdr shdr;
    char* shstrtab;
    int ret;

    /* Read ELF header */
    if (fread(&ehdr, sizeof(Elf32_Ehdr), 1, elf) != 1)
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

    /* Read the strtab shdr */
    if (fseek(elf, ehdr.e_shoff + ehdr.e_shstrndx * sizeof(Elf32_Shdr), SEEK_SET) != 0)
    {
        fprintf(stderr, "Error: Failed to seek to shstrtab shdr\n");
        return 1;
    }

    ret = fread(&shdr, sizeof(Elf32_Shdr), 1, elf);
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
    if (fseek(elf, shdr.sh_offset, SEEK_SET) != 0)
    {
        free(shstrtab);
        fprintf(stderr, "Error: Failed to seek to shstrtab\n");
        return 1;
    }
    if (fread(shstrtab, shdr.sh_size, 1, elf) != 1)
    {
        free(shstrtab);
        fprintf(stderr, "Error: Failed to read shstrtab\n");
        return 1;
    }

    free(shstrtab);
    return 0;
}

static int loadElf(State* st, const char* elfPath)
{
    FILE* elf;
    int ret;

    elf = fopen(elfPath, "rb");
    if (elf == NULL)
    {
        fprintf(stderr, "Error: Failed to open %s\n", elfPath);
        return 1;
    }
    ret = loadElfFile(st, elf);
    fclose(elf);
    return ret;
}

int main(int argc, char** argv)
{
    State state;

    if (argc != 3)
        return 1;

    memset(&state, 0, sizeof(State));
    if (loadElf(&state, argv[1]) != 0)
        return 1;
    return 0;
}
