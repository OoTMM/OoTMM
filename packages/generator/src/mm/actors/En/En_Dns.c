#include <combo.h>
#include <combo/entrance.h>
#include <combo/player.h>
#include <combo/mm/save.h>

typedef struct EnDns {
    Actor actor;
} EnDns;

u32 SurfaceType_GetSceneExitIndex(CollisionContext* colCtx, CollisionPoly* poly, s32 bgId);

void EnDns_AfterDetection(void)
{
    PlayState* play;
    s32 override;

    play = gPlay;
    if (!play->nextEntrance)
        return;

    override = comboEntranceOverride(ENTR_MM_DEKU_PALACE_EXTERIOR_FROM_THRONE);
    if (override != -1)
        comboTransitionOverride(play, (u32)override);
}

s32 EnDns_IsHumanWithCustomSkullMask(PlayState* play)
{
    Player* player;

    player = GET_PLAYER(play);

    return player->transformation == MM_PLAYER_FORM_HUMAN &&
           gCustomSave.customMask == PLAYER_CUSTOM_MASK_SKULL;
}

s32 EnDns_ShouldDetectPlayer(PlayState* play)
{
    Player* player;
    s16 bgId;

    player = GET_PLAYER(play);

    if (EnDns_IsHumanWithCustomSkullMask(play)) {
        return false;
    }

    if (!Play_InCsMode(play) &&
        (player->actor.bgCheckFlags & BGCHECKFLAG_GROUND) &&
        (player->transformation != MM_PLAYER_FORM_DEKU)) {
        bgId = player->actor.floorBgId;

        if (SurfaceType_GetSceneExitIndex(&play->colCtx, player->actor.floorPoly, bgId) != 4) {
            return true;
        }
        }

    return false;
}

PATCH_FUNC(0x8092CC68, EnDns_ShouldDetectPlayer);

static const u8 sEnDnsGuard0MsgScript[] = {
    0x00, 0x17, 0x20, 0x00,
    0x05, 0x0e, 0x08, 0x2f,
    0x0c, 0x10, 0x0e, 0x08,
    0x20, 0x0c, 0x10, 0x00,
};

static const u8 sEnDnsGuard1MsgScript[] = {
    0x00, 0x17, 0x20, 0x00,
    0x05, 0x0e, 0x08, 0x30,
    0x0c, 0x10, 0x0e, 0x08,
    0x21, 0x0c, 0x10, 0x00,
};

static const u8 sEnDnsGuard2MsgScript[] = {
    0x00, 0x17, 0x20, 0x00,
    0x05, 0x0e, 0x08, 0x31,
    0x0c, 0x10, 0x0e, 0x08,
    0x22, 0x0c, 0x10, 0x00,
};

static const u8 sEnDnsGuard3MsgScript[] = {
    0x00, 0x17, 0x20, 0x00,
    0x05, 0x0e, 0x08, 0x32,
    0x0c, 0x10, 0x0e, 0x08,
    0x23, 0x0c, 0x10, 0x00,
};

static const u8 sEnDnsCaughtMsgScript[] = {
    0x0e, 0x08, 0x33, 0x0c,
    0x09, 0x00, 0x00, 0x15,
    0x1c, 0x01, 0x40, 0x16,
    0x10, 0x00,
};

static const u8* EnDns_GetGuardMsgScript_Custom(EnDns* this)
{
    switch (this->actor.params & 7) {
        case 0: return sEnDnsGuard0MsgScript;
        case 1: return sEnDnsGuard1MsgScript;
        case 2: return sEnDnsGuard2MsgScript;
        case 3: return sEnDnsGuard3MsgScript;
        default: return NULL;
    }
}

MsgScript* EnDns_GetMsgScript_Custom(EnDns* this, PlayState* play)
{
    Player* player = GET_PLAYER(play);

    if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_23_20)) {
        if (player->transformation != MM_PLAYER_FORM_DEKU) {
            if (!(player->transformation == MM_PLAYER_FORM_HUMAN &&
                  gCustomSave.customMask == PLAYER_CUSTOM_MASK_SKULL)) {
                return (MsgScript*)sEnDnsCaughtMsgScript;
                  }
        }
    }

    return (MsgScript*)EnDns_GetGuardMsgScript_Custom(this);
}