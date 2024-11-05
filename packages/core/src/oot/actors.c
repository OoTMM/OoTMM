#include <combo.h>
#include <combo/dungeon.h>
#include <combo/souls.h>
#include <combo/player.h>
#include <combo/mask.h>
#include <combo/config.h>
#include <combo/global.h>
#include <combo/actor.h>
#include <combo/multi.h>

static s16 sActorIdToSpawn;

static int opt(int x)
{
    return x > 0 ? 1 : -1;
}

static int canSpawnSoul(PlayState* play, s16 actorId, u16 variable)
{
    if (g.isCredits)
        return 1;

    switch (actorId)
    {
    case ACTOR_EN_DS:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_OLD_HAG));
    case ACTOR_EN_JS:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_CARPET_MAN));
    case ACTOR_EN_HS:
    case ACTOR_EN_HS2:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_GROG));
    case ACTOR_EN_IN:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_GORMAN));
    case ACTOR_EN_MK:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_SCIENTIST));
    case ACTOR_EN_MS:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_BEAN_SALESMAN));
    case ACTOR_EN_TG:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_HONEY_DARLING));
    case ACTOR_EN_TAKARA_MAN:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_CHEST_GAME_OWNER));
    case ACTOR_EN_POH:
        switch (variable)
        {
        case 2:
        case 3:
            return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_COMPOSER_BROS));
        default:
            return 1;
        }
    case ACTOR_EN_DNS:
    case ACTOR_EN_SHOPNUTS:
        return opt(comboHasSoulOot(GI_OOT_SOUL_MISC_BUSINESS_SCRUB));
    case ACTOR_EN_TEST:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_STALFOS);
    case ACTOR_BG_BDAN_OBJECTS:
        if (variable & 0xff)
            return 1;
        /* Fallthrough */
    case ACTOR_EN_OKUTA:
    case ACTOR_EN_BIGOKUTA:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_OCTOROK);
    case ACTOR_EN_WALLMAS:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_WALLMASTER);
    case ACTOR_EN_DODONGO:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_DODONGO);
    case ACTOR_EN_FIREFLY:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_KEESE);
    case ACTOR_EN_TITE:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_TEKTITE);
    case ACTOR_EN_PEEHAT:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_PEAHAT);
    case ACTOR_EN_ZF:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_LIZALFOS_DINALFOS);
    case ACTOR_EN_GOMA:
        if (play->sceneId == SCE_OOT_LAIR_GOHMA)
            return 1;
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_GOHMA_LARVA);
    case ACTOR_EN_BUBBLE:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_SHABOM);
    case ACTOR_EN_DODOJR:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_BABY_DODONGO);
    case ACTOR_EN_TORCH2:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_DARK_LINK);
    case ACTOR_EN_BILI:
    case ACTOR_EN_VALI:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_BIRI_BARI);
    case ACTOR_EN_TP:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_TAILPASARN);
    case ACTOR_EN_ST:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_SKULLTULA);
    case ACTOR_EN_BW:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_TORCH_SLUG);
    case ACTOR_EN_MB:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_MOBLIN);
    case ACTOR_EN_AM:
        if (variable == 0)
            return 1;
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_ARMOS);
    case ACTOR_EN_DEKUBABA:
    case ACTOR_EN_KAREBABA:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_DEKU_BABA);
    case ACTOR_EN_DEKUNUTS:
    case ACTOR_EN_HINTNUTS:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_DEKU_SCRUB);
    case ACTOR_EN_BB:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_BUBBLE);
    case ACTOR_EN_VM:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_BEAMOS);
    case ACTOR_EN_FLOORMAS:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_FLOORMASTER);
    case ACTOR_EN_RD:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_REDEAD_GIBDO);
    case ACTOR_EN_SW:
        if (variable & 0xe000)
            return opt(comboHasSoulOot(GI_OOT_SOUL_MISC_GS));
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_SKULLWALLTULA);
    case ACTOR_EN_FD:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_FLARE_DANCER);
    case ACTOR_EN_DH:
    case ACTOR_EN_DHA:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_DEAD_HAND);
    case ACTOR_EN_SB:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_SHELL_BLADE);
    case ACTOR_EN_RR:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_LIKE_LIKE);
    case ACTOR_EN_NY:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_SPIKE);
    case ACTOR_EN_ANUBICE_TAG:
    case ACTOR_EN_ANUBICE:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_ANUBIS);
    case ACTOR_EN_IK:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_IRON_KNUCKLE);
    case ACTOR_EN_SKJ:
        if (gSave.age == AGE_CHILD)
            return 1;
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_SKULL_KID);
    case ACTOR_EN_TUBO_TRAP:
        return opt(comboHasSoulOot(GI_OOT_SOUL_ENEMY_FLYING_POT));
    case ACTOR_EN_FZ:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_FREEZARD);
    case ACTOR_EN_WEIYER:
    case ACTOR_EN_EIYER:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_STINGER);
    case ACTOR_EN_WF:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_WOLFOS);
    case ACTOR_EN_CROW:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_GUAY);
    case ACTOR_BOSS_GOMA:
        return comboHasSoulOot(GI_OOT_SOUL_BOSS_QUEEN_GOHMA);
    case ACTOR_BOSS_DODONGO:
        return comboHasSoulOot(GI_OOT_SOUL_BOSS_KING_DODONGO);
    case ACTOR_BOSS_VA:
        return comboHasSoulOot(GI_OOT_SOUL_BOSS_BARINADE);
    case ACTOR_BOSS_GANONDROF:
        return comboHasSoulOot(GI_OOT_SOUL_BOSS_PHANTOM_GANON);
    case ACTOR_BOSS_FD:
        return comboHasSoulOot(GI_OOT_SOUL_BOSS_VOLVAGIA);
    case ACTOR_BOSS_MO:
        return comboHasSoulOot(GI_OOT_SOUL_BOSS_MORPHA);
    case ACTOR_BOSS_SST:
        return comboHasSoulOot(GI_OOT_SOUL_BOSS_BONGO_BONGO);
    case ACTOR_BOSS_TW:
        return comboHasSoulOot(GI_OOT_SOUL_BOSS_TWINROVA);
    case ACTOR_EN_BA:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_PARASITE);
    case ACTOR_EN_REEBA:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_LEEVER);
    case ACTOR_EN_SKB:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_STALCHILD);
    case ACTOR_EN_SA:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_SARIA));
    case ACTOR_EN_DU:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_DARUNIA));
    case ACTOR_EN_RU1:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_RUTO));
    case ACTOR_EN_KZ:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_KING_ZORA));
    case ACTOR_EN_NIW_LADY:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_ANJU));
    case ACTOR_EN_TORYO:
    case ACTOR_EN_DAIKU:
    case ACTOR_EN_DAIKU_KAKARIKO:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_CARPENTERS));
    case ACTOR_EN_FU:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_GURU_GURU));
    case ACTOR_EN_MD:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_MIDO));
    case ACTOR_EN_KO:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_KOKIRI));
    case ACTOR_EN_OSSAN:
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
    case ACTOR_EN_HEISHI2:
    case ACTOR_EN_HEISHI3:
    case ACTOR_EN_HEISHI4:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_HYLIAN_GUARD));
    case ACTOR_EN_ANI:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_ROOFTOP_MAN));
    case ACTOR_EN_MU:
    case ACTOR_EN_NIW_GIRL:
    case ACTOR_EN_MM:
    case ACTOR_EN_MM2:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_CITIZEN));
    case ACTOR_EN_HY:
        switch (variable & 0x3f)
        {
        case 0x00: return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_DOG_LADY));
        case 0x05: return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_BANKER));
        case 0x07: return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_ASTRONOMER));
        default: return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_CITIZEN));
        }
    case ACTOR_EN_MA1:
    case ACTOR_EN_MA2:
    case ACTOR_EN_MA3:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_MALON));
    case ACTOR_BG_SPOT15_RRBOX:
        if(play->sceneId != SCE_OOT_HYRULE_CASTLE)
            return 1;
        /* Fallthrough */
    case ACTOR_EN_TA:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_TALON));
    case ACTOR_EN_FISHING:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_FISHING_POND_OWNER));
    case ACTOR_EN_GO:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_GORON));
    case ACTOR_EN_GO2:
        switch (variable & 0x1f)
        {
        case 1: return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_GORON_CHILD));
        case 2: return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_BIGGORON));
        default: return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_GORON));
        }
        break;
    case ACTOR_EN_GM:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_MEDIGORON));
    case ACTOR_EN_ZO:
    case ACTOR_EN_DIVING_GAME:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_ZORA));
    case ACTOR_EN_BOM_BOWL_MAN:
    case ACTOR_BG_BOWL_WALL:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_BOMBCHU_BOWLING_LADY));
    case ACTOR_EN_SYATEKI_MAN:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_SHOOTING_GALLERY_OWNER));
    case ACTOR_EN_TK:
    case ACTOR_EN_PO_RELAY:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_DAMPE));
    case ACTOR_EN_CS:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_BOMBERS));
    case ACTOR_EN_GB:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_POE_COLLECTOR));
    case ACTOR_EN_XC:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_SHEIK));
    case ACTOR_EN_ZL1:
    case ACTOR_EN_ZL3:
    case ACTOR_EN_ZL4:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_ZELDA));
    case ACTOR_EN_GE1:
    case ACTOR_EN_GE2:
        return opt(comboHasSoulOot(GI_OOT_SOUL_NPC_THIEVES));
    case ACTOR_EN_GELDB:
        return comboHasSoulOot(GI_OOT_SOUL_ENEMY_THIEVES);
    default:
        return 1;
    }
}

static int canSpawnActor(PlayState* play, s16 actorId, u16 param)
{
    switch (actorId)
    {
    case ACTOR_EN_SYATEKI_MAN:
        if(gSave.entrance == ENTR_OOT_CHILD_ARCHERY && gSave.age == AGE_CHILD)
            return 1;
        if(gSave.entrance == ENTR_OOT_ADULT_ARCHERY && gSave.age == AGE_ADULT)
            return 1;
        else
            return 0;
    case ACTOR_BG_JYA_BLOCK:
        return !Config_Flag(CFG_OOT_AGELESS_STRENGTH);
    case ACTOR_ITEM_OCARINA:
        return gSave.info.inventory.quest.stoneEmerald && gSave.info.inventory.quest.stoneRuby && gSave.info.inventory.quest.stoneSapphire && comboHasSoulOot(GI_OOT_SOUL_NPC_ZELDA);
    case ACTOR_EN_MM:
        if (param == 0x01)
            return gSave.info.inventory.quest.stoneEmerald && gSave.info.inventory.quest.stoneRuby && gSave.info.inventory.quest.stoneSapphire;
        return 1;
    default:
        return 1;
    }
}

static void ZeroActor(Actor* this, int size)
{
    memset(this, 0, size);
    this->actorIndex = g.actorIndex;
}

PATCH_CALL(0x800252f8, ZeroActor);

Actor* Actor_SpawnWrapper(ActorContext* actorCtx, PlayState *play, short actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable)
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
    if (actorId == ACTOR_OBJ_HANA && ((variable & 3) == 2))
    {
        actorId = ACTOR_EN_KUSA;
        variable = 0xff00;
    }

    if (Config_Flag(CFG_OOT_OPEN_MASK_SHOP) && play->sceneId == SCE_OOT_MARKET_CHILD_NIGHT && actorId == ACTOR_EN_DOOR)
        if (((variable >> 7 & 7) == 0x5) && ((variable & 0x3f) == 0x10))
            variable = 0x1bf;

    if (Config_Flag(CFG_OOT_OPEN_ZD_SHORTCUT) && actorId == ACTOR_BG_SPOT06_OBJECTS && play->sceneId == SCE_OOT_LAKE_HYLIA)
        if (((variable >> 8) & 0xff) == 3)
            return NULL;

    sActorIdToSpawn = actorId;
    actor = _Actor_Spawn(actorCtx, play, actorId, x, y, z, rx, ry, rz, variable);
    if (actorId == ACTOR_ARMS_HOOK && gSave.age == AGE_ADULT)
        actor->objectSlot = Object_GetSlot(&play->objectCtx, 0x14);
    return actor;
}

static int GetRoomClearFlagForActor(PlayState* play, int flag)
{
    int res;

    res = GetRoomClearFlag(play, flag);
    if (Config_Flag(CFG_ER_WALLMASTERS) && sActorIdToSpawn == ACTOR_EN_WALLMAS)
        res = 0;
    return res;
}

PATCH_CALL(0x80025284, GetRoomClearFlagForActor);

static int shouldActorIgnorePlayer(Actor* this, Player* link)
{
    u16 variable;

    if (link->mask != MASK_STONE)
        return 0;

    variable = this->params;

    switch (this->id)
    {
    case ACTOR_EN_AM:
        return !!(variable != 0);
    case ACTOR_EN_NY:
    case ACTOR_EN_SB:
    case ACTOR_EN_RR:
    case ACTOR_EN_TORCH2:
    case ACTOR_EN_FZ:
    case ACTOR_EN_WEIYER:
    case ACTOR_EN_EIYER:
    case ACTOR_EN_BB:
    case ACTOR_EN_ANUBICE:
    case ACTOR_EN_TP:
    case ACTOR_EN_BA:
    case ACTOR_EN_GOMA:
    case ACTOR_EN_DODOJR:
    case ACTOR_EN_SKJ:
    case ACTOR_EN_CROW:
    case ACTOR_EN_HINTNUTS:
    case ACTOR_EN_WALLMAS:
    case ACTOR_EN_REEBA:
    case ACTOR_EN_FLOORMAS:
    case ACTOR_EN_DEKUNUTS:
    case ACTOR_EN_DNS:
    case ACTOR_EN_MB:
    case ACTOR_EN_TITE:
    case ACTOR_EN_PEEHAT:
    case ACTOR_EN_FIREFLY:
    case ACTOR_EN_DODONGO:
    case ACTOR_EN_SW:
    case ACTOR_EN_VM:
    case ACTOR_EN_ST:
    case ACTOR_EN_GE1:
    case ACTOR_EN_GE2:
    case ACTOR_EN_BILI:
    case ACTOR_EN_VALI:
    case ACTOR_EN_HEISHI2:
    case ACTOR_EN_HEISHI3:
    case ACTOR_EN_HEISHI4:
        return 1;
    default:
        return 0;
    }
}

static int GetDamage(DamageTable* tbl, int type)
{
    return tbl->attack[type] & 0xf;
}

static int GetDamageEffect(DamageTable* tbl, int type)
{
    return tbl->attack[type] >> 4;
}

static void SetDamage(DamageTable* tbl, int type, int value)
{
    tbl->attack[type] = (tbl->attack[type] & 0xf0) | (value & 0xf);
}

static void SetDamageEffect(DamageTable* tbl, int type, int value)
{
    tbl->attack[type] = (tbl->attack[type] & 0xf) | (value << 4);
}

static void Actor_UpdateDamageTable(Actor* this)
{
    DamageTable* tbl;
    int dmg;
    int dmgEffectKokiri;
    int dmgEffectGiant;

    tbl = this->colChkInfo.damageTable;
    if (!tbl)
        return;
    if (GetDamage(tbl, 10) != 4)
        return; /* Weird table, better not touch */

    dmgEffectKokiri = GetDamageEffect(tbl, 8);
    dmgEffectGiant = GetDamageEffect(tbl, 10);

    if (GetDamage(tbl, 8) == 0 && !gSharedCustomSave.extraSwordsOot)
        return;
    dmg = 1 + gSharedCustomSave.extraSwordsOot;

    /* Dead hand takes MS-level damage from KS */
    if (dmg < 2 && (this->id == ACTOR_EN_DH || this->id == ACTOR_EN_DHA))
        dmg = 2;

    SetDamage(tbl, 8, dmg);
    SetDamage(tbl, 22, dmg);
    SetDamage(tbl, 25, dmg * 2);

    if (gSharedCustomSave.extraSwordsOot && (dmgEffectKokiri != dmgEffectGiant))
    {
        SetDamageEffect(tbl, 8, dmgEffectGiant);
        SetDamageEffect(tbl, 22, dmgEffectGiant);
        SetDamageEffect(tbl, 25, dmgEffectGiant);
    }
}

void Actor_RunUpdate(Actor* this, PlayState* play, ActorFunc update)
{
    int ignorePlayer;
    s16 yawTowardsPlayer;
    f32 xyzDistToPlayerSq;
    f32 xzDistToPlayer;
    f32 yDistanceFromLink;

    Actor_UpdateDamageTable(this);
    ignorePlayer = shouldActorIgnorePlayer(this, GET_PLAYER(play));
    if (ignorePlayer)
    {
        yawTowardsPlayer = this->yawTowardsPlayer;
        xyzDistToPlayerSq = this->xyzDistToPlayerSq;
        xzDistToPlayer = this->xzDistToPlayer;
        yDistanceFromLink = this->yDistanceFromLink;

        this->yawTowardsPlayer = (s16)(u16)(Rand_ZeroOne() * 0x10000);
        this->xyzDistToPlayerSq = 10000.f;
        this->xzDistToPlayer = 10000.f;
        this->yDistanceFromLink = 10000.f;
    }
    update(this, play);
    if (ignorePlayer)
    {
        this->yawTowardsPlayer = yawTowardsPlayer;
        this->xyzDistToPlayerSq = xyzDistToPlayerSq;
        this->xzDistToPlayer = xzDistToPlayer;
        this->yDistanceFromLink = yDistanceFromLink;
    }
}

void Actor_AfterDrawAll(void)
{
    Multi_DrawWisps(gPlay);
}
