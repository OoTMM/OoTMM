#include <combo.h>
#include <combo/dma.h>
#include <combo/player.h>
#include <combo/mask.h>
#include <combo/config.h>
#include <combo/dpad.h>
#include <combo/global.h>
#include <combo/draw.h>
#include <combo/inventory.h>

#define DPAD_UP         0
#define DPAD_UP_LEFT    1
#define DPAD_UP_RIGHT   2
#define DPAD_LEFT       3
#define DPAD_RIGHT      4
#define DPAD_DOWN_LEFT  5
#define DPAD_DOWN       6
#define DPAD_DOWN_RIGHT 7

#define DPAD_PAGE_TOP       0
#define DPAD_PAGE_CENTER    1
#define DPAD_PAGE_BOTTOM    2

static u8 sDpadPage = DPAD_PAGE_TOP;
static u8 sDpadPageSkip = 0;


static void* sDpadIconBuffer;
static u8 sDpadItems[] = { ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };
static u8 sDpadItemsOld[] = { ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };
static u8 sDpadItemBrightness[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
static const int kDpadPosX = 275;
static const int kDpadPosY = 75;

static const int kDpadOffX[] = {  0, -1,  1, -1, 1, -1, 0, 1 };
static const int kDpadOffY[] = { -2, -1, -1,  0, 0,  1, 2, 1 };

static float kDpadItemScale = 0.4f;

static Color_RGB8 sDpadColor = { 0x60, 0x60, 0x60 };
EXPORT_SYMBOL(DPAD_COLOR, sDpadColor);

static int canShowDpad(void)
{
    if (sDpadItems[DPAD_DOWN] == ITEM_NONE
        && sDpadItems[DPAD_DOWN_LEFT] == ITEM_NONE
        && sDpadItems[DPAD_DOWN_RIGHT] == ITEM_NONE
        && sDpadItems[DPAD_UP] == ITEM_NONE
        && sDpadItems[DPAD_UP_LEFT] == ITEM_NONE
        && sDpadItems[DPAD_UP_RIGHT] == ITEM_NONE
        && sDpadItems[DPAD_LEFT] == ITEM_NONE
        && sDpadItems[DPAD_RIGHT] == ITEM_NONE)
        return 0;

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
    if (itemId == ITEM_OOT_BOOTS_IRON || itemId == ITEM_OOT_BOOTS_HOVER || itemId == ITEM_OOT_TUNIC_GORON || itemId == ITEM_OOT_TUNIC_ZORA)
    {
        isEquip = 1;
    }
    if ((itemId == ITEM_OOT_OCARINA_FAIRY || itemId == ITEM_OOT_OCARINA_TIME) && play->interfaceCtx.restrictions.ocarina)
        return 0;
#else
    if (gSave.equippedMask != MASK_GIANT && (itemId == ITEM_MM_BOOTS_IRON || itemId == ITEM_MM_BOOTS_HOVER || itemId == ITEM_MM_TUNIC_GORON || itemId == ITEM_MM_TUNIC_ZORA))
        isEquip = 1;
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
        sDpadIconBuffer = malloc(32 * 32 * 4 * ARRAY_COUNT(sDpadItems));
    }

    for (int i = 0; i < ARRAY_COUNT(sDpadItems); ++i)
    {
        if (sDpadItems[i] != sDpadItemsOld[i] && sDpadItems[i] != ITEM_NONE)
        {
            comboItemIcon(sDpadIconBuffer + (i * 32 * 32 * 4), sDpadItems[i]);
            sDpadItemsOld[i] = sDpadItems[i];
        }
    }
}

void Dpad_Refresh()
{
    memset(&sDpadItemsOld, 0xff, sizeof(sDpadItemsOld));
}

void Dpad_Draw(PlayState* play)
{
    s16 itemId;
    u8 alpha;
    float x;
    float y;
    float dx;
    float dy;
    float skipY;
    float scale;

    if (!canShowDpad())
        return;

    reloadIcons(play);

    alpha = (u8)play->interfaceCtx.healthAlpha;

    /* Init */
    OPEN_DISPS(play->state.gfxCtx);
    gDPPipeSync(OVERLAY_DISP++);
    gSPSegment(OVERLAY_DISP++, 0x06, g.customKeep);
    gSPSegment(OVERLAY_DISP++, 0x07, sDpadIconBuffer);
    gDPSetPrimColor(OVERLAY_DISP++, 0, 0x80, sDpadColor.r, sDpadColor.g, sDpadColor.b, alpha);

    /* Draw */
    Draw_Init2D(&OVERLAY_DISP);
    Draw_Blit2D_RGBA16(&OVERLAY_DISP, 0x06000000, 32, 32, kDpadPosX, kDpadPosY, 0.5f);

    for (int i = 0; i < ARRAY_COUNT(sDpadItems); ++i)
    {
        itemId = sDpadItems[i];
        if (itemId != ITEM_NONE)
        {
            dx = kDpadOffX[i];
            dy = kDpadOffY[i];
            skipY = 0.0f;
            if (sDpadPageSkip & (1 << DPAD_PAGE_TOP)) {
                if (sDpadPage < DPAD_PAGE_TOP && dy > -1.0f)
                {
                    skipY--;
                }
                else if (sDpadPage > DPAD_PAGE_TOP && dy < -1.0f)
                {
                    skipY++;
                }
            }
            if (sDpadPageSkip & (1 << DPAD_PAGE_CENTER)) {
                if (sDpadPage < DPAD_PAGE_CENTER && dy > 0.0f)
                {
                    skipY--;
                }
                else if (sDpadPage > DPAD_PAGE_CENTER && dy < 0.0f)
                {
                    skipY++;
                }
            }
            if (sDpadPageSkip & (1 << DPAD_PAGE_BOTTOM)) {
                if (sDpadPage < DPAD_PAGE_BOTTOM && dy > 1.0f)
                {
                    skipY--;
                }
                else if (sDpadPage > DPAD_PAGE_BOTTOM && dy < 1.0f)
                {
                    skipY++;
                }
            }
            dy += skipY;
            switch (sDpadPage)
            {
            case DPAD_PAGE_TOP:
                dy++;
                break;
            case DPAD_PAGE_BOTTOM:
                dy--;
                break;
            }
            if (dy > 1) {
                dy = ((dy - 1) / 2) + 1;
            }
            if (dy < -1) {
                dy = ((dy + 1) / 2) - 1;
            }
            if (dy != 0.0f) {
                dx *= 0.5f;
            }
            x = kDpadPosX + dx * 32 * kDpadItemScale + 1.5f;
            y = kDpadPosY + dy * 32 * kDpadItemScale + 1;
            u8 a = alpha;
            scale = kDpadItemScale;
            f32 distance = fabsf(dx) + fabsf(dy);
            if (distance > 1.0f)
            {
                f32 scaleAmount = 3.0f / (distance + 2.0f);
                a >>= 1;
                scale *= scaleAmount;
                x += 16.0f * (1 - scaleAmount) * kDpadItemScale;
                y += 16.0f * (1 - scaleAmount) * kDpadItemScale;
            }
            u8 brightness = sDpadItemBrightness[i];
            if (!canUseDpadItem(play, itemId, DPF_ITEMS | DPF_EQUIP))
            {
                if (brightness > 0x80)
                    brightness = 0x80;
                a *= 0.5f;
            }
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0x80, brightness, brightness, brightness, a);
            Draw_Blit2D_RGBA32(&OVERLAY_DISP, 0x07000000 | (i * 32 * 32 * 4), 32, 32, x, y, scale);
        }
    }
    CLOSE_DISPS();
}

static void skipEmptyDpadPages()
{
    sDpadPageSkip = 0;
    if (sDpadItems[DPAD_UP_LEFT] == ITEM_NONE && sDpadItems[DPAD_UP_RIGHT] == ITEM_NONE)
    {
        sDpadPageSkip |= (1 << DPAD_PAGE_TOP);
        if (sDpadPage == DPAD_PAGE_TOP)
            sDpadPage++;
    }
    if (sDpadItems[DPAD_LEFT] == ITEM_NONE && sDpadItems[DPAD_RIGHT] == ITEM_NONE)
    {
        sDpadPageSkip |= (1 << DPAD_PAGE_CENTER);
        if (sDpadPage == DPAD_PAGE_CENTER)
            sDpadPage++;
    }
    if (sDpadItems[DPAD_DOWN_LEFT] == ITEM_NONE && sDpadItems[DPAD_DOWN_RIGHT] == ITEM_NONE)
    {
        sDpadPageSkip |= (1 << DPAD_PAGE_BOTTOM);
        if (sDpadPage == DPAD_PAGE_BOTTOM)
            sDpadPage = DPAD_PAGE_TOP;
    }
}

#if defined(GAME_OOT)
void Dpad_Update(PlayState* play)
{
    if (Config_Flag(CFG_QOL_EXTEND_DPAD)) {
        sDpadItems[DPAD_UP_LEFT] = ITEM_NONE;
        u16 nextShield = gSave.info.equips.equipment.shields;
        do
        {
            if (!Config_Flag(CFG_OOT_AGELESS_SHIELDS))
            {
                if (gSave.age == AGE_CHILD)
                {
                    if (nextShield == 2)
                    {
                        nextShield++; //  = 0;
                        continue;
                    }
                }
                else
                {
                    if (nextShield == 0)
                    {
                        nextShield++;
                        continue;
                    }
                }
            }
            if (gSave.info.inventory.equipment.shields & (1 << nextShield))
            {
                sDpadItems[DPAD_UP_LEFT] = ITEM_OOT_SHIELD_DEKU + nextShield;
                sDpadItemBrightness[DPAD_UP_LEFT] = (gSave.info.equips.equipment.shields == (nextShield + 1)) ? 0 : 0xff;
                break;
            }
            nextShield++;
            if (nextShield > 2)
            {
                if (gSave.info.equips.equipment.shields)
                {
                    sDpadItems[DPAD_UP_LEFT] = ITEM_OOT_SHIELD_DEKU + gSave.info.equips.equipment.shields - 1;
                    sDpadItemBrightness[DPAD_UP_LEFT] = 0;
                }
                break;
            }
        } while (nextShield != gSave.info.equips.equipment.shields);

        sDpadItems[DPAD_UP_RIGHT] = ITEM_NONE;
        u16 nextSword = gSave.info.equips.equipment.swords;
        if (nextSword > 2)
            nextSword = 0;
        do
        {
            if (!Config_Flag(CFG_OOT_AGELESS_SWORDS))
            {
                if (gSave.age == AGE_CHILD)
                {
                    break;
                }
                if (nextSword == 0)
                {
                    nextSword++;
                    continue;
                }
            }
            if (gSave.info.inventory.equipment.swords & (1 << nextSword))
            {
                sDpadItems[DPAD_UP_RIGHT] = ITEM_OOT_SWORD_KOKIRI + nextSword;
                break;
            }
            nextSword++;
            if (nextSword > 2)
                nextSword = 0;
        } while (nextSword != gSave.info.equips.equipment.swords);

        sDpadItems[DPAD_LEFT] = (gSave.info.inventory.equipment.tunics & EQ_OOT_TUNIC_GORON) ? ITEM_OOT_TUNIC_GORON : ITEM_NONE;
        sDpadItems[DPAD_RIGHT] = (gSave.info.inventory.equipment.tunics & EQ_OOT_TUNIC_ZORA) ? ITEM_OOT_TUNIC_ZORA : ITEM_NONE;
    } else {
        sDpadItems[DPAD_LEFT] = ITEM_NONE;
        sDpadItems[DPAD_RIGHT] = ITEM_NONE;
        sDpadItems[DPAD_UP_LEFT] = ITEM_NONE;
        sDpadItems[DPAD_UP_RIGHT] = ITEM_NONE;
    }
    sDpadItems[DPAD_DOWN_LEFT] = (gSave.info.inventory.equipment.boots & EQ_OOT_BOOTS_IRON) ? ITEM_OOT_BOOTS_IRON : ITEM_NONE;
    sDpadItems[DPAD_DOWN_RIGHT] = (gSave.info.inventory.equipment.boots & EQ_OOT_BOOTS_HOVER) ? ITEM_OOT_BOOTS_HOVER : ITEM_NONE;
    sDpadItems[DPAD_DOWN] = gSave.info.inventory.items[ITS_OOT_OCARINA];

    sDpadItems[DPAD_UP] = gSave.info.inventory.items[ITS_OOT_TRADE_ADULT];
    if (gSave.age == AGE_CHILD)
    {
        sDpadItems[DPAD_UP] = gSave.info.inventory.items[ITS_OOT_TRADE_CHILD];
        if (!Config_Flag(CFG_OOT_AGELESS_BOOTS))
        {
            sDpadItems[DPAD_DOWN_LEFT] = ITEM_NONE;
            sDpadItems[DPAD_DOWN_RIGHT] = ITEM_NONE;
        }
        if (!Config_Flag(CFG_OOT_AGELESS_TUNICS))
        {
            sDpadItems[DPAD_LEFT] = ITEM_NONE;
            sDpadItems[DPAD_RIGHT] = ITEM_NONE;
        }
    }
    else if (Config_Flag(CFG_OOT_AGELESS_CHILD_TRADE))
        sDpadItems[DPAD_UP] = gSave.info.inventory.items[ITS_OOT_TRADE_CHILD];

    /* Ban bottles */
    for (int i = 0; i < ARRAY_COUNT(sDpadItems); ++i)
    {
        if (comboIsTradeBottleOot(sDpadItems[i]))
            sDpadItems[i] = ITEM_NONE;
    }

    skipEmptyDpadPages();
}
#endif

#if defined(GAME_MM)
void Dpad_Update(PlayState* play)
{
    sDpadPageSkip = 0;
    sDpadItems[DPAD_UP] = gSave.info.inventory.items[ITS_MM_MASK_DEKU];
    sDpadItems[DPAD_UP_LEFT] = gSave.info.inventory.items[ITS_MM_MASK_GORON];
    sDpadItems[DPAD_UP_RIGHT] = gSave.info.inventory.items[ITS_MM_MASK_ZORA];
    if (Config_Flag(CFG_QOL_EXTEND_DPAD)) {
        sDpadItems[DPAD_LEFT] = (gMmExtraTrade.trade2 & (1 << XITEM_MM_TRADE2_TUNIC_GORON)) ? ITEM_MM_TUNIC_GORON : ITEM_NONE;
        sDpadItems[DPAD_RIGHT] = (gMmExtraTrade.trade3 & (1 << XITEM_MM_TRADE3_TUNIC_ZORA)) ? ITEM_MM_TUNIC_ZORA : ITEM_NONE;
        sDpadItems[DPAD_DOWN_LEFT] = (gMmExtraTrade.trade2 & (1 << XITEM_MM_TRADE2_BOOTS_IRON)) ? ITEM_MM_BOOTS_IRON : ITEM_NONE;
        sDpadItems[DPAD_DOWN_RIGHT] = (gMmExtraTrade.trade3 & (1 << XITEM_MM_TRADE3_BOOTS_HOVER)) ? ITEM_MM_BOOTS_HOVER : ITEM_NONE;
    } else {
        sDpadItems[DPAD_LEFT] = ITEM_NONE;
        sDpadItems[DPAD_RIGHT] = ITEM_NONE;
        sDpadItems[DPAD_DOWN_LEFT] = ITEM_NONE;
        sDpadItems[DPAD_DOWN_RIGHT] = ITEM_NONE;
    }
    sDpadItems[DPAD_DOWN] = gSave.info.inventory.items[ITS_MM_OCARINA];

    skipEmptyDpadPages();
}
#endif

static int dpadNavigate(PlayState* play, int* index, int flags)
{
    s8 newPage = sDpadPage;
    int result = 0;
    switch (*index)
    {
        case DPAD_DOWN:
            newPage = sDpadPage + 1;
            while (newPage <= DPAD_PAGE_BOTTOM)
            {
                if (!(sDpadPageSkip & (1 << newPage)))
                {
                    result = 1;
                    break;
                }
                newPage++;
            }
            break;
        case DPAD_UP:
            newPage = sDpadPage - 1;
            while (newPage >= DPAD_PAGE_TOP)
            {
                if (!(sDpadPageSkip & (1 << newPage)))
                {
                    result = 1;
                    break;
                }
                newPage--;
            }
            break;
        case DPAD_LEFT:
            switch (sDpadPage)
            {
                case DPAD_PAGE_TOP:
                    *index = DPAD_UP_LEFT;
                    break;
                case DPAD_PAGE_BOTTOM:
                    *index = DPAD_DOWN_LEFT;
                    break;
            }
            break;
        case DPAD_RIGHT:
            switch (sDpadPage)
            {
                case DPAD_PAGE_TOP:
                    *index = DPAD_UP_RIGHT;
                    break;
                case DPAD_PAGE_BOTTOM:
                    *index = DPAD_DOWN_RIGHT;
                    break;
            }
            break;
    }

    if (result && (flags & DPF_EQUIP)) {
        sDpadPage = newPage;
    }
    return result;
}

static void dpadUseItem(PlayState* play, int index, int flags)
{
    s16 itemId;

    itemId = sDpadItems[index];
    if (!canUseDpadItem(play, itemId, flags))
        return;
    Player_UseItem(play, GET_PLAYER(play), itemId, 1);
}

int Dpad_Use(PlayState* play, int flags)
{
    u32 buttons;
    int index = -1;

    /* Detect button press */
    buttons = play->state.input[0].press.button;
    if (buttons & U_JPAD)
    {
        index = DPAD_UP;
    }
    if (buttons & D_JPAD)
    {
        index = DPAD_DOWN;
    }
    if (buttons & L_JPAD)
    {
        index = DPAD_LEFT;
    }
    if (buttons & R_JPAD)
    {
        index = DPAD_RIGHT;
    }

    if (index < 0)
        return 0;

    if (dpadNavigate(play, &index, flags))
        return 1;

    if (!canUseDpad(play))
        return 0;

    dpadUseItem(play, index, flags);

    return 1;
}
