#include <combo.h>

#if defined(GAME_OOT)
# define APPLY_DAMAGE_ADDR 0x80835d08
#endif

#if defined(GAME_MM)
# define APPLY_DAMAGE_ADDR 0x80833b18
#endif

void Play_UpdateIceTrap(PlayState* play)
{
    Player* player;
    void (*Player_ApplyDamage)(PlayState* play, Player* this, s32 damageResponseType, f32 speed, f32 yVelocity, s16 yRot, s32 invincibilityTimer);

    if (!gSharedCustomSave.pendingIceTraps)
        return;
    if (gSaveContext.gameMode)
        return;

    player = GET_PLAYER(play);
    if (player->stateFlags1 & (PLAYER_ACTOR_STATE_CLIMB | PLAYER_ACTOR_STATE_DEATH | PLAYER_ACTOR_STATE_FROZEN | PLAYER_ACTOR_STATE_EPONA | PLAYER_ACTOR_STATE_GET_ITEM))
        return;
    if (player->invincibilityTimer)
        return;

    Player_ApplyDamage = OverlayAddr(APPLY_DAMAGE_ADDR);
    player->actor.colChkInfo.damage = 0;
    Player_ApplyDamage(play, player, 0x03, 0, 0, 0, 20);
    gSharedCustomSave.pendingIceTraps--;
}
