#include <combo.h>
#include <combo/dma.h>
#include <combo/menu.h>

void KaleidoScope_AfterSetCutsorColor(GameState_Play* play)
{
    u16 cursorSlot;
    int press;
    int effect;

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

    if (cursorSlot >= ITS_MM_BOTTLE && cursorSlot <= ITS_MM_BOTTLE6 && gSave.inventory.items[cursorSlot] == ITEM_MM_BOTTLED_SPRING_WATER_HOT)
    {
        play->pauseCtx.cursorColorIndex = 4;
        if (press)
        {
            gSave.inventory.items[cursorSlot] = ITEM_MM_BOTTLED_SPRING_WATER;
            for (int i = 1; i <= 3; ++i)
            {
                if (gMmSave.itemEquips.cButtonSlots[0][i] == cursorSlot)
                {
                    gMmSave.itemEquips.buttonItems[0][i] = ITEM_MM_BOTTLED_SPRING_WATER;
                }
            }
            for (int i = 1; i < 4; i++)
                Interface_LoadItemIconImpl(play, i);
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
    if (messageId == 0x1705)
    {
        b = play->textBuffer;
        b[2] = 0x4C; // Use Ocarina of Time icon.
        b += 11;
        comboTextAppendStr(&b, TEXT_COLOR_RED "Fairy Ocarina" TEXT_NL);
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, "This is a memento from" TEXT_NL "Saria." TEXT_NL TEXT_BOX_BREAK_2 "Play it with \xB0 and the four \xB2" TEXT_NL "Buttons. Press \xB1 to stop." TEXT_END);
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

static void menuSave(GameState_Play* play)
{
    /* Can't save in some scenes */
    switch (play->sceneId)
    {
    case SCE_MM_CLOCK_TOWER_ROOFTOP:
    case SCE_MM_MOON:
    case SCE_MM_MOON_DEKU:
    case SCE_MM_MOON_GORON:
    case SCE_MM_MOON_LINK:
    case SCE_MM_MOON_ZORA:
    case SCE_MM_LAIR_MAJORA:
        return;
    }

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
