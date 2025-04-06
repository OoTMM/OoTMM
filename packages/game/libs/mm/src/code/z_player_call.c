#include "global.h"
#include "kaleido_manager.h"

#define FLAGS                                                                                                 \
    (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY | ACTOR_FLAG_10 | ACTOR_FLAG_20 | ACTOR_FLAG_200000 | \
     ACTOR_FLAG_UPDATE_DURING_OCARINA | ACTOR_FLAG_CAN_PRESS_SWITCHES | ACTOR_FLAG_MINIMAP_ICON_ENABLED)

ActorFunc sPlayerCallInitFunc;
ActorFunc sPlayerCallDestroyFunc;
ActorFunc sPlayerCallUpdateFunc;
ActorFunc sPlayerCallDrawFunc;

ActorProfile Player_Profile = {
    /**/ ACTOR_PLAYER,
    /**/ ACTORCAT_PLAYER,
    /**/ FLAGS,
    /**/ GAMEPLAY_KEEP,
    /**/ sizeof(Player),
    /**/ PlayerCall_Init,
    /**/ PlayerCall_Destroy,
    /**/ PlayerCall_Update,
    /**/ PlayerCall_Draw,
};

void Player_Init(Actor* thisx, PlayState* play);
void Player_Destroy(Actor* thisx, PlayState* play);
void Player_Update(Actor* thisx, PlayState* play);
void Player_Draw(Actor* thisx, PlayState* play);

void PlayerCall_InitFuncPtrs(void) {
    sPlayerCallInitFunc = KaleidoManager_GetRamAddr(Player_Init);
    sPlayerCallDestroyFunc = KaleidoManager_GetRamAddr(Player_Destroy);
    sPlayerCallUpdateFunc = KaleidoManager_GetRamAddr(Player_Update);
    sPlayerCallDrawFunc = KaleidoManager_GetRamAddr(Player_Draw);
}

void PlayerCall_Init(Actor* thisx, PlayState* play) {
    KaleidoScopeCall_LoadPlayer();
    PlayerCall_InitFuncPtrs();
    sPlayerCallInitFunc(thisx, play);
}

void PlayerCall_Destroy(Actor* thisx, PlayState* play) {
    KaleidoScopeCall_LoadPlayer();
    sPlayerCallDestroyFunc(thisx, play);
}

void PlayerCall_Update(Actor* thisx, PlayState* play) {
    KaleidoScopeCall_LoadPlayer();
    sPlayerCallUpdateFunc(thisx, play);
}

void PlayerCall_Draw(Actor* thisx, PlayState* play) {
    KaleidoScopeCall_LoadPlayer();
    sPlayerCallDrawFunc(thisx, play);
}
