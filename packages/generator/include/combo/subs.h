#ifndef COMBO_SUBS_H
#define COMBO_SUBS_H

#include <combo/types.h>

typedef struct Actor Actor;
typedef struct PlayState PlayState;

s32 SubS_FillCutscenesList(Actor* actor, s16 csIdList[], s16 numCutscenes);
s32 SubS_OfferTalkExchangeFacing(Actor* actor, PlayState* play, f32 xzRange, f32 yRange, s32 exchangeItemAction, s16 playerYawRange, s16 actorYawRange);
s32 SubS_StartCutscene(Actor* actor, s16 nextCsId, s16 curCsId, s32 type);

#if defined(GAME_MM)
typedef enum {
    /* 0 */ SUBS_CUTSCENE_WITH_PLAYER,
    /* 1 */ SUBS_CUTSCENE_NORMAL,
    /* 2 */ SUBS_CUTSCENE_WITH_PLAYER_SET_FLAG
} SubSCutsceneType;
#endif

#endif
