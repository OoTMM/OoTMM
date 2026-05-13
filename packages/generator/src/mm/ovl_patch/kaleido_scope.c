#include <combo.h>
#include <combo/dma.h>
#include <combo/dungeon.h>
#include <combo/menu.h>
#include <combo/item.h>
#include <combo/player.h>
#include <combo/global.h>
#include <combo/dpad.h>
#include <combo/inventory.h>
#include <combo/entrance.h>
#include <combo/misc.h>
#include <combo/common/Kaleido_Scope.h>

void KaleidoScope_AfterSetCutsorColor(PlayState* play)
{
    u16 cursorSlot;
    int press;
    int effect;

    /* Update Dpad */
    Dpad_Update(play);

    cursorSlot = play->pauseCtx.cursorSlot[0];
    press = !!(play->state.input[0].press.button & (L_TRIG | U_CBUTTONS));
    effect = 0;

    u8* itemPtr;
    u32 flags;
    const u8* table;
    u32 tableSize;
    if (comboGetSlotExtras(cursorSlot, &itemPtr, &flags, &table, &tableSize) >= 0 && play->pauseCtx.cursorItem[0] != 999 && popcount(flags) > 1)
    {
        play->pauseCtx.cursorColorIndex = 4;
        if (press)
        {
            comboToggleSlot(cursorSlot);
            effect = 1;
        }
    }

    if (cursorSlot >= ITS_MM_BOTTLE && cursorSlot <= ITS_MM_BOTTLE6 && gSave.info.inventory.items[cursorSlot] == ITEM_MM_SPRING_WATER_HOT)
    {
        play->pauseCtx.cursorColorIndex = 4;
        if (press)
        {
            gSave.info.inventory.items[cursorSlot] = ITEM_MM_SPRING_WATER;
            reloadSlotMm(gPlay, cursorSlot);
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
    case ITEM_MM_SHIELD_HERO:
        if (gSharedCustomSave.mmShieldIsDeku)
        {
            isForeign = 1;
            texIndex = 0x7b + ITEM_OOT_SHIELD_DEKU;
        }
        break;
    case ITEM_MM_OCARINA_FAIRY:
        isForeign = 1;
        texIndex = 0x7b + ITEM_OOT_OCARINA_FAIRY;
        break;
    case ITEM_MM_HOOKSHOT_SHORT:
        isForeign = 1;
        texIndex = 0x7b + ITEM_OOT_HOOKSHOT;
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
    case ITEM_MM_BOOTS_IRON:
        isForeign = 1;
        texIndex = 0x7b + ITEM_OOT_BOOTS_IRON;
        break;
    case ITEM_MM_BOOTS_HOVER:
        isForeign = 1;
        texIndex = 0x7b + ITEM_OOT_BOOTS_HOVER;
        break;
    case ITEM_MM_TUNIC_GORON:
        isForeign = 1;
        texIndex = 0x7b + ITEM_OOT_TUNIC_GORON;
        break;
    case ITEM_MM_TUNIC_ZORA:
        isForeign = 1;
        texIndex = 0x7b + ITEM_OOT_TUNIC_ZORA;
        break;
    case ITEM_MM_HAMMER:
        isForeign = 1;
        texIndex = 0x7b + ITEM_OOT_HAMMER;
        break;
    case ITEM_MM_BOOMERANG:
        isForeign = 1;
        texIndex = 0x7b + ITEM_OOT_BOOMERANG;
        break;
    case ITEM_MM_RUTO_LETTER:
        isForeign = 1;
        texIndex = 0x7b + ITEM_OOT_RUTO_LETTER;
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

void KaleidoScope_ShowEquipMessage(PlayState* play, u16 messageId, u8 yPosition)
{
    char* b;
    Message_ShowMessageAtYPosition(play, messageId, yPosition);
    s16 itemId = messageId - 0x1737;
    switch (itemId)
    {
    case ITEM_MM_SHIELD_HERO:
        if (!gSharedCustomSave.mmShieldIsDeku)
            break;
        b = play->msgCtx.font.textBuffer.schar;
        b[2] = 0xFE; /* Use No Icon */
        b += 11;
        comboTextAppendStr(&b, TEXT_COLOR_YELLOW "Deku Shield" TEXT_NL);
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, "A basic shield." TEXT_NL "Vulnerable to fire." TEXT_END);
        break;
    }
}

PATCH_CALL(0x808184e4, KaleidoScope_ShowEquipMessage);
PATCH_CALL(0x80818528, KaleidoScope_ShowEquipMessage);

void KaleidoScope_ShowItemMessage(PlayState* play, u16 messageId, u8 yPosition)
{
    char* b;
    if (messageId == 0x1711)
    {
        messageId = 0x170f; /* Use Hookshot message instead of broken OoT Hookshot message */
    }
    Message_ShowMessageAtYPosition(play, messageId, yPosition);
    s16 itemId = messageId - 0x1700;
    switch (itemId)
    {
    case ITEM_MM_OCARINA_FAIRY:
        b = play->msgCtx.font.textBuffer.schar;
        b[2] = 0x4C; /* Use Ocarina of Time icon. */
        b += 11;
        comboTextAppendStr(&b, TEXT_COLOR_RED "Fairy Ocarina" TEXT_NL);
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, "This is a memento from" TEXT_NL "Saria." TEXT_NL TEXT_BOX_BREAK_2 "Play it with \xB0 and the four \xB2" TEXT_NL "Buttons. Press \xB1 to stop." TEXT_END);
        break;
    case ITEM_MM_SPELL_WIND:
        b = play->msgCtx.font.textBuffer.schar;
        b[2] = 0xFE; /* Use No Icon */
        b += 11;
        comboTextAppendStr(&b, TEXT_COLOR_RED "Farore's Wind" TEXT_NL);
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, "This is warp magic you can use" TEXT_NL "with \xB2. Warp when you are in" TEXT_NL "danger!" TEXT_END);
        break;
    case ITEM_MM_SPELL_LOVE:
        b = play->msgCtx.font.textBuffer.schar;
        b[2] = 0xFE; /* Use No Icon */
        b += 11;
        comboTextAppendStr(&b, TEXT_COLOR_RED "Nayru's Love" TEXT_NL);
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, "Cast this to create a powerful" TEXT_NL "protective barrier. It's defensive" TEXT_NL "magic you can use with \xB2." TEXT_END);
        break;
    case ITEM_MM_SPELL_FIRE:
        b = play->msgCtx.font.textBuffer.schar;
        b[2] = 0xFE; /* Use No Icon */
        b += 11;
        comboTextAppendStr(&b, TEXT_COLOR_RED "Din's Fire" TEXT_NL);
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, "Its fireball engulfs everything!" TEXT_NL "It's attack magic you can use" TEXT_NL "with \xB2." TEXT_END);
        break;
    case ITEM_MM_BOOTS_IRON:
        b = play->msgCtx.font.textBuffer.schar;
        b[2] = 0xFE; /* Use No Icon */
        b += 11;
        comboTextAppendStr(&b, TEXT_COLOR_RED "Iron Boots" TEXT_NL);
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, "So heavy, you can't run." TEXT_NL "So heavy, you can't float." TEXT_END);
        break;
    case ITEM_MM_BOOTS_HOVER:
        b = play->msgCtx.font.textBuffer.schar;
        b[2] = 0xFE; /* Use No Icon */
        b += 11;
        comboTextAppendStr(&b, TEXT_COLOR_RED "Hover Boots" TEXT_NL);
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, "With these mysterious boots" TEXT_NL "you can hover above the ground." TEXT_NL "The downside? No traction!" TEXT_END);
        break;
    case ITEM_MM_TUNIC_GORON:
        b = play->msgCtx.font.textBuffer.schar;
        b[2] = 0xFE; /* Use No Icon */
        b += 11;
        comboTextAppendStr(&b, TEXT_COLOR_RED "Goron Tunic" TEXT_NL);
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, "Wearing this heat-resistant tunic" TEXT_NL "allows you to go to hot places." TEXT_END);
        break;
    case ITEM_MM_TUNIC_ZORA:
        b = play->msgCtx.font.textBuffer.schar;
        b[2] = 0xFE; /* Use No Icon */
        b += 11;
        comboTextAppendStr(&b, TEXT_COLOR_RED "Zora Tunic" TEXT_NL);
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, "Wear this diving suit and you" TEXT_NL "won't drown underwater." TEXT_END);
        break;
    case ITEM_MM_HAMMER:
        b = play->msgCtx.font.textBuffer.schar;
        b[2] = 0xFE; /* Use No Icon */
        b += 11;
        comboTextAppendStr(&b, TEXT_COLOR_RED "Megaton Hammer" TEXT_NL);
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, "Press " TEXT_COLOR_YELLOW "\xB2");
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, " to smash and break " TEXT_NL "junk! It's so heavy, you need to " TEXT_NL "use two hands to swing it!" TEXT_END);
        break;
    case ITEM_MM_BOOMERANG:
        b = play->msgCtx.font.textBuffer.schar;
        b[2] = 0xFE; /* Use No Icon */
        b += 11;
        comboTextAppendStr(&b, TEXT_COLOR_RED "Boomerang" TEXT_NL);
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, "Press " TEXT_COLOR_YELLOW "\xB2");
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, " to throw and watch it" TEXT_NL " come back! The boomerang can stun or " TEXT_NL "defeat enemies!" TEXT_END);
        break;
    case ITEM_MM_BLUE_FIRE:
        b = play->msgCtx.font.textBuffer.schar;
        b[2] = 0xFE; /* Use No Icon */
        b += 11;
        comboTextAppendStr(&b, TEXT_COLOR_RED "Blue Fire" TEXT_NL);
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, "This is a cool flame you can" TEXT_NL "use with \xB2." TEXT_END);
        break;
    case ITEM_MM_RUTO_LETTER:
        b = play->msgCtx.font.textBuffer.schar;
        b[2] = 0xFE; /* Use No Icon */
        b += 11;
        comboTextAppendStr(&b, TEXT_COLOR_RED "Letter" TEXT_NL);
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, "It looks like there is something" TEXT_NL "already inside this bottle." TEXT_END);
        break;
    }
}

typedef void (*KaleidoScopeHandler)(PlayState*);

static void KaleidoScope_DrawMapDungeonMenu(PlayState* play, u32 overlayAddr)
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

static void KaleidoScope_UpdateMapDungeonMenu(PlayState* play, u32 overlayAddr)
{
    KaleidoScopeHandler handler;

    if (play->state.input[0].press.button & (L_TRIG | U_CBUTTONS))
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

static void KaleidoScope_DrawMapMenu(PlayState *play)
{
    KaleidoScope_DrawMapDungeonMenu(play, 0x8081e7d8);
}

PATCH_CALL(0x80822a14, KaleidoScope_DrawMapMenu);
PATCH_CALL(0x808230e4, KaleidoScope_DrawMapMenu);

static void KaleidoScope_DrawDungeonMenu(PlayState *play)
{
    KaleidoScope_DrawMapDungeonMenu(play, 0x8081d6dc);
}

PATCH_CALL(0x808229cc, KaleidoScope_DrawDungeonMenu);
PATCH_CALL(0x80822f34, KaleidoScope_DrawDungeonMenu);

static void KaleidoScope_UpdateMapMenu(PlayState* play)
{
    KaleidoScope_UpdateMapDungeonMenu(play, 0x8081fb1c);
}

PATCH_CALL(0x8082ae00, KaleidoScope_UpdateMapMenu);

static void KaleidoScope_UpdateDungeonMenu(PlayState* play)
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

u32 gCustomIconAddr;

static u32 sCustomIcons[] = {
    ITEM_MM_SPELL_WIND,
    ITEM_MM_SPELL_LOVE,
    ITEM_MM_SPELL_FIRE,
    ITEM_MM_BOOTS_IRON,
    ITEM_MM_BOOTS_HOVER,
    ITEM_MM_TUNIC_GORON,
    ITEM_MM_TUNIC_ZORA,
    ITEM_MM_HAMMER,
    ITEM_MM_BOOMERANG,
    ITEM_MM_RUTO_LETTER,
};

s8 gPlayerFormCustomItemRestrictions[5][ITEM_MM_CUSTOM_MAX - ITEM_MM_CUSTOM_MIN] =
{
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 1, 1, -1, -1, -1, -1, 1, 1, 1 },
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

    for (u32 i = 0; i < ARRAY_COUNT(sCustomIcons); i++)
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
        case ITEM_MM_BOOTS_IRON:
            foreignIcon = ITEM_OOT_BOOTS_IRON;
            break;
        case ITEM_MM_BOOTS_HOVER:
            foreignIcon = ITEM_OOT_BOOTS_HOVER;
            break;
        case ITEM_MM_TUNIC_GORON:
            foreignIcon = ITEM_OOT_TUNIC_GORON;
            break;
        case ITEM_MM_TUNIC_ZORA:
            foreignIcon = ITEM_OOT_TUNIC_ZORA;
            break;
        case ITEM_MM_HAMMER:
            foreignIcon = ITEM_OOT_HAMMER;
            break;
        case ITEM_MM_BOOMERANG:
            foreignIcon = ITEM_OOT_BOOMERANG;
            break;
        case ITEM_MM_RUTO_LETTER:
            foreignIcon = ITEM_OOT_RUTO_LETTER;
            break;
        default:
            continue;
        }
        u32 textureOffset = customIconSize * foreignIcon;
        u32 customDestination = gCustomIconAddr + (i * customIconSize);
        DMARomToRam((textureFileAddress + textureOffset) | PI_DOM1_ADDR2, (void*)customDestination, customIconSize);

        u8 customItemIndex = icon - ITEM_MM_CUSTOM_MIN;
        if (customItemIndex >= (ITEM_MM_CUSTOM_MAX - ITEM_MM_CUSTOM_MIN) || !gPlayerFormCustomItemRestrictions[gSaveContext.save.playerForm][customItemIndex])
        {
            KaleidoScope_GrayOutTextureRGBA32((u32*)customDestination, customIconSize);
        }

        *size += customIconSize;
    }

    /* Replace the Hero's Shield texture with Deku Shield */
    if (gSharedCustomSave.mmShieldIsDeku)
    {
        DMARomToRam((textureFileAddress + customIconSize * ITEM_OOT_SHIELD_DEKU) | PI_DOM1_ADDR2, (char*)dst + customIconSize * ITEM_MM_SHIELD_HERO, customIconSize);
    }
}

static u32 GetItemTexture(u8 item)
{
    u32* gItemIcons = (u32*)0x801c1e6c;
    if (item < ITEM_MM_CUSTOM_MIN)
    {
        return gItemIcons[item];
    }
    else
    {
        u8 customItem = item - ITEM_MM_CUSTOM_MIN;
        return gCustomIconAddr + (customIconSize * customItem);
    }
}

static u8 GetNextItem(u32 slot, s32* outTableIndex)
{
    u32 flags;
    const u8* table;
    u32 tableSize;
    u8* itemPtr;
    *outTableIndex = comboGetSlotExtras(slot, &itemPtr, &flags, &table, &tableSize);
    if (*outTableIndex >= 0)
    {
        return comboGetNextTrade(*itemPtr, flags, table, tableSize);
    }
    return ITEM_NONE;
}

/* Vertex buffers. */
static Vtx gVertexBufs[(4 * 7) * 2];

/* Vertex buffer pointers. */
static Vtx* gVertex[7] = {
    &gVertexBufs[(4 * 0) * 2],
    &gVertexBufs[(4 * 1) * 2],
    &gVertexBufs[(4 * 2) * 2],
    &gVertexBufs[(4 * 3) * 2],
    &gVertexBufs[(4 * 4) * 2],
    &gVertexBufs[(4 * 5) * 2],
    &gVertexBufs[(4 * 6) * 2],
};

static Vtx* GetVtxBuffer(PlayState* play, u32 vertIdx, u32 slot) {
    /* Get vertex of current icon drawing to Item Select screen */
    const Vtx* srcVtx = play->pauseCtx.vtxBuf + vertIdx;

    /* Get dest Vtx (factor in frame counter) */
    int framebufIdx = play->state.gfxCtx->displayListCounter & 1;
    Vtx* dstVtx = gVertex[slot] + (framebufIdx * 4);

    /* Copy source Vtx over to dest Vtx */
    for (int i = 0; i < 4; i++) {
        dstVtx[i] = srcVtx[i];
    }

    /* Adjust X position */
    dstVtx[0].v.ob[0] += 0x10;
    dstVtx[2].v.ob[0] += 0x10;

    /* Adjust Y position */
    dstVtx[0].v.ob[1] -= 0x10;
    dstVtx[1].v.ob[1] -= 0x10;

    return dstVtx;
}

static void DrawIcon(GraphicsContext* gfxCtx, const Vtx* vtx, u32 segAddr, u16 width, u16 height, u16 qidx) {
    OPEN_DISPS(gfxCtx);
    /* Instructions that happen before function */
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 0xFF, 0xFF, 0xFF, gfxCtx->play->pauseCtx.itemAlpha & 0xFF);
    gSPVertex(POLY_OPA_DISP++, vtx, 4, 0); /* Loads 4 vertices from RDRAM */
    /* Instructions that happen during function. */
    gDPLoadTextureBlock(POLY_OPA_DISP++, segAddr, G_IM_FMT_RGBA, G_IM_SIZ_32b, width, height, 0,
                        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSP1Quadrangle(POLY_OPA_DISP++, qidx + 0, qidx + 2, qidx + 3, qidx + 1, 0);
    CLOSE_DISPS();
}

typedef void (*KaleidoScope_DrawIcon)(GraphicsContext* gfxCtx, u32 texture, u16 width, u16 height, u16 point);

void KaleidoScope_DrawIconCustom(GraphicsContext* gfxCtx, u8 item, u16 width, u16 height, u32 slot, u16 point, u16 vertIdx)
{
    u32 texture = GetItemTexture(item);

    KaleidoScope_DrawIcon KaleidoScope_DrawIcon = OverlayAddr(0x80821ad4);
    KaleidoScope_DrawIcon(gfxCtx, texture, width, height, point);

    s32 tableIndex;
    u8 next = GetNextItem(slot, &tableIndex);
    if (next != ITEM_NONE && next != item)
    {
        texture = GetItemTexture(next);
        Vtx* vtx = GetVtxBuffer(gfxCtx->play, vertIdx, tableIndex);
        DrawIcon(gfxCtx, vtx, texture, width, height, point);
    }
}

static s32 KaleidoScope_IsAmmoRestricted(s16 item)
{
    return !gPlayerFormItemRestrictions[gSaveContext.save.playerForm][item];
}

typedef Gfx* (*DrawTexRectIA8Func)(Gfx*, u8*, s16, s16, s16, s16, s16, s16, s32, s32);

#define KALEIDO_DRAW_TEX_RECT_IA8_ADDR 0x8010CD98
#define KALEIDO_AMMO_DIGIT_0_TEX_ADDR  0x02004aa0
#define KALEIDO_AMMO_RECT_LEFT_ADDR    0x8082b444
#define KALEIDO_AMMO_RECT_HEIGHT_ADDR  0x8082b454

static s32 KaleidoScope_ItemHasAmmoCount(s16 item)
{
    switch (item)
    {
        case ITEM_MM_BOMB:
        case ITEM_MM_BOW:
        case ITEM_MM_NUT:
        case ITEM_MM_BOMBCHU:
        case ITEM_MM_POWDER_KEG:
        case ITEM_MM_STICK:
        case ITEM_MM_MAGIC_BEAN:
        case ITEM_MM_PICTOGRAPH_BOX:
            return 1;

        default:
            return 0;
    }
}

static s32 KaleidoScope_FindSlotForItem(s16 item)
{
    s32 i;

    for (i = 0; i < ARRAY_COUNT(gSave.info.inventory.items); i++)
    {
        if (gSave.info.inventory.items[i] == item)
        {
            return i;
        }
    }

    return -1;
}

static s16 KaleidoScope_GetShownAmmoItem(s16 item, s32* outSlot)
{
    s32 slot;
    u8* itemPtr;
    u32 flags;
    const u8* table;
    u32 tableSize;

    slot = KaleidoScope_FindSlotForItem(item);
    *outSlot = slot;

    if (slot < 0)
    {
        return item;
    }

    if (comboGetSlotExtras(slot, &itemPtr, &flags, &table, &tableSize) >= 0 && popcount(flags) > 1)
    {
        return *itemPtr;
    }

    return item;
}
#define MM_QUEST_PICTOGRAPH 0x19

#define MM_CHECK_QUEST_ITEM(item) \
(gSave.info.inventory.quest.value & (1u << (item)))
static s16 KaleidoScope_GetAmmo(s16 item, s32 slot)
{
    switch (item)
    {
        case ITEM_MM_PICTOGRAPH_BOX:
            return MM_CHECK_QUEST_ITEM(MM_QUEST_PICTOGRAPH) ? 1 : 0;

        case ITEM_MM_POWDER_KEG:
            return gSave.info.inventory.ammo[slot];

        default:
            if (slot >= 0)
            {
                return gSave.info.inventory.ammo[slot];
            }
            return 0;
    }
}

#define MM_UPG_QUIVER      gSave.info.inventory.upgrades.quiver
#define MM_UPG_BOMB_BAG    gSave.info.inventory.upgrades.bombBag
#define MM_UPG_DEKU_STICKS gSave.info.inventory.upgrades.dekuStick
#define MM_UPG_DEKU_NUTS   gSave.info.inventory.upgrades.dekuNut

#define MM_CAPACITY_BOMB_BAG(value)    ((s16[]){ 0, 20, 30, 40 }[(value)])
#define MM_CAPACITY_QUIVER(value)      ((s16[]){ 0, 30, 40, 50 }[(value)])
#define MM_CAPACITY_DEKU_STICKS(value) ((s16[]){ 0, 10, 20, 30 }[(value)])
#define MM_CAPACITY_DEKU_NUTS(value)   ((s16[]){ 0, 20, 30, 40 }[(value)])

#define MM_CUR_BOMB_BAG_CAPACITY()    MM_CAPACITY_BOMB_BAG(MM_UPG_BOMB_BAG)
#define MM_CUR_QUIVER_CAPACITY()      MM_CAPACITY_QUIVER(MM_UPG_QUIVER)
#define MM_CUR_DEKU_STICKS_CAPACITY() MM_CAPACITY_DEKU_STICKS(MM_UPG_DEKU_STICKS)
#define MM_CUR_DEKU_NUTS_CAPACITY()   MM_CAPACITY_DEKU_NUTS(MM_UPG_DEKU_NUTS)


static s32 KaleidoScope_IsAmmoAtCapacity(s16 item, s16 ammo)
{
    return (((item == ITEM_MM_BOMB) && (ammo == MM_CUR_BOMB_BAG_CAPACITY())) ||
            ((item == ITEM_MM_BOW) && (ammo == MM_CUR_QUIVER_CAPACITY())) ||
            ((item == ITEM_MM_STICK) && (ammo == MM_CUR_DEKU_STICKS_CAPACITY())) ||
            ((item == ITEM_MM_NUT) && (ammo == MM_CUR_DEKU_NUTS_CAPACITY())) ||
            ((item == ITEM_MM_BOMBCHU) && (ammo == gMaxBombchuMm)) ||
            ((item == ITEM_MM_POWDER_KEG) && (ammo == 1)) ||
            ((item == ITEM_MM_PICTOGRAPH_BOX) && (ammo == 1)) ||
            ((item == ITEM_MM_MAGIC_BEAN) && (ammo == 10)));
}

void KaleidoScope_DrawAmmoCountCustom(PauseContext* pauseCtx, GraphicsContext* gfxCtx, s16 item, u16 ammoIndex)
{
    s16 ammoUpperDigit;
    s16 ammo;
    s16 shownItem;
    s32 slot;
    u8 alpha;
    DrawTexRectIA8Func DrawTexRectIA8;
    u8* ammoDigit0Tex;
    s16* ammoRectLeft;
    s16* ammoRectHeight;

    shownItem = KaleidoScope_GetShownAmmoItem(item, &slot);

    if (!KaleidoScope_ItemHasAmmoCount(shownItem))
    {
        return;
    }

    ammo = KaleidoScope_GetAmmo(shownItem, slot);
    alpha = pauseCtx->itemAlpha & 0xff;

    DrawTexRectIA8 = (DrawTexRectIA8Func)KALEIDO_DRAW_TEX_RECT_IA8_ADDR;
    ammoDigit0Tex = (u8*)KALEIDO_AMMO_DIGIT_0_TEX_ADDR;
    ammoRectLeft = OverlayAddr(KALEIDO_AMMO_RECT_LEFT_ADDR);
    ammoRectHeight = OverlayAddr(KALEIDO_AMMO_RECT_HEIGHT_ADDR);

    OPEN_DISPS(gfxCtx);

    gDPPipeSync(POLY_OPA_DISP++);

    if (KaleidoScope_IsAmmoRestricted(shownItem))
    {
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 100, 100, 100, alpha);
    }
    else
    {
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, alpha);

        if (ammo == 0)
        {
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 130, 130, 130, alpha);
        }
        else if (KaleidoScope_IsAmmoAtCapacity(shownItem, ammo))
        {
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 120, 255, 0, alpha);
        }
    }

    for (ammoUpperDigit = 0; ammo >= 10; ammoUpperDigit++)
    {
        ammo -= 10;
    }

    gDPPipeSync(POLY_OPA_DISP++);

    if (ammoUpperDigit != 0)
    {
        POLY_OPA_DISP =
            DrawTexRectIA8(POLY_OPA_DISP, ammoDigit0Tex + (8 * 8 * ammoUpperDigit), 8, 8,
                           ammoRectLeft[ammoIndex], ammoRectHeight[ammoIndex], 8, 8, 1 << 10, 1 << 10);
    }

    POLY_OPA_DISP =
        DrawTexRectIA8(POLY_OPA_DISP, ammoDigit0Tex + (8 * 8 * ammo), 8, 8,
                       ammoRectLeft[ammoIndex] + 6, ammoRectHeight[ammoIndex], 8, 8, 1 << 10, 1 << 10);

    CLOSE_DISPS();
}

PATCH_FUNC(0x8081B240, KaleidoScope_DrawAmmoCountCustom);

void KaleidoScope_SetSaveButton(PlayState* play, s16 bButtonDoAction)
{
    DmaEntry dma;
    if (KaleidoScope_CanSave(play))
    {
        s16 texIndex = 14 + 29; /* DO_ACTION_SAVE + DO_ACTION_MAX */
        comboDmaLookupForeignId(&dma, 17);
        u32 textureFileAddress = dma.pstart;
        u32 textureOffset = DO_ACTION_TEX_SIZE * texIndex;
        DMARomToRam((textureFileAddress + textureOffset) | PI_DOM1_ADDR2, play->interfaceCtx.doActionSegment + 3 * DO_ACTION_TEX_SIZE, DO_ACTION_TEX_SIZE);
        play->interfaceCtx.bButtonInterfaceDoActionActive = 1;
    }
    else
    {
        Interface_SetBButtonInterfaceDoAction(play, bButtonDoAction);
    }
}

PATCH_CALL(0x80828908, KaleidoScope_SetSaveButton);
