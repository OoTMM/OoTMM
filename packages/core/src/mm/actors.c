#include <combo.h>

void EnGo_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
void EnDnh_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
void EnShn_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
void EnPm_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b);
void EnAn_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
void EnPst_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
void EnNb_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
void EnAl_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
void EnBjt_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
void EnTab_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);

typedef void (*TextBoxCallback)(Actor*, GameState_Play*, s16);
void EnGo_AfterTextBox(Actor* this, GameState_Play* play, s16 messageId);
void EnTab_AfterTextBox(Actor* this, GameState_Play* play, s16 messageId);

static Actor* sByteCodeActor;

static void Actor_ByteCode_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    void (*func)(Actor*, GameState_Play*, s16, float, float);

    switch (actor->id)
    {
    case 0x138:
        func = EnGo_GiveItem;
        break;
    case 0x168:
        func = EnDnh_GiveItem;
        break;
    case 0x1c5:
        func = EnShn_GiveItem;
        break;
    case 0x1d5:
        func = EnPm_GiveItem;
        break;
    case 0x1f2:
        func = EnPst_GiveItem;
        break;
    case 0x202:
        func = EnAn_GiveItem;
        break;
    case 0x243:
        func = EnNb_GiveItem;
        break;
    case 0x262:
        func = EnAl_GiveItem;
        break;
    case AC_EN_TAB:
        func = EnTab_GiveItem;
        break;
    case 0x27d:
        func = EnBjt_GiveItem;
        break;
    default:
        func = GiveItem;
        break;
    }

    func(actor, play, gi, a, b);
}

PATCH_CALL(0x8010aa34, Actor_ByteCode_GiveItem);

void EnGo_AfterGivingItem(Actor* actor);
void EnDnh_AfterGivingItem(Actor* actor);
void EnPm_AfterGivingItem(Actor* actor);
void EnAn_AfterGivingItem(Actor* actor);
void EnNb_AfterGivingItem(Actor* actor);
void EnTab_AfterGivingItem(Actor* actor);

static int Actor_ByteCode_HasParent(Actor* actor)
{
    int ret;
    void (*func)(Actor*);

    ret = Actor_HasParent(actor);
    if (ret)
    {
        switch (actor->id)
        {
        case 0x138:
            func = EnGo_AfterGivingItem;
            break;
        case 0x168:
            func = EnDnh_AfterGivingItem;
            break;
        case 0x1d5:
            func = EnPm_AfterGivingItem;
            break;
        case 0x202:
            func = EnAn_AfterGivingItem;
            break;
        case AC_EN_TAB:
            func = EnTab_AfterGivingItem;
            break;
        case 0x243:
            func = EnNb_AfterGivingItem;
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

int Actor_RunByteCodeWrapper(Actor* this, GameState_Play* play, void* bytecode, void* unk1, void* unk2)
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

static void Actor_ByteCode_DispatchTextBox(GameState_Play* play, s16 messageId)
{
    Actor* this;
    TextBoxCallback cb;

    this = sByteCodeActor;
    cb = NULL;
    switch (this->id)
    {
    case 0x138:
        cb = EnGo_AfterTextBox;
        break;
    case AC_EN_TAB:
        cb = EnTab_AfterTextBox;
        break;
    }
    if (cb)
    {
        cb(this, play, messageId);
    }
}

static void Actor_ByteCode_PlayerDisplayTextBox_Hook(GameState_Play* play, s16 messageId, void* unk)
{
    PlayerDisplayTextBox(play, messageId, unk);
    Actor_ByteCode_DispatchTextBox(play, messageId);
}

PATCH_CALL(0x8010af04, Actor_ByteCode_PlayerDisplayTextBox_Hook);

static void Actor_ByteCode_DisplayTextBox2_Hook(GameState_Play* play, s16 messageId)
{
    DisplayTextBox2(play, messageId);
    Actor_ByteCode_DispatchTextBox(play, messageId);
}

PATCH_CALL(0x8010af50, Actor_ByteCode_DisplayTextBox2_Hook);

static int hasSoul(s16 gi)
{
    int soulId;

    soulId = gi - GI_MM_SOUL_OCTOROK;
    if (soulId >= 32)
        return gMmSouls2 & (1 << (soulId - 32));
    else
        return gMmSouls1 & (1 << soulId);
}

static int canSpawnSoul(GameState_Play* play, s16 actorId, u16 variable)
{
    switch (actorId)
    {
    case AC_EN_OKUTA:
    case AC_EN_BIGOKUTA:
        return hasSoul(GI_MM_SOUL_OCTOROK);
    case AC_EN_WALLMAS:
        return hasSoul(GI_MM_SOUL_WALLMASTER);
    case AC_EN_DODONGO:
        return hasSoul(GI_MM_SOUL_DODONGO);
    case AC_EN_FIREFLY:
        return hasSoul(GI_MM_SOUL_KEESE);
    case AC_EN_TITE:
        return hasSoul(GI_MM_SOUL_TEKTITE);
    case AC_EN_PEEHAT:
        return hasSoul(GI_MM_SOUL_PEAHAT);
    case AC_EN_DINOFOS:
        return hasSoul(GI_MM_SOUL_LIZALFOS_DINALFOS);
    case AC_EN_ST:
        return hasSoul(GI_MM_SOUL_SKULLTULA);
    case AC_EN_AM:
    case AC_EN_FAMOS:
        return hasSoul(GI_MM_SOUL_ARMOS);
    case AC_EN_DEKUBABA:
    case AC_EN_KAREBABA:
        return hasSoul(GI_MM_SOUL_DEKU_BABA);
    case AC_EN_DEKUNUTS:
        return hasSoul(GI_MM_SOUL_DEKU_SCRUB);
    case AC_EN_BBFALL:
    case AC_EN_BB:
        return hasSoul(GI_MM_SOUL_BUBBLE);
    case AC_EN_VM:
        return hasSoul(GI_MM_SOUL_BEAMOS);
    case AC_EN_RD:
    case AC_EN_TALK_GIBUD:
    case AC_EN_RAILGIBUD:
        return hasSoul(GI_MM_SOUL_REDEAD_GIBDO);
    case AC_EN_SW:
        if (variable & 0x03)
            return 1;
        return hasSoul(GI_MM_SOUL_SKULLWALLTULA);
    case AC_EN_SB:
        return hasSoul(GI_MM_SOUL_SHELL_BLADE);
    case AC_EN_RR:
        return hasSoul(GI_MM_SOUL_LIKE_LIKE);
    case AC_EN_IK:
        return hasSoul(GI_MM_SOUL_IRON_KNUCKLE);
    case AC_EN_FZ:
        return hasSoul(GI_MM_SOUL_FREEZARD);
    case AC_EN_WF:
        return hasSoul(GI_MM_SOUL_WOLFOS);
    case AC_EN_CROW:
        return hasSoul(GI_MM_SOUL_GUAY);
    case AC_EN_TUBO_TRAP:
        return hasSoul(GI_MM_SOUL_FLYING_POT);
    case AC_EN_FLOORMAS:
        return hasSoul(GI_MM_SOUL_FLOORMASTER);
    case AC_EN_SLIME:
        return hasSoul(GI_MM_SOUL_CHUCHU);
    case AC_EN_DRAGON:
        return hasSoul(GI_MM_SOUL_DEEP_PYTHON);
    case AC_EN_PR:
    case AC_EN_PR2:
    case AC_EN_PRZ:
        return hasSoul(GI_MM_SOUL_SKULLFISH);
    case AC_EN_WDHAND:
        return hasSoul(GI_MM_SOUL_DEXIHAND);
    case AC_EN_GRASSHOPPER:
        return hasSoul(GI_MM_SOUL_DRAGONFLY);
    case AC_EN_SNOWMAN:
        return hasSoul(GI_MM_SOUL_EENO);
    case AC_EN_EGOL:
        return hasSoul(GI_MM_SOUL_EYEGORE);
    case AC_EN_PP:
        return hasSoul(GI_MM_SOUL_HIPLOOP);
    case AC_EN_RAT:
        return hasSoul(GI_MM_SOUL_REAL_BOMBCHU);
    case AC_EN_THIEFBIRD:
        return hasSoul(GI_MM_SOUL_TAKKURI);
    case AC_EN_MKK:
        return hasSoul(GI_MM_SOUL_BOE);
    case AC_EN_BAGUO:
        return hasSoul(GI_MM_SOUL_NEJIRON);
    case AC_BOSS_05:
        return hasSoul(GI_MM_SOUL_BIO_BABA);
    case AC_EN_JSO:
    case AC_EN_JSO2:
        return hasSoul(GI_MM_SOUL_GARO);
    case AC_EN_WIZ:
    case AC_EN_WIZ_BROCK:
        return hasSoul(GI_MM_SOUL_WIZZROBE);
    case AC_EN_DEATH:
        return hasSoul(GI_MM_SOUL_GOMESS);
    case AC_EN_PAMETFROG:
    case AC_EN_BIGSLIME:
    case AC_EN_BIGPAMET:
        return hasSoul(GI_MM_SOUL_GEKKO);
    case AC_EN_BAT:
        return hasSoul(GI_MM_SOUL_BAD_BAT);
    case AC_EN_KAME:
        return hasSoul(GI_MM_SOUL_SNAPPER);
    case AC_BOSS_04:
        return hasSoul(GI_MM_SOUL_WART);
    case AC_EN_BSB:
        return hasSoul(GI_MM_SOUL_CAPTAIN_KEETA);
    case AC_EN_KNIGHT:
    case AC_BOSS_06:
        return hasSoul(GI_MM_SOUL_IGOS);
    case AC_BOSS_01:
        return hasSoul(GI_MM_SOUL_ODOLWA);
    case AC_BOSS_HAKUGIN:
        return hasSoul(GI_MM_SOUL_GOHT);
    case AC_BOSS_03:
        return hasSoul(GI_MM_SOUL_GYORG);
    case AC_BOSS_02:
        return hasSoul(GI_MM_SOUL_TWINMOLD);
    default:
        return 1;
    }
}

Actor* comboSpawnActorEx(void* unk, GameState_Play *play, short actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable, int ex1, int ex2, int ex3)
{
    if (!canSpawnSoul(play, actorId, variable))
    {
        g.roomEnemyLackSoul = 1;
        return NULL;
    }
    return SpawnActorEx(unk, play, actorId, x, y, z, rx, ry, rz, variable, ex1, ex2, ex3);
}
