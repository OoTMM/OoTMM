#include <combo.h>

void Interface_LoadItemIconCustom(u32 vrom, s32 id, void* dst, size_t size)
{
    DmaEntry dma;

    if (id < ITEM_MM_CUSTOM_MIN)
    {
        LoadIcon(vrom, id, dst, size);
    }
    else
    {
        switch(id)
        {
        case ITEM_MM_SPELL_FIRE:
            id = ITEM_OOT_SPELL_FIRE;
            break;
        case ITEM_MM_SPELL_WIND:
            id = ITEM_OOT_SPELL_WIND;
            break;
        case ITEM_MM_SPELL_LOVE:
            id = ITEM_OOT_SPELL_LOVE;
            break;
        case ITEM_MM_BOOTS_IRON:
            id = ITEM_OOT_BOOTS_IRON;
            break;
        case ITEM_MM_BOOTS_HOVER:
            id = ITEM_OOT_BOOTS_HOVER;
            break;
        case ITEM_MM_TUNIC_GORON:
            id = ITEM_OOT_TUNIC_GORON;
            break;
        case ITEM_MM_TUNIC_ZORA:
            id = ITEM_OOT_TUNIC_ZORA;
            break;
        }

        comboDmaLookupForeignId(&dma, 8);
        u32 textureFileAddress = dma.pstart;
        u32 textureOffset = 0x1000 * id;
        DMARomToRam((textureFileAddress + textureOffset) | PI_DOM1_ADDR2, dst, size);
    }
}

PATCH_CALL(0x80112BCC, Interface_LoadItemIconCustom)

extern u32 gCustomIconAddr;
extern size_t customIconSize;

u32 Interface_GetCustomIconTexture(GameState_Play* play, PauseContext* pauseCtx)
{
    u32* gItemIcons = (u32*)0x801c1e6c;
    u32 texture;
    u16 item = pauseCtx->equipTargetItem;
    if (item < ITEM_MM_CUSTOM_MIN)
    {
        texture = gItemIcons[item];
    }
    else
    {
        u8 customItem = item - ITEM_MM_CUSTOM_MIN;
        texture = gCustomIconAddr + (customIconSize * customItem);
    }
    return texture;
}

extern s8 gPlayerFormCustomItemRestrictions[5][8];

/* button and item are stored in SP10 and SP14 by HOOK_SAVE */
s8 Interface_GetItemRestriction(u8 playerForm, GameState_Play* play, s16* restoreHudVisibility, s32 nothing, u8 item, s16 button)
{
    s8 (*gPlayerFormItemRestrictions)[0x72] = (s8(*)[0x72])0x801c2410;
    if (item < ITEM_MM_CUSTOM_MIN)
    {
        return gPlayerFormItemRestrictions[playerForm][item];
    }

    if (item == ITEM_MM_SPELL_WIND && play->interfaceCtx.restrictions.songOfSoaring)
    {
        return 0;
    }

    u8 customItem = item - ITEM_MM_CUSTOM_MIN;
    s8 result = gPlayerFormCustomItemRestrictions[playerForm][customItem];
    if (result < 0)
    {
        if (gSaveContext.buttonStatus[button] == 0xFF) /* BTN_DISABLED */
        {
            *restoreHudVisibility = 1;
            gSaveContext.buttonStatus[button] = 0; /* BTN_ENABLED */
        }
    }
    return result;
}

s32 Items_ShouldCheckItemUsabilityWhileSwimming(GameState_Play* play, u8 item)
{
    if (item == ITEM_MM_MASK_ZORA)
    {
        return 0;
    }

    Actor_Player* player = GET_LINK(play);
    if (player->transformation == MM_PLAYER_FORM_HUMAN)
    {
        switch (item)
        {
        case ITEM_MM_BOOTS_IRON:
        case ITEM_MM_BOOTS_HOVER:
        case ITEM_MM_TUNIC_ZORA:
        case ITEM_MM_TUNIC_GORON:
            return 0;
        case ITEM_MM_HOOKSHOT:
        case 0x11: /* Short Hookshot */
            return Player_GetEnvironmentalHazard(play) != PLAYER_ENV_HAZARD_UNDERWATER_FLOOR;
        }

        if (item >= ITEM_MM_BOTTLE_EMPTY && item <= ITEM_MM_CHATEAU)
        {
            return Player_GetEnvironmentalHazard(play) != PLAYER_ENV_HAZARD_UNDERWATER_FLOOR;
        }
    }
    return 1;
}

s32 Interface_ShouldStartHazardTimer(Actor_Player* player, s16 envHazard)
{
    return envHazard == PLAYER_ENV_HAZARD_HOTROOM
        || envHazard == PLAYER_ENV_HAZARD_UNDERWATER_FREE
        || (player->transformation != MM_PLAYER_FORM_ZORA && envHazard == PLAYER_ENV_HAZARD_UNDERWATER_FLOOR);
}
