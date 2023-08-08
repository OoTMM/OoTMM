#include <combo.h>

static int hasSoul(s16 gi)
{
    int soulId;

    soulId = gi - GI_OOT_SOUL_STALFOS;
    if (soulId >= 32)
        return !!(gOotSouls2 & (1 << (soulId - 32)));
    else
        return !!(gOotSouls1 & (1 << soulId));
}

static int canSpawnSoul(GameState_Play* play, s16 actorId, u16 variable)
{
    switch (actorId)
    {
    case AC_EN_TEST:
        return hasSoul(GI_OOT_SOUL_STALFOS);
    case AC_BG_BDAN_OBJECTS:
        if (variable & 0xff)
            return 1;
        /* Fallthrough */
    case AC_EN_OKUTA:
    case AC_EN_BIGOKUTA:
        return hasSoul(GI_OOT_SOUL_OCTOROK);
    case AC_EN_WALLMAS:
        return hasSoul(GI_OOT_SOUL_WALLMASTER);
    case AC_EN_DODONGO:
        return hasSoul(GI_OOT_SOUL_DODONGO);
    case AC_EN_FIREFLY:
        return hasSoul(GI_OOT_SOUL_KEESE);
    case AC_EN_TITE:
        return hasSoul(GI_OOT_SOUL_TEKTITE);
    case AC_EN_PEEHAT:
        return hasSoul(GI_OOT_SOUL_PEAHAT);
    case AC_EN_ZF:
        return hasSoul(GI_OOT_SOUL_LIZALFOS_DINALFOS);
    case AC_EN_GOMA:
        if (play->sceneId == SCE_OOT_LAIR_GOHMA)
            return 1;
        return hasSoul(GI_OOT_SOUL_GOHMA_LARVA);
    case AC_EN_BUBBLE:
        return hasSoul(GI_OOT_SOUL_SHABOM);
    case AC_EN_DODOJR:
        return hasSoul(GI_OOT_SOUL_BABY_DODONGO);
    case AC_EN_BILI:
    case AC_EN_VALI:
        return hasSoul(GI_OOT_SOUL_BIRI_BARI);
    case AC_EN_TP:
        return hasSoul(GI_OOT_SOUL_TAILPASARN);
    case AC_EN_ST:
        return hasSoul(GI_OOT_SOUL_SKULLTULA);
    case AC_EN_BW:
        return hasSoul(GI_OOT_SOUL_TORCH_SLUG);
    case AC_EN_MB:
        return hasSoul(GI_OOT_SOUL_MOBLIN);
    case AC_EN_AM:
        if (variable == 0)
            return 1;
        return hasSoul(GI_OOT_SOUL_ARMOS);
    case AC_EN_DEKUBABA:
    case AC_EN_KAREBABA:
        return hasSoul(GI_OOT_SOUL_DEKU_BABA);
    case AC_EN_DEKUNUTS:
    case AC_EN_HINTNUTS:
        return hasSoul(GI_OOT_SOUL_DEKU_SCRUB);
    case AC_EN_BB:
        return hasSoul(GI_OOT_SOUL_BUBBLE);
    case AC_EN_VM:
        return hasSoul(GI_OOT_SOUL_BEAMOS);
    case AC_EN_FLOORMAS:
        return hasSoul(GI_OOT_SOUL_FLOORMASTER);
    case AC_EN_RD:
        return hasSoul(GI_OOT_SOUL_REDEAD_GIBDO);
    case AC_EN_SW:
        if (variable & 0xe000)
            return 1;
        return hasSoul(GI_OOT_SOUL_SKULLWALLTULA);
    case AC_EN_FD:
        return hasSoul(GI_OOT_SOUL_FLARE_DANCER);
    case AC_EN_DH:
    case AC_EN_DHA:
        return hasSoul(GI_OOT_SOUL_DEAD_HAND);
    case AC_EN_SB:
        return hasSoul(GI_OOT_SOUL_SHELL_BLADE);
    case AC_EN_RR:
        return hasSoul(GI_OOT_SOUL_LIKE_LIKE);
    case AC_EN_NY:
        return hasSoul(GI_OOT_SOUL_SPIKE);
    case AC_EN_ANUBICE_TAG:
    case AC_EN_ANUBICE:
        return hasSoul(GI_OOT_SOUL_ANUBIS);
    case AC_EN_IK:
        return hasSoul(GI_OOT_SOUL_IRON_KNUCKLE);
    case AC_EN_SKJ:
        if (gSave.age == AGE_CHILD)
            return 1;
        return hasSoul(GI_OOT_SOUL_SKULL_KID);
    case AC_EN_TUBO_TRAP:
        return hasSoul(GI_OOT_SOUL_FLYING_POT) ? 1 : -1;
    case AC_EN_FZ:
        return hasSoul(GI_OOT_SOUL_FREEZARD);
    case AC_EN_WEIYER:
    case AC_EN_EIYER:
        return hasSoul(GI_OOT_SOUL_STINGER);
    case AC_EN_WF:
        return hasSoul(GI_OOT_SOUL_WOLFOS);
    case AC_EN_CROW:
        return hasSoul(GI_OOT_SOUL_GUAY);
    case AC_BOSS_GOMA:
        return hasSoul(GI_OOT_SOUL_QUEEN_GOHMA);
    case AC_BOSS_DODONGO:
        return hasSoul(GI_OOT_SOUL_KING_DODONGO);
    case AC_BOSS_VA:
        return hasSoul(GI_OOT_SOUL_BARINADE);
    case AC_BOSS_GANONDROF:
        return hasSoul(GI_OOT_SOUL_PHANTOM_GANON);
    case AC_BOSS_FD:
        return hasSoul(GI_OOT_SOUL_VOLVAGIA);
    case AC_BOSS_MO:
        return hasSoul(GI_OOT_SOUL_MORPHA);
    case AC_BOSS_SST:
        return hasSoul(GI_OOT_SOUL_BONGO_BONGO);
    case AC_BOSS_TW:
        return hasSoul(GI_OOT_SOUL_TWINROVA);
    case AC_EN_BA:
        return hasSoul(GI_OOT_SOUL_PARASITE);
    default:
        return 1;
    }
}

static int canSpawnActor(GameState_Play* play, s16 actorId, u16 valid)
{
    switch (actorId)
    {
    case AC_EN_SYATEKI_MAN:
        if(gSave.entrance == ENTR_OOT_CHILD_ARCHERY && gSave.age == AGE_CHILD)
            return 1;
        if(gSave.entrance == ENTR_OOT_ADULT_ARCHERY && gSave.age == AGE_ADULT)
            return 1;
        else
            return 0;
    default:
        return 1;
    }
}

Actor* comboSpawnActor(void* unk, GameState_Play *play, short actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable)
{
    int ret;
    Actor* actor;

    if (!canSpawnActor(play, actorId, variable))
        return NULL;

    ret = canSpawnSoul(play, actorId, variable);
    if (ret <= 0)
    {
        if (ret == 0)
            g.roomEnemyLackSoul = 1;
        return NULL;
    }

    /* Inert grass */
    if (actorId == AC_OBJ_HANA && ((variable & 3) == 2))
    {
        actorId = AC_EN_KUSA;
        variable = 0xff00;
    }

    actor = SpawnActor(unk, play, actorId, x, y, z, rx, ry, rz, variable);
    if (actorId == AC_ARMS_HOOK && gSave.age == AGE_ADULT)
        actor->objTableIndex = GetObject(&play->objectCtx, 0x14);
    return actor;
}
