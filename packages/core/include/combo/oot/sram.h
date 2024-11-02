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

typedef struct FileSelectState FileSelectState;

void Sram_InitNewSave(void);
void Sram_InitDebugSave(void);
void Sram_OpenSave(SramContext* sramCtx);
void Sram_WriteSave(SramContext* sramCtx);
void Sram_VerifyAndLoadAllSaves(FileSelectState* fileSelect, SramContext* sramCtx);
void Sram_InitSave(FileSelectState* fileSelect, SramContext* sramCtx);
void Sram_EraseSave(FileSelectState* fileSelect, SramContext* sramCtx);
void Sram_CopySave(FileSelectState* fileSelect, SramContext* sramCtx);
void Sram_WriteSramHeader(SramContext* sramCtx);
void Sram_InitSram(GameState* gameState, SramContext* sramCtx);
void Sram_Alloc(GameState* gameState, SramContext* sramCtx);
void Sram_Init(PlayState* play, SramContext* sramCtx);
void SsSram_Init(s32 addr, u8 handleType, u8 handleDomain, u8 handleLatency, u8 handlePageSize, u8 handleRelDuration, u8 handlePulse, u32 handleSpeed);
void SsSram_Dma(void* dramAddr, size_t size, s32 direction);
void SsSram_ReadWrite(s32 addr, void* dramAddr, size_t size, s32 direction);

extern u16 gSramSlotOffsets[];

#endif
