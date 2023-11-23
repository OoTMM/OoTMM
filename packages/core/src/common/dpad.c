#include <combo.h>
#include <combo/dma.h>

#define DPAD_DOWN   0
#define DPAD_UP     1
#define DPAD_LEFT   2
#define DPAD_RIGHT  3

static void* sDpadIconBuffer;
static u8 sDpadItems[] = { ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };
static u8 sDpadItemsOld[] = { ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };

static const int kDpadPosX = 275;
static const int kDpadPosY = 75;

static const int kDpadOffX[] = { 0, 0, -1, 1 };
static const int kDpadOffY[] = { 1, -1, 0, 0 };

static float kDpadItemScale = 0.4f;

static int canShowDpad(void)
{
    if (gSaveContext.gameMode)
        return 0;
    return 1;
}

static int canUseDpad(GameState_Play* play)
{
    Actor_Player* link;

    link = GET_LINK(play);
    if (!canShowDpad())
        return 0;
    if (link->state & (PLAYER_ACTOR_STATE_CLIMB | PLAYER_ACTOR_STATE_CLIMB2 | PLAYER_ACTOR_STATE_EPONA | PLAYER_ACTOR_STATE_USE_ITEM | PLAYER_ACTOR_STATE_CUTSCENE_FROZEN | PLAYER_ACTOR_STATE_GET_ITEM | PLAYER_ACTOR_STATE_DEATH | PLAYER_ACTOR_STATE_TRANSITION | PLAYER_ACTOR_STATE_TRANSFORM))
        return 0;
    return 1;
}

static int canUseDpadItem(GameState_Play* play, s16 itemId, int flags)
{
    Actor_Player* link;
    int isEquip;

    link = GET_LINK(play);
    isEquip = 0;

    /* No item == no use */
    if (itemId == ITEM_NONE)
        return 0;

    /* Boots */
#if defined(GAME_OOT)
    if (itemId == ITEM_OOT_BOOTS_IRON || itemId == ITEM_OOT_BOOTS_HOVER)
        isEquip = 1;
    if ((itemId == ITEM_OOT_OCARINA_FAIRY || itemId == ITEM_OOT_OCARINA_TIME) && play->interfaceCtx.restrictions.ocarina)
        return 0;
#endif

    if (isEquip)
    {
        return !!(flags & DPF_EQUIP);
    }

    if (!(flags & DPF_ITEMS))
        return 0;

    /* Giant mask can't use any item */
#if defined(GAME_MM)
    if (gSave.equippedMask == PLAYER_MASK_GIANT)
        return 0;
#endif

    /* Underwater - disable everything except zora mask */
    if (link->state & PLAYER_ACTOR_STATE_WATER)
    {
#if defined(GAME_MM)
        if (itemId != ITEM_MM_MASK_ZORA && !(gSave.playerForm == MM_PLAYER_FORM_ZORA && (itemId == ITEM_MM_OCARINA_FAIRY || itemId == ITEM_MM_OCARINA_OF_TIME)))
#endif
            return 0;
    }

    return 1;
}

static void reloadIcons(GameState_Play* play)
{
    if (!sDpadIconBuffer)
    {
        sDpadIconBuffer = malloc(32 * 32 * 4 * 4);
    }

    for (int i = 0; i < 4; ++i)
    {
        if (sDpadItems[i] != sDpadItemsOld[i] && sDpadItems[i] != ITEM_NONE)
        {
#if defined(GAME_OOT)
            DMARomToRam((gDmaData[8].pstart + 0x1000 * sDpadItems[i]) | PI_DOM1_ADDR2, sDpadIconBuffer + (i * 32 * 32 * 4), 32 * 32 * 4);
#else
            LoadIcon(0xa36c10, sDpadItems[i], sDpadIconBuffer + (i * 32 * 32 * 4), 0x1000);
#endif
            sDpadItemsOld[i] = sDpadItems[i];
        }
    }
}

void comboDpadDraw(GameState_Play* play)
{
    s16 itemId;
    u8 alpha;
    float x;
    float y;

    if (!canShowDpad())
        return;

    reloadIcons(play);
    alpha = (u8)play->interfaceCtx.alpha.health;

    /* Init */
    OPEN_DISPS(play->gs.gfx);
    gDPPipeSync(OVERLAY_DISP++);
    gSPSegment(OVERLAY_DISP++, 0x06, gCustomKeep);
    gSPSegment(OVERLAY_DISP++, 0x07, sDpadIconBuffer);
    gDPSetPrimColor(OVERLAY_DISP++, 0, 0x80, 0xff, 0xff, 0xff, alpha);

    /* Draw */
    comboDrawInit2D(&OVERLAY_DISP);
    comboDrawBlit2D(&OVERLAY_DISP, 0x06000000, 32, 32, kDpadPosX, kDpadPosY, 0.5f);

    for (int i = 0; i < 4; ++i)
    {
        itemId = sDpadItems[i];
        if (itemId != ITEM_NONE)
        {
            if (canUseDpadItem(play, itemId, DPF_ITEMS | DPF_EQUIP))
            {
                gDPSetPrimColor(OVERLAY_DISP++, 0, 0x80, 0xff, 0xff, 0xff, alpha);
            }
            else
            {
                gDPSetPrimColor(OVERLAY_DISP++, 0, 0x80, 0x80, 0x80, 0x80, alpha * 0.5f);
            }
            x = kDpadPosX + kDpadOffX[i] * 32 * kDpadItemScale + 1.5f;
            y = kDpadPosY + kDpadOffY[i] * 32 * kDpadItemScale + 1;
            comboDrawBlit2D(&OVERLAY_DISP, 0x07000000 | (i * 32 * 32 * 4), 32, 32, x, y, kDpadItemScale);
        }
    }
    CLOSE_DISPS();
}

#if defined(GAME_OOT)
static void toggleBoots(GameState_Play* play, s16 itemId)
{
    u16 targetBoots;

    targetBoots = (itemId == ITEM_OOT_BOOTS_HOVER) ? 3 : 2;
    if (gSave.equips.equipment.boots == targetBoots)
        gSave.equips.equipment.boots = 1;
    else
        gSave.equips.equipment.boots = targetBoots;
    UpdateEquipment(play, GET_LINK(play));
    PlaySound(0x835);
}
#endif

#if defined(GAME_OOT)
static void dpadUseItem(GameState_Play* play, int index, int flags)
{
    s16 itemId;
    void (*Player_UseItem)(GameState_Play* play, Actor_Player* link, s16 itemId);

    itemId = sDpadItems[index];
    if (!canUseDpadItem(play, itemId, flags))
        return;
    if (itemId == ITEM_OOT_BOOTS_HOVER || itemId == ITEM_OOT_BOOTS_IRON)
    {
        toggleBoots(play, itemId);
    }
    else
    {
        if (itemId == ITEM_OOT_WEIRD_EGG)
        {
            gComboTriggersData.events.weirdEgg = 1;
            return;
        }
        if (itemId == ITEM_OOT_POCKET_EGG)
        {
            gComboTriggersData.events.pocketEgg = 1;
            return;
        }
        Player_UseItem = OverlayAddr(0x80834000);
        Player_UseItem(play, GET_LINK(play), itemId);
    }
}
#endif

#if defined(GAME_MM)
static void dpadUseItem(GameState_Play* play, int index, int flags)
{
    s16 itemId;
    void (*Player_UseItem)(GameState_Play* play, Actor_Player* link, s16 itemId);

    itemId = sDpadItems[index];
    if (!canUseDpadItem(play, itemId, flags))
        return;
    Player_UseItem = OverlayAddr(0x80831990);
    Player_UseItem(play, GET_LINK(play), itemId);
}
#endif

#if defined(GAME_OOT)
void comboDpadUpdate(GameState_Play* play)
{
    /* Update the items */
    sDpadItems[DPAD_DOWN] = gSave.inventory.items[ITS_OOT_OCARINA];
    sDpadItems[DPAD_LEFT] = (gSave.inventory.equipment.boots & EQ_OOT_BOOTS_IRON) ? ITEM_OOT_BOOTS_IRON : ITEM_NONE;
    sDpadItems[DPAD_RIGHT] = (gSave.inventory.equipment.boots & EQ_OOT_BOOTS_HOVER) ? ITEM_OOT_BOOTS_HOVER : ITEM_NONE;
    sDpadItems[DPAD_UP] = gSave.inventory.items[ITS_OOT_TRADE_ADULT];
    if (gSave.age == AGE_CHILD) 
    {
        sDpadItems[DPAD_UP] = gSave.inventory.items[ITS_OOT_TRADE_CHILD];
        if (!comboConfig(CFG_OOT_AGELESS_BOOTS))
        {
            sDpadItems[DPAD_LEFT] = ITEM_NONE;
            sDpadItems[DPAD_RIGHT] = ITEM_NONE;
        }
    }
    else if(comboConfig(CFG_OOT_AGELESS_CHILD_TRADE))
        sDpadItems[DPAD_UP] = gSave.inventory.items[ITS_OOT_TRADE_CHILD];
}
#endif

#if defined(GAME_MM)
void comboDpadUpdate(GameState_Play* play)
{
    /* Update the items */
    sDpadItems[DPAD_DOWN] = gSave.inventory.items[ITS_MM_OCARINA];
    sDpadItems[DPAD_UP] = gSave.inventory.items[ITS_MM_MASK_DEKU];
    sDpadItems[DPAD_LEFT] = gSave.inventory.items[ITS_MM_MASK_GORON];
    sDpadItems[DPAD_RIGHT] = gSave.inventory.items[ITS_MM_MASK_ZORA];
}
#endif

int comboDpadUse(GameState_Play* play, int flags)
{
    u32 buttons;
    if (!canUseDpad(play))
        return 0;

    /* Detect button press */
    buttons = play->gs.input[0].pressed.buttons;
    if (buttons & U_JPAD)
    {
        dpadUseItem(play, DPAD_UP, flags);
        return 1;
    }
    if (buttons & D_JPAD)
    {
        dpadUseItem(play, DPAD_DOWN, flags);
        return 1;
    }
    if (buttons & L_JPAD)
    {
        dpadUseItem(play, DPAD_LEFT, flags);
        return 1;
    }
    if (buttons & R_JPAD)
    {
        dpadUseItem(play, DPAD_RIGHT, flags);
        return 1;
    }
    return 0;
}
