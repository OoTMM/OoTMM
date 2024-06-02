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

static int loadElfFile(State* st, FILE* elf)
{
    Elf32_Ehdr ehdr;
    Elf32_Shdr shdr;
    char* shstrtab;

    /* Read ELF header */
    if (fread(&ehdr, sizeof(Elf32_Ehdr), 1, elf) != 1)
        return 1;

    if (ehdr.e_ident[EI_MAG0] != ELFMAG0 ||
        ehdr.e_ident[EI_MAG1] != ELFMAG1 ||
        ehdr.e_ident[EI_MAG2] != ELFMAG2 ||
        ehdr.e_ident[EI_MAG3] != ELFMAG3)
        return 1;

    /* Read the strtab shdr */
    if (fseek(elf, ehdr.e_shoff + ehdr.e_shstrndx * sizeof(Elf32_Shdr), SEEK_SET) != 0)
        return 1;

    if (fread(&shdr, sizeof(Elf32_Shdr), 1, elf) != 1)
        return 1;

    /* Read the shstrtab */
    shstrtab = malloc(shdr.sh_size + 1);
    if (shstrtab == NULL)
        return 1;
    shstrtab[shdr.sh_size] = 0;
    if (fseek(elf, shdr.sh_offset, SEEK_SET) != 0)
    {
        free(shstrtab);
        return 1;
    }
    if (fread(shstrtab, shdr.sh_size, 1, elf) != 1)
    {
        free(shstrtab);
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
        return 1;
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
    return 0;
}
