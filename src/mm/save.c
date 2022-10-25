#include <combo.h>

#define ENTRANCE_CLOCKTOWN  0xd800
#define ENTRANCE_WOODS      0xc200
#define ENTRANCE_DKING      0x7610
#define ENTRANCE_TEMPLE1    0x3000
#define ENTRANCE_BOSS1      0x3800
#define ENTRANCE_SWAMP      0x8420
#define ENTRANCE_RANCH      0x6400

void Sram_AfterOpenSave(void)
{
    gSave.entranceIndex = ENTRANCE_CLOCKTOWN;

#if defined(DEBUG)
    gSave.entranceIndex = ENTRANCE_RANCH;
#endif
}
