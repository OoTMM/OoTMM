#include <combo.h>
#include <combo/dungeon.h>

int comboBossDungeon(int dungeonId)
{
    for (int i = 0; i <= BOSSID_TWINMOLD; ++i)
    {
        if (gComboData.boss[i] == dungeonId)
            return i;
    }
    return -1;
}
