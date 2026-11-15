#include <combo.h>
#include <combo/souls.h>
#include <combo/enemy_soul_preview.h>

extern const u16 kAddItemParams[];

static void soulInfosOot(u8** bm, int* index, s16 gi)
{
    u16 param;
    u8 type;

    param = kAddItemParams[gi - 1];
    *index = param & 0xfff;
    type = param >> 12;

    switch (type)
    {
    case 0:
        *bm = gSharedCustomSave.soulsEnemyOot;
        if (*index >= 64)
        {
            *bm = gSharedCustomSave.soulsEnemyOotOverflow;
            *index -= 64;
        }
        break;
    case 1: *bm = gSharedCustomSave.soulsBossOot; break;
    case 2: *bm = gSharedCustomSave.soulsNpcOot; break;
    case 3: *bm = gSharedCustomSave.soulsAnimalsOot; break;
    case 4: *bm = gSharedCustomSave.soulsMiscOot; break;
    default: UNREACHABLE(); break;
    }
}

static void soulInfosMm(u8** bm, int* index, s16 gi)
{
    u16 param;
    u8 type;

    param = kAddItemParams[gi - 1];
    *index = param & 0xfff;
    type = param >> 12;

    switch (type)
    {
    case 0:
        *bm = gSharedCustomSave.soulsEnemyMm;
        if (*index >= 64)
        {
            *bm = gSharedCustomSave.soulsEnemyMmOverflow;
            *index -= 64;
        }
        break;
    case 1: *bm = gSharedCustomSave.soulsBossMm; break;
    case 2: *bm = gSharedCustomSave.soulsNpcMm; break;
    case 3: *bm = gSharedCustomSave.soulsAnimalsMm; break;
    case 4: *bm = gSharedCustomSave.soulsMiscMm; break;
    default: UNREACHABLE(); break;
    }
}

void comboAddSoulOot(s16 gi)
{
    u8* bm;
    int index;

    soulInfosOot(&bm, &index, gi);
    BITMAP8_SET(bm, index);
}

void comboAddSoulMm(s16 gi)
{
    u8* bm;
    int index;

    soulInfosMm(&bm, &index, gi);
    BITMAP8_SET(bm, index);
}

int comboHasSoulOot(s16 gi)
{
    u8* bm;
    int index;

    soulInfosOot(&bm, &index, gi);
    int present = !!BITMAP8_GET(bm, index);
    comboEnemySoulPreviewObserve(0, gi, present, (kAddItemParams[gi - 1] >> 12) == 0);
    return present;
}

int comboHasSoulMm(s16 gi)
{
    u8* bm;
    int index;

    soulInfosMm(&bm, &index, gi);
    int present = !!BITMAP8_GET(bm, index);
    comboEnemySoulPreviewObserve(1, gi, present, (kAddItemParams[gi - 1] >> 12) == 0);
    return present;
}
