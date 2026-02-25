#include <combo.h>
#include <combo/player.h>
#include <combo/config.h>

void Player_Freeze(PlayState* play)
{
    Player* link;
    link = GET_PLAYER(play);
    link->actor.freezeTimer = 100;
    link->stateFlags1 |= PLAYER_ACTOR_STATE_FROZEN;
}

void Player_Unfreeze(PlayState* play)
{
    Player* link;
    link = GET_PLAYER(play);
    link->actor.freezeTimer = 0;
    link->stateFlags1 &= ~PLAYER_ACTOR_STATE_FROZEN;
}

s32 Play_ApplyPlayerDamage(PlayState *play, Player *this, s32 damage)
{
    u8 multiplier = 1;
    #if defined(GAME_OOT)
    if ((this->invincibilityTimer != 0) || (this->actor.type != ACTORCAT_PLAYER))
    {
        return true;
    }

    if(Config_Flag(CFG_OOT_DMG_MULT_2))
    {
        multiplier = 2;
    }
    else if(Config_Flag(CFG_OOT_DMG_MULT_4))
    {
        multiplier = 4;
    }
    else if(Config_Flag(CFG_OOT_DMG_MULT_8))
    {
        multiplier = 8;
    }
    else if(Config_Flag(CFG_OOT_DMG_MULT_OHKO))
    {
        damage = -gSaveContext.save.info.playerData.health;
    }

    #endif

    #if defined(GAME_MM)
    if ((this->invincibilityTimer != 0) || (this->stateFlags3 & PLAYER_STATE3_MM_400000) ||
        (this->actor.id != ACTOR_PLAYER))
    {
        return true;
    }

    if (this->actor.type != ACTORCAT_PLAYER)
    {
        this->actor.colChkInfo.damage = -damage;
        return Actor_ApplyDamage(&this->actor);
    }

    if (this->currentMask == PLAYER_MASK_GIANT)
    {
        damage >>= 2;
    }

    if (Config_Flag(CFG_MM_DMG_MULT_2))
    {
        multiplier = 2;
    }
    else if (Config_Flag(CFG_MM_DMG_MULT_4))
    {
        multiplier = 4;
    }
    else if (Config_Flag(CFG_MM_DMG_MULT_8))
    {
        multiplier = 8;
    }
    else if (Config_Flag(CFG_MM_DMG_MULT_OHKO))
    {
        damage = -gSaveContext.save.info.playerData.health;
    }
    #endif

    return Health_ChangeBy(play, damage * multiplier);
}

#if defined(GAME_OOT)
PATCH_FUNC(0x80835C14, Play_ApplyPlayerDamage);
#endif

#if defined(GAME_MM)
PATCH_FUNC(0x808339D4, Play_ApplyPlayerDamage);
#endif
