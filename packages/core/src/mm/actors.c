#include <combo.h>
#include <combo/souls.h>
#include <combo/entrance.h>
#include <combo/player.h>
#include <combo/math.h>
#include <combo/config.h>
#include <combo/global.h>
#include <combo/inventory.h>
#include <combo/multi.h>

void EnGo_GiveItem(Actor* actor, PlayState* play, s16 gi, float a, float b);
void EnDnh_GiveItem(Actor* actor, PlayState* play, s16 gi, float a, float b);
void EnShn_GiveItem(Actor* actor, PlayState* play, s16 gi, float a, float b);
void EnPm_GiveItem(Actor* this, PlayState* play, s16 gi, float a, float b);
void EnAn_GiveItem(Actor* actor, PlayState* play, s16 gi, float a, float b);
void EnPst_GiveItem(Actor* actor, PlayState* play, s16 gi, float a, float b);
void EnNb_GiveItem(Actor* actor, PlayState* play, s16 gi, float a, float b);
void EnAl_GiveItem(Actor* actor, PlayState* play, s16 gi, float a, float b);
void EnBjt_GiveItem(Actor* actor, PlayState* play, s16 gi, float a, float b);
void EnTab_GiveItem(Actor* actor, PlayState* play, s16 gi, float a, float b);

typedef void (*TextBoxCallback)(Actor*, PlayState*, s16);
void EnGo_AfterTextBox(Actor* this, PlayState* play, s16 messageId);
void EnTab_AfterTextBox(Actor* this, PlayState* play, s16 messageId);
void EnBjt_AfterTextBox(Actor* this, PlayState* play, s16 messageId);

static Actor* sByteCodeActor;

static int opt(int x)
{
    return x > 0 ? 1 : -1;
}

/* TODO: Patch every bytecode and remove this */
static s16 convertGi(s16 initial)
{
    switch (initial)
    {
    case 0x01: return GI_MM_RUPEE_GREEN;
    case 0x02: return GI_MM_RUPEE_BLUE;
    case 0x04: return GI_MM_RUPEE_RED;
    case 0x05: return GI_MM_RUPEE_PURPLE;
    case 0x06: return GI_MM_RUPEE_SILVER;
    case 0x07: return GI_MM_RUPEE_GOLD;
    case 0x0a: return GI_MM_RECOVERY_HEART;
    case 0x0c: return GI_MM_HEART_PIECE;
    case 0x34: return GI_MM_POWDER_KEG;
    case 0x43: return GI_MM_PICTOGRAPH_BOX;
    case 0x6f: return GI_MM_BOTTLE_CHATEAU;
    case 0x84: return GI_MM_MASK_POSTMAN;
    case 0x8f: return GI_MM_MASK_KAFEI;
    case 0x91: return GI_MM_CHATEAU;
    case 0x92: return GI_MM_MILK;
    case 0xa0: return GI_MM_ROOM_KEY;
    case 0xaa: return GI_MM_LETTER_TO_KAFEI;
    }

    return initial;
}

static void Actor_ByteCode_GiveItem(Actor* actor, PlayState* play, s16 gi, float a, float b)
{
    void (*func)(Actor*, PlayState*, s16, float, float);

    if (actor->id != ACTOR_EN_NB)
        gi = convertGi(gi);

    switch (actor->id)
    {
    case ACTOR_EN_GO:
        func = EnGo_GiveItem;
        break;
    case ACTOR_EN_DNH:
        func = EnDnh_GiveItem;
        break;
    case ACTOR_EN_SHN:
        func = EnShn_GiveItem;
        break;
    case ACTOR_EN_PM:
        func = EnPm_GiveItem;
        break;
    case ACTOR_EN_PST:
        func = EnPst_GiveItem;
        break;
    case ACTOR_EN_AN:
        func = EnAn_GiveItem;
        break;
    case ACTOR_EN_NB:
        func = EnNb_GiveItem;
        break;
    case ACTOR_EN_AL:
        func = EnAl_GiveItem;
        break;
    case ACTOR_EN_TAB:
        func = EnTab_GiveItem;
        break;
    case ACTOR_EN_BJT:
        func = EnBjt_GiveItem;
        break;
    default:
        func = (void*)Actor_OfferGetItem;
        break;
    }

    func(actor, play, gi, a, b);
}

PATCH_CALL(0x8010aa34, Actor_ByteCode_GiveItem);

void EnGo_AfterGivingItem(Actor* actor);
void EnDnh_AfterGivingItem(Actor* actor);
void EnPm_AfterGivingItem(Actor* actor);
void EnAn_AfterGivingItem(Actor* actor);
void EnTab_AfterGivingItem(Actor* actor);

static int Actor_ByteCode_HasParent(Actor* actor)
{
    int ret;
    void (*func)(Actor*);

    ret = Actor_HasParentZ(actor);
    if (ret)
    {
        switch (actor->id)
        {
        case ACTOR_EN_GO:
            func = EnGo_AfterGivingItem;
            break;
        case ACTOR_EN_DNH:
            func = EnDnh_AfterGivingItem;
            break;
        case ACTOR_EN_PM:
            func = EnPm_AfterGivingItem;
            break;
        case ACTOR_EN_AN:
            func = EnAn_AfterGivingItem;
            break;
        case ACTOR_EN_TAB:
            func = EnTab_AfterGivingItem;
            break;
        default:
            func = NULL;
            break;
        }

        if (func)
            func(actor);
    }
    return ret;
}

PATCH_CALL(0x8010a9f8, Actor_ByteCode_HasParent);

static void Actor_ByteCode_RemoveItem(s16 item, s16 slot)
{
    switch (item)
    {
    case ITEM_MM_LETTER_TO_KAFEI:
        comboRemoveTradeItem3(XITEM_MM_TRADE3_LETTER_TO_KAFEI);
        break;
    case ITEM_MM_PENDANT_OF_MEMORIES:
        comboRemoveTradeItem3(XITEM_MM_TRADE3_PENDANT_OF_MEMORIES);
        break;
    case ITEM_MM_LETTER_TO_MAMA:
        comboRemoveTradeItem2(XITEM_MM_TRADE2_LETTER_TO_MAMA);
        break;
    default:
        RemoveItem(item, slot);
        break;
    }
}

PATCH_CALL(0x8010bc60, Actor_ByteCode_RemoveItem);

int Actor_RunByteCodeWrapper(Actor* this, PlayState* play, void* bytecode, void* unk1, void* unk2)
{
    sByteCodeActor = this;
    return Actor_RunByteCode(this, play, bytecode, unk1, unk2);
}

PATCH_CALL(0x8092d53c, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80950de0, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80a157f8, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80a50f58, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80a52fe0, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80a88208, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80ae6a84, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80afa61c, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80b2be74, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80b3d41c, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80b3efb4, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80b57a64, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80bc0ecc, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80bc2328, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80bd379c, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80bdf6e4, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80be137c, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80bf2bf4, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80bfda74, Actor_RunByteCodeWrapper);
PATCH_CALL(0x80c22a14, Actor_RunByteCodeWrapper);

static void Actor_ByteCode_DispatchTextBox(PlayState* play, s16 messageId)
{
    Actor* this;
    TextBoxCallback cb;

    this = sByteCodeActor;
    cb = NULL;
    switch (this->id)
    {
    case ACTOR_EN_GO:
        cb = EnGo_AfterTextBox;
        break;
    case ACTOR_EN_TAB:
        cb = EnTab_AfterTextBox;
        break;
    case ACTOR_EN_BJT:
        cb = EnBjt_AfterTextBox;
        break;
    }
    if (cb)
    {
        cb(this, play, messageId);
    }
}

static void Actor_ByteCode_PlayerDisplayTextBox_Hook(PlayState* play, s16 messageId, void* unk)
{
    PlayerDisplayTextBox(play, messageId, unk);
    Actor_ByteCode_DispatchTextBox(play, messageId);
}

PATCH_CALL(0x8010af04, Actor_ByteCode_PlayerDisplayTextBox_Hook);

static void Actor_ByteCode_DisplayTextBox2_Hook(PlayState* play, s16 messageId)
{
    DisplayTextBox2(play, messageId);
    Actor_ByteCode_DispatchTextBox(play, messageId);
}

PATCH_CALL(0x8010af50, Actor_ByteCode_DisplayTextBox2_Hook);

static int canSpawnSoul(PlayState* play, s16 actorId, u16 variable)
{
    if (g.isCredits)
        return 1;

    switch (actorId)
    {
    case ACTOR_EN_GINKO_MAN:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_BANKER));
    case ACTOR_EN_AN:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_ANJU));
    case ACTOR_EN_GURUGURU:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_GURU_GURU));
    case ACTOR_EN_DAIKU:
    case ACTOR_EN_DAIKU2:
        if(play->sceneId == SCE_MM_MAYOR_HOUSE && play->roomCtx.curRoom.num == 0x01)
            return opt(comboHasSoulMm(GI_MM_SOUL_NPC_MAYOR_DOTOUR));
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_CARPENTERS));
    case ACTOR_EN_MS:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_BEAN_SALESMAN));
    case ACTOR_EN_MA_YTO:
    case ACTOR_EN_MA_YTS:
    case ACTOR_EN_MA4:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_MALON));
    case ACTOR_EN_INVADEPOH:
        switch ((variable >> 4) & 0xf)
        {
        case 0x4:
        case 0x5:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xb:
        case 0xc:
            return opt(comboHasSoulMm(GI_MM_SOUL_NPC_MALON));
        }
        break;
    case ACTOR_EN_ZOT:
    case ACTOR_EN_ZOW:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_ZORA));
    case ACTOR_EN_BJT:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_TOILET_HAND));
    case ACTOR_EN_BAL:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_TINGLE));
    case ACTOR_EN_TAKARAYA:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_BOMBCHU_BOWLING_LADY));
    case ACTOR_EN_KBT:
    case ACTOR_EN_KGY:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_BLACKSMITHS));
    case ACTOR_EN_GB2:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_POE_COLLECTOR));
    case ACTOR_EN_TAB:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_TALON));
    case ACTOR_EN_BJI_01:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_ASTRONOMER));
    case ACTOR_EN_MUTO:
    case ACTOR_EN_HEISHI:
    case ACTOR_EN_BAISEN:
        if(play->sceneId == SCE_MM_MAYOR_HOUSE && play->roomCtx.curRoom.num != 0x01)
            return 1;
        /* Fallthrough */
    case ACTOR_EN_DT:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_MAYOR_DOTOUR));
    case ACTOR_EN_TRT:
    case ACTOR_EN_TRT2:
    case ACTOR_EN_TRU:
    case ACTOR_EN_TRU_MT:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_KOUME_KOTAKE));
    case ACTOR_EN_KITAN:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_KEATON));
    case ACTOR_EN_TEST3:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_KAFEI));
    case ACTOR_EN_TOTO:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_TOTO));
    case ACTOR_EN_ZOV:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_RUTO));
    case ACTOR_EN_ZOD:
    case ACTOR_EN_ZOS:
    case ACTOR_EN_ZOB:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_ZORA_MUSICIANS));
    case ACTOR_EN_FU:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_HONEY_DARLING));
    case ACTOR_EN_SOB1:
        switch (variable & 0xf)
        {
        case 0: return opt(comboHasSoulMm(GI_MM_SOUL_NPC_ZORA_SHOPKEEPER));
        case 1: return opt(comboHasSoulMm(GI_MM_SOUL_NPC_GORON_SHOPKEEPER));
        case 2: return opt(comboHasSoulMm(GI_MM_SOUL_NPC_BOMBCHU_SHOPKEEPER));
        default: return 1;
        }
    case ACTOR_EN_JGAME_TSN:
    case ACTOR_EN_TSN:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_CHEST_GAME_OWNER));
    case ACTOR_EN_LIFT_NUTS:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_PLAYGROUND_SCRUBS));
    case ACTOR_EN_DNP:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_DEKU_PRINCESS));
    case ACTOR_EN_DNQ:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_DEKU_KING));
    case ACTOR_EN_TK:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_DAMPE));
    case ACTOR_EN_PO_COMPOSER:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_COMPOSER_BROS));
    case ACTOR_EN_STH:
    case ACTOR_EN_JA:
    case ACTOR_EN_YB:
    case ACTOR_EN_RZ:
    case ACTOR_EN_MM3:
    case ACTOR_EN_PM:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_CITIZEN));
    case ACTOR_EN_DNO:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_BUTLER_DEKU));
    case ACTOR_EN_BOM_BOWL_MAN:
    case ACTOR_EN_BOMJIMA:
    case ACTOR_EN_BOMJIMB:
    case ACTOR_EN_BOMBERS:
    case ACTOR_EN_BOMBERS2:
    case ACTOR_EN_BOMBAL:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_BOMBERS));
    case ACTOR_EN_RSN:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_BOMBCHU_SHOPKEEPER));
    case ACTOR_EN_OSSAN:
        switch (variable & 0xf)
        {
        case 0x00: return opt(comboHasSoulMm(GI_MM_SOUL_NPC_FISHING_POND_OWNER));
        case 0x01: return opt(comboHasSoulMm(GI_MM_SOUL_NPC_ROOFTOP_MAN));
        default: UNREACHABLE();
        }
    case ACTOR_EN_ANI:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_ROOFTOP_MAN));
    case ACTOR_EN_GO:
        switch (variable)
        {
        case 0x08: return opt(comboHasSoulMm(GI_MM_SOUL_NPC_MEDIGORON));
        default: return opt(comboHasSoulMm(GI_MM_SOUL_NPC_GORON));
        }
    case ACTOR_EN_S_GORO:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_GORON));
    case ACTOR_EN_GK:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_GORON_CHILD));
    case ACTOR_EN_JG:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_GORON_ELDER));
    case ACTOR_EN_DAI:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_BIGGORON));
    case ACTOR_EN_SYATEKI_MAN:
        if (play->sceneId == SCE_MM_SHOOTING_GALLERY_SWAMP)
            return opt(comboHasSoulMm(GI_MM_SOUL_NPC_BAZAAR_SHOPKEEPER));
        else
            return opt(comboHasSoulMm(GI_MM_SOUL_NPC_SHOOTING_GALLERY_OWNER));
    case ACTOR_EN_SYATEKI_OKUTA:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_SHOOTING_GALLERY_OWNER));
    case ACTOR_EN_MK:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_SCIENTIST));
    case ACTOR_EN_IN:
    case ACTOR_EN_GM:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_GORMAN));
    case ACTOR_EN_HS:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_GROG));
    case ACTOR_EN_AOB_01:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_DOG_LADY));
    case ACTOR_EN_AL:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_AROMA));
    case ACTOR_EN_JS:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_MOON_CHILDREN));
    case ACTOR_EN_SHN:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_TOURIST_CENTER));
    case ACTOR_EN_NB:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_OLD_HAG));
    case ACTOR_EN_KENDO_JS:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_CARPET_MAN));
    case ACTOR_EN_OKUTA:
    case ACTOR_EN_BIGOKUTA:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_OCTOROK);
    case ACTOR_EN_WALLMAS:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_WALLMASTER);
    case ACTOR_EN_DODONGO:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_DODONGO);
    case ACTOR_EN_FIREFLY:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_KEESE);
    case ACTOR_EN_TITE:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_TEKTITE);
    case ACTOR_EN_PEEHAT:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_PEAHAT);
    case ACTOR_EN_DINOFOS:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_LIZALFOS_DINOLFOS);
    case ACTOR_EN_ST:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_SKULLTULA);
    case ACTOR_EN_AM:
    case ACTOR_EN_FAMOS:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_ARMOS);
    case ACTOR_EN_DEKUBABA:
    case ACTOR_EN_KAREBABA:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_DEKU_BABA);
    case ACTOR_EN_DEKUNUTS:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_DEKU_SCRUB);
    case ACTOR_EN_BBFALL:
    case ACTOR_EN_BB:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_BUBBLE);
    case ACTOR_EN_VM:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_BEAMOS);
    case ACTOR_EN_RD:
    case ACTOR_EN_TALK_GIBUD:
    case ACTOR_EN_RAILGIBUD:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_REDEAD_GIBDO);
    case ACTOR_EN_SW:
        if (variable & 0x03)
            return opt(comboHasSoulMm(GI_MM_SOUL_MISC_GS));
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_SKULLWALLTULA);
    case ACTOR_OBJ_MAKEKINSUTA:
        return opt(comboHasSoulMm(GI_MM_SOUL_MISC_GS));
    case ACTOR_EN_SB:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_SHELL_BLADE);
    case ACTOR_EN_RR:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_LIKE_LIKE);
    case ACTOR_EN_IK:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_IRON_KNUCKLE);
    case ACTOR_EN_FZ:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_FREEZARD);
    case ACTOR_EN_WF:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_WOLFOS);
    case ACTOR_EN_CROW:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_GUAY);
    case ACTOR_EN_TUBO_TRAP:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_FLYING_POT) ? 1 : -1;
    case ACTOR_EN_FLOORMAS:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_FLOORMASTER);
    case ACTOR_EN_SLIME:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_CHUCHU);
    case ACTOR_EN_DRAGON:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_DEEP_PYTHON);
    case ACTOR_EN_PR:
    case ACTOR_EN_PR2:
    case ACTOR_EN_PRZ:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_SKULLFISH);
    case ACTOR_EN_WDHAND:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_DEXIHAND);
    case ACTOR_EN_GRASSHOPPER:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_DRAGONFLY);
    case ACTOR_EN_SNOWMAN:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_EENO);
    case ACTOR_EN_EGOL:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_EYEGORE);
    case ACTOR_EN_PP:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_HIPLOOP);
    case ACTOR_EN_RAT:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_REAL_BOMBCHU);
    case ACTOR_EN_THIEFBIRD:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_TAKKURI);
    case ACTOR_EN_MKK:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_BOE);
    case ACTOR_EN_BAGUO:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_NEJIRON);
    case ACTOR_BOSS_05:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_BIO_BABA);
    case ACTOR_EN_JSO:
    case ACTOR_EN_JSO2:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_GARO);
    case ACTOR_EN_WIZ:
    case ACTOR_EN_WIZ_BROCK:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_WIZZROBE);
    case ACTOR_EN_DEATH:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_GOMESS);
    case ACTOR_EN_PAMETFROG:
    case ACTOR_EN_BIGSLIME:
    case ACTOR_EN_BIGPAMET:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_GEKKO);
    case ACTOR_EN_BAT:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_BAD_BAT);
    case ACTOR_EN_KAME:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_SNAPPER);
    case ACTOR_BOSS_04:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_WART);
    case ACTOR_EN_BSB:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_CAPTAIN_KEETA);
    case ACTOR_BOSS_01:
        return comboHasSoulMm(GI_MM_SOUL_BOSS_ODOLWA);
    case ACTOR_BOSS_HAKUGIN:
        return comboHasSoulMm(GI_MM_SOUL_BOSS_GOHT);
    case ACTOR_BOSS_03:
        return comboHasSoulMm(GI_MM_SOUL_BOSS_GYORG);
    case ACTOR_BOSS_02:
        return comboHasSoulMm(GI_MM_SOUL_BOSS_TWINMOLD);
    case ACTOR_EN_KNIGHT:
    case ACTOR_BOSS_06:
        return comboHasSoulMm(GI_MM_SOUL_BOSS_IGOS);
    case ACTOR_EN_NEO_REEBA:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_LEEVER);
    case ACTOR_EN_SKB:
        if ((play->sceneId == SCE_MM_IKANA_GRAVEYARD) && (play->roomCtx.curRoom.num == 0x01)) /* Upper graveyard */
            return 1;
        /* Fallthrough */
    case ACTOR_EN_HINT_SKB:
    case ACTOR_EN_RAIL_SKB:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_STALCHILD);
    case ACTOR_EN_SELLNUTS:
    case ACTOR_EN_SCOPENUTS:
    case ACTOR_EN_AKINDONUTS:
        return opt(comboHasSoulMm(GI_MM_SOUL_MISC_BUSINESS_SCRUB));
    case ACTOR_EN_GE1:
    case ACTOR_EN_GE2:
    case ACTOR_EN_GE3:
        return opt(comboHasSoulMm(GI_MM_SOUL_NPC_THIEVES));
    case ACTOR_EN_KAIZOKU:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_THIEVES);
    }

    return 1;
}

static int canSpawnActor(PlayState* play, s16 actorId, u16 variable)
{
    switch (actorId)
    {
    case ACTOR_EN_SYATEKI_OKUTA:
        if (play->sceneId == SCE_MM_SHOOTING_GALLERY && (gSave.day > 3 || gSave.day < 1))
            return 0;
        /* Fallthrough */
    default:
        return 1;
    }
}

static s16 sActorIdToSpawn;

Actor* Actor_SpawnAsChildAndCutsceneWrapper(ActorContext* actorCtx, PlayState *play, short actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable, int ex1, int ex2, int ex3)
{
    int ret;

    if (!canSpawnActor(play, actorId, variable))
        return NULL;

    /* Patch for gorman on donkeys */
    if (actorId == ACTOR_EN_HORSE && ((variable & 0x1fff) == 0x13 || (variable & 0x1fff) == 0x14) && !comboHasSoulMm(GI_MM_SOUL_NPC_GORMAN))
        variable = (variable & 0xe000);

    ret = canSpawnSoul(play, actorId, variable);
    if (ret <= 0)
    {
        if (ret == 0)
            g.roomEnemyLackSoul = 1;
        return NULL;
    }
    sActorIdToSpawn = actorId;
    return _Actor_SpawnAsChildAndCutscene(actorCtx, play, actorId, x, y, z, rx, ry, rz, variable, ex1, ex2, ex3);
}

static int GetRoomClearFlagForActor(PlayState* play, int flag)
{
    int res;

    res = Flags_GetClear(play, flag);
    if (Config_Flag(CFG_ER_WALLMASTERS) && sActorIdToSpawn == ACTOR_EN_WALLMAS)
        res = 0;
    return res;
}

PATCH_CALL(0x800baea0, GetRoomClearFlagForActor);

static LightInfo D_8015BC00;
static LightNode* D_8015BC10;
static s32 D_8015BC14;
static f32 D_8015BC18;

void Actor_InitFaroresWind(PlayState* play) {
    Vec3f lightPos;

    RespawnData* fw = &gCustomSave.fw[gOotSave.age];

    if (fw->data)
    {
        gSaveContext.respawn[RESPAWN_MODE_HUMAN] = *fw;
    }
    else
    {
        gSaveContext.respawn[RESPAWN_MODE_HUMAN].data = 0;
        gSaveContext.respawn[RESPAWN_MODE_HUMAN].pos.x = 0.0f;
        gSaveContext.respawn[RESPAWN_MODE_HUMAN].pos.y = 0.0f;
        gSaveContext.respawn[RESPAWN_MODE_HUMAN].pos.z = 0.0f;
    }

    lightPos = fw->pos;
    lightPos.y += 60.0f;

    Lights_PointNoGlowSetInfo(&D_8015BC00, lightPos.x, lightPos.y, lightPos.z, 0xFF, 0xFF, 0xFF, -1);

    D_8015BC10 = LightContext_InsertLight(play, &play->lightCtx, &D_8015BC00);
    D_8015BC14 = 0;
    D_8015BC18 = 0.0f;
}

void Actor_DrawFaroresWindPointer(PlayState* play)
{
    s32 lightRadius = -1;
    s32 params;

    OPEN_DISPS(play->state.gfxCtx);

    params = gSaveContext.respawn[RESPAWN_MODE_HUMAN].data;

    if (params && gSaveContext.respawn[RESPAWN_MODE_HUMAN].entrance != ENTR_FW_CROSS)
    {
        f32 yOffset = comboIsLinkAdult() ? 80.0f : 60.0f;
        f32 ratio = 1.0f;
        s32 alpha = 255;
        s32 temp = params - 40;

        if (temp < 0)
        {
            gSaveContext.respawn[RESPAWN_MODE_HUMAN].data = ++params;
            ratio = ABS(params) * 0.025f;
            D_8015BC14 = 60;
            D_8015BC18 = 1.0f;
        }
        else if (D_8015BC14)
        {
            D_8015BC14--;
        }
        else if (D_8015BC18 > 0.0f)
        {
            static Vec3f effectVel = { 0.0f, -0.05f, 0.0f };
            static Vec3f effectAccel = { 0.0f, -0.025f, 0.0f };
            static Color_RGBA8 effectPrimCol = { 255, 255, 255, 0 };
            static Color_RGBA8 effectEnvCol = { 100, 200, 0, 0 };
            Vec3f* curPos = &gSaveContext.respawn[RESPAWN_MODE_HUMAN].pos;
            Vec3f* nextPos = &gSaveContext.respawn[RESPAWN_MODE_DOWN].pos;
            f32 prevNum = D_8015BC18;
            Vec3f dist;
            f32 diff = Math_Vec3f_DistXYZAndStoreDiff(nextPos, curPos, &dist);
            Vec3f effectPos;
            f32 factor;
            f32 length;
            f32 dx;
            f32 speed;

            if (diff < 20.0f)
            {
                D_8015BC18 = 0.0f;
                Math_Vec3f_Copy(curPos, nextPos);
            }
            else
            {
                length = diff * (1.0f / D_8015BC18);
                speed = 20.0f / length;
                speed = CLAMP_MIN(speed, 0.05f);
                Math_StepToF(&D_8015BC18, 0.0f, speed);
                factor = (diff * (D_8015BC18 / prevNum)) / diff;
                curPos->x = nextPos->x + (dist.x * factor);
                curPos->y = nextPos->y + (dist.y * factor);
                curPos->z = nextPos->z + (dist.z * factor);
                length *= 0.5f;
                dx = diff - length;
                yOffset += sqrtf(SQ(length) - SQ(dx)) * 0.2f;
            }

            effectPos.x = curPos->x + Rand_CenteredFloat(6.0f);
            effectPos.y = curPos->y + 80.0f + (6.0f * Rand_ZeroOne());
            effectPos.z = curPos->z + Rand_CenteredFloat(6.0f);

            EffectSsKiraKira_SpawnDispersed(play, &effectPos, &effectVel, &effectAccel, &effectPrimCol, &effectEnvCol,
                                            1000, 16);

            if (D_8015BC18 == 0.0f)
            {
                gSaveContext.respawn[RESPAWN_MODE_HUMAN] = gSaveContext.respawn[RESPAWN_MODE_DOWN];
                gSaveContext.respawn[RESPAWN_MODE_HUMAN].playerParams = 0x6ff;
                gSaveContext.respawn[RESPAWN_MODE_HUMAN].data = 40;
            }
        }
        else if (temp > 0)
        {
            Vec3f* curPos = &gSaveContext.respawn[RESPAWN_MODE_HUMAN].pos;
            f32 nextRatio = 1.0f - temp * 0.1f;
            f32 curRatio = 1.0f - (f32)(temp - 1) * 0.1f;
            Vec3f eye;
            Vec3f dist;
            f32 diff;

            if (nextRatio > 0.0f)
            {
                eye.x = play->view.eye.x;
                eye.y = play->view.eye.y - yOffset;
                eye.z = play->view.eye.z;
                diff = Math_Vec3f_DistXYZAndStoreDiff(&eye, curPos, &dist);
                diff = (diff * (nextRatio / curRatio)) / diff;
                curPos->x = eye.x + (dist.x * diff);
                curPos->y = eye.y + (dist.y * diff);
                curPos->z = eye.z + (dist.z * diff);
            }

            alpha = 255 - (temp * 30);

            if (alpha < 0)
            {
                gCustomSave.fw[gOotSave.age].data = 0;
                gSaveContext.respawn[RESPAWN_MODE_HUMAN].data = 0;
                alpha = 0;
            }
            else
            {
                gSaveContext.respawn[RESPAWN_MODE_HUMAN].data = ++params;
            }

            ratio = 1.0f + ((f32)temp * 0.2); /* required to match */
        }

        lightRadius = 500.0f * ratio;

        s32 fwSceneId = Entrance_GetSceneIdAbsolute(gSaveContext.respawn[RESPAWN_MODE_HUMAN].entrance);
        s32 fwRoomId = gSaveContext.respawn[RESPAWN_MODE_HUMAN].roomIndex;

        s32 sceneMatches = fwSceneId == play->sceneId;

        f32 yPos = gSaveContext.respawn[RESPAWN_MODE_HUMAN].pos.y + yOffset;

        if (!sceneMatches)
        {
            switch (play->sceneId)
            {
            case SCE_MM_TWIN_ISLANDS_WINTER:
                sceneMatches = fwSceneId == SCE_MM_TWIN_ISLANDS_SPRING;
                break;
            case SCE_MM_GORON_VILLAGE_WINTER:
                sceneMatches = fwSceneId == SCE_MM_GORON_VILLAGE_SPRING;
                break;
            case SCE_MM_MOUNTAIN_VILLAGE_WINTER:
                sceneMatches = fwSceneId == SCE_MM_MOUNTAIN_VILLAGE_SPRING;
                break;
            case SCE_MM_TWIN_ISLANDS_SPRING:
                sceneMatches = fwSceneId == SCE_MM_TWIN_ISLANDS_WINTER;
                break;
            case SCE_MM_GORON_VILLAGE_SPRING:
                sceneMatches = fwSceneId == SCE_MM_GORON_VILLAGE_WINTER;
                break;
            case SCE_MM_MOUNTAIN_VILLAGE_SPRING:
                sceneMatches = fwSceneId == SCE_MM_MOUNTAIN_VILLAGE_WINTER;
                break;
            case SCE_MM_SOUTHERN_SWAMP_CLEAR:
                sceneMatches = fwSceneId == SCE_MM_SOUTHERN_SWAMP;
                break;
            case SCE_MM_SOUTHERN_SWAMP:
                sceneMatches = fwSceneId == SCE_MM_SOUTHERN_SWAMP_CLEAR;
                break;
            case SCE_MM_TEMPLE_STONE_TOWER:
                sceneMatches = fwSceneId == SCE_MM_TEMPLE_STONE_TOWER_INVERTED;
                yPos *= -1.0f;
                break;
            case SCE_MM_TEMPLE_STONE_TOWER_INVERTED:
                sceneMatches = fwSceneId == SCE_MM_TEMPLE_STONE_TOWER;
                yPos *= -1.0f;
                break;
            }
        }

        s32 shouldDraw = sceneMatches
                && (D_8015BC18 != 0.0f || fwRoomId == play->roomCtx.curRoom.num || fwRoomId == play->roomCtx.prefRoom.num);

        if ((play->csCtx.state == CS_STATE_IDLE) && shouldDraw)
        {
            f32 scale = 0.025f * ratio;

            POLY_XLU_DISP = Gfx_SetupDL(POLY_XLU_DISP, 25); /* SETUPDL_25 */

            Matrix_Translate(gSaveContext.respawn[RESPAWN_MODE_HUMAN].pos.x, yPos, gSaveContext.respawn[RESPAWN_MODE_HUMAN].pos.z, MTXMODE_NEW);
            Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
            ModelViewMult(&play->billboardMtxF, MTXMODE_APPLY);
            Matrix_Push();

            gDPPipeSync(POLY_XLU_DISP++);
            gDPSetPrimColor(POLY_XLU_DISP++, 128, 128, 255, 255, 200, alpha);
            gDPSetEnvColor(POLY_XLU_DISP++, 100, 200, 0, 255);

            Matrix_RotateZ(BINANG_TO_RAD_ALT2((play->gameplayFrames * 1500) & 0xFFFF), MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx),
                      G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
            gSPDisplayList(POLY_XLU_DISP++, 0x04000000 | 0x23210); /* gEffFlash1DL */

            Matrix_Pop();
            Matrix_RotateZ(BINANG_TO_RAD_ALT2(~((play->gameplayFrames * 1200) & 0xFFFF)), MTXMODE_APPLY);

            gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx),
                      G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
            gSPDisplayList(POLY_XLU_DISP++, 0x04000000 | 0x23210); /* gEffFlash1DL */

            Lights_PointNoGlowSetInfo(&D_8015BC00, gSaveContext.respawn[RESPAWN_MODE_HUMAN].pos.x,
                                    yPos,
                                    gSaveContext.respawn[RESPAWN_MODE_HUMAN].pos.z, 255, 255, 255, lightRadius);
        }
        CLOSE_DISPS();
    }
}

void Actor_AfterDrawAll(PlayState* play)
{
    /* Displaced code: */
    if (play->actorCtx.lensMaskSize != 0)
    {
        play->actorCtx.lensActorsDrawn = 1;
        Actor_DrawLensActors(play, play->actorCtx.numLensActors, play->actorCtx.lensActors);
    }

    Actor_DrawFaroresWindPointer(play);
    Multi_DrawWisps(play);
}

typedef struct
{
    u32 offset;
    u16 value;
}
OverlayPatch;

static OverlayPatch adultEponaPatch[] = {
    { 0x865a, 0x0600 },
    { 0x865e, 0x1e2c },
    { 0x867a, 0x0600 },
    { 0x868e, 0x3cec },
    { 0x8706, 0x0600 },
    { 0x870a, 0x3cec },
    { 0x872a, 0x0600 },
    { 0x8736, 0x3cec },
    { 0x879a, 0x0600 },
    { 0x87a2, 0x75f0 },
    { 0x87ae, 0x0600 },
    { 0x87be, 0x75f0 },
    { 0x880e, 0x0600 },
    { 0x8812, 0x6d50 },
    { 0x8832, 0x0600 },
    { 0x883e, 0x6d50 },
    { 0x887e, 0x0600 },
    { 0x8886, 0x6d50 },
    { 0x8c22, 0x0600 },
    { 0x8c2e, 0x3cec },
    { 0x8c3a, 0x0600 },
    { 0x8c4e, 0x3cec },
    { 0x8d52, 0x0600 },
    { 0x8d5e, 0x1e2c },
    { 0x8d6a, 0x0600 },
    { 0x8d7e, 0x1e2c },
    { 0x8dde, 0x0600 },
    { 0x8de6, 0x1e2c },
    { 0x8df2, 0x0600 },
    { 0x8e06, 0x1e2c },
    { 0x8e72, 0x0600 },
    { 0x8e7e, 0x6d50 },
    { 0x8e92, 0x0600 },
    { 0x8e9e, 0x6d50 },
    { 0x8f06, 0x0600 },
    { 0x8f0e, 0x6d50 },
    { 0x8f1a, 0x0600 },
    { 0x8f2e, 0x6d50 },
    { 0xbbde, 0x3c23 },
    { 0xc8ac, 0x1000 },
    { 0xd792, 0x6d50 },
    { 0xd796, 0x5584 },
    { 0xd79a, 0x4dec },
    { 0xd79e, 0x3cec },
    { 0xd7a2, 0x75f0 },
    { 0xd7a6, 0x32b0 },
    { 0xd7aa, 0x1e2c },
    { 0xd7ae, 0x2470 },
    { 0xd7b2, 0x2c38 },
    { 0xd81a, 0x9d74 },
    { 0xdad6, 0x9f80 },
    { 0xdada, 0xa180 },
    { 0xdade, 0xa380 },
};

static void Actor_PatchAdultHorse(void* loadedRam)
{
    if (!comboIsLinkAdult())
    {
        return;
    }

    for (int i = 0; i < ARRAY_COUNT(adultEponaPatch); i++)
    {
        u16* patchPointer = (u16*)(((u8*)loadedRam) + adultEponaPatch[i].offset);
        *patchPointer = adultEponaPatch[i].value;
    }
}

static PlayerAgeProperties sPlayerAgePropertiesAdult = {
    56.0f,
    90.0f,
    1.0f,
    111.0f,
    70.0f,
    79.4f,
    59.0f,
    41.0f,
    19.0f,
    36.0f,
    50.0f,
    56.0f,
    68.0f,
    70.0f,
    18.0f,
    23.0f,
    70.0f,
    { 9, 0x123F, 0x167 },
    {
        { 8, 0x1256, 0x17C },
        { 9, 0x17EA, 0x167 },
        { 8, 0x1256, 0x17C },
        { 9, 0x17EA, 0x167 },
    },
    {
        { 9, 0x17EA, 0x167 },
        { 9, 0x1E0D, 0x17C },
        { 9, 0x17EA, 0x167 },
        { 9, 0x1E0D, 0x17C },
    },
    {
        { 8, 0x1256, 0x17C },
        { 9, 0x17EA, 0x167 },
        { -0x638, 0x1256, 0x17C },
        { -0x637, 0x17EA, 0x167 },
    },
    0,
    0x80,
    22.0f,
    36.0f,
    (PlayerAnimationHeader*)0x0400e300, /* gPlayerAnim_pz_Tbox_open */
    (PlayerAnimationHeader*)0x0400d548, /* gPlayerAnim_link_demo_back_to_past */
    (PlayerAnimationHeader*)0x0400d660, /* gPlayerAnim_link_demo_return_to_past */
    (PlayerAnimationHeader*)0x0400e378, /* gPlayerAnim_pz_climb_startA */
    (PlayerAnimationHeader*)0x0400e380, /* gPlayerAnim_pz_climb_startB */
    {
        (PlayerAnimationHeader*)0x0400e388, /* gPlayerAnim_pz_climb_upL */
        (PlayerAnimationHeader*)0x0400e390, /* gPlayerAnim_pz_climb_upR */
        (PlayerAnimationHeader*)0x0400dab0, /* gPlayerAnim_link_normal_Fclimb_upL */
        (PlayerAnimationHeader*)0x0400dab8, /* gPlayerAnim_link_normal_Fclimb_upR */
    },
    {
        (PlayerAnimationHeader*)0x0400da90, /* gPlayerAnim_link_normal_Fclimb_sideL */
        (PlayerAnimationHeader*)0x0400da98, /* gPlayerAnim_link_normal_Fclimb_sideR */
    },
    {
        (PlayerAnimationHeader*)0x0400e358, /* gPlayerAnim_pz_climb_endAL */
        (PlayerAnimationHeader*)0x04008360, /* gPlayerAnim_pz_climb_endAR */
    },
    {
        (PlayerAnimationHeader*)0x0400e370, /* gPlayerAnim_pz_climb_endBR */
        (PlayerAnimationHeader*)0x0400e368, /* gPlayerAnim_pz_climb_endBL */
    },
};

static void Actor_PatchPlayerFormProperties()
{
    if (comboIsLinkAdult())
    {
        /* Maybe better to move this to when the overlay is loaded? */
        PlayerAgeProperties* playerAgeProperties = (PlayerAgeProperties*) OverlayAddr(0x8085ba38);
        playerAgeProperties[MM_PLAYER_FORM_HUMAN] = sPlayerAgePropertiesAdult;

        u32* playerAnimationHeaders = (u32*)OverlayAddr(0x8085be84);
        playerAnimationHeaders[0x36] = 0x0400e3c8;
        playerAnimationHeaders[0x37] = 0x0400e3c8;
        playerAnimationHeaders[0x38] = 0x0400e3c8;
        playerAnimationHeaders[0x39] = 0x0400e3c8;
        playerAnimationHeaders[0x3a] = 0x0400e3c8;
        playerAnimationHeaders[0x3b] = 0x0400e3c8;

        playerAnimationHeaders[0x42] = 0x0400e3d0;
        playerAnimationHeaders[0x43] = 0x0400e3d0;
        playerAnimationHeaders[0x44] = 0x0400e3d0;
        playerAnimationHeaders[0x45] = 0x0400e3d0;
        playerAnimationHeaders[0x46] = 0x0400e3d0;
        playerAnimationHeaders[0x47] = 0x0400e3d0;
    }
}

ActorProfile* Actor_LoadOverlayCustom(ActorContext* actorCtx, s16 index)
{
    ActorProfile* ActorProfile = Actor_LoadOverlay(actorCtx, index);

    ActorOvl* overlayEntry = &gActorOvl[index];

    if (overlayEntry->count == 0 && overlayEntry->data)
    {
        switch (index)
        {
        case ACTOR_EN_HORSE:
            Actor_PatchAdultHorse(overlayEntry->data);
            break;
        }
    }

    return ActorProfile;
}

PATCH_CALL(0x800bae44, Actor_LoadOverlayCustom);

void KaleidoManager_LoadPlayerOvl(void* ovl)
{
    KaleidoManager_LoadOvl(ovl);
    Actor_PatchPlayerFormProperties();
}

PATCH_CALL(0x801639d4, KaleidoManager_LoadPlayerOvl);
