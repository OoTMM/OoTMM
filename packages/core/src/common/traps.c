#include <combo.h>
#include <combo/traps.h>

#if defined(GAME_OOT)
# define APPLY_DAMAGE_ADDR 0x80835d08
#endif

#if defined(GAME_MM)
# define APPLY_DAMAGE_ADDR 0x80833b18
#endif

typedef int (*TrapHandler)(PlayState* play, Player* player);
static void (*Player_ApplyDamage)(PlayState* play, Player* this, s32 damageResponseType, f32 speed, f32 yVelocity, s16 yRot, s32 invincibilityTimer);

static int Play_GetFirstPendingTrapType(void)
{
    for (int i = 0; i < TRAP_MAX; ++i)
    {
        if (gSharedCustomSave.traps[i])
        {
            return i;
        }
    }

    return -1;
}

static int Play_UpdateTrapsIce(PlayState* play, Player* player)
{
    player->actor.colChkInfo.damage = 0;
    Player_ApplyDamage(play, player, 0x03, 0, 0, 0, 20);
    return 1;
}

static int Play_UpdateTrapsFire(PlayState* play, Player* player)
{
#if defined(GAME_OOT)
    if (player->currentTunic == 2)
        return 1;
#endif

#if defined(GAME_MM)
    if (Player_IsGoronOrGoronTunic(player))
        return 1;
#endif

    player->actor.colChkInfo.damage = 0;
    Player_ApplyDamage(play, player, 0, 0, 0, 0, 20);
    player->isBurning = 1;
    for (int i = 0; i < PLAYER_BODYPART_MAX; ++i)
        player->flameTimers[i] = Rand_S16Offset(0, 200);
    return 1;
}

static int Play_UpdateTrapsShock(PlayState* play, Player* player)
{
    player->actor.colChkInfo.damage = 0;
    Player_ApplyDamage(play, player, 0x04, 0, 0, 0, 20);
    return 1;
}

static int Play_UpdateTrapsDrain(PlayState* play, Player* player)
{
    gSave.info.playerData.health = 1;
    player->actor.colChkInfo.damage = 0;
    Player_ApplyDamage(play, player, 0x00, 0, 0, 0, 20);
    return 1;
}

static int Play_UpdateTrapsAntiMagic(PlayState* play, Player* player)
{
    gSave.info.playerData.magic = 0;
    if (comboIsChateauActive())
        MM_CLEAR_EVENT_WEEK(EV_MM_WEEK_DRANK_CHATEAU_ROMANI);
    player->actor.colChkInfo.damage = 0;
    Player_ApplyDamage(play, player, 0x00, 0, 0, 0, 20);
    return 1;
}

static int Play_UpdateTrapsKnockback(PlayState* play, Player* player)
{
    player->actor.colChkInfo.damage = 0;
    Player_ApplyDamage(play, player, 0x01, Rand_ZeroFloat(20.f), Rand_ZeroFloat(15.f) , Rand_Next() & 0xffff, 20);
    return 1;
}

static const TrapHandler kTrapHandlers[] = {
    [TRAP_ICE] = Play_UpdateTrapsIce,
    [TRAP_FIRE] = Play_UpdateTrapsFire,
    [TRAP_SHOCK] = Play_UpdateTrapsShock,
    [TRAP_DRAIN] = Play_UpdateTrapsDrain,
    [TRAP_ANTI_MAGIC] = Play_UpdateTrapsAntiMagic,
    [TRAP_KNOCKBACK] = Play_UpdateTrapsKnockback,
};

void Play_UpdateTraps(PlayState* play)
{
    int type;
    Player* player;

    if (gSaveContext.gameMode)
        return;

    type = Play_GetFirstPendingTrapType();
    if (type < 0)
        return;

    player = GET_PLAYER(play);
    if (player->stateFlags1 & (PLAYER_ACTOR_STATE_CLIMB | PLAYER_ACTOR_STATE_TALKING | PLAYER_ACTOR_STATE_DEATH | PLAYER_ACTOR_STATE_FROZEN | PLAYER_ACTOR_STATE_EPONA | PLAYER_ACTOR_STATE_GET_ITEM))
        return;
    if (player->invincibilityTimer)
        return;
#if defined(GAME_MM)
    /* Shops safety */
    if (player->gi && player->interactRangeActor && (player->interactRangeActor->id == ACTOR_EN_OSSAN || player->interactRangeActor->id == ACTOR_EN_TRT || player->interactRangeActor->id == ACTOR_EN_SOB1))
        return;
#endif

    Player_ApplyDamage = OverlayAddr(APPLY_DAMAGE_ADDR);
    if (kTrapHandlers[type](play, player))
        gSharedCustomSave.traps[type]--;
}
