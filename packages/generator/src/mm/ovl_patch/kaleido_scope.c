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
#include "combo/custom.h"
#include "combo/mask.h"


static u32 comboResolvePauseSlot(PlayState* play, u32 slot)
{
    if (play && play->pauseCtx.pageIndex == PAUSE_MASK)
        return slot + ITEM_NUM_SLOTS;
    return slot;
}

static int KaleidoScope_NormalizeMoonMaskSlot(u32 slot);
static void KaleidoScope_ToggleMaskSlotSkipHidden(u32 slot);

static u8 KaleidoScope_CustomMaskToItem(s32 customMask)
{
    switch (customMask)
    {
    case PLAYER_CUSTOM_MASK_GERUDO:
        return ITEM_MM_MASK_GERUDO;
    case PLAYER_CUSTOM_MASK_SKULL:
        return ITEM_MM_MASK_SKULL;
    case PLAYER_CUSTOM_MASK_SPOOKY:
        return ITEM_MM_MASK_SPOOKY;
    default:
        return ITEM_NONE;
    }
}

static s32 KaleidoScope_IsWornCustomMaskSlot(u32 slot)
{
    s32 i;
    u8 wornItem;
    u32 flags;
    const u8* table;
    u32 tableSize;
    u8* itemPtr;

    wornItem = KaleidoScope_CustomMaskToItem(gCustomSave.customMask);
    if (wornItem == ITEM_NONE)
        return 0;

    for (i = EQUIP_SLOT_C_LEFT; i <= EQUIP_SLOT_C_RIGHT; i++)
    {
        if (BUTTON_ITEM_EQUIP(0, i) == wornItem && C_SLOT_EQUIP(0, i) == slot)
            return 1;
    }
    if (comboGetSlotExtras(slot, &itemPtr, &flags, &table, &tableSize) >= 0)
        return *itemPtr == wornItem;

    return 0;
}

s32 Player_GetCurMaskItemId_Custom(PlayState* play)
{
    Player* player;
    s32 customItem;

    customItem = KaleidoScope_CustomMaskToItem(gCustomSave.customMask);
    if (customItem != ITEM_NONE)
        return customItem;

    player = GET_PLAYER(play);

    if (player->currentMask != PLAYER_MASK_NONE)
        return Player_MaskIdToItemId(player->currentMask - 1);

    return ITEM_NONE;
}

PATCH_FUNC(0x80122eec, Player_GetCurMaskItemId_Custom);

static void removeMmSlotFromCurrentCButton(u16 slot, u8 item)
{
    for (int button = 1; button < 4; ++button)
    {
        if (gMmSave.info.itemEquips.cButtonSlots[0][button] == slot ||
            gMmSave.info.itemEquips.buttonItems[0][button] == item)
        {
            gMmSave.info.itemEquips.buttonItems[0][button] = ITEM_NONE;
            gMmSave.info.itemEquips.cButtonSlots[0][button] = 0xff;
        }
    }
}

int KaleidoScope_CheckMmItemAgeReq(u8 item);

void KaleidoScope_AfterSetCutsorColor(PlayState* play)
{
    u16 cursorSlot;
    int press;
    int effect;

    /* Update Dpad */
    Dpad_Update(play);

    cursorSlot = play->pauseCtx.cursorSlot[play->pauseCtx.pageIndex];
    cursorSlot = comboResolvePauseSlot(play, cursorSlot);
    press = !!(play->state.input[0].press.button & (L_TRIG | U_CBUTTONS));
    effect = 0;

    u8* itemPtr;
    u32 flags;
    const u8* table;
    u32 tableSize;

    if (comboGetSlotExtras(cursorSlot, &itemPtr, &flags, &table, &tableSize) >= 0 &&
play->pauseCtx.cursorItem[play->pauseCtx.pageIndex] != 999 &&
popcount(flags) > 1)
    {
        if (play->pauseCtx.pageIndex == PAUSE_MASK)
        {
            if (KaleidoScope_NormalizeMoonMaskSlot(cursorSlot))
                effect = 1;
        }

        play->pauseCtx.cursorColorIndex = 4;

        if (press)
        {
            if (KaleidoScope_IsWornCustomMaskSlot(cursorSlot))
            {
                PlaySound(0x4806); /* NA_SE_SY_ERROR */
                return;
            }

            if (play->pauseCtx.pageIndex == PAUSE_MASK)
                KaleidoScope_ToggleMaskSlotSkipHidden(cursorSlot);
            else
                comboToggleSlot(cursorSlot);
            if (!KaleidoScope_CheckMmItemAgeReq(*itemPtr))
            {
                removeMmSlotFromCurrentCButton(cursorSlot, *itemPtr);
            }

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
    case ITEM_MM_MASK_ADULT:
        {
        void* src = comboCacheGetFile(CUSTOM_ADULT_MASK_TEXT_ADDR);

        if (src)
            memcpy(segment, src, 0x400);
        else
            bzero(segment, 0x400);

        return;
        }
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
    case ITEM_MM_SLINGSHOT:
        isForeign = 1;
        texIndex = 0x7b + ITEM_OOT_SLINGSHOT;
        break;
    case ITEM_MM_RUTO_LETTER:
        isForeign = 1;
        texIndex = 0x7b + ITEM_OOT_RUTO_LETTER;
        break;
    case ITEM_MM_MASK_GERUDO:
        isForeign = 1;
        texIndex = 0x7b + ITEM_OOT_GERUDO_MASK;
        break;
    case ITEM_MM_MASK_SKULL:
        isForeign = 1;
        texIndex = 0x7b + ITEM_OOT_SKULL_MASK;
        break;
    case ITEM_MM_MASK_SPOOKY:
        isForeign = 1;
        texIndex = 0x7b + ITEM_OOT_SPOOKY_MASK;
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
        comboTextAppendStr(&b, " to throw and watch it come" TEXT_NL "back! The boomerang can stun or" TEXT_NL "defeat enemies!" TEXT_END);
        break;
    case ITEM_MM_SLINGSHOT:
        b = play->msgCtx.font.textBuffer.schar;
        b[2] = 0xFE; /* Use No Icon */
        b += 11;
        comboTextAppendStr(&b, TEXT_COLOR_RED "Fairy Slingshot" TEXT_NL);
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, "Press " TEXT_COLOR_YELLOW "\xB2");
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, " to unleash a deku seed" TEXT_NL "at your target!" TEXT_END);
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
    case ITEM_MM_MASK_GERUDO:
        b = play->msgCtx.font.textBuffer.schar;
        b[2] = 0xFE; /* Use No Icon */
        b += 11;
        comboTextAppendStr(&b, TEXT_COLOR_RED "Gerudo Mask" TEXT_NL);
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, "With its charming eyes, it makes" TEXT_NL "a great lady's disguise." TEXT_END);
        break;
    case ITEM_MM_MASK_SKULL:
            b = play->msgCtx.font.textBuffer.schar;
        b[2] = 0xFE; /* Use No Icon */
        b += 11;
        comboTextAppendStr(&b, TEXT_COLOR_RED "Skull Mask" TEXT_NL);
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, "A mysterious aura emanates from" TEXT_NL "this mask." TEXT_END);
        break;
    case ITEM_MM_MASK_SPOOKY:
            b = play->msgCtx.font.textBuffer.schar;
        b[2] = 0xFE; /* Use No Icon */
        b += 11;
        comboTextAppendStr(&b, TEXT_COLOR_RED "Spooky Mask" TEXT_NL);
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, "This mask was manufactured from" TEXT_NL "the plank of a coffin." TEXT_END);
        break;
    case ITEM_MM_MASK_ADULT:
            b = play->msgCtx.font.textBuffer.schar;
        b[2] = 0xFE; /* Use No Icon */
        b += 11;
        comboTextAppendStr(&b, TEXT_COLOR_RED "Adult Mask" TEXT_NL);
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, "Wear it with " TEXT_COLOR_YELLOW "\xB2");
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, " to assume Adult" TEXT_NL);
        comboTextAppendStr(&b, "form. Use " TEXT_COLOR_YELLOW "\xB2");
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, " to change back." TEXT_END);
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
    ITEM_MM_SLINGSHOT,
    ITEM_MM_RUTO_LETTER,
    ITEM_MM_MASK_GERUDO,
    ITEM_MM_MASK_SKULL,
    ITEM_MM_MASK_SPOOKY,
    ITEM_MM_MASK_ADULT,
};

typedef enum MmAgeReq {
    MM_AGE_REQ_NONE  = 0,
    MM_AGE_REQ_CHILD = 1,
    MM_AGE_REQ_ADULT = 2,
} MmAgeReq;

typedef struct MmItemAgeReqConfig {
    u8 item;
    u16 adultFlag;
    u16 childFlag;
} MmItemAgeReqConfig;

static const MmItemAgeReqConfig kMmItemAgeReqConfigs[] = {
    { ITEM_MM_HOOKSHOT,           CFG_MM_AGE_REQ_ADULT_HOOKSHOT,           CFG_MM_AGE_REQ_CHILD_HOOKSHOT },
    { ITEM_MM_HOOKSHOT_SHORT,     CFG_MM_AGE_REQ_ADULT_HOOKSHOT_SHORT,     CFG_MM_AGE_REQ_CHILD_HOOKSHOT_SHORT },
    { ITEM_MM_BOW,                CFG_MM_AGE_REQ_ADULT_BOW,                CFG_MM_AGE_REQ_CHILD_BOW },
    { ITEM_MM_ARROW_FIRE,         CFG_MM_AGE_REQ_ADULT_ARROW_FIRE,         CFG_MM_AGE_REQ_CHILD_ARROW_FIRE },
    { ITEM_MM_ARROW_ICE,          CFG_MM_AGE_REQ_ADULT_ARROW_ICE,          CFG_MM_AGE_REQ_CHILD_ARROW_ICE },
    { ITEM_MM_ARROW_LIGHT,        CFG_MM_AGE_REQ_ADULT_ARROW_LIGHT,        CFG_MM_AGE_REQ_CHILD_ARROW_LIGHT },
    { ITEM_MM_OCARINA_OF_TIME,    CFG_MM_AGE_REQ_ADULT_OCARINA_OF_TIME,    CFG_MM_AGE_REQ_CHILD_OCARINA_OF_TIME },
    { ITEM_MM_OCARINA_FAIRY,      CFG_MM_AGE_REQ_ADULT_OCARINA_FAIRY,      CFG_MM_AGE_REQ_CHILD_OCARINA_FAIRY },
    { ITEM_MM_BOMB,               CFG_MM_AGE_REQ_ADULT_BOMB,               CFG_MM_AGE_REQ_CHILD_BOMB },
    { ITEM_MM_BOMBCHU,            CFG_MM_AGE_REQ_ADULT_BOMBCHU,            CFG_MM_AGE_REQ_CHILD_BOMBCHU },
    { ITEM_MM_STICK,              CFG_MM_AGE_REQ_ADULT_STICK,              CFG_MM_AGE_REQ_CHILD_STICK },
    { ITEM_MM_NUT,                CFG_MM_AGE_REQ_ADULT_DEKU_NUTS,          CFG_MM_AGE_REQ_CHILD_DEKU_NUTS },
    { ITEM_MM_MAGIC_BEAN,         CFG_MM_AGE_REQ_ADULT_MAGIC_BEAN,         CFG_MM_AGE_REQ_CHILD_MAGIC_BEAN },
    { ITEM_MM_POWDER_KEG,         CFG_MM_AGE_REQ_ADULT_POWDER_KEG,         CFG_MM_AGE_REQ_CHILD_POWDER_KEG },
    { ITEM_MM_PICTOGRAPH_BOX,     CFG_MM_AGE_REQ_ADULT_PICTOGRAPH_BOX,     CFG_MM_AGE_REQ_CHILD_PICTOGRAPH_BOX },
    { ITEM_MM_LENS_OF_TRUTH,      CFG_MM_AGE_REQ_ADULT_LENS_OF_TRUTH,      CFG_MM_AGE_REQ_CHILD_LENS_OF_TRUTH },
    { ITEM_MM_GREAT_FAIRY_SWORD,  CFG_MM_AGE_REQ_ADULT_GREAT_FAIRY_SWORD,  CFG_MM_AGE_REQ_CHILD_GREAT_FAIRY_SWORD },
    { ITEM_MM_HAMMER,             CFG_MM_AGE_REQ_ADULT_HAMMER,             CFG_MM_AGE_REQ_CHILD_HAMMER },
    { ITEM_MM_BOOTS_IRON,         CFG_MM_AGE_REQ_ADULT_BOOTS_IRON,         CFG_MM_AGE_REQ_CHILD_BOOTS_IRON },
    { ITEM_MM_BOOTS_HOVER,        CFG_MM_AGE_REQ_ADULT_BOOTS_HOVER,        CFG_MM_AGE_REQ_CHILD_BOOTS_HOVER },
    { ITEM_MM_TUNIC_GORON,        CFG_MM_AGE_REQ_ADULT_TUNIC_GORON,        CFG_MM_AGE_REQ_CHILD_TUNIC_GORON },
    { ITEM_MM_TUNIC_ZORA,         CFG_MM_AGE_REQ_ADULT_TUNIC_ZORA,         CFG_MM_AGE_REQ_CHILD_TUNIC_ZORA },
    { ITEM_MM_SPELL_WIND,         CFG_MM_AGE_REQ_ADULT_SPELL_WIND,         CFG_MM_AGE_REQ_CHILD_SPELL_WIND },
    { ITEM_MM_SPELL_LOVE,         CFG_MM_AGE_REQ_ADULT_SPELL_LOVE,         CFG_MM_AGE_REQ_CHILD_SPELL_LOVE },
    { ITEM_MM_SPELL_FIRE,         CFG_MM_AGE_REQ_ADULT_SPELL_FIRE,         CFG_MM_AGE_REQ_CHILD_SPELL_FIRE },
    { ITEM_MM_BOOMERANG,          CFG_MM_AGE_REQ_ADULT_BOOMERANG,          CFG_MM_AGE_REQ_CHILD_BOOMERANG },
    { ITEM_MM_SLINGSHOT,          CFG_MM_AGE_REQ_ADULT_SLINGSHOT,          CFG_MM_AGE_REQ_CHILD_SLINGSHOT },
    { ITEM_MM_MASK_DEKU,          CFG_MM_AGE_REQ_ADULT_MASK_DEKU,          CFG_MM_AGE_REQ_CHILD_MASK_DEKU },
    { ITEM_MM_MASK_GORON,         CFG_MM_AGE_REQ_ADULT_MASK_GORON,         CFG_MM_AGE_REQ_CHILD_MASK_GORON },
    { ITEM_MM_MASK_ZORA,          CFG_MM_AGE_REQ_ADULT_MASK_ZORA,          CFG_MM_AGE_REQ_CHILD_MASK_ZORA },
    { ITEM_MM_MASK_FIERCE_DEITY,  CFG_MM_AGE_REQ_ADULT_MASK_FIERCE_DEITY,  CFG_MM_AGE_REQ_CHILD_MASK_FIERCE_DEITY },
    { ITEM_MM_MASK_TRUTH,         CFG_MM_AGE_REQ_ADULT_MASK_TRUTH,         CFG_MM_AGE_REQ_CHILD_MASK_TRUTH },
    { ITEM_MM_MASK_KAFEI,         CFG_MM_AGE_REQ_ADULT_MASK_KAFEI,         CFG_MM_AGE_REQ_CHILD_MASK_KAFEI },
    { ITEM_MM_MASK_ALL_NIGHT,     CFG_MM_AGE_REQ_ADULT_MASK_ALL_NIGHT,     CFG_MM_AGE_REQ_CHILD_MASK_ALL_NIGHT },
    { ITEM_MM_MASK_BUNNY,         CFG_MM_AGE_REQ_ADULT_MASK_BUNNY,         CFG_MM_AGE_REQ_CHILD_MASK_BUNNY },
    { ITEM_MM_MASK_KEATON,        CFG_MM_AGE_REQ_ADULT_MASK_KEATON,        CFG_MM_AGE_REQ_CHILD_MASK_KEATON },
    { ITEM_MM_MASK_GARO,          CFG_MM_AGE_REQ_ADULT_MASK_GARO,          CFG_MM_AGE_REQ_CHILD_MASK_GARO },
    { ITEM_MM_MASK_ROMANI,        CFG_MM_AGE_REQ_ADULT_MASK_ROMANI,        CFG_MM_AGE_REQ_CHILD_MASK_ROMANI },
    { ITEM_MM_MASK_TROUPE_LEADER, CFG_MM_AGE_REQ_ADULT_MASK_TROUPE_LEADER, CFG_MM_AGE_REQ_CHILD_MASK_TROUPE_LEADER },
    { ITEM_MM_MASK_POSTMAN,       CFG_MM_AGE_REQ_ADULT_MASK_POSTMAN,       CFG_MM_AGE_REQ_CHILD_MASK_POSTMAN },
    { ITEM_MM_MASK_COUPLE,        CFG_MM_AGE_REQ_ADULT_MASK_COUPLE,        CFG_MM_AGE_REQ_CHILD_MASK_COUPLE },
    { ITEM_MM_MASK_GREAT_FAIRY,   CFG_MM_AGE_REQ_ADULT_MASK_GREAT_FAIRY,   CFG_MM_AGE_REQ_CHILD_MASK_GREAT_FAIRY },
    { ITEM_MM_MASK_GIBDO,         CFG_MM_AGE_REQ_ADULT_MASK_GIBDO,         CFG_MM_AGE_REQ_CHILD_MASK_GIBDO },
    { ITEM_MM_MASK_DON_GERO,      CFG_MM_AGE_REQ_ADULT_MASK_DON_GERO,      CFG_MM_AGE_REQ_CHILD_MASK_DON_GERO },
    { ITEM_MM_MASK_KAMARO,        CFG_MM_AGE_REQ_ADULT_MASK_KAMARO,        CFG_MM_AGE_REQ_CHILD_MASK_KAMARO },
    { ITEM_MM_MASK_CAPTAIN,       CFG_MM_AGE_REQ_ADULT_MASK_CAPTAIN,       CFG_MM_AGE_REQ_CHILD_MASK_CAPTAIN },
    { ITEM_MM_MASK_STONE,         CFG_MM_AGE_REQ_ADULT_MASK_STONE,         CFG_MM_AGE_REQ_CHILD_MASK_STONE },
    { ITEM_MM_MASK_BREMEN,        CFG_MM_AGE_REQ_ADULT_MASK_BREMEN,        CFG_MM_AGE_REQ_CHILD_MASK_BREMEN },
    { ITEM_MM_MASK_BLAST,         CFG_MM_AGE_REQ_ADULT_MASK_BLAST,         CFG_MM_AGE_REQ_CHILD_MASK_BLAST },
    { ITEM_MM_MASK_SCENTS,        CFG_MM_AGE_REQ_ADULT_MASK_SCENTS,        CFG_MM_AGE_REQ_CHILD_MASK_SCENTS },
    { ITEM_MM_MASK_GIANT,         CFG_MM_AGE_REQ_ADULT_MASK_GIANT,         CFG_MM_AGE_REQ_CHILD_MASK_GIANT },
    { ITEM_MM_MASK_GERUDO,        CFG_MM_AGE_REQ_ADULT_MASK_GERUDO,        CFG_MM_AGE_REQ_CHILD_MASK_GERUDO },
    { ITEM_MM_MASK_SKULL,         CFG_MM_AGE_REQ_ADULT_MASK_SKULL,         CFG_MM_AGE_REQ_CHILD_MASK_SKULL },
    { ITEM_MM_MASK_SPOOKY,        CFG_MM_AGE_REQ_ADULT_MASK_SPOOKY,        CFG_MM_AGE_REQ_CHILD_MASK_SPOOKY },
    { ITEM_MM_MOON_TEAR,          CFG_MM_AGE_REQ_ADULT_MOON_TEAR,          CFG_MM_AGE_REQ_CHILD_MOON_TEAR },
    { ITEM_MM_DEED_LAND,          CFG_MM_AGE_REQ_ADULT_DEED_LAND,          CFG_MM_AGE_REQ_CHILD_DEED_LAND },
    { ITEM_MM_DEED_SWAMP,         CFG_MM_AGE_REQ_ADULT_DEED_SWAMP,         CFG_MM_AGE_REQ_CHILD_DEED_SWAMP },
    { ITEM_MM_DEED_MOUNTAIN,      CFG_MM_AGE_REQ_ADULT_DEED_MOUNTAIN,      CFG_MM_AGE_REQ_CHILD_DEED_MOUNTAIN },
    { ITEM_MM_DEED_OCEAN,         CFG_MM_AGE_REQ_ADULT_DEED_OCEAN,         CFG_MM_AGE_REQ_CHILD_DEED_OCEAN },
    { ITEM_MM_LETTER_TO_MAMA,     CFG_MM_AGE_REQ_ADULT_LETTER_TO_MAMA,     CFG_MM_AGE_REQ_CHILD_LETTER_TO_MAMA },
    { ITEM_MM_LETTER_TO_KAFEI,    CFG_MM_AGE_REQ_ADULT_LETTER_TO_KAFEI,    CFG_MM_AGE_REQ_CHILD_LETTER_TO_KAFEI },
    { ITEM_MM_PENDANT_OF_MEMORIES,CFG_MM_AGE_REQ_ADULT_PENDANT_OF_MEMORIES,CFG_MM_AGE_REQ_CHILD_PENDANT_OF_MEMORIES },
    { ITEM_NONE, 0, 0 },
};

static u8 KaleidoScope_GetMmItemAgeReq(u8 item)
{
    const MmItemAgeReqConfig* entry;
    for (u32 i = 0; kMmItemAgeReqConfigs[i].item != ITEM_NONE; i++)
    {
        entry = &kMmItemAgeReqConfigs[i];
        if (entry->item != item)
            continue;
        if (Config_Flag(entry->adultFlag))
            return MM_AGE_REQ_ADULT;
        if (Config_Flag(entry->childFlag))
            return MM_AGE_REQ_CHILD;
        return MM_AGE_REQ_NONE;
    }
    return MM_AGE_REQ_NONE;
}

static int KaleidoScope_CheckMmAgeReqValue(u8 req)
{
    switch (req)
    {
        case MM_AGE_REQ_ADULT:
            return comboIsLinkAdult();

        case MM_AGE_REQ_CHILD:
            return !comboIsLinkAdult();

        default:
            return 1;
    }
}

int KaleidoScope_CheckMmItemAgeReq(u8 item)
{
    if (item == ITEM_NONE || item >= 0xff)
        return 1;

    return KaleidoScope_CheckMmAgeReqValue(KaleidoScope_GetMmItemAgeReq(item));
}

#define PLAYER_STATE1_MOUNTED 0x00800000

static u8 sHorseBowManaged;
static u8 sHorsePreviousBItem;

static void Interface_UpdateButtons_Horse(PlayState* play)
{
    Player* player;
    u8* bItem;
    u8 savedBowItem;
    u8 itemBefore;
    int mounted;
    int spoofBow;

    player = GET_PLAYER(play);
    bItem = &gMmSave.info.itemEquips.buttonItems[0][EQUIP_SLOT_B];
    mounted = !!(player->stateFlags1 & PLAYER_STATE1_MOUNTED);
    if (sHorseBowManaged)
    {
        gSaveContext.buttonStatus[EQUIP_SLOT_B] = sHorsePreviousBItem;

        if (*bItem == ITEM_NONE)
            *bItem = ITEM_MM_BOW;
    }

    itemBefore = *bItem;
    savedBowItem = gMmSave.info.inventory.items[ITS_MM_BOW];
    spoofBow = mounted && savedBowItem == ITEM_NONE;
    if (spoofBow)
        gMmSave.info.inventory.items[ITS_MM_BOW] = ITEM_MM_BOW;
    ((void (*)(PlayState*))0x80111cb4)(play);

    if (spoofBow)
        gMmSave.info.inventory.items[ITS_MM_BOW] = savedBowItem;
    player = GET_PLAYER(play);
    mounted = !!(player->stateFlags1 & PLAYER_STATE1_MOUNTED);
    if (!mounted)
    {
        sHorseBowManaged = 0;
        return;
    }
    if (*bItem == ITEM_MM_BOW &&
        itemBefore != ITEM_MM_BOW &&
        itemBefore != ITEM_MM_BOMB &&
        itemBefore != ITEM_MM_BOMBCHU)
    {
        sHorsePreviousBItem = itemBefore;
        sHorseBowManaged = 1;
    }

    if (!sHorseBowManaged)
        return;
    if (*bItem != ITEM_MM_BOW)
    {
        sHorseBowManaged = 0;
        return;
    }
    if (!KaleidoScope_CheckMmItemAgeReq(ITEM_MM_BOW))
        *bItem = ITEM_NONE;
}

PATCH_CALL(0x80112e68, Interface_UpdateButtons_Horse);
PATCH_CALL(0x80121140, Interface_UpdateButtons_Horse);

static u8 KaleidoScope_GetMmSlotItem(u16 slot)
{
    u8* itemPtr;
    u32 flags;
    const u8* table;
    u32 tableSize;

    if (slot > ITS_MM_MASK_FIERCE_DEITY)
        return ITEM_NONE;

    if (comboGetSlotExtras(slot, &itemPtr, &flags, &table, &tableSize) >= 0)
        return *itemPtr;

    return gMmSave.info.inventory.items[slot];
}

int KaleidoScope_CheckMmSlotAgeReq(u16 slot)
{
    u8 item;

    item = KaleidoScope_GetMmSlotItem(slot);

    if (item != ITEM_NONE && item < 0xff)
        return KaleidoScope_CheckMmItemAgeReq(item);

    return 1;
}

s8 gPlayerFormCustomItemRestrictions[5][ITEM_MM_CUSTOM_USABLE_MAX - ITEM_MM_CUSTOM_MIN] =
{
    { 0, 0, 0,  0,  0,  0,  0, 0, 0, 0, 1, 0, 0, 0, 0 },
    { 0, 0, 0,  0,  0,  0,  0, 0, 0, 0, 1, 0, 0, 0, 0 },
    { 0, 0, 0,  0,  0,  0,  0, 0, 0, 0, 1, 0, 0, 0, 0 },
    { 0, 0, 0,  0,  0,  0,  0, 0, 0, 0, 1, 0, 0, 0, 0 },
    { 1, 1, 1, -1, -1, -1, -1, 1, 1, 1, 1, 1, 1, 1, 1 },
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
        case ITEM_MM_SLINGSHOT:
            foreignIcon = ITEM_OOT_SLINGSHOT;
            break;
        case ITEM_MM_RUTO_LETTER:
            foreignIcon = ITEM_OOT_RUTO_LETTER;
            break;
        case ITEM_MM_MASK_GERUDO:
            foreignIcon = ITEM_OOT_GERUDO_MASK;
            break;
        case ITEM_MM_MASK_SKULL:
            foreignIcon = ITEM_OOT_SKULL_MASK;
            break;
        case ITEM_MM_MASK_SPOOKY:
            foreignIcon = ITEM_OOT_SPOOKY_MASK;
            break;
        case ITEM_MM_MASK_ADULT:
        {
            void* src;
            u32 customDestination;

            customDestination = gCustomIconAddr + (i * customIconSize);
            src = comboCacheGetFile(CUSTOM_ADULT_MASK_ICON_ADDR);

            if (src)
                memcpy((void*)customDestination, src, customIconSize);

            *size += customIconSize;
            continue;
        }
        default:
            continue;
        }
        u32 textureOffset = customIconSize * foreignIcon;
        u32 customDestination = gCustomIconAddr + (i * customIconSize);
        DMARomToRam((textureFileAddress + textureOffset) | PI_DOM1_ADDR2, (void*)customDestination, customIconSize);

        u8 customItemIndex = icon - ITEM_MM_CUSTOM_MIN;
        if (customItemIndex >= (ITEM_MM_CUSTOM_USABLE_MAX - ITEM_MM_CUSTOM_MIN) || !gPlayerFormCustomItemRestrictions[gSaveContext.save.playerForm][customItemIndex] || !KaleidoScope_CheckMmItemAgeReq(icon))
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
static Vtx gVertexBufs[(4 * 12) * 2];

/* Vertex buffer pointers. */
static Vtx* gVertex[12] = {
    &gVertexBufs[(4 * 0) * 2],
    &gVertexBufs[(4 * 1) * 2],
    &gVertexBufs[(4 * 2) * 2],
    &gVertexBufs[(4 * 3) * 2],
    &gVertexBufs[(4 * 4) * 2],
    &gVertexBufs[(4 * 5) * 2],
    &gVertexBufs[(4 * 6) * 2],
    &gVertexBufs[(4 * 7) * 2],
    &gVertexBufs[(4 * 8) * 2],
    &gVertexBufs[(4 * 9) * 2],
    &gVertexBufs[(4 * 10) * 2],
    &gVertexBufs[(4 * 11) * 2],
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

#define MOON_MASK_BIT(i, f) ((u16)(((i) << 8) | (f)))
#define MOON_MASK_BYTES     ((u8*)0x801f3f3a)
#define MOON_MASK_GIVEN(i)  (MOON_MASK_BYTES[sMasksGivenOnMoonBits_Custom[i] >> 8] & (u8)sMasksGivenOnMoonBits_Custom[i])

static const u16 sMasksGivenOnMoonBits_Custom[MASK_NUM_SLOTS] = {
    MOON_MASK_BIT(1, 0x01), MOON_MASK_BIT(0, 0x04), MOON_MASK_BIT(2, 0x02), MOON_MASK_BIT(1, 0x80),
    MOON_MASK_BIT(1, 0x04), MOON_MASK_BIT(2, 0x10), MOON_MASK_BIT(0, 0x10), MOON_MASK_BIT(2, 0x01),
    MOON_MASK_BIT(0, 0x08), MOON_MASK_BIT(1, 0x10), MOON_MASK_BIT(2, 0x04), MOON_MASK_BIT(2, 0x20),
    MOON_MASK_BIT(0, 0x40), MOON_MASK_BIT(0, 0x80), MOON_MASK_BIT(0, 0x02), MOON_MASK_BIT(1, 0x02),
    MOON_MASK_BIT(0, 0x01), MOON_MASK_BIT(2, 0x40), MOON_MASK_BIT(1, 0x20), MOON_MASK_BIT(1, 0x08),
    MOON_MASK_BIT(0, 0x20), MOON_MASK_BIT(1, 0x40), MOON_MASK_BIT(2, 0x08), MOON_MASK_BIT(2, 0x80),
};

typedef struct {
    u32 flags;
    u32 tableSize;
    u8* itemPtr;
    const u8* table;
    s32 index;
} MaskSlotExtras;

static int GetMaskSlotExtras(u32 slot, MaskSlotExtras* e) {
    e->index = comboGetSlotExtras(slot, &e->itemPtr, &e->flags, &e->table, &e->tableSize);
    return e->index >= 0 && e->tableSize != 0;
}

static int KaleidoScope_IsMoonGivenParam0Mask(u32 slot, u8 item) {
    u32 maskSlot = slot - ITEM_NUM_SLOTS;
    MaskSlotExtras e;
    if (slot < ITEM_NUM_SLOTS || maskSlot >= MASK_NUM_SLOTS || !MOON_MASK_GIVEN(maskSlot))
        return 0;
    return GetMaskSlotExtras(slot, &e) ? item == e.table[0] : item == gSave.info.inventory.items[slot];
}

static s32 GetMaskItemTableIndex(u32 slot, u8 item) {
    MaskSlotExtras e;
    s32 i;
    if (!GetMaskSlotExtras(slot, &e))
        return -1;
    for (i = 0; i < (s32)e.tableSize; i++)
        if (e.table[i] == item)
            return i;
    return -1;
}

static u8 GetNextVisibleMaskTrade(u32 slot, u8 currentItem) {
    MaskSlotExtras e;
    u8 next, probe = currentItem;
    u32 i;
    if (!GetMaskSlotExtras(slot, &e))
        return ITEM_NONE;
    for (i = 0; i < e.tableSize; i++) {
        next = comboGetNextTrade(probe, e.flags, e.table, e.tableSize);
        if (next == ITEM_NONE || next == currentItem)
            return ITEM_NONE;
        if (!KaleidoScope_IsMoonGivenParam0Mask(slot, next))
            return next;
        probe = next;
    }
    return ITEM_NONE;
}

static u8 GetCurrentVisibleMaskItem(u32 slot, s32* outActiveIndex) {
    MaskSlotExtras e;
    u8 next;
    if (!GetMaskSlotExtras(slot, &e)) {
        *outActiveIndex = -1;
        return ITEM_NONE;
    }
    *outActiveIndex = e.index;
    if (!KaleidoScope_IsMoonGivenParam0Mask(slot, *e.itemPtr))
        return *e.itemPtr;
    next = GetNextVisibleMaskTrade(slot, *e.itemPtr);
    if (next == ITEM_NONE)
        return ITEM_NONE;
    *e.itemPtr = next;
    *outActiveIndex = GetMaskItemTableIndex(slot, next);
    return next;
}

static int KaleidoScope_NormalizeMoonMaskSlot(u32 slot) {
    MaskSlotExtras e;
    u8 next;
    if (slot < ITEM_NUM_SLOTS || slot >= ITEM_NUM_SLOTS + MASK_NUM_SLOTS || !GetMaskSlotExtras(slot, &e))
        return 0;
    if (!KaleidoScope_IsMoonGivenParam0Mask(slot, *e.itemPtr))
        return 0;
    next = GetNextVisibleMaskTrade(slot, *e.itemPtr);
    if (next == ITEM_NONE)
        return 0;
    *e.itemPtr = next;
    return 1;
}

static u8 GetNextVisibleMaskOverlayItem(u32 slot, u8 currentItem, s32* outVtxBufferIndex) {
    MaskSlotExtras e;
    u8 next;
    if (!GetMaskSlotExtras(slot, &e)) {
        *outVtxBufferIndex = -1;
        return ITEM_NONE;
    }
    *outVtxBufferIndex = e.index;
    next = GetNextVisibleMaskTrade(slot, currentItem);
    return next != currentItem ? next : ITEM_NONE;
}

static void KaleidoScope_ToggleMaskSlotSkipHidden(u32 slot) {
    MaskSlotExtras e;
    u32 i;
    if (!GetMaskSlotExtras(slot, &e))
        return;
    for (i = 0; i < e.tableSize; i++) {
        comboToggleSlot(slot);
        if (!GetMaskSlotExtras(slot, &e) || !KaleidoScope_IsMoonGivenParam0Mask(slot, *e.itemPtr))
            return;
    }
}

u16 KaleidoScope_ResolveMoonMaskCursorItem(u16 maskSlot, u16 cursorItem) {
    u32 slot = maskSlot + ITEM_NUM_SLOTS;
    s32 activeIndex;
    u8 activeItem;
    if (cursorItem == ITEM_NONE || cursorItem == 999 || !KaleidoScope_IsMoonGivenParam0Mask(slot, cursorItem))
        return cursorItem;
    activeItem = GetCurrentVisibleMaskItem(slot, &activeIndex);
    return activeItem != ITEM_NONE ? activeItem : ITEM_NONE;
}

void KaleidoScope_DrawMaskIconCustom(GraphicsContext* gfxCtx, u8 item, u16 width, u16 height, u32 maskSlot, u16 point, u16 vertIdx)
{
    u32 slot;
    u32 texture;
    s32 tableIndex;
    u8 primary;
    u8 next;
    KaleidoScope_DrawIcon KaleidoScope_DrawIcon;

    maskSlot = vertIdx >> 2;
    slot = maskSlot + ITEM_NUM_SLOTS;

    KaleidoScope_DrawIcon = OverlayAddr(0x80821ad4);
    if (!KaleidoScope_IsMoonGivenParam0Mask(slot, item))
    {
        texture = GetItemTexture(item);
        KaleidoScope_DrawIcon(gfxCtx, texture, width, height, point);

        next = GetNextVisibleMaskOverlayItem(slot, item, &tableIndex);
        if (next != ITEM_NONE && next != item && tableIndex >= 0)
        {
            texture = GetItemTexture(next);
            Vtx* vtx = GetVtxBuffer(gfxCtx->play, vertIdx, tableIndex);
            DrawIcon(gfxCtx, vtx, texture, width, height, point);
        }
        return;
    }
    primary = GetCurrentVisibleMaskItem(slot, &tableIndex);
    if (primary == ITEM_NONE)
        return;
    texture = GetItemTexture(primary);
    KaleidoScope_DrawIcon(gfxCtx, texture, width, height, point);
    next = GetNextVisibleMaskOverlayItem(slot, primary, &tableIndex);
    if (next != ITEM_NONE && next != primary && tableIndex >= 0)
    {
        texture = GetItemTexture(next);
        Vtx* vtx = GetVtxBuffer(gfxCtx->play, vertIdx, tableIndex);
        DrawIcon(gfxCtx, vtx, texture, width, height, point);
    }
}

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

typedef void (*KaleidoScope_DrawAmmoCount)(PauseContext*, GraphicsContext*, s16, u16);

const static u8* gAmmoDigit0Tex = (u8*)0x02004aa0;

static s16 sAmmoRectLeft[] = {
    95,  // SLOT_BOW
    62,  // SLOT_BOMB
    95,  // SLOT_BOMBCHU
    128, // SLOT_DEKU_STICK
    161, // SLOT_DEKU_NUT
    194, // SLOT_MAGIC_BEANS
    62,  // SLOT_POWDER_KEG
    95,  // SLOT_PICTOGRAPH_BOX
};

static s16 sAmmoRectHeight[] = {
    85,  // SLOT_BOW
    117, // SLOT_BOMB
    117, // SLOT_BOMBCHU
    117, // SLOT_DEKU_STICK
    117, // SLOT_DEKU_NUT
    117, // SLOT_MAGIC_BEANS
    150, // SLOT_POWDER_KEG
    150, // SLOT_PICTOGRAPH_BOX
};

void KaleidoScope_CustomDrawAmmoCount(PauseContext* pauseCtx, GraphicsContext* gfxCtx, s16 item, u16 ammoIndex)
{
    s16 ammo;
    s16 ammoTens;
    s16 maxAmmo;
    s32 canEquip = 0;

    OPEN_DISPS(gfxCtx);

    switch (item)
    {
    case ITEM_MM_BOW:
        ammo = gSave.info.inventory.ammo[ITS_MM_BOW];
        maxAmmo = kMaxArrows[gMmSave.info.inventory.upgrades.quiver];
        canEquip = gPlayerFormItemRestrictions[gSaveContext.save.playerForm][item] && KaleidoScope_CheckMmItemAgeReq(item);
        break;

    case ITEM_MM_BOMB:
        ammo = gSave.info.inventory.ammo[ITS_MM_BOMBS];
        maxAmmo = kMaxBombs[gMmSave.info.inventory.upgrades.bombBag];
        canEquip = gPlayerFormItemRestrictions[gSaveContext.save.playerForm][item] && KaleidoScope_CheckMmItemAgeReq(item);
        break;

    case ITEM_MM_BOMBCHU:
        ammo = gSave.info.inventory.ammo[ITS_MM_BOMBCHU];
        maxAmmo = gMaxBombchuMm;
        canEquip = gPlayerFormItemRestrictions[gSaveContext.save.playerForm][item] && KaleidoScope_CheckMmItemAgeReq(item);
        break;

    case ITEM_MM_STICK:
        ammo = gSave.info.inventory.ammo[ITS_MM_STICKS];
        maxAmmo = kMaxSticks[gMmSave.info.inventory.upgrades.dekuStick];
        canEquip = gPlayerFormItemRestrictions[gSaveContext.save.playerForm][item] && KaleidoScope_CheckMmItemAgeReq(item);
        break;

    case ITEM_MM_NUT:
        ammo = gSave.info.inventory.ammo[ITS_MM_NUTS];
        maxAmmo = kMaxNuts[gMmSave.info.inventory.upgrades.dekuNut];
        canEquip = gPlayerFormItemRestrictions[gSaveContext.save.playerForm][item] && KaleidoScope_CheckMmItemAgeReq(item);
        break;

    case ITEM_MM_MAGIC_BEAN:
        ammo = gSave.info.inventory.ammo[ITS_MM_BEANS];
        maxAmmo = 20;
        canEquip = gPlayerFormItemRestrictions[gSaveContext.save.playerForm][item] && KaleidoScope_CheckMmItemAgeReq(item);
        break;

    case ITEM_MM_POWDER_KEG:
        ammo = gSave.info.inventory.ammo[ITS_MM_KEG];
        maxAmmo = 1;
        canEquip = gPlayerFormItemRestrictions[gSaveContext.save.playerForm][item] && KaleidoScope_CheckMmItemAgeReq(item);
        break;

    case ITEM_MM_PICTOGRAPH_BOX:
        ammo = (gSave.info.inventory.quest.value & (1 << 0x19)) ? 1 : 0;
        maxAmmo = 1;
        canEquip = gPlayerFormItemRestrictions[gSaveContext.save.playerForm][item] && KaleidoScope_CheckMmItemAgeReq(item);
        break;

    case ITEM_MM_SLINGSHOT:
        ammo = gMmExtraAmmo.slingshotSeeds;
        maxAmmo = kMaxSeeds[gMmSave.info.inventory.upgrades.bulletBag];
        canEquip = gPlayerFormCustomItemRestrictions[gSaveContext.save.playerForm][item - ITEM_MM_CUSTOM_MIN] && KaleidoScope_CheckMmItemAgeReq(item);
        break;
    default:
        return;
    }

    gDPPipeSync(POLY_OPA_DISP++);

    if (!canEquip) {
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 100, 100, 100, pauseCtx->itemAlpha);
    } else {
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, pauseCtx->itemAlpha);

        if (ammo == 0) {
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 130, 130, 130, pauseCtx->itemAlpha);
        } else if (ammo == maxAmmo) {
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 120, 255, 0, pauseCtx->itemAlpha);
        }
    }

    for (ammoTens = 0; ammo >= 10; ammoTens++) {
        ammo -= 10;
    }

    gDPPipeSync(POLY_OPA_DISP++);

    if (ammoTens != 0) {
        POLY_OPA_DISP =
            Gfx_TextureIA8(POLY_OPA_DISP, ((u8*)gAmmoDigit0Tex + (8 * 8 * ammoTens)), 8, 8,
                               sAmmoRectLeft[ammoIndex], sAmmoRectHeight[ammoIndex], 8, 8, 1 << 10, 1 << 10);
    }

    POLY_OPA_DISP =
        Gfx_TextureIA8(POLY_OPA_DISP, ((u8*)gAmmoDigit0Tex + (8 * 8 * ammo)), 8, 8, sAmmoRectLeft[ammoIndex] + 6,
                           sAmmoRectHeight[ammoIndex], 8, 8, 1 << 10, 1 << 10);

    CLOSE_DISPS();
}

void KaleidoScope_DrawAmmoCountWrapper(PauseContext* pauseCtx, GraphicsContext* gfxCtx, s16 item, u16 ammoIndex)
{
    switch (item)
    {
    case ITEM_MM_BOW:
    case ITEM_MM_BOMB:
    case ITEM_MM_BOMBCHU:
    case ITEM_MM_STICK:
    case ITEM_MM_NUT:
    case ITEM_MM_MAGIC_BEAN:
    case ITEM_MM_POWDER_KEG:
    case ITEM_MM_PICTOGRAPH_BOX:
    case ITEM_MM_SLINGSHOT:
        KaleidoScope_CustomDrawAmmoCount(pauseCtx, gfxCtx, item, ammoIndex);
        break;
    case ITEM_MM_BOOMERANG:
        /* No ammo, draw nothing */
        break;
    default:
        KaleidoScope_DrawAmmoCount KaleidoScope_DrawAmmoCount = OverlayAddr(0x8081b240);
        KaleidoScope_DrawAmmoCount(pauseCtx, gfxCtx, item, ammoIndex);
        break;
    }
}

PATCH_CALL(0x8081bc4c, KaleidoScope_DrawAmmoCountWrapper)
