#include <combo.h>
#include <combo/dungeon.h>
#include <combo/souls.h>

static int canSpawnSoul(GameState_Play* play, s16 actorId, u16 variable)
{
    switch (actorId)
    {
    case AC_EN_TEST:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_STALFOS);
    case AC_BG_BDAN_OBJECTS:
        if (variable & 0xff)
            return 1;
        /* Fallthrough */
    case AC_EN_OKUTA:
    case AC_EN_BIGOKUTA:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_OCTOROK);
    case AC_EN_WALLMAS:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_WALLMASTER);
    case AC_EN_DODONGO:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_DODONGO);
    case AC_EN_FIREFLY:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_KEESE);
    case AC_EN_TITE:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_TEKTITE);
    case AC_EN_PEEHAT:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_PEAHAT);
    case AC_EN_ZF:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_LIZALFOS_DINALFOS);
    case AC_EN_GOMA:
        if (play->sceneId == SCE_OOT_LAIR_GOHMA)
            return 1;
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_GOHMA_LARVA);
    case AC_EN_BUBBLE:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_SHABOM);
    case AC_EN_DODOJR:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_BABY_DODONGO);
    case AC_EN_TORCH2:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_DARK_LINK);
    case AC_EN_BILI:
    case AC_EN_VALI:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_BIRI_BARI);
    case AC_EN_TP:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_TAILPASARN);
    case AC_EN_ST:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_SKULLTULA);
    case AC_EN_BW:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_TORCH_SLUG);
    case AC_EN_MB:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_MOBLIN);
    case AC_EN_AM:
        if (variable == 0)
            return 1;
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_ARMOS);
    case AC_EN_DEKUBABA:
    case AC_EN_KAREBABA:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_DEKU_BABA);
    case AC_EN_DEKUNUTS:
    case AC_EN_HINTNUTS:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_DEKU_SCRUB);
    case AC_EN_BB:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_BUBBLE);
    case AC_EN_VM:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_BEAMOS);
    case AC_EN_FLOORMAS:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_FLOORMASTER);
    case AC_EN_RD:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_REDEAD_GIBDO);
    case AC_EN_SW:
        if (variable & 0xe000)
            return 1;
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_SKULLWALLTULA);
    case AC_EN_FD:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_FLARE_DANCER);
    case AC_EN_DH:
    case AC_EN_DHA:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_DEAD_HAND);
    case AC_EN_SB:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_SHELL_BLADE);
    case AC_EN_RR:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_LIKE_LIKE);
    case AC_EN_NY:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_SPIKE);
    case AC_EN_ANUBICE_TAG:
    case AC_EN_ANUBICE:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_ANUBIS);
    case AC_EN_IK:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_IRON_KNUCKLE);
    case AC_EN_SKJ:
        if (gSave.age == AGE_CHILD)
            return 1;
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_SKULL_KID);
    case AC_EN_TUBO_TRAP:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_FLYING_POT) ? 1 : -1;
    case AC_EN_FZ:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_FREEZARD);
    case AC_EN_WEIYER:
    case AC_EN_EIYER:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_STINGER);
    case AC_EN_WF:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_WOLFOS);
    case AC_EN_CROW:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_GUAY);
    case AC_BOSS_GOMA:
        return comboHasSoulOot(GI_OOT_SOUL_BOSS_QUEEN_GOHMA);
    case AC_BOSS_DODONGO:
        return comboHasSoulOot(GI_OOT_SOUL_BOSS_KING_DODONGO);
    case AC_BOSS_VA:
        return comboHasSoulOot(GI_OOT_SOUL_BOSS_BARINADE);
    case AC_BOSS_GANONDROF:
        return comboHasSoulOot(GI_OOT_SOUL_BOSS_PHANTOM_GANON);
    case AC_BOSS_FD:
        return comboHasSoulOot(GI_OOT_SOUL_BOSS_VOLVAGIA);
    case AC_BOSS_MO:
        return comboHasSoulOot(GI_OOT_SOUL_BOSS_MORPHA);
    case AC_BOSS_SST:
        return comboHasSoulOot(GI_OOT_SOUL_BOSS_BONGO_BONGO);
    case AC_BOSS_TW:
        return comboHasSoulOot(GI_OOT_SOUL_BOSS_TWINROVA);
    case AC_EN_BA:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_PARASITE);
    case AC_EN_REEBA:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_LEEVER);
    case AC_EN_SKB:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_STALCHILD);
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

    /* MQ Spirit has some fire keese that fails to load */
    if (play->sceneId == SCE_OOT_TEMPLE_SPIRIT && (gComboData.mq & (1 << MQ_TEMPLE_SPIRIT)) && actorId == AC_EN_FIREFLY && play->roomCtx.curRoom.num == 0x01)
        ret = -1;

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
