#ifndef INCLUDED_COMBO_OOT_SRAM_H
#define INCLUDED_COMBO_OOT_SRAM_H

#include <combo/util.h>

typedef struct SramContext
{
    u8* readBuff;
}
SramContext;

#define SRAM_SIZE           0x8000
#define SRAM_HEADER_SIZE    0x10

typedef enum SramHeaderField
{
    SRAM_HEADER_SOUND,
    SRAM_HEADER_ZTARGET,
    SRAM_HEADER_LANGUAGE,
    SRAM_HEADER_MAGIC,
}
SramHeaderField;

#endif
