#include <combo.h>
#include <combo/player.h>
#include <combo/souls.h>

void Interface_LoadItemIconCustom(u32 vrom, s32 id, void* dst, size_t size)
{
    if (id < ITEM_MM_CUSTOM_MIN)
    {
        LoadIcon(vrom, id, dst, size);
    }
    else
    {
        comboCustomItemIcon(dst, id);
    }
}

PATCH_CALL(0x80112BCC, Interface_LoadItemIconCustom)

extern u32 gCustomIconAddr;
extern size_t customIconSize;

u32 Interface_GetCustomIconTexture(PlayState* play, PauseContext* pauseCtx)
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

extern s8 gPlayerFormCustomItemRestrictions[5][ITEM_MM_CUSTOM_MAX - ITEM_MM_CUSTOM_MIN];

/* button and item are stored in SP10 and SP14 by HOOK_SAVE */
s8 Interface_GetItemRestriction(u8 playerForm, PlayState* play, s16* restoreHudVisibility, s32 nothing, u8 item, s16 button)
{
    if (item == ITEM_MM_MASK_GIANT && !comboHasSoulMm(GI_MM_SOUL_BOSS_TWINMOLD))
    {
        return 0;
    }

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

s32 Items_ShouldCheckItemUsabilityWhileSwimming(PlayState* play, u8 item)
{
    if (item == ITEM_MM_MASK_ZORA)
    {
        return 0;
    }

    Player* player = GET_PLAYER(play);
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

s32 Interface_ShouldStartHazardTimer(Player* player, s16 envHazard)
{
    return envHazard == PLAYER_ENV_HAZARD_HOTROOM
        || envHazard == PLAYER_ENV_HAZARD_UNDERWATER_FREE
        || (player->transformation != MM_PLAYER_FORM_ZORA && envHazard == PLAYER_ENV_HAZARD_UNDERWATER_FLOOR);
}

const static u8* gAmmoDigit0Tex = (u8*)0x02004aa0;

void Interface_CustomDrawAmmoCount(PlayState* play, s16 button, s16 alpha)
{
    static s16 sAmmoDigitsXPositions[] = { 162, 228, 250, 272 };
    static s16 sAmmoDigitsYPositions[] = { 35, 35, 51, 35 };
    u8 i;
    s16 ammo;
    s16 maxAmmo;

    OPEN_DISPS(play->state.gfxCtx);

    i = GET_CUR_FORM_BTN_ITEM(button);

    switch (i)
    {
    case ITEM_MM_BOMBCHU:
        ammo = gSave.info.inventory.ammo[ITS_MM_BOMBCHU];
        maxAmmo = gMaxBombchuMm;
        break;
    default:
        return;
    }

    gDPPipeSync(OVERLAY_DISP++);

    if (button == 0 && gSaveContext.minigameState == 1) /* MINIGAME_STATUS_ACTIVE */
    {
        ammo = play->interfaceCtx.minigameAmmo;
    }
    else if (button == 0 && play->bButtonAmmoPlusOne > 1)
    {
        ammo = play->bButtonAmmoPlusOne - 1;
    }
    else if (ammo == maxAmmo)
    {
        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 120, 255, 0, alpha);
    }

    if (ammo == 0)
    {
        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 100, 100, 100, alpha);
    }

    i = 0;
    while (ammo >= 10)
    {
        i++;
        ammo -= 10;
    }

    if (i != 0)
    {
        OVERLAY_DISP = Gfx_TextureIA8(OVERLAY_DISP, ((u8*)gAmmoDigit0Tex + ((8 * 8) * i)), 8, 8,
                                        sAmmoDigitsXPositions[button], sAmmoDigitsYPositions[button], 8, 8, 1 << 10, 1 << 10);
    }

    OVERLAY_DISP = Gfx_TextureIA8(OVERLAY_DISP, ((u8*)gAmmoDigit0Tex + ((8 * 8) * ammo)), 8, 8,
                                    sAmmoDigitsXPositions[button] + 6, sAmmoDigitsYPositions[button], 8, 8, 1 << 10, 1 << 10);

    CLOSE_DISPS();
}

void Interface_DrawAmmoCountWrapper(PlayState* play, s16 button, s16 alpha)
{
    u8 item = GET_CUR_FORM_BTN_ITEM(button);
    switch (item)
    {
    case ITEM_MM_BOMBCHU:
        Interface_CustomDrawAmmoCount(play, button, alpha);
        break;
    case ITEM_MM_BOOMERANG:
        /* No ammo, draw nothing */
        break;
    default:
        Interface_DrawAmmoCount(play, button, alpha);
        break;
    }
}

PATCH_CALL(0x801181a4, Interface_DrawAmmoCountWrapper)
PATCH_CALL(0x801183e8, Interface_DrawAmmoCountWrapper)
PATCH_CALL(0x80118998, Interface_DrawAmmoCountWrapper)
PATCH_CALL(0x80118a94, Interface_DrawAmmoCountWrapper)
PATCH_CALL(0x80118b8c, Interface_DrawAmmoCountWrapper)
