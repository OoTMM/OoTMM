#include <combo.h>

#if !defined(GAME_OOT)
ALIGNED(16) OotSave gOotSave;
#endif

#if !defined(GAME_MM)
ALIGNED(16) MmSave gMmSave;
#endif

void comboLoadForeignSave(int fileIndex)
{
#if !defined(GAME_OOT)
    comboReadWriteFlash(0x20 + 0x1450 * fileIndex, &gOotSave, sizeof(OotSave), OS_READ);
#endif

#if !defined(GAME_MM)
    comboReadWriteFlash(0x8000 + 0x4000 * fileIndex, &gMmSave, sizeof(MmSave), OS_READ);
#endif
}

void comboSaveWrite(void)
{

}
