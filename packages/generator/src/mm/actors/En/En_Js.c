#include <combo.h>
#include <combo/item.h>
#include <combo/config.h>
#include <combo/actor.h>

static void EnJs_ItemQuery(ComboItemQuery* q)
{
    bzero(q, sizeof(*q));

    if (!gMmExtraFlags2.maskFierceDeity)
    {
        q->ovType = OV_NPC;
        q->gi = GI_MM_MASK_FIERCE_DEITY;
        q->id = NPC_MM_MASK_FIERCE_DEITY;
    }
    else
    {
        q->ovType = OV_NONE;
        q->gi = GI_MM_RECOVERY_HEART;
    }
}

static void EnJs_AskForFight(PlayState* play, u16 unk, Actor* this)
{
    char* b;
    char* start;

    PlayerDisplayTextBox(play, 0x21fe, this);
    b = play->msgCtx.font.textBuffer.schar;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "So...you'll play?" TEXT_NL TEXT_NL TEXT_COLOR_GREEN TEXT_CHOICE2 "Yes" TEXT_NL "No" TEXT_END);
    comboTextAutoLineBreaks(start);
}

PATCH_CALL(0x8096a25c, EnJs_AskForFight);

int EnJs_HasGivenItem(Actor* this)
{
    if (Actor_HasParentZ(this))
    {
        gMmExtraFlags2.maskFierceDeity = 1;
        return 1;
    }
    return 0;
}

PATCH_CALL(0x8096a2fc, EnJs_HasGivenItem);

void EnJs_GiveItem(Actor* this, PlayState* play, s16 gi, float a, float b)
{
    ComboItemQuery q;

    EnJs_ItemQuery(&q);
    comboGiveItem(this, play, &q, a, b);
}

PATCH_CALL(0x8096a370, EnJs_GiveItem);

static void EnJs_DisplayHint(PlayState* play, s16 messageId)
{
    ComboItemQuery q;
    char* b;
    char* start;

    /* Hint */
    EnJs_ItemQuery(&q);
    DisplayTextBox2(play, messageId);
    b = play->msgCtx.font.textBuffer.schar;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "You have only weak masks..." TEXT_NL "Having better masks would give you ");
    comboTextAppendItemNameQueryEx(&b, &q, TF_PREPOS | TF_PROGRESSIVE, gComboConfig.staticHintsImportance[9]);
    comboTextAppendStr(&b, "..." TEXT_BB "So...you'll play?" TEXT_NL TEXT_NL TEXT_COLOR_GREEN TEXT_CHOICE2 "Yes" TEXT_NL "No" TEXT_END);
    comboTextAutoLineBreaks(start);
}

PATCH_CALL(0x8096a4c8, EnJs_DisplayHint);

static void EnJs_DisplayWeak(Actor* this, PlayState* play)
{
    char* b;

    DisplayTextBox2(play, 0x21ff);
    b = play->msgCtx.font.textBuffer.schar;
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b, "You're too weak..." TEXT_SIGNAL TEXT_END);
}

void EnJs_TryStartFight(Actor* this)
{
    int canFight;
    void (*EnJs_SetFreeCamera)(Actor*, int);

    canFight = 0;
    if (Config_Flag(CFG_GOAL_TRIFORCE) || Config_Flag(CFG_GOAL_TRIFORCE3))
    {
        if (gOotExtraFlags.triforceWin)
            canFight = 1;
    }
    else if (!Config_Flag(CFG_MM_MAJORA_CHILD_CUSTOM) || SpecialConds_Eval(SPECIAL_MAJORA))
    {
        canFight = 1;
    }

    if (canFight)
    {
        /* Start the fight*/
        DisplayTextBox2(gPlay, 0x2200);
        EnJs_SetFreeCamera = actorAddr(0xbf, 0x809696ec);
        EnJs_SetFreeCamera(this, 0);
    }
    else
    {
        /* Too weak */
        EnJs_DisplayWeak(this, gPlay);
    }
}

PATCH_CALL(0x8096a534, EnJs_TryStartFight);

#define ENJS_MOON_MASKS_GIVEN ((u8*)((u8*)&gSaveContext + 0x48ca))
#define ENJS_INV_CONTENT(slot) gSave.info.inventory.items[(slot)]

#define ACTOR_TEXT_ID(actor) (*(u16*)((u8*)(actor) + 0x116))

#define ENJS_GET_TYPE(thisx) ((thisx)->params & 0xf)

typedef struct EnJs EnJs;
typedef void (*EnJsActionFunc)(EnJs*, PlayState*);

struct EnJs {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
};

#define ENJS_ACTION_FUNC(this) (*(EnJsActionFunc*)((u8*)(this) + 0x2c4))
#define ENJS_ACTION(addr) ((EnJsActionFunc)actorAddr(0xbf, (addr)))

static u32 EnJs_GetSharedSlotFlags(u8 slot)
{
    switch (slot)
    {
    case ITS_MM_MASK_STONE:
        return gMmExtraItems.stoneGerudoSkull;

    case ITS_MM_MASK_GIBDO:
        return gMmExtraItems.gibdoSpooky;

    default:
        return 0;
    }
}

static void EnJs_SetSharedSlotFlags(u8 slot, u32 flags)
{
    switch (slot)
    {
    case ITS_MM_MASK_STONE:
        gMmExtraItems.stoneGerudoSkull = flags;
        break;

    case ITS_MM_MASK_GIBDO:
        gMmExtraItems.gibdoSpooky = flags;
        break;
    }
}

static void EnJs_SelectReplacementForSharedVanillaMask(u8 slot)
{
    u8 replacement;
    u32 flags;

    replacement = ITEM_NONE;
    flags = EnJs_GetSharedSlotFlags(slot);

    switch (slot)
    {
    case ITS_MM_MASK_STONE:
        if (Config_Flag(CFG_MM_MASK_GERUDO) && (flags & (1 << 1)))
            replacement = ITEM_MM_MASK_GERUDO;
        else if (Config_Flag(CFG_MM_MASK_SKULL) && (flags & (1 << 2)))
            replacement = ITEM_MM_MASK_SKULL;
        break;

    case ITS_MM_MASK_GIBDO:
        if (Config_Flag(CFG_MM_MASK_SPOOKY) && (flags & (1 << 1)))
            replacement = ITEM_MM_MASK_SPOOKY;
        break;
    }

    ENJS_INV_CONTENT(slot) = replacement;
    reloadSlotMm(gPlay, slot);
}

static int EnJs_RemoveSharedVanillaMask(u8 item)
{
    u8 slot;
    u32 flags;

    switch (item)
    {
    case ITEM_MM_MASK_STONE:
        slot = ITS_MM_MASK_STONE;
        break;

    case ITEM_MM_MASK_GIBDO:
        slot = ITS_MM_MASK_GIBDO;
        break;

    default:
        return 0;
    }
    flags = EnJs_GetSharedSlotFlags(slot);
    flags &= ~(1 << 0);
    EnJs_SetSharedSlotFlags(slot, flags);
    if (ENJS_INV_CONTENT(slot) == item)
        EnJs_SelectReplacementForSharedVanillaMask(slot);

    return 1;
}

static int EnJs_IsTransformationMaskAction(PlayerItemAction itemAction)
{
    return itemAction >= PLAYER_IA_MASK_TRANSFORMATION_MIN && itemAction <= PLAYER_IA_MASK_MAX;
}

s32 EnJs_CountGivenMasks(s32 childType)
{
    u8* masksGivenOnMoon;
    s32 count;
    s32 i;
    u8 bit;

    if (childType < 0 || childType >= 9)
        return 0;

    masksGivenOnMoon = ENJS_MOON_MASKS_GIVEN + childType * 3;
    count = 0;
    for (i = 0; i < 20; i++)
    {
        bit = 1 << (i & 7);

        if (masksGivenOnMoon[i >> 3] & bit)
            count++;
    }

    return count;
}

s32 EnJs_GetRemainingMasks_Custom(void)
{
    s32 count;

    count = 0;

    if (ENJS_INV_CONTENT(ITS_MM_MASK_TRUTH) == ITEM_MM_MASK_TRUTH) count++;
    if (ENJS_INV_CONTENT(ITS_MM_MASK_KAFEI) == ITEM_MM_MASK_KAFEI) count++;
    if (ENJS_INV_CONTENT(ITS_MM_MASK_ALL_NIGHT) == ITEM_MM_MASK_ALL_NIGHT) count++;
    if (ENJS_INV_CONTENT(ITS_MM_MASK_BUNNY) == ITEM_MM_MASK_BUNNY) count++;
    if (ENJS_INV_CONTENT(ITS_MM_MASK_KEATON) == ITEM_MM_MASK_KEATON) count++;
    if (ENJS_INV_CONTENT(ITS_MM_MASK_GARO) == ITEM_MM_MASK_GARO) count++;
    if (ENJS_INV_CONTENT(ITS_MM_MASK_ROMANI) == ITEM_MM_MASK_ROMANI) count++;
    if (ENJS_INV_CONTENT(ITS_MM_MASK_TROUPE_LEADER) == ITEM_MM_MASK_TROUPE_LEADER) count++;
    if (ENJS_INV_CONTENT(ITS_MM_MASK_POSTMAN) == ITEM_MM_MASK_POSTMAN) count++;
    if (ENJS_INV_CONTENT(ITS_MM_MASK_COUPLE) == ITEM_MM_MASK_COUPLE) count++;
    if (ENJS_INV_CONTENT(ITS_MM_MASK_GREAT_FAIRY) == ITEM_MM_MASK_GREAT_FAIRY) count++;

    if ((ENJS_INV_CONTENT(ITS_MM_MASK_GIBDO) == ITEM_MM_MASK_GIBDO) ||
        (EnJs_GetSharedSlotFlags(ITS_MM_MASK_GIBDO) & (1 << 0)))
        count++;

    if (ENJS_INV_CONTENT(ITS_MM_MASK_DON_GERO) == ITEM_MM_MASK_DON_GERO) count++;
    if (ENJS_INV_CONTENT(ITS_MM_MASK_KAMARO) == ITEM_MM_MASK_KAMARO) count++;
    if (ENJS_INV_CONTENT(ITS_MM_MASK_CAPTAIN) == ITEM_MM_MASK_CAPTAIN) count++;

    if ((ENJS_INV_CONTENT(ITS_MM_MASK_STONE) == ITEM_MM_MASK_STONE) ||
        (EnJs_GetSharedSlotFlags(ITS_MM_MASK_STONE) & (1 << 0)))
        count++;

    if (ENJS_INV_CONTENT(ITS_MM_MASK_BREMEN) == ITEM_MM_MASK_BREMEN) count++;
    if (ENJS_INV_CONTENT(ITS_MM_MASK_BLAST) == ITEM_MM_MASK_BLAST) count++;
    if (ENJS_INV_CONTENT(ITS_MM_MASK_SCENTS) == ITEM_MM_MASK_SCENTS) count++;
    if (ENJS_INV_CONTENT(ITS_MM_MASK_GIANT) == ITEM_MM_MASK_GIANT) count++;

    count -= EnJs_CountGivenMasks(0);
    return count;
}

s32 EnJs_TakeMask_Custom(s32 itemAction, s32 childType)
{
    u8* masksGivenOnMoon;
    s32 maskIndex;
    s32 byteIndex;
    u8 bit;
    u8 item;

    if (childType < 0 || childType >= 9)
        return 0;
    if (itemAction < PLAYER_IA_MASK_MIN || itemAction >= PLAYER_IA_MASK_TRANSFORMATION_MIN)
        return 0;

    maskIndex = itemAction - PLAYER_IA_MASK_MIN;

    if (maskIndex < 0 || maskIndex >= 20)
        return 0;

    byteIndex = maskIndex >> 3;
    bit = 1 << (maskIndex & 7);

    masksGivenOnMoon = ENJS_MOON_MASKS_GIVEN + childType * 3;
    masksGivenOnMoon[byteIndex] |= bit;
    ENJS_MOON_MASKS_GIVEN[byteIndex] |= bit;

    item = itemAction - 4;
    if (!EnJs_RemoveSharedVanillaMask(item))
        Inventory_DeleteItem(item, itemAction - 4);

    return 1;
}

s32 EnJs_HasGivenEnoughMasks(s32 childType)
{
    switch (childType)
    {
    case 1:
    case 2:
    case 3:
    case 4:
        return EnJs_CountGivenMasks(childType) >= childType;

    case 5:
    case 6:
    case 7:
    case 8:
        return EnJs_CountGivenMasks(childType) >= childType - 4;

    default:
        return false;
    }
}

s32 EnJs_CanEventuallyGiveEnoughMasks(s32 childType)
{
    s32 required;
    s32 available;

    switch (childType)
    {
    case 1:
    case 2:
    case 3:
    case 4:
        required = childType;
        break;

    case 5:
    case 6:
    case 7:
    case 8:
        required = childType - 4;
        break;

    default:
        return false;
    }

    available = EnJs_CountGivenMasks(childType) + EnJs_GetRemainingMasks_Custom();
    return available >= required;
}

void EnJs_ClearMoonChildMasks_Custom(s32 childType)
{
    u8* masksGivenOnMoon;
    u8 childBytes[3];

    if (childType < 0 || childType >= 9)
        return;

    masksGivenOnMoon = ENJS_MOON_MASKS_GIVEN + childType * 3;

    childBytes[0] = masksGivenOnMoon[0];
    childBytes[1] = masksGivenOnMoon[1];
    childBytes[2] = masksGivenOnMoon[2];

    ENJS_MOON_MASKS_GIVEN[0] &= ~childBytes[0];
    ENJS_MOON_MASKS_GIVEN[1] &= ~childBytes[1];
    ENJS_MOON_MASKS_GIVEN[2] &= ~childBytes[2];

    masksGivenOnMoon[0] = 0;
    masksGivenOnMoon[1] = 0;
    masksGivenOnMoon[2] = 0;
}

static int EnJs_IsMovingTradeResultText(u16 textId)
{
    switch (textId)
    {
    case 0x2210:
    case 0x2211:
    case 0x2212:
    case 0x2213:
        return 1;

    default:
        return 0;
    }
}

static int EnJs_IsSittingTradeResultText(u16 textId)
{
    switch (textId)
    {
    case 0x221d:
    case 0x2220:
    case 0x2221:
    case 0x2222:
        return 1;

    default:
        return 0;
    }
}

void EnJs_MovingChildAfterMaskSelect_Custom(EnJs* this, PlayState* play)
{
    Player* player;
    EnJsActionFunc vanillaAction;

    player = GET_PLAYER(play);

    vanillaAction = ENJS_ACTION(0x80969898);
    vanillaAction(this, play);

    if (Message_GetState(&play->msgCtx) == TEXT_STATE_EVENT &&
        EnJs_IsMovingTradeResultText(play->msgCtx.currentTextId))
    {
        player->exchangeItemAction = PLAYER_IA_NONE;
    }
}

void EnJs_SittingChildAfterMaskSelect_Custom(EnJs* this, PlayState* play)
{
    Player* player;
    EnJsActionFunc vanillaAction;

    player = GET_PLAYER(play);

    vanillaAction = ENJS_ACTION(0x80969da4);
    vanillaAction(this, play);

    if (Message_GetState(&play->msgCtx) == TEXT_STATE_EVENT &&
        EnJs_IsSittingTradeResultText(play->msgCtx.currentTextId))
    {
        player->exchangeItemAction = PLAYER_IA_NONE;
    }
}

void EnJs_SelectMaskMovingChild_Custom(EnJs* this, PlayState* play)
{
    PlayerItemAction itemAction;
    Player* player;

    player = GET_PLAYER(play);

    SkelAnime_Update(&this->skelAnime);
    Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 6, 0x1838, 0x64);
    this->actor.shape.rot.y = this->actor.world.rot.y;

    if (Message_GetState(&play->msgCtx) == TEXT_STATE_PAUSE_MENU)
    {
        itemAction = func_80123810(play);

        if (itemAction != PLAYER_IA_NONE)
            ENJS_ACTION_FUNC(this) = EnJs_MovingChildAfterMaskSelect_Custom;

        if (itemAction > PLAYER_IA_NONE)
        {
            Message_CloseTextbox(play);

            if (itemAction >= PLAYER_IA_MASK_MIN && itemAction < PLAYER_IA_MASK_TRANSFORMATION_MIN)
            {
                if (EnJs_TakeMask_Custom(itemAction, ENJS_GET_TYPE(&this->actor)))
                {
                    Inventory_UnequipItem(itemAction - 4);

                    if (!EnJs_HasGivenEnoughMasks(ENJS_GET_TYPE(&this->actor)))
                        ACTOR_TEXT_ID(&player->actor) = 0x2212;
                    else
                        ACTOR_TEXT_ID(&player->actor) = 0x2213;
                }
                else
                {
                    ACTOR_TEXT_ID(&player->actor) = 0x2210;
                }
            }
            else if (EnJs_IsTransformationMaskAction(itemAction))
            {
                ACTOR_TEXT_ID(&player->actor) = 0x2211;
            }
            else
            {
                ACTOR_TEXT_ID(&player->actor) = 0x2210;
            }
        }

        if (itemAction <= PLAYER_IA_MINUS1)
            Message_ContinueTextbox(play, 0x2216);
    }
}

void EnJs_SelectMaskSittingChild_Custom(EnJs* this, PlayState* play)
{
    PlayerItemAction itemAction;
    Player* player;

    player = GET_PLAYER(play);

    SkelAnime_Update(&this->skelAnime);
    Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 6, 0x1838, 0x64);
    this->actor.shape.rot.y = this->actor.world.rot.y;

    if (Message_GetState(&play->msgCtx) == TEXT_STATE_PAUSE_MENU)
    {
        itemAction = func_80123810(play);

        if (itemAction != PLAYER_IA_NONE)
            ENJS_ACTION_FUNC(this) = EnJs_SittingChildAfterMaskSelect_Custom;

        if (itemAction > PLAYER_IA_NONE)
        {
            Message_CloseTextbox(play);

            if (itemAction >= PLAYER_IA_MASK_MIN && itemAction < PLAYER_IA_MASK_TRANSFORMATION_MIN)
            {
                if (EnJs_TakeMask_Custom(itemAction, ENJS_GET_TYPE(&this->actor)))
                {
                    Inventory_UnequipItem(itemAction - 4);

                    if (!EnJs_HasGivenEnoughMasks(ENJS_GET_TYPE(&this->actor)))
                        ACTOR_TEXT_ID(&player->actor) = 0x2221;
                    else
                        ACTOR_TEXT_ID(&player->actor) = 0x2222;
                }
                else
                {
                    ACTOR_TEXT_ID(&player->actor) = 0x221d;
                }
            }
            else if (EnJs_IsTransformationMaskAction(itemAction))
            {
                ACTOR_TEXT_ID(&player->actor) = 0x2220;
            }
            else
            {
                ACTOR_TEXT_ID(&player->actor) = 0x221d;
            }
        }

        if (itemAction <= PLAYER_IA_MINUS1)
            Message_ContinueTextbox(play, 0x221e);
    }
}

PATCH_FUNC(0x80968e38, EnJs_CountGivenMasks);
PATCH_FUNC(0x80968f48, EnJs_GetRemainingMasks_Custom);
PATCH_FUNC(0x809691b8, EnJs_TakeMask_Custom);
PATCH_FUNC(0x809692a8, EnJs_HasGivenEnoughMasks);
PATCH_FUNC(0x8096933c, EnJs_CanEventuallyGiveEnoughMasks);
PATCH_FUNC(0x80969400, EnJs_ClearMoonChildMasks_Custom);
PATCH_FUNC(0x80969748, EnJs_SelectMaskMovingChild_Custom);
PATCH_FUNC(0x80969c54, EnJs_SelectMaskSittingChild_Custom);