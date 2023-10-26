#include <combo.h>
#include <combo/souls.h>

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

static int canSpawnSoul(GameState_Play* play, s16 actorId, u16 variable)
{
    switch (actorId)
    {
    case AC_EN_OKUTA:
    case AC_EN_BIGOKUTA:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_OCTOROK);
    case AC_EN_WALLMAS:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_WALLMASTER);
    case AC_EN_DODONGO:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_DODONGO);
    case AC_EN_FIREFLY:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_KEESE);
    case AC_EN_TITE:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_TEKTITE);
    case AC_EN_PEEHAT:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_PEAHAT);
    case AC_EN_DINOFOS:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_LIZALFOS_DINALFOS);
    case AC_EN_ST:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_SKULLTULA);
    case AC_EN_AM:
    case AC_EN_FAMOS:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_ARMOS);
    case AC_EN_DEKUBABA:
    case AC_EN_KAREBABA:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_DEKU_BABA);
    case AC_EN_DEKUNUTS:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_DEKU_SCRUB);
    case AC_EN_BBFALL:
    case AC_EN_BB:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_BUBBLE);
    case AC_EN_VM:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_BEAMOS);
    case AC_EN_RD:
    case AC_EN_TALK_GIBUD:
    case AC_EN_RAILGIBUD:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_REDEAD_GIBDO);
    case AC_EN_SW:
        if (variable & 0x03)
            return 1;
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_SKULLWALLTULA);
    case AC_EN_SB:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_SHELL_BLADE);
    case AC_EN_RR:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_LIKE_LIKE);
    case AC_EN_IK:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_IRON_KNUCKLE);
    case AC_EN_FZ:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_FREEZARD);
    case AC_EN_WF:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_WOLFOS);
    case AC_EN_CROW:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_GUAY);
    case AC_EN_TUBO_TRAP:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_FLYING_POT) ? 1 : -1;
    case AC_EN_FLOORMAS:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_FLOORMASTER);
    case AC_EN_SLIME:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_CHUCHU);
    case AC_EN_DRAGON:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_DEEP_PYTHON);
    case AC_EN_PR:
    case AC_EN_PR2:
    case AC_EN_PRZ:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_SKULLFISH);
    case AC_EN_WDHAND:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_DEXIHAND);
    case AC_EN_GRASSHOPPER:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_DRAGONFLY);
    case AC_EN_SNOWMAN:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_EENO);
    case AC_EN_EGOL:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_EYEGORE);
    case AC_EN_PP:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_HIPLOOP);
    case AC_EN_RAT:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_REAL_BOMBCHU);
    case AC_EN_THIEFBIRD:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_TAKKURI);
    case AC_EN_MKK:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_BOE);
    case AC_EN_BAGUO:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_NEJIRON);
    case AC_BOSS_05:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_BIO_BABA);
    case AC_EN_JSO:
    case AC_EN_JSO2:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_GARO);
    case AC_EN_WIZ:
    case AC_EN_WIZ_BROCK:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_WIZZROBE);
    case AC_EN_DEATH:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_GOMESS);
    case AC_EN_PAMETFROG:
    case AC_EN_BIGSLIME:
    case AC_EN_BIGPAMET:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_GEKKO);
    case AC_EN_BAT:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_BAD_BAT);
    case AC_EN_KAME:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_SNAPPER);
    case AC_BOSS_04:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_WART);
    case AC_EN_BSB:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_CAPTAIN_KEETA);
    case AC_BOSS_01:
        return comboHasSoulMm(GI_MM_SOUL_BOSS_ODOLWA);
    case AC_BOSS_HAKUGIN:
        return comboHasSoulMm(GI_MM_SOUL_BOSS_GOHT);
    case AC_BOSS_03:
        return comboHasSoulMm(GI_MM_SOUL_BOSS_GYORG);
    case AC_BOSS_02:
        return comboHasSoulMm(GI_MM_SOUL_BOSS_TWINMOLD);
    case AC_EN_KNIGHT:
    case AC_BOSS_06:
        return comboHasSoulMm(GI_MM_SOUL_BOSS_IGOS);
    case AC_EN_NEO_REEBA:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_LEEVER);
    case AC_EN_SKB:
        if ((play->sceneId == SCE_MM_IKANA_GRAVEYARD) && (play->roomCtx.curRoom.id == 0x01)) // Upper graveyard
            return 1;
        /* Fallthrough */
    case AC_EN_HINT_SKB:
    case AC_EN_RAIL_SKB:
        return comboHasSoulMm(GI_MM_SOUL_ENEMY_STALCHILD);
    default:
        return 1;
    }
}

static int canSpawnActor(GameState_Play* play, s16 actorId, u16 variable)
{
    
    switch (actorId)
    {
    case AC_EN_SYATEKI_OKUTA:
        if (play->sceneId == SCE_MM_SHOOTING_GALLERY && gSave.day > 3)
            return 0;
        
    default:
        return 1;
    }
}

Actor* comboSpawnActorEx(void* unk, GameState_Play *play, short actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable, int ex1, int ex2, int ex3)
{
    int ret;

    if (!canSpawnActor(play, actorId, variable))
        return NULL;

    ret = canSpawnSoul(play, actorId, variable);
    if (ret <= 0)
    {
        if (ret == 0)
            g.roomEnemyLackSoul = 1;
        return NULL;
    }
    return SpawnActorEx(unk, play, actorId, x, y, z, rx, ry, rz, variable, ex1, ex2, ex3);
}
