#include <combo.h>
#include <combo/mm/dark_link.h>
#include <combo/sfx.h>

extern void comboEnemySfxGeneral(u16, Vec3f*, u8, f32*, f32*, s8*);

void comboMmDarkLinkControllerVoice(Actor* actor, u16 sfxId)
{
    static f32 frequency = 0.7950898f;
    static s8 reverb = 35;

    if (actor && actor->id == MM_DARK_LINK_ACTOR_ID && (sfxId - 0x6800u <= 2 || sfxId == 0x6814))
        comboEnemySfxGeneral(sfxId, &actor->projectedPos, 4, &frequency, &gSfxDefaultFreqAndVolScale, &reverb);
}

extern s32 DarkLink_OriginalHostileLockOn(Player* player);

static s32 DarkLink_UpdateHostileLockOn(Player* player)
{
    Actor* target = player->lockOnActor;
    if (player->actor.id == MM_DARK_LINK_ACTOR_ID && target && target->id == ACTOR_PLAYER)
    {
        player->stateFlags3 |= PLAYER_STATE3_MM_HOSTILE_LOCK_ON;
        player->stateFlags1 &= ~PLAYER_STATE1_MM_FRIENDLY_ACTOR_FOCUS;
        return 1;
    }
    return DarkLink_OriginalHostileLockOn(player);
}

PATCH_FUNC(0x8082fb68, DarkLink_UpdateHostileLockOn);

static s32 Player_DarkLinkHazardGuard(Player* player, PlayState* play)
{
    if (player->actor.id == MM_DARK_LINK_ACTOR_ID && player != GET_PLAYER(play))
    {
        player->unk_D6A = 0;
        if ((Player_GetHeight(player) - 8.f < player->unk_AB8 * player->actor.scale.y) ||
            (player->actor.bgCheckFlags & BGCHECKFLAG_CRUSHED) ||
            *(s32*)OverlayAddr(0x80862b08) == 9 ||
            (player->stateFlags2 & PLAYER_STATE2_MM_80000000))
        {
            player->stateFlags2 |= PLAYER_STATE2_MM_80000000;
            return 1;
        }
    }
    return ((s32(*)(Player*, PlayState*))OverlayAddr(0x80834600))(player, play);
}

PATCH_CALL(0x80845778, Player_DarkLinkHazardGuard);

static void Player_DarkLinkFallSetup(PlayState* play, Player* player)
{
    if (player->actor.id == MM_DARK_LINK_ACTOR_ID && player != GET_PLAYER(play))
    {
        player->stateFlags2 |= PLAYER_STATE2_MM_80000000;
        return;
    }
    ((void (*)(PlayState*, Player*))OverlayAddr(0x80834104))(play, player);
}

PATCH_CALL(0x8083416c, Player_DarkLinkFallSetup);
PATCH_CALL(0x80835458, Player_DarkLinkFallSetup);
PATCH_CALL(0x80835b6c, Player_DarkLinkFallSetup);
PATCH_CALL(0x80854774, Player_DarkLinkFallSetup);

s32 Player_InflictEnvironmentalDamage(PlayState* play, s32 damage, Player* actor)
{
    if (actor != GET_PLAYER(play) && actor->actor.id == MM_DARK_LINK_ACTOR_ID)
        return 0;
    return ((s32(*)(PlayState*, s32))OverlayAddr(0x8085b3e0))(play, damage);
}
static PlayState* sDarkLinkPerchPlay;
static Actor* sDarkLinkPerchOwner;
static PlayerActionFunc sDarkLinkPerchAction;
static u32 sDarkLinkPerchFrame;

static int DarkLink_PerchOwnerAlive(PlayState* play)
{
    for (Actor* actor = play->actorCtx.actors[ACTORCAT_ENEMY].first; actor; actor = actor->next)
        if (actor == sDarkLinkPerchOwner)
            return actor->id == MM_DARK_LINK_ACTOR_ID && actor->update && actor->colChkInfo.health;
    return 0;
}

static int DarkLink_PerchActive(PlayState* play)
{
    return sDarkLinkPerchPlay == play && sDarkLinkPerchOwner &&
        play->gameplayFrames - sDarkLinkPerchFrame <= 1 && DarkLink_PerchOwnerAlive(play);
}

static void DarkLink_ClearPerch(void)
{
    sDarkLinkPerchPlay = NULL;
    sDarkLinkPerchOwner = NULL;
    sDarkLinkPerchAction = NULL;
}

int comboMmDarkLinkPerch(Actor* owner, PlayState* play, int hold)
{
    Player* player = GET_PLAYER(play);
    if (!owner || owner->id != MM_DARK_LINK_ACTOR_ID)
        return 0;
    if (!hold)
    {
        if (sDarkLinkPerchPlay == play && sDarkLinkPerchOwner == owner)
            DarkLink_ClearPerch();
        return 0;
    }
    if (DarkLink_PerchActive(play))
    {
        if (sDarkLinkPerchOwner != owner)
            return 0;
        if (player->actionFunc != sDarkLinkPerchAction)
        {
            DarkLink_ClearPerch();
            return 0;
        }
    }
    sDarkLinkPerchPlay = play;
    sDarkLinkPerchOwner = owner;
    sDarkLinkPerchAction = player->actionFunc;
    sDarkLinkPerchFrame = play->gameplayFrames;
    return 1;
}

static void Player_DarkLinkPerch(Player* player, PlayState* play)
{
    f32 speed = player->skelAnime.playSpeed;
    player->stateFlags2 |= PLAYER_STATE2_MM_20 | PLAYER_STATE2_MM_40;
    player->linearVelocity = player->actor.speed = 0.f;
    player->skelAnime.curFrame = 2.f;
    player->skelAnime.playSpeed = 0.f;
    PlayerAnimation_Update(play, &player->skelAnime);
    player->skelAnime.playSpeed = speed;
}

void comboMmDarkLinkUpdatePlayer(Player* player, PlayState* play)
{
    if (player == GET_PLAYER(play) && DarkLink_PerchActive(play) &&
        player->actionFunc == sDarkLinkPerchAction)
    {
        PlayerActionFunc action = sDarkLinkPerchAction;
        player->actionFunc = Player_DarkLinkPerch;
        Player_Update(player, play);
        if (player->actionFunc == Player_DarkLinkPerch)
            player->actionFunc = action;
    }
    else
    {
        Player_Update(player, play);
    }
}
