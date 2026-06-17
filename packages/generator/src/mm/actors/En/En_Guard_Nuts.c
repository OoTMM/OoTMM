#include <combo.h>
#include <combo/custom.h>
#include <combo/player.h>
#include <combo/common/events.h>

#define ENGUARDNUTS_GUARD_TEXT_INDEX(actor) (*(s32*)((u8*)(actor) + 0x220))
#define ENGUARDNUTS_HAS_COMPLETED(actor)    (*(u8*)((u8*)(actor) + 0x224))
#define ACTOR_TEXT_ID(actor)                (*(u16*)((u8*)(actor) + 0x116))

static const u16 sEnGuardNutsTextIds[] = {
    0x0824, 0x0825, 0x0826, 0x082D, 0x0827,
    0x0828, 0x0829, 0x082A, 0x082B, 0x082C
};

static s32 EnGuardNuts_IsHumanWithCustomSkullMask(PlayState* play)
{
    Player* player = GET_PLAYER(play);

    return player->transformation == MM_PLAYER_FORM_HUMAN &&
           gCustomSave.customMask == PLAYER_CUSTOM_MASK_SKULL;
}

void EnGuardNuts_OfferTalk_CustomSkullMask(Actor* actor, PlayState* play, f32 radius)
{
    if (EnGuardNuts_IsHumanWithCustomSkullMask(play)) {
        s32 guardTextIndex = 0;

        if (MM_GET_EVENT_WEEK(EV_MM_WEEK_WEEKEVENTREG_17_2) &&
            !ENGUARDNUTS_HAS_COMPLETED(actor)) {
            guardTextIndex = 7;
            } else if (MM_GET_EVENT_WEEK(EV_MM_WEEK_WEEKEVENTREG_12_6)) {
                guardTextIndex = 4;
            }

        ENGUARDNUTS_GUARD_TEXT_INDEX(actor) = guardTextIndex;
        ACTOR_TEXT_ID(actor) = sEnGuardNutsTextIds[guardTextIndex];
    }

    Actor_OfferTalk(actor, play, radius);
}

PATCH_CALL(0x80ABB524, EnGuardNuts_OfferTalk_CustomSkullMask);