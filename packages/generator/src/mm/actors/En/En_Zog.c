#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>

void EnZog_GiveItem(Actor* this, PlayState* play)
{
    if (Actor_HasParentZ(this))
    {
        if (!gMmExtraFlags.maskZora && !(GET_PLAYER(play)->stateFlags1 & PLAYER_ACTOR_STATE_GET_ITEM))
        {
            gMmExtraFlags.maskZora = 1;
            play->nextEntrance = ENTR_MM_COAST_FROM_MIKAU_CS;
            play->transitionTrigger = TRANS_TRIGGER_START;
            play->transitionType = TRANS_TYPE_FADE_BLACK;
        }
        return;
    }

    comboGiveItemNpc(this, play, GI_MM_MASK_ZORA, NPC_MM_MASK_ZORA, 9999.f, 9999.f);
}

PATCH_FUNC(0x80b943c0, EnZog_GiveItem);

void EnZog_InitSetScaleHook(Actor* this, float scale)
{
    Actor_SetScale(this, scale);
    if (gMmExtraFlags.maskZora)
        Actor_Kill(this);
}

PATCH_CALL(0x80b935dc, EnZog_InitSetScaleHook);

Actor* EnZog_SpawnAsChild(ActorContext* actorCtx, Actor* parent, PlayState* play, s16 actorId, f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s32 params)
{
    if (gMmExtraFlags.maskZora)
        return NULL;
    return Actor_SpawnAsChild(actorCtx, parent, play, actorId, posX, posY, posZ, rotX, rotY, rotZ, params);
}

PATCH_CALL(0x80b93874, EnZog_SpawnAsChild);

s32 EnZog_OfferGrab(Actor* actor, PlayState* play, s32 getItemId, f32 xzRange, f32 yRange)
{
    if (comboIsLinkAdult())
    {
        yRange = 96.0f;
    }
    return Actor_OfferGetItem(actor, play, getItemId, xzRange, yRange);
}

PATCH_CALL(0x80b95024, EnZog_OfferGrab);
