#include <combo/dungeon.h>
#include <combo/doors.h>
#include <combo/custom.h>
#include "En_Door.h"

#include <assets/oot/objects/gameplay_keep.h>
#include <assets/oot/objects/gameplay_field_keep.h>
#include <assets/oot/objects/object_hidan_objects.h>
#include <assets/oot/objects/object_mizu_objects.h>
#include <assets/oot/objects/object_haka_door.h>

#define FLAGS ACTOR_FLAG_UPDATE_CULLING_DISABLED

#define DOOR_AJAR_SLAM_RANGE 120.0f
#define DOOR_AJAR_OPEN_RANGE (2 * DOOR_AJAR_SLAM_RANGE)

#define DOOR_CHECK_RANGE 40.0f

void EnDoor_Init(Actor* thisx, PlayState* play2);
void EnDoor_Destroy(Actor* thisx, PlayState* play);
void EnDoor_Update(Actor* thisx, PlayState* play);
void EnDoor_Draw(Actor* thisx, PlayState* play);

void EnDoor_SetupType(EnDoor* this, PlayState* play);
void EnDoor_Idle(EnDoor* this, PlayState* play);
void EnDoor_WaitForCheck(EnDoor* this, PlayState* play);
void EnDoor_Check(EnDoor* this, PlayState* play);
void EnDoor_AjarWait(EnDoor* this, PlayState* play);
void EnDoor_AjarOpen(EnDoor* this, PlayState* play);
void EnDoor_AjarClose(EnDoor* this, PlayState* play);
void EnDoor_Open(EnDoor* this, PlayState* play);

ActorProfile En_Door_Profile = {
    /**/ ACTOR_EN_DOOR,
    /**/ ACTORCAT_DOOR,
    /**/ FLAGS,
    /**/ OBJECT_GAMEPLAY_KEEP,
    /**/ sizeof(EnDoor),
    /**/ EnDoor_Init,
    /**/ EnDoor_Destroy,
    /**/ EnDoor_Update,
    /**/ EnDoor_Draw,
};

OVL_INFO_ACTOR(ACTOR_EN_DOOR, En_Door_Profile);

typedef struct EnDoorInfo {
    /* 0x00 */ s16 sceneId;
    /* 0x02 */ u8 dListIndex;
    /* 0x04 */ s16 objectId;
} EnDoorInfo;

typedef enum EnDoorDListIndex {
    /* 0 */ DOOR_DL_DEFAULT,
    /* 1 */ DOOR_DL_FIRE_TEMPLE,
    /* 2 */ DOOR_DL_WATER_TEMPLE,
    /* 3 */ DOOR_DL_SHADOW,
    /* 4 */ DOOR_DL_DEFAULT_FIELD_KEEP,
    /* 5 */ DOOR_DL_MAX
} EnDoorDListIndex;

/**
 * Controls which object and display lists to use in a given scene
 */
static EnDoorInfo sDoorInfo[] = {
    { SCE_OOT_TEMPLE_FIRE, DOOR_DL_FIRE_TEMPLE, OBJECT_HIDAN_OBJECTS },
    { SCE_OOT_TEMPLE_WATER, DOOR_DL_WATER_TEMPLE, OBJECT_MIZU_OBJECTS },
    { SCE_OOT_TEMPLE_SHADOW, DOOR_DL_SHADOW, OBJECT_HAKA_DOOR },
    { SCE_OOT_BOTTOM_OF_THE_WELL, DOOR_DL_SHADOW, OBJECT_HAKA_DOOR },
    // KEEP objects should remain last and in this order
    { -1, DOOR_DL_DEFAULT, OBJECT_GAMEPLAY_KEEP },
    { -1, DOOR_DL_DEFAULT_FIELD_KEEP, OBJECT_GAMEPLAY_FIELD_KEEP },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_U8(attentionRangeType, ATTENTION_RANGE_0, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeDistance, 4000, ICHAIN_STOP),
};

static AnimationHeader* sDoorAnims[] = {
    (AnimationHeader*)&gDoorAdultOpeningLeftAnim,  // DOOR_OPEN_ANIM_ADULT_L
    (AnimationHeader*)&gDoorChildOpeningLeftAnim,  // DOOR_OPEN_ANIM_CHILD_L
    (AnimationHeader*)&gDoorAdultOpeningRightAnim, // DOOR_OPEN_ANIM_ADULT_R
    (AnimationHeader*)&gDoorChildOpeningRightAnim, // DOOR_OPEN_ANIM_CHILD_R
};

static u8 sDoorAnimOpenFrames[] = {
    25, // DOOR_OPEN_ANIM_ADULT_L
    25, // DOOR_OPEN_ANIM_CHILD_L
    25, // DOOR_OPEN_ANIM_ADULT_R
    25, // DOOR_OPEN_ANIM_CHILD_R
};

static u8 sDoorAnimCloseFrames[] = {
    60, // DOOR_OPEN_ANIM_ADULT_L
    70, // DOOR_OPEN_ANIM_CHILD_L
    60, // DOOR_OPEN_ANIM_ADULT_R
    70, // DOOR_OPEN_ANIM_CHILD_R
};

static Gfx* sDoorDLists[DOOR_DL_MAX][2] = {
    { gDoorLeftDL, gDoorRightDL },                                         // DOOR_DL_DEFAULT
    { gFireTempleDoorWithHandleLeftDL, gFireTempleDoorWithHandleRightDL }, // DOOR_DL_FIRE_TEMPLE
    { gWaterTempleDoorLeftDL, gWaterTempleDoorRightDL },                   // DOOR_DL_WATER_TEMPLE
    { gShadowDoorLeftDL, gShadowDoorRightDL },                             // DOOR_DL_SHADOW
    { gFieldDoorLeftDL, gFieldDoorRightDL },                               // DOOR_DL_DEFAULT_FIELD_KEEP
};

static int EnDoor_GetID(PlayState* play, int id)
{
    switch (play->sceneId)
    {
    case SCE_OOT_MARKET_ENTRANCE_ADULT:
    case SCE_OOT_MARKET_ENTRANCE_CHILD_DAY:
    case SCE_OOT_MARKET_ENTRANCE_CHILD_NIGHT:
    case SCE_OOT_GUARD_HOUSE:
        return DOORID_OOT_GUARD_HOUSE;
    case SCE_OOT_BACK_ALLEY_DAY:
    case SCE_OOT_BACK_ALLEY_NIGHT:
        switch (id)
        {
        case 0: return DOORID_OOT_DOG_LADY_HOUSE;
        case 1: return DOORID_OOT_BACK_ALLEY_HOUSE;
        case 2: return DOORID_OOT_BOMBCHU_SHOP;
        }
        break;
    case SCE_OOT_BACK_ALLEY_HOUSE2:
        return DOORID_OOT_DOG_LADY_HOUSE;
    case SCE_OOT_BACK_ALLEY_HOUSE:
        return DOORID_OOT_BACK_ALLEY_HOUSE;
    case SCE_OOT_HYRULE_CASTLE:
        return DOORID_OOT_HYRULE_CASTLE;
    case SCE_OOT_TREASURE_SHOP:
        return DOORID_OOT_TREASURE_CHEST_GAME;
    case SCE_OOT_MARKET_CHILD_DAY:
    case SCE_OOT_MARKET_CHILD_NIGHT:
        switch (id)
        {
        case 0: return DOORID_OOT_TREASURE_CHEST_GAME;
        case 1: return DOORID_OOT_CHILD_POTION_SHOP;
        case 2: return DOORID_OOT_CHILD_SHOOTING_GALLERY;
        case 3: return DOORID_OOT_MASK_SHOP;
        case 4: return DOORID_OOT_CHILD_BAZAAR;
        case 5: return DOORID_OOT_BOMBCHU_BOWLING;
        }
        break;
    case SCE_OOT_BOMBCHU_BOWLING_ALLEY:
        return DOORID_OOT_BOMBCHU_BOWLING;
    case SCE_OOT_SHOOTING_GALLERY:
        switch (gSave.entrance)
        {
        case ENTR_OOT_ADULT_ARCHERY:
            return DOORID_OOT_ADULT_SHOOTING_GALLERY;
        case ENTR_OOT_CHILD_ARCHERY:
            return DOORID_OOT_CHILD_SHOOTING_GALLERY;
        }
        break;
    case SCE_OOT_LAKE_HYLIA:
        switch (id)
        {
        case 0: return DOORID_OOT_LABORATORY;
        case 1: return DOORID_OOT_FISHING_POND;
        }
        break;
    case SCE_OOT_LABORATORY:
        return DOORID_OOT_LABORATORY;
    case SCE_OOT_FISHING_POND:
        return DOORID_OOT_FISHING_POND;
    case SCE_OOT_LON_LON_RANCH:
        switch (id)
        {
        case 0: return DOORID_OOT_SILO;
        case 1: return DOORID_OOT_RANCH_STABLE;
        case 2: return DOORID_OOT_RANCH_HOUSE;
        }
        break;
    case SCE_OOT_STABLE:
        return DOORID_OOT_RANCH_STABLE;
    case SCE_OOT_RANCH_HOUSE_SILO:
        switch (id)
        {
        case 0: return DOORID_OOT_RANCH_STABLE;
        case 1: return DOORID_OOT_RANCH_HOUSE;
        case 2: return DOORID_OOT_RANCH_HOUSE_ROOM;
        }
        break;
    case SCE_OOT_GRAVEYARD:
        return DOORID_OOT_GRAVEYARD;
        case SCE_OOT_KAKARIKO_VILLAGE:
        switch (id)
        {
            case 0: return DOORID_OOT_WINDMILL;
            case 1: return DOORID_OOT_IMPA_HOUSE;
            case 2: return DOORID_OOT_CARPENTER_HOUSE;
            case 3: return DOORID_OOT_GRANNY_POTION_SHOP;
            case 4: return DOORID_OOT_ADULT_SHOOTING_GALLERY;
            case 5: return DOORID_OOT_SKULLTULA_HOUSE;
            case 6: return DOORID_OOT_ADULT_BAZAAR;
            case 7: return DOORID_OOT_ADULT_POTION_SHOP;
            case 8: return DOORID_OOT_ADULT_POTION_SHOP_BACK;
        }
        break;
    case SCE_OOT_TOMB_DAMPE_WINDMILL:
        return DOORID_OOT_WINDMILL;
    case SCE_OOT_HOUSE_OF_SKULLTULA:
        return DOORID_OOT_SKULLTULA_HOUSE;
    case SCE_OOT_CARPENTER_BOSS_HOUSE:
        return DOORID_OOT_CARPENTER_HOUSE;
    case SCE_OOT_IMPA_HOUSE:
        return DOORID_OOT_IMPA_HOUSE;
    case SCE_OOT_GRAVEKEEPER_HUT:
        return DOORID_OOT_GRAVEYARD;
    case SCE_OOT_GRANNY_POTION_SHOP:
        return DOORID_OOT_GRANNY_POTION_SHOP;
    }

    return -1;
}

static int EnDoor_IsRustyLocked(PlayState* play, Actor* this)
{
    int id;

    id = EnDoor_GetID(play, GET_TRANSITION_ACTOR_INDEX(this));
    if (id < 0)
        return 0;
    return !BITMAP8_GET(gSharedCustomSave.rustyKeysOot, id);
}

void EnDoor_Init(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnDoor* this = (EnDoor*)thisx;
    EnDoorInfo* objectInfo;
    s32 i;
    s32 objectSlot;
    f32 xOffset;
    f32 zOffset;

    objectInfo = &sDoorInfo[0];
    Actor_ProcessInitChain(&this->actor, sInitChain);
    SkelAnime_Init(play, &this->skelAnime, (SkeletonHeader*)&gDoorSkel, (AnimationHeader*)&gDoorAdultOpeningLeftAnim, this->jointTable, this->morphTable,
                   5);
    for (i = 0; i < ARRAY_COUNT(sDoorInfo) - 2; i++, objectInfo++) {
        if (play->sceneId == objectInfo->sceneId) {
            break;
        }
    }
    if (i >= ARRAY_COUNT(sDoorInfo) - 2 && _Object_GetSlot(&play->objectCtx, OBJECT_GAMEPLAY_FIELD_KEEP) >= 0) {
        objectInfo++;
    }

    this->dListIndex = objectInfo->dListIndex;
    objectSlot = _Object_GetSlot(&play->objectCtx, objectInfo->objectId);
    if (objectSlot < 0) {
        Actor_Kill(&this->actor);
        return;
    }

    this->requiredObjectSlot = objectSlot;
    this->dListIndex = objectInfo->dListIndex;
    if (this->actor.objectSlot == this->requiredObjectSlot) {
        EnDoor_SetupType(this, play);
    } else {
        this->actionFunc = EnDoor_SetupType;
    }

    // Double doors
    if (ENDOOR_GET_IS_DOUBLE_DOOR(&this->actor)) {
        EnDoor* other;

        xOffset = Math_CosS(this->actor.shape.rot.y) * 30.0f;
        zOffset = Math_SinS(this->actor.shape.rot.y) * 30.0f;
        other = (EnDoor*)Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_EN_DOOR,
                                            this->actor.world.pos.x + xOffset, this->actor.world.pos.y,
                                            this->actor.world.pos.z - zOffset, 0, this->actor.shape.rot.y + 0x8000, 0,
                                            this->actor.params & ~ENDOOR_PARAMS_IS_DOUBLE_DOOR_MASK);
        if (other != NULL) {
            other->unk_192 = 1;
        }
        this->actor.world.pos.x -= xOffset;
        this->actor.world.pos.z += zOffset;
    }
    Actor_SetFocus(&this->actor, 70.0f);
}

void EnDoor_Destroy(Actor* thisx, PlayState* play) {
    TransitionActorEntry* transitionEntry;
    EnDoor* this = (EnDoor*)thisx;

    transitionEntry = &play->transiActorCtx.list[GET_TRANSITION_ACTOR_INDEX(&this->actor)];
    if (transitionEntry->id < 0) {
        transitionEntry->id = -transitionEntry->id;
    }
}

void EnDoor_SetupType(EnDoor* this, PlayState* play) {
    if (Object_IsLoaded(&play->objectCtx, this->requiredObjectSlot)) {
        s32 doorType = ENDOOR_GET_TYPE(&this->actor);

        this->actor.flags &= ~ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        this->actor.objectSlot = this->requiredObjectSlot;
        this->actionFunc = EnDoor_Idle;
        if (doorType == DOOR_EVENING) {
            doorType = (gSaveContext.save.dayTime > CLOCK_TIME(18, 0) && gSaveContext.save.dayTime < CLOCK_TIME(21, 0))
                           ? DOOR_SCENEEXIT
                           : DOOR_CHECKABLE;
        }
        this->actor.world.rot.y = 0x0000;
        if (doorType == DOOR_LOCKED) {
            if (!comboDoorIsUnlocked(play, &this->actor)) {
                this->lockTimer = 10;
            }
        } else if (doorType == DOOR_AJAR) {
            Player* player = GET_PLAYER(play);

            if (Actor_WorldDistXZToActor(&this->actor, &player->actor) > DOOR_AJAR_SLAM_RANGE) {
                this->actionFunc = EnDoor_AjarWait;
                this->actor.world.rot.y = -0x1800;
            }
        } else if (doorType == DOOR_CHECKABLE) {
            this->actor.messageId = ENDOOR_GET_CHECKABLE_TEXT_ID(&this->actor) + 0x0200;
            if (this->actor.messageId == 0x0229 && !GET_EVENTCHKINF(0x14 /*EVENTCHKINF_TALON_RETURNED_FROM_CASTLE*/)) {
                // Talon's house door. If Talon has not been woken up at Hyrule Castle
                // this door should be openable at any time of day.
                // Note that there is no check for time of day, as the night layers for Lon Lon
                // have a door with a different text ID.
                doorType = DOOR_SCENEEXIT;
            } else {
                this->actionFunc = EnDoor_WaitForCheck;
                this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY | ACTOR_FLAG_LOCK_ON_DISABLED;
            }
        }
        // Replace the door type it was loaded with by the new type
        this->actor.params = (this->actor.params & ~ENDOOR_PARAMS_TYPE_MASK) | (doorType << ENDOOR_PARAMS_TYPE_SHIFT);
    }
}

void EnDoor_Idle(EnDoor* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 doorType;
    Vec3f playerPosRelToDoor;

    doorType = ENDOOR_GET_TYPE(&this->actor);
    Actor_WorldToActorCoords(&this->actor, &playerPosRelToDoor, &player->actor.world.pos);
    if (this->playerIsOpening) {
        this->actionFunc = EnDoor_Open;
        Animation_PlayOnceSetSpeed(&this->skelAnime, sDoorAnims[this->openAnim],
                                   (player->stateFlags1 & PLAYER_STATE1_27) ? 0.75f : 1.5f);
        if (this->lockTimer != 0) {
            gSaveContext.save.info.inventory.dungeonKeys[gSaveContext.mapIndex] -= 1;
            Flags_SetSwitch(play, ENDOOR_GET_LOCKED_SWITCH_FLAG(&this->actor));
            Actor_PlaySfx(&this->actor, NA_SE_EV_CHAIN_KEY_UNLOCK);
        }
    } else if (!Player_InCsMode(play) && !EnDoor_IsRustyLocked(play, &this->actor)) {
        if (fabsf(playerPosRelToDoor.y) < 20.0f && fabsf(playerPosRelToDoor.x) < 20.0f &&
            fabsf(playerPosRelToDoor.z) < 50.0f) {
            s16 yawDiff = player->actor.shape.rot.y - this->actor.shape.rot.y;

            if (playerPosRelToDoor.z > 0.0f) {
                yawDiff = 0x8000 - yawDiff;
            }
            if (ABS(yawDiff) < 0x3000) {
                if (this->lockTimer != 0) {
                    if (gSaveContext.save.info.inventory.dungeonKeys[gSaveContext.mapIndex] <= 0) {
                        Player* player2 = GET_PLAYER(play);

                        player2->naviTextId = -0x203;
                        return;
                    } else {
                        player->doorTimer = 10;
                    }
                }
                player->doorType = (doorType == DOOR_AJAR) ? PLAYER_DOORTYPE_AJAR : PLAYER_DOORTYPE_HANDLE;
                player->doorDirection = (playerPosRelToDoor.z >= 0.0f) ? 1.0f : -1.0f;
                player->doorActor = &this->actor;
            }
        } else if (doorType == DOOR_AJAR && this->actor.xzDistToPlayer > DOOR_AJAR_OPEN_RANGE) {
            this->actionFunc = EnDoor_AjarOpen;
        }
    }
}

void EnDoor_WaitForCheck(EnDoor* this, PlayState* play) {
    if (Actor_TalkOfferAccepted(&this->actor, play)) {
        this->actionFunc = EnDoor_Check;
    } else {
        Actor_OfferTalk(&this->actor, play, DOOR_CHECK_RANGE);
    }
}

void EnDoor_Check(EnDoor* this, PlayState* play) {
    if (Actor_TextboxIsClosing(&this->actor, play)) {
        this->actionFunc = EnDoor_WaitForCheck;
    }
}

void EnDoor_AjarWait(EnDoor* this, PlayState* play) {
    if (this->actor.xzDistToPlayer < DOOR_AJAR_SLAM_RANGE) {
        this->actionFunc = EnDoor_AjarClose;
    }
}

void EnDoor_AjarOpen(EnDoor* this, PlayState* play) {
    if (this->actor.xzDistToPlayer < DOOR_AJAR_SLAM_RANGE) {
        this->actionFunc = EnDoor_AjarClose;
    } else if (Math_ScaledStepToS(&this->actor.world.rot.y, -0x1800, 0x100)) {
        this->actionFunc = EnDoor_AjarWait;
    }
}

void EnDoor_AjarClose(EnDoor* this, PlayState* play) {
    if (Math_ScaledStepToS(&this->actor.world.rot.y, 0, 0x700)) {
        this->actionFunc = EnDoor_Idle;
    }
}

void EnDoor_Open(EnDoor* this, PlayState* play) {
    if (DECR(this->lockTimer) == 0) {
        if (SkelAnime_Update(&this->skelAnime)) {
            this->actionFunc = EnDoor_Idle;
            this->playerIsOpening = false;
        } else if (Animation_OnFrame(&this->skelAnime, sDoorAnimOpenFrames[this->openAnim])) {
            Actor_PlaySfx(&this->actor,
                          (play->sceneId == SCE_OOT_TEMPLE_SHADOW || play->sceneId == SCE_OOT_BOTTOM_OF_THE_WELL ||
                           play->sceneId == SCE_OOT_TEMPLE_FIRE)
                              ? NA_SE_EV_IRON_DOOR_OPEN
                              : NA_SE_OC_DOOR_OPEN);
            if (this->skelAnime.playSpeed < 1.5f) {
                s32 numEffects = (s32)(Rand_ZeroOne() * 30.0f) + 50;
                s32 i;

                for (i = 0; i < numEffects; i++) {
                    EffectSsBubble_Spawn(play, &this->actor.world.pos, 60.0f, 100.0f, 50.0f, 0.15f);
                }
            }
        } else if (Animation_OnFrame(&this->skelAnime, sDoorAnimCloseFrames[this->openAnim])) {
            Actor_PlaySfx(&this->actor,
                          (play->sceneId == SCE_OOT_TEMPLE_SHADOW || play->sceneId == SCE_OOT_BOTTOM_OF_THE_WELL ||
                           play->sceneId == SCE_OOT_TEMPLE_FIRE)
                              ? NA_SE_EV_IRON_DOOR_CLOSE
                              : NA_SE_EV_DOOR_CLOSE);
        }
    }
}

void EnDoor_Update(Actor* thisx, PlayState* play) {
    EnDoor* this = (EnDoor*)thisx;

    this->actionFunc(this, play);
}

s32 EnDoor_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    if (limbIndex == 4) {
        EnDoor* this = (EnDoor*)thisx;
        TransitionActorEntry* transitionEntry;
        Gfx** doorDLists = sDoorDLists[this->dListIndex];

        transitionEntry = &play->transiActorCtx.list[GET_TRANSITION_ACTOR_INDEX(&this->actor)];
        rot->z += this->actor.world.rot.y;
        if ((play->roomCtx.prevRoom.num >= 0) || (transitionEntry->sides[0].room == transitionEntry->sides[1].room)) {
            // Draw the side of the door that is visible to the camera
            s16 rotDiff = this->actor.shape.rot.y + this->skelAnime.jointTable[3].z + rot->z -
                          Math_Vec3f_Yaw(&play->view.eye, &this->actor.world.pos);
            *dList = (ABS(rotDiff) < 0x4000) ? doorDLists[0] : doorDLists[1];
        } else {
            s32 doorDListIndex = this->unk_192;

            if (transitionEntry->sides[0].room != this->actor.room) {
                doorDListIndex ^= 1;
            }
            *dList = doorDLists[doorDListIndex];
        }
    }
    return false;
}

static void EnDoor_DrawLock(EnDoor* this, PlayState* play, int type, int isRusty)
{
    if (isRusty) {
        Door_DrawRustyLock(play);
    } else {
        Actor_DrawDoorLock(play, this->lockTimer, type);
    }
}

void EnDoor_Draw(Actor* thisx, PlayState* play) {
    EnDoor* this = (EnDoor*)thisx;

    if (this->actor.objectSlot == this->requiredObjectSlot) {
        OPEN_DISPS(play->state.gfxCtx);

        Gfx_SetupDL_25Opa(play->state.gfxCtx);
        SkelAnime_DrawOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, EnDoor_OverrideLimbDraw, NULL,
                          &this->actor);
        if (this->actor.world.rot.y != 0) {
            if (this->actor.world.rot.y > 0) {
                gSPDisplayList(POLY_OPA_DISP++, gDoorRightDL);
            } else {
                gSPDisplayList(POLY_OPA_DISP++, gDoorLeftDL);
            }
        }

        s32 yaw = Math_Vec3f_Yaw(&play->view.eye, &this->actor.world.pos);
        if (ABS((s16)(this->actor.shape.rot.y - yaw)) < 0x4000) {
            Matrix_RotateY(M_PI, MTXMODE_APPLY);
        }

        if (EnDoor_IsRustyLocked(play, &this->actor)) {
            EnDoor_DrawLock(this, play, 0, 1);
        } else if (this->lockTimer != 0) {
            EnDoor_DrawLock(this, play, 0, 0);
        }

        CLOSE_DISPS();
    }
}
