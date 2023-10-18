#include <combo.h>
#include <combo/souls.h>

static void soulInfosOot(u8** bm, int* index, s16 gi)
{
    if (gi >= GI_OOT_SOUL_NPC_SARIA)
    {
        *index = gi - GI_OOT_SOUL_NPC_SARIA;
        *bm = gSharedCustomSave.soulsNpcOot;
    }
    else if (gi >= GI_OOT_SOUL_BOSS_QUEEN_GOHMA)
    {
        *index = gi - GI_OOT_SOUL_BOSS_QUEEN_GOHMA;
        *bm = gSharedCustomSave.soulsBossOot;
    }
    else
    {
        *index = gi - GI_OOT_SOUL_ENEMY_STALFOS;
        *bm = gSharedCustomSave.soulsEnemyOot;
    }
}

static void soulInfosMm(u8** bm, int* index, s16 gi)
{
    if (gi >= GI_MM_SOUL_BOSS_ODOLWA)
    {
        *index = gi - GI_MM_SOUL_BOSS_ODOLWA;
        *bm = gSharedCustomSave.soulsBossMm;
    }
    else
    {
        *index = gi - GI_MM_SOUL_ENEMY_OCTOROK;
        *bm = gSharedCustomSave.soulsEnemyMm;
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
