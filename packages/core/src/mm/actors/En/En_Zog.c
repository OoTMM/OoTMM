#include <combo.h>
#include <combo/item.h>

void EnZog_GiveItem(Actor* this, GameState_Play* play)
{
    if (Actor_HasParent(this))
    {
        gMmExtraFlags.maskZora = 1;
        if (!(GET_LINK(play)->state & PLAYER_ACTOR_STATE_GET_ITEM))
        {
            play->nextEntrance = ENTR_MM_COAST_FROM_MIKAU_CS;
            play->transitionTrigger = TRANS_TRIGGER_NORMAL;
            play->transitionType = TRANS_TYPE_FADE_BLACK;
            ActorDestroy(this);
        }
        return;
    }

    comboGiveItemNpc(this, play, GI_MM_MASK_ZORA, NPC_MM_MASK_ZORA, 9999.f, 9999.f);
}

PATCH_FUNC(0x80b943c0, EnZog_GiveItem);

void EnZog_InitSetScaleHook(Actor* this, float scale)
{
    ActorSetScale(this, scale);
    if (gMmExtraFlags.maskZora)
        ActorDestroy(this);
}

PATCH_CALL(0x80b935dc, EnZog_InitSetScaleHook);

s32 EnZog_OfferGrab(Actor* actor, GameState_Play* play, s32 getItemId, f32 xzRange, f32 yRange)
{
    if (comboIsLinkAdult())
    {
        yRange = 96.0f;
    }
    return GiveItem(actor, play, getItemId, xzRange, yRange);
}

PATCH_CALL(0x80b95024, EnZog_OfferGrab);
