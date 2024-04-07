#include <combo.h>
#include <combo/dungeon.h>
#include <combo/souls.h>

static s16 sActorIdToSpawn;

static int opt(int x)
{
    return x > 0 ? 1 : -1;
}

static int canSpawnSoul(GameState_Play* play, s16 actorId, u16 variable)
{
    switch (actorId)
    {
    case AC_EN_DNS:
    case AC_EN_SHOPNUTS:
        return opt(comboHasSoulOot(GI_OOT_SOUL_MISC_BUSINESS_SCRUB));
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
            return opt(comboHasSoulOot(GI_OOT_SOUL_MISC_GS));
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
        return opt(comboHasSoulOot(GI_OOT_SOUL_ENEMY_FLYING_POT));
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
    case AC_EN_SA:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_SARIA));
    case AC_EN_DU:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_DARUNIA));
    case AC_EN_RU1:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_RUTO));
    case AC_EN_KZ:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_KING_ZORA));
    case AC_EN_NIW_LADY:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_CUCCO_LADY));
    case AC_EN_TORYO:
    case AC_EN_DAIKU:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_CARPENTERS));
    case AC_EN_FU:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_GURU_GURU));
    case AC_EN_MD:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_MIDO));
    case AC_EN_KO:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_KOKIRI));
    case AC_EN_OSSAN:
        switch (variable)
        {
        case 0: return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_KOKIRI_SHOPKEEPER));
        case 1: return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_POTION_SHOPKEEPER));
        case 2: return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_BOMBCHU_SHOPKEEPER));
        case 3: return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_POTION_SHOPKEEPER));
        case 4: return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_BAZAAR_SHOPKEEPER));
        case 7: return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_ZORA_SHOPKEEPER));
        case 8: return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_GORON_SHOPKEEPER));
        default: return 1;
        }
        break;
    case AC_EN_HEISHI2:
    case AC_EN_HEISHI3:
    case AC_EN_HEISHI4:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_HYLIAN_GUARD));
    case AC_EN_HY:
    case AC_EN_ANI:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_HYLIAN_CITIZEN));
    case AC_EN_MA1:
    case AC_EN_MA2:
    case AC_EN_MA3:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_MALON));
    case AC_BG_SPOT15_RRBOX:
        if(play->sceneId != SCE_OOT_HYRULE_CASTLE)
            return 1;
        /* Fallthrough */
    case AC_EN_TA:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_TALON));
    case AC_EN_FISHING:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_FISHING_POND_OWNER));
    case AC_EN_GO:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_GORON));
    case AC_EN_GO2:
        switch (variable & 0x1f)
        {
        case 1: return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_GORON_CHILD));
        case 2: return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_BIGGORON));
        default: return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_GORON));
        }
        break;
    case AC_EN_GM:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_MEDIGORON));
    case AC_EN_ZO:
    case AC_EN_DIVING_GAME:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_ZORA));
    case AC_EN_BOM_BOWL_MAN:
    case AC_BG_BOWL_WALL:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_BOMBCHU_BOWLING_LADY));
    case AC_EN_TAKARA_MAN:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_TRASURE_CHEST_GAME_OWNER));
    case AC_EN_SYATEKI_MAN:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_SHOOTING_GALLERY_OWNER));
    case AC_EN_TK:
    case AC_EN_PO_RELAY:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_DAMPE));
    case AC_EN_CS:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_GRAVEYARD_KID));
    case AC_EN_GB:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_POE_COLLECTOR));
    case AC_EN_XC:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_SHEIK));
    case AC_EN_ZL1:
    case AC_EN_ZL3:
    case AC_EN_ZL4:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_ZELDA));
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

Actor* comboSpawnActor(ActorContext* actorCtx, GameState_Play *play, short actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable)
{
    int ret;
    Actor* actor;

    if (actorId < 0 || !canSpawnActor(play, actorId, variable))
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

    if (comboConfig(CFG_OOT_OPEN_MASK_SHOP) && play->sceneId == SCE_OOT_MARKET_CHILD_NIGHT && actorId == AC_EN_DOOR)
        if (((variable >> 7 & 7) == 0x5) && ((variable & 0x3f) == 0x10))
            variable = 0x1bf;

    if (comboConfig(CFG_OOT_OPEN_ZD_SHORTCUT) && actorId == AC_BG_SPOT06_OBJECTS && play->sceneId == SCE_OOT_LAKE_HYLIA)
        if (((variable >> 8) & 0xff) == 3)
            return NULL;

    sActorIdToSpawn = actorId;
    actor = SpawnActor(actorCtx, play, actorId, x, y, z, rx, ry, rz, variable);
    if (actorId == AC_ARMS_HOOK && gSave.age == AGE_ADULT)
        actor->objTableIndex = GetObjectSlot(&play->objectCtx, 0x14);
    return actor;
}

static int GetRoomClearFlagForActor(GameState_Play* play, int flag)
{
    int res;

    res = GetRoomClearFlag(play, flag);
    if (comboConfig(CFG_ER_WALLMASTERS) && sActorIdToSpawn == AC_EN_WALLMAS)
        res = 0;
    return res;
}

PATCH_CALL(0x80025284, GetRoomClearFlagForActor);

static int shouldActorIgnorePlayer(Actor* this, Actor_Player* link)
{
    u16 variable;

    if (link->mask != PLAYER_MASK_STONE)
        return 0;

    variable = this->variable;

    switch (this->id)
    {
    case AC_EN_AM:
        return !!(variable != 0);
    case AC_EN_NY:
    case AC_EN_SB:
    case AC_EN_RR:
    case AC_EN_TORCH2:
    case AC_EN_FZ:
    case AC_EN_WEIYER:
    case AC_EN_EIYER:
    case AC_EN_BB:
    case AC_EN_ANUBICE:
    case AC_EN_TP:
    case AC_EN_BA:
    case AC_EN_GOMA:
    case AC_EN_DODOJR:
    case AC_EN_SKJ:
    case AC_EN_CROW:
    case AC_EN_HINTNUTS:
    case AC_EN_WALLMAS:
    case AC_EN_REEBA:
    case AC_EN_FLOORMAS:
    case AC_EN_DEKUNUTS:
    case AC_EN_DNS:
    case AC_EN_MB:
    case AC_EN_TITE:
    case AC_EN_PEEHAT:
    case AC_EN_FIREFLY:
    case AC_EN_DODONGO:
    case AC_EN_SW:
    case AC_EN_VM:
    case AC_EN_ST:
    case AC_EN_GE1:
    case AC_EN_GE2:
    case AC_EN_BILI:
    case AC_EN_VALI:
        return 1;
    default:
        return 0;
    }
}

void Actor_RunUpdate(Actor* this, GameState_Play* play, ActorFunc update)
{
    int ignorePlayer;
    s16 yawTowardsPlayer;
    f32 xyzDistToPlayerSq;
    f32 xzDistanceFromLink;
    f32 yDistanceFromLink;

    ignorePlayer = shouldActorIgnorePlayer(this, GET_LINK(play));
    if (ignorePlayer)
    {
        yawTowardsPlayer = this->yawTowardsPlayer;
        xyzDistToPlayerSq = this->xyzDistToPlayerSq;
        xzDistanceFromLink = this->xzDistanceFromLink;
        yDistanceFromLink = this->yDistanceFromLink;

        this->yawTowardsPlayer = (s16)(u16)(RandFloat() * 0x10000);
        this->xyzDistToPlayerSq = 10000.f;
        this->xzDistanceFromLink = 10000.f;
        this->yDistanceFromLink = 10000.f;
    }
    update(this, play);
    if (ignorePlayer)
    {
        this->yawTowardsPlayer = yawTowardsPlayer;
        this->xyzDistToPlayerSq = xyzDistToPlayerSq;
        this->xzDistanceFromLink = xzDistanceFromLink;
        this->yDistanceFromLink = yDistanceFromLink;
    }
}
