#include <combo.h>
#include <combo/souls.h>

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
    case 0: *bm = gSharedCustomSave.soulsEnemyOot; break;
    case 1: *bm = gSharedCustomSave.soulsBossOot; break;
    case 2: *bm = gSharedCustomSave.soulsNpcOot; break;
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
    case 0: *bm = gSharedCustomSave.soulsEnemyMm; break;
    case 1: *bm = gSharedCustomSave.soulsBossMm; break;
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
    return !!BITMAP8_GET(bm, index);
}

int comboHasSoulMm(s16 gi)
{
    u8* bm;
    int index;

    soulInfosMm(&bm, &index, gi);
    return !!BITMAP8_GET(bm, index);
}
