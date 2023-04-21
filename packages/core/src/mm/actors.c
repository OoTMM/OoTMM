#include <combo.h>

int EnGo_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
int EnDnh_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
int EnShn_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
int EnPm_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b);
int EnAn_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
int EnPst_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
int EnNb_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
int EnAl_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);
int EnBjt_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b);

typedef void (*TextBoxCallback)(Actor*, GameState_Play*, s16);
void EnGo_AfterTextBox(Actor* this, GameState_Play* play, s16 messageId);

static Actor* sByteCodeActor;

static int Actor_ByteCode_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    switch (actor->id)
    {
    case 0x138:
        return EnGo_GiveItem(actor, play, gi, a, b);
    case 0x168:
        return EnDnh_GiveItem(actor, play, gi, a, b);
    case 0x1c5:
        return EnShn_GiveItem(actor, play, gi, a, b);
    case 0x1d5:
        return EnPm_GiveItem(actor, play, gi, a, b);
    case 0x1f2:
        return EnPst_GiveItem(actor, play, gi, a, b);
    case 0x202:
        return EnAn_GiveItem(actor, play, gi, a, b);
    case 0x243:
        return EnNb_GiveItem(actor, play, gi, a, b);
    case 0x262:
        return EnAl_GiveItem(actor, play, gi, a, b);
    case 0x27d:
        return EnBjt_GiveItem(actor, play, gi, a, b);
    default:
        return GiveItem(actor, play, gi, a, b);
    }
}

PATCH_CALL(0x8010aa34, Actor_ByteCode_GiveItem);

void EnGo_AfterGivingItem(Actor* actor);
void EnDnh_AfterGivingItem(Actor* actor);
void EnPm_AfterGivingItem(Actor* actor);
void EnAn_AfterGivingItem(Actor* actor);
void EnNb_AfterGivingItem(Actor* actor);

static int Actor_ByteCode_HasParent(Actor* actor)
{
    int ret;

    ret = Actor_HasParent(actor);
    if (ret)
    {
        switch (actor->id)
        {
        case 0x138:
            EnGo_AfterGivingItem(actor);
            break;
        case 0x168:
            EnDnh_AfterGivingItem(actor);
            break;
        case 0x1d5:
            EnPm_AfterGivingItem(actor);
            break;
        case 0x202:
            EnAn_AfterGivingItem(actor);
            break;
        case 0x243:
            EnNb_AfterGivingItem(actor);
            break;
        }
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
