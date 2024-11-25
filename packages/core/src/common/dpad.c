#include <combo.h>
#include <combo/dma.h>
#include <combo/player.h>
#include <combo/mask.h>
#include <combo/config.h>
#include <combo/dpad.h>
#include <combo/global.h>
#include <combo/draw.h>

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

static Color_RGB8 sDpadColor = { 0x60, 0x60, 0x60 };
EXPORT_SYMBOL(DPAD_COLOR, sDpadColor);

static int canShowDpad(void)
{
    if (gSaveContext.gameMode || (gSaveContext.minigameState == 1))
        return 0;
    return 1;
}

static int canUseDpad(PlayState* play)
{
    Player* link;

    link = GET_PLAYER(play);
    if (!canShowDpad())
        return 0;
    if (link->stateFlags1 & (PLAYER_ACTOR_STATE_CLIMB | PLAYER_ACTOR_STATE_CLIMB2 | PLAYER_ACTOR_STATE_EPONA | PLAYER_ACTOR_STATE_USE_ITEM | PLAYER_ACTOR_STATE_CUTSCENE_FROZEN | PLAYER_ACTOR_STATE_GET_ITEM | PLAYER_ACTOR_STATE_DEATH | PLAYER_ACTOR_STATE_TRANSITION | PLAYER_ACTOR_STATE_TRANSFORM))
        return 0;
    return 1;
}

static int canUseDpadItem(PlayState* play, s16 itemId, int flags)
{
    Player* link;
    int isEquip;

    link = GET_PLAYER(play);
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
    if (gSave.equippedMask == MASK_GIANT)
        return 0;

    /* These states seem to handle minigames - and everything should be disabled during these */
    if(link->stateFlags3 & (1 << 22) || link->stateFlags1 & (1 << 5))
        return 0;
#endif

    /* Underwater - disable everything except zora mask */
    if (link->stateFlags1 & PLAYER_ACTOR_STATE_WATER)
    {
#if defined(GAME_MM)
        if (itemId != ITEM_MM_MASK_ZORA && !(gSave.playerForm == MM_PLAYER_FORM_ZORA && (itemId == ITEM_MM_OCARINA_FAIRY || itemId == ITEM_MM_OCARINA_OF_TIME)))
#endif
            return 0;
    }

    return 1;
}

static void reloadIcons(PlayState* play)
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
            comboItemIcon(sDpadIconBuffer + (i * 32 * 32 * 4), sDpadItems[i]);
#else
            LoadIcon(0xa36c10, sDpadItems[i], sDpadIconBuffer + (i * 32 * 32 * 4), 0x1000);
#endif
            sDpadItemsOld[i] = sDpadItems[i];
        }
    }
}

void Dpad_Draw(PlayState* play)
{
    s16 itemId;
    u8 alpha;
    float x;
    float y;

    if (!canShowDpad())
        return;

    reloadIcons(play);

#if defined(GAME_OOT)
    alpha = (u8)play->interfaceCtx.healthAlpha;
#else
    alpha = (u8)play->interfaceCtx.alpha.health;
#endif

    /* Init */
    OPEN_DISPS(play->state.gfxCtx);
    gDPPipeSync(OVERLAY_DISP++);
    gSPSegment(OVERLAY_DISP++, 0x06, g.customKeep);
    gSPSegment(OVERLAY_DISP++, 0x07, sDpadIconBuffer);
    gDPSetPrimColor(OVERLAY_DISP++, 0, 0x80, sDpadColor.r, sDpadColor.g, sDpadColor.b, alpha);

    /* Draw */
    Draw_Init2D(&OVERLAY_DISP);
    Draw_Blit2D_RGBA16(&OVERLAY_DISP, 0x06000000, 32, 32, kDpadPosX, kDpadPosY, 0.5f);

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
            Draw_Blit2D_RGBA32(&OVERLAY_DISP, 0x07000000 | (i * 32 * 32 * 4), 32, 32, x, y, kDpadItemScale);
        }
    }
    CLOSE_DISPS();
}

static void dpadUseItem(PlayState* play, int index, int flags)
{
    s16 itemId;

    itemId = sDpadItems[index];
    if (!canUseDpadItem(play, itemId, flags))
        return;
    Player_UseItem(play, GET_PLAYER(play), itemId);
}

#if defined(GAME_OOT)
void Dpad_Update(PlayState* play)
{
    /* Update the items */
    sDpadItems[DPAD_DOWN] = gSave.info.inventory.items[ITS_OOT_OCARINA];
    sDpadItems[DPAD_LEFT] = (gSave.info.inventory.equipment.boots & EQ_OOT_BOOTS_IRON) ? ITEM_OOT_BOOTS_IRON : ITEM_NONE;
    sDpadItems[DPAD_RIGHT] = (gSave.info.inventory.equipment.boots & EQ_OOT_BOOTS_HOVER) ? ITEM_OOT_BOOTS_HOVER : ITEM_NONE;
    sDpadItems[DPAD_UP] = gSave.info.inventory.items[ITS_OOT_TRADE_ADULT];
    if (gSave.age == AGE_CHILD)
    {
        sDpadItems[DPAD_UP] = gSave.info.inventory.items[ITS_OOT_TRADE_CHILD];
        if (!Config_Flag(CFG_OOT_AGELESS_BOOTS))
        {
            sDpadItems[DPAD_LEFT] = ITEM_NONE;
            sDpadItems[DPAD_RIGHT] = ITEM_NONE;
        }
    }
    else if(Config_Flag(CFG_OOT_AGELESS_CHILD_TRADE))
        sDpadItems[DPAD_UP] = gSave.info.inventory.items[ITS_OOT_TRADE_CHILD];
}
#endif

#if defined(GAME_MM)
void Dpad_Update(PlayState* play)
{
    /* Update the items */
    sDpadItems[DPAD_DOWN] = gSave.info.inventory.items[ITS_MM_OCARINA];
    sDpadItems[DPAD_UP] = gSave.info.inventory.items[ITS_MM_MASK_DEKU];
    sDpadItems[DPAD_LEFT] = gSave.info.inventory.items[ITS_MM_MASK_GORON];
    sDpadItems[DPAD_RIGHT] = gSave.info.inventory.items[ITS_MM_MASK_ZORA];
}
#endif

int Dpad_Use(PlayState* play, int flags)
{
    u32 buttons;
    if (!canUseDpad(play))
        return 0;

    /* Detect button press */
    buttons = play->state.input[0].press.button;
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
