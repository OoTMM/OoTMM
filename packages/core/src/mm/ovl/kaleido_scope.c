#include <combo.h>
#include <combo/dma.h>
#include <combo/dungeon.h>
#include <combo/menu.h>
#include <combo/item.h>

void KaleidoScope_AfterSetCutsorColor(GameState_Play* play)
{
    u16 cursorSlot;
    int press;
    int effect;

    /* Update Dpad */
    comboDpadUpdate(play);

    cursorSlot = play->pauseCtx.cursorSlot[0];
    press = !!(play->gs.input[0].pressed.buttons & (L_TRIG | U_CBUTTONS));
    effect = 0;

    if (cursorSlot == ITS_MM_TRADE1 && popcount(gMmExtraTrade.trade1) > 1)
    {
        play->pauseCtx.cursorColorIndex = 4;
        if (press)
        {
            comboToggleTrade1();
            effect = 1;
        }
    }

    if (cursorSlot == ITS_MM_TRADE2 && popcount(gMmExtraTrade.trade2) > 1)
    {
        play->pauseCtx.cursorColorIndex = 4;
        if (press)
        {
            comboToggleTrade2();
            effect = 1;
        }
    }

    if (cursorSlot == ITS_MM_TRADE3 && popcount(gMmExtraTrade.trade3) > 1)
    {
        play->pauseCtx.cursorColorIndex = 4;
        if (press)
        {
            comboToggleTrade3();
            effect = 1;
        }
    }

    if (cursorSlot == ITS_MM_HOOKSHOT && popcount(gMmExtraItems.hookshot) > 1)
    {
        play->pauseCtx.cursorColorIndex = 4;
        if (press)
        {
            comboToggleHookshot();
            effect = 1;
        }
    }

    if (cursorSlot == ITS_MM_OCARINA && play->pauseCtx.cursorItem[0] != 0x3e7 && popcount(gMmExtraItems.ocarina) > 1)
    {
        play->pauseCtx.cursorColorIndex = 4;
        if (press)
        {
            comboToggleOcarina();
            effect = 1;
        }
    }

    if (cursorSlot >= ITS_MM_BOTTLE && cursorSlot <= ITS_MM_BOTTLE6 && gSave.inventory.items[cursorSlot] == ITEM_MM_SPRING_WATER_HOT)
    {
        play->pauseCtx.cursorColorIndex = 4;
        if (press)
        {
            gSave.inventory.items[cursorSlot] = ITEM_MM_SPRING_WATER;
            reloadSlotOot(gPlay, cursorSlot);
            effect = 1;
        }
    }

    if (effect)
    {
        PlaySound(0x4809);
    }
}

void KaleidoScope_LoadNamedItemCustom(void* segment, u32 texIndex)
{
    DmaEntry dma;
    u32 isForeign = 0;
    switch (texIndex)
    {
    case 0x5: // MM Fairy Ocarina
        isForeign = 1;
        texIndex = 0x7b + 0x7; // OoT Fairy Ocarina
        break;
    case 0x11: // MM OoT Hookshot
        isForeign = 1;
        texIndex = 0x7b + 0xA; // OoT OoT Hookshot
        break;
    case ITEM_MM_SPELL_FIRE:
        isForeign = 1;
        texIndex = 0x7b + ITEM_OOT_SPELL_FIRE;
        break;
    case ITEM_MM_SPELL_WIND:
        isForeign = 1;
        texIndex = 0x7b + ITEM_OOT_SPELL_WIND;
        break;
    case ITEM_MM_SPELL_LOVE:
        isForeign = 1;
        texIndex = 0x7b + ITEM_OOT_SPELL_LOVE;
        break;
    }
    if (isForeign)
    {
        comboDmaLookupForeignId(&dma, 0xf);
        u32 textureFileAddress = dma.pstart;
        u32 textureOffset = 0x400 * texIndex;
        DMARomToRam((textureFileAddress + textureOffset) | PI_DOM1_ADDR2, segment, 0x400);
    }
    else
    {
        LoadIcon(0x00A27660, texIndex, segment, 0x400);
    }

}

void KaleidoScope_ShowItemMessage(GameState_Play* play, u16 messageId, u8 yPosition)
{
    char* b;
    if (messageId == 0x1711)
    {
        messageId = 0x170f; // Use Hookshot message instead of broken OoT Hookshot message
    }
    Message_ShowMessageAtYPosition(play, messageId, yPosition);
    s16 itemId = messageId - 0x1700;
    switch (itemId)
    {
    case ITEM_MM_OCARINA_FAIRY:
        b = play->msgCtx.font.textBuffer.schar;
        b[2] = 0x4C; // Use Ocarina of Time icon.
        b += 11;
        comboTextAppendStr(&b, TEXT_COLOR_RED "Fairy Ocarina" TEXT_NL);
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, "This is a memento from" TEXT_NL "Saria." TEXT_NL TEXT_BOX_BREAK_2 "Play it with \xB0 and the four \xB2" TEXT_NL "Buttons. Press \xB1 to stop." TEXT_END);
        break;
    case ITEM_MM_SPELL_WIND:
        b = play->msgCtx.font.textBuffer.schar;
        b[2] = 0xFE; // Use No Icon
        b += 11;
        comboTextAppendStr(&b, TEXT_COLOR_RED "Farore's Wind" TEXT_NL);
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, "This is warp magic you can use" TEXT_NL "with \xB2. Warp when you are in" TEXT_NL "danger!" TEXT_END);
        break;
    case ITEM_MM_SPELL_LOVE:
        b = play->msgCtx.font.textBuffer.schar;
        b[2] = 0xFE; // Use No Icon
        b += 11;
        comboTextAppendStr(&b, TEXT_COLOR_RED "Nayru's Love" TEXT_NL);
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, "Cast this to create a powerful" TEXT_NL "protective barrier. It's defensive" TEXT_NL "magic you can use with \xB2." TEXT_END);
        break;
    case ITEM_MM_SPELL_FIRE:
        b = play->msgCtx.font.textBuffer.schar;
        b[2] = 0xFE; // Use No Icon
        b += 11;
        comboTextAppendStr(&b, TEXT_COLOR_RED "Din's Fire" TEXT_NL);
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, "Its fireball engulgs everything!" TEXT_NL "It's attack magic you can use" TEXT_NL "with \xB2." TEXT_END);
        break;
    }
}

typedef void (*KaleidoScopeHandler)(GameState_Play*);

static void KaleidoScope_DrawMapDungeonMenu(GameState_Play* play, u32 overlayAddr)
{
    KaleidoScopeHandler handler;

    if (g.menuScreen)
    {
        comboMenuDraw(play);
    }
    else
    {
        handler = OverlayAddr(overlayAddr);
        handler(play);
    }
}

static void KaleidoScope_UpdateMapDungeonMenu(GameState_Play* play, u32 overlayAddr)
{
    KaleidoScopeHandler handler;

    if (play->gs.input[0].pressed.buttons & (L_TRIG | U_CBUTTONS))
        comboMenuNext();

    if (g.menuScreen)
    {
        comboMenuUpdate(play);
    }
    else
    {
        handler = OverlayAddr(overlayAddr);
        handler(play);
    }
}

static void KaleidoScope_DrawMapMenu(GameState_Play *play)
{
    KaleidoScope_DrawMapDungeonMenu(play, 0x8081e7d8);
}

PATCH_CALL(0x80822a14, KaleidoScope_DrawMapMenu);
PATCH_CALL(0x808230e4, KaleidoScope_DrawMapMenu);

static void KaleidoScope_DrawDungeonMenu(GameState_Play *play)
{
    KaleidoScope_DrawMapDungeonMenu(play, 0x8081d6dc);
}

PATCH_CALL(0x808229cc, KaleidoScope_DrawDungeonMenu);
PATCH_CALL(0x80822f34, KaleidoScope_DrawDungeonMenu);

static void KaleidoScope_UpdateMapMenu(GameState_Play* play)
{
    KaleidoScope_UpdateMapDungeonMenu(play, 0x8081fb1c);
}

PATCH_CALL(0x8082ae00, KaleidoScope_UpdateMapMenu);

static void KaleidoScope_UpdateDungeonMenu(GameState_Play* play)
{
    KaleidoScope_UpdateMapDungeonMenu(play, 0x8081e118);
}

PATCH_CALL(0x8082adf0, KaleidoScope_UpdateDungeonMenu);

static void KaleidoScope_DrawDungeonUnk1(void* unk)
{
    if (!g.menuScreen)
        Gfx_SetupDL_42Opa(unk);
}

PATCH_CALL(0x808229d4, KaleidoScope_DrawDungeonUnk1);
PATCH_CALL(0x80822f3c, KaleidoScope_DrawDungeonUnk1);

static void KaleidoScope_DrawDungeonUnk2(void* unk)
{
    if (!g.menuScreen)
        DrawDungeonUnk2(unk);
}

PATCH_CALL(0x80822a00, KaleidoScope_DrawDungeonUnk2);
PATCH_CALL(0x80822f68, KaleidoScope_DrawDungeonUnk2);

static int canSave(GameState_Play* play)
{
    Actor_Player* link;

    link = GET_LINK(play);
    if (link->state & PLAYER_ACTOR_STATE_EPONA)
        return 0;

    switch (play->sceneId)
    {
    case SCE_MM_MOON:
    case SCE_MM_MOON_DEKU:
    case SCE_MM_MOON_GORON:
    case SCE_MM_MOON_LINK:
    case SCE_MM_MOON_ZORA:
    case SCE_MM_LAIR_MAJORA:
        return 0;
    case SCE_MM_CLOCK_TOWER_ROOFTOP:
        return !!comboConfig(CFG_ER_MOON);
    case SCE_MM_SOUTHERN_SWAMP:
        return gSave.entranceIndex != 0x8460 && !MM_CHECK_EVENT_INF(0x35) && !MM_CHECK_EVENT_INF(0x41);
    default:
        return 1;
    }
}

static void menuSave(GameState_Play* play)
{
    /* Can't save in some scenes */
    if (!canSave(play))
        return;

    /* Save the game */
    comboSave(play, SF_OWL);
    PlaySound(0x4823);
}

static void KaleidoScope_UpdateSomeMenu(GameState_Play* play)
{
    KaleidoScopeHandler handler;

    /* Call the handler */
    handler = OverlayAddr(0x80817b5c);
    handler(play);

    if (play->gs.input[0].pressed.buttons & (L_TRIG | U_CBUTTONS))
    {
        menuSave(play);
    }
}

PATCH_CALL(0x8082ae10, KaleidoScope_UpdateSomeMenu);

u32 gCustomIconAddr;

static u32 sCustomIcons[] = {
    ITEM_MM_SPELL_WIND,
    ITEM_MM_SPELL_LOVE,
    ITEM_MM_SPELL_FIRE,
};

u8 gPlayerFormCustomItemRestrictions[5][8] =
{
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 1, 1, 0, 0, 0, 0, 0 },
};

typedef void (*KaleidoScope_GrayOutTextureRGBA32)(u32*, u16);

const size_t customIconSize = 0x1000;

void KaleidoScope_LoadIcons(u32 vrom, void* dst, size_t* size)
{
    DmaEntry dma;
    KaleidoScope_GrayOutTextureRGBA32 KaleidoScope_GrayOutTextureRGBA32 = OverlayAddr(0x808286D8);

    CmpDma_LoadAllFiles(vrom, dst, *size);

    gCustomIconAddr = (u32)dst + *size;

    comboDmaLookupForeignId(&dma, 8);
    u32 textureFileAddress = dma.pstart;

    for (u32 i = 0; i < ARRAY_SIZE(sCustomIcons); i++)
    {
        u32 icon = sCustomIcons[i];
        u32 foreignIcon;
        switch (icon)
        {
        case ITEM_MM_SPELL_FIRE:
            foreignIcon = ITEM_OOT_SPELL_FIRE;
            break;
        case ITEM_MM_SPELL_WIND:
            foreignIcon = ITEM_OOT_SPELL_WIND;
            break;
        case ITEM_MM_SPELL_LOVE:
            foreignIcon = ITEM_OOT_SPELL_LOVE;
            break;
        default:
            continue;
        }
        u32 textureOffset = customIconSize * foreignIcon;
        u32 customDestination = gCustomIconAddr + (i * customIconSize);
        DMARomToRam((textureFileAddress + textureOffset) | PI_DOM1_ADDR2, (void*)customDestination, customIconSize);

        u8 customItemIndex = icon - ITEM_MM_CUSTOM_MIN;
        if (customItemIndex >= 8 || !gPlayerFormCustomItemRestrictions[gSaveContext.save.playerForm][customItemIndex])
        {
            KaleidoScope_GrayOutTextureRGBA32((u32*)customDestination, customIconSize);
        }

        *size += customIconSize;
    }
}

typedef void (*KaleidoScope_DrawIcon)(GfxContext* gfxCtx, u32 texture, u16 width, u16 height, u16 point);

void KaleidoScope_DrawIconCustom(GfxContext* gfxCtx, u8 item, u16 width, u16 height, u16 point)
{
    u32* gItemIcons = (u32*)0x801c1e6c;
    KaleidoScope_DrawIcon KaleidoScope_DrawIcon = OverlayAddr(0x80821ad4);
    u32 texture;
    if (item < ITEM_MM_CUSTOM_MIN)
    {
        texture = gItemIcons[item];
    }
    else
    {
        u8 customItem = item - ITEM_MM_CUSTOM_MIN;
        texture = gCustomIconAddr + (customIconSize * customItem);
    }

    KaleidoScope_DrawIcon(gfxCtx, texture, width, height, point);
}
