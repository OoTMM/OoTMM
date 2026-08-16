#include <combo.h>
#include <combo/player.h>
#include <combo/mask.h>

s32 Player_GetMask_SkullEnemy(PlayState* play)
{
    Player* player = GET_PLAYER(play);
    if (player->transformation == MM_PLAYER_FORM_HUMAN && gCustomSave.customMask == PLAYER_CUSTOM_MASK_SKULL)
    {
        return PLAYER_MASK_STONE;
    }
    return Player_GetMask(play);
}

/* Keese */
PATCH_CALL(0x80879EF8, Player_GetMask_SkullEnemy); /* EnFirefly_FlyIdle */
PATCH_CALL(0x8087A4BC, Player_GetMask_SkullEnemy); /* EnFirefly_DiveAttack */

/* Guay */
PATCH_CALL(0x8099B040, Player_GetMask_SkullEnemy); /* EnCrow_FlyIdle */
PATCH_CALL(0x8099B27C, Player_GetMask_SkullEnemy); /* EnCrow_DiveAttack */

/* Bad Bat */
PATCH_CALL(0x80A43C30, Player_GetMask_SkullEnemy); /* EnBat_FlyIdle */
PATCH_CALL(0x80A43E34, Player_GetMask_SkullEnemy); /* EnBat_DiveAttack */