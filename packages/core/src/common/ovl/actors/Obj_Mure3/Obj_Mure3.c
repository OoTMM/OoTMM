#include "Obj_Mure3.h"

#if defined(GAME_OOT)
#define FLAGS 0

void ObjMure3_Init(Actor* thisx, PlayState* play);
void ObjMure3_Destroy(Actor* thisx, PlayState* play);
void ObjMure3_Update(Actor* thisx, PlayState* play);

void func_80B9AF24(Actor_ObjMure3* this);
void func_80B9AF34(Actor_ObjMure3* this, PlayState* play);
void func_80B9AF54(Actor_ObjMure3* this);
void func_80B9AF64(Actor_ObjMure3* this, PlayState* play);
void func_80B9AFEC(Actor_ObjMure3* this);
void func_80B9AFFC(Actor_ObjMure3* this, PlayState* play);

static s16 sRupeeCounts[] = { 5, 5, 7, 0 };

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(cullingVolumeDistance, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeScale, 1800, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeDownward, 100, ICHAIN_STOP),
};

void func_80B9A9D0(Actor_ObjMure3* this, PlayState* play) {
    s32 i;
    Vec3f spawnPos;

    Math_Vec3f_Copy(&spawnPos, &this->actor.world.pos);
    for (i = 0; i < 5; i++, spawnPos.y += 20.0f) {
        if (!((this->childrenBits >> i) & 1)) {
            this->children[i] = (Actor_EnItem00*)Item_DropCollectible2(play, &spawnPos, 0x4000 | ITEM00_RUPEE_BLUE);
            if (this->children[i] != NULL) {
                this->children[i]->actor.room = this->actor.room;
            }
        }
    }
}

void func_80B9AA90(Actor_ObjMure3* this, PlayState* play) {
    s32 i;
    Vec3f spawnPos;
    f32 sn = Math_SinS(this->actor.world.rot.y);
    f32 cos = Math_CosS(this->actor.world.rot.y);
    f32 radius;

    spawnPos.y = this->actor.world.pos.y;

    for (i = 0, radius = -40.0f; i < 5; i++, radius += 20.0f) {
        if (!((this->childrenBits >> i) & 1)) {
            spawnPos.x = this->actor.world.pos.x + (sn * radius);
            spawnPos.z = this->actor.world.pos.z + (cos * radius);
            this->children[i] = (Actor_EnItem00*)Item_DropCollectible2(play, &spawnPos, 0x4000 | ITEM00_RUPEE_GREEN);
            if (this->children[i] != NULL) {
                this->children[i]->actor.room = this->actor.room;
            }
        }
    }
}

void func_80B9ABA0(Actor_ObjMure3* this, PlayState* play) {
    s32 i;
    Vec3f spawnPos;
    s16 yRot;

    spawnPos.y = this->actor.world.pos.y;
    yRot = this->actor.world.rot.y;
    for (i = 0; i < 6; i++) {
        if (!((this->childrenBits >> i) & 1)) {
            spawnPos.x = (Math_SinS(yRot) * 40.0f) + this->actor.world.pos.x;
            spawnPos.z = (Math_CosS(yRot) * 40.0f) + this->actor.world.pos.z;
            this->children[i] = (Actor_EnItem00*)Item_DropCollectible2(play, &spawnPos, 0x4000 | ITEM00_RUPEE_GREEN);
            if (this->children[i] != NULL) {
                this->children[i]->actor.room = this->actor.room;
            }
        }
        yRot += 0x2AAA;
    }
    if (!((this->childrenBits >> 6) & 1)) {
        spawnPos.x = this->actor.world.pos.x;
        spawnPos.z = this->actor.world.pos.z;
        this->children[6] = (Actor_EnItem00*)Item_DropCollectible2(play, &spawnPos, 0x4000 | ITEM00_RUPEE_RED);
        if (this->children[6] != NULL) {
            this->children[6]->actor.room = this->actor.room;
        }
    }
}

void func_80B9ACE4(Actor_ObjMure3* this, PlayState* play)
{
    s16 count = sRupeeCounts[PARAMS_GET_U(this->actor.params, 13, 3)];
    s32 i;
    Actor_EnItem00** collectible;

    for (i = 0; i < count; i++) {
        collectible = &this->children[i];

        if (!((this->childrenBits >> i) & 1) && (*collectible != NULL)) {
            if (Actor_HasParentZ(&(*collectible)->actor) || ((*collectible)->actor.update == NULL)) {
                this->childrenBits |= (1 << i);
            } else {
                Actor_Kill(&(*collectible)->actor);
            }
        }
        *collectible = NULL;
    }
}

void func_80B9ADCC(Actor_ObjMure3* this, PlayState* play) {
    s16 count = sRupeeCounts[PARAMS_GET_U(this->actor.params, 13, 3)];
    s32 i;

    for (i = 0; i < count; i++) {
        Actor_EnItem00** collectible = &this->children[i];

        if ((*collectible != NULL) && !((this->childrenBits >> i) & 1)) {
            if (Actor_HasParentZ(&(*collectible)->actor)) {
                Flags_SetSwitch(play, PARAMS_GET_U(this->actor.params, 0, 6));
            }
            if ((*collectible)->actor.update == NULL) {
                this->childrenBits |= (1 << i);
                this->children[i] = NULL;
            }
        }
    }
}

void ObjMure3_Init(Actor* thisx, PlayState* play) {
    Actor_ObjMure3* this = (Actor_ObjMure3*)thisx;

    if (Flags_GetSwitch(play, PARAMS_GET_U(this->actor.params, 0, 6))) {
        Actor_Kill(&this->actor);
        return;
    }
    Actor_ProcessInitChain(&this->actor, sInitChain);
    func_80B9AF24(this);
}

void ObjMure3_Destroy(Actor* thisx, PlayState* play) {
}

void func_80B9AF24(Actor_ObjMure3* this) {
    this->actionFunc = func_80B9AF34;
}

void func_80B9AF34(Actor_ObjMure3* this, PlayState* play) {
    func_80B9AF54(this);
}

void func_80B9AF54(Actor_ObjMure3* this) {
    this->actionFunc = func_80B9AF64;
}

void func_80B9AF64(Actor_ObjMure3* this, PlayState* play) {
    static Actor_ObjMure3SpawnFunc spawnFuncs[] = { func_80B9A9D0, func_80B9AA90, func_80B9ABA0 };

    if (Math3D_Dist1DSq(this->actor.projectedPos.x, this->actor.projectedPos.z) < SQ(1150.0f)) {
        this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        spawnFuncs[PARAMS_GET_U(this->actor.params, 13, 3)](this, play);
        func_80B9AFEC(this);
    }
}

void func_80B9AFEC(Actor_ObjMure3* this) {
    this->actionFunc = func_80B9AFFC;
}

void func_80B9AFFC(Actor_ObjMure3* this, PlayState* play) {
    func_80B9ADCC(this, play);
    if (Math3D_Dist1DSq(this->actor.projectedPos.x, this->actor.projectedPos.z) >= SQ(1450.0f)) {
        this->actor.flags &= ~ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        func_80B9ACE4(this, play);
        func_80B9AF54(this);
    }
}

void ObjMure3_Update(Actor* thisx, PlayState* play) {
    Actor_ObjMure3* this = (Actor_ObjMure3*)thisx;

    this->actionFunc(this, play);
}

static ActorProfile Obj_Mure3_Profile = {
    /**/ ACTOR_OBJ_MURE3,
    /**/ ACTORCAT_BG,
    /**/ FLAGS,
    /**/ OBJECT_GAMEPLAY_KEEP,
    /**/ sizeof(Actor_ObjMure3),
    /**/ ObjMure3_Init,
    /**/ ObjMure3_Destroy,
    /**/ ObjMure3_Update,
    /**/ NULL,
};

OVL_INFO_ACTOR(ACTOR_OBJ_MURE3, Obj_Mure3_Profile);
#endif

#if defined(GAME_MM)
#define FLAGS 0x00000000

void ObjMure3_Init(Actor* thisx, PlayState* play);
void ObjMure3_Destroy(Actor* thisx, PlayState* play);
void ObjMure3_Update(Actor* thisx, PlayState* play);

void func_8098F598(Actor_ObjMure3* this);
void func_8098F5AC(Actor_ObjMure3* this, PlayState* play);
void func_8098F5D0(Actor_ObjMure3* this);
void func_8098F5E4(Actor_ObjMure3* this, PlayState* play);
void func_8098F66C(Actor_ObjMure3* this);
void func_8098F680(Actor_ObjMure3* this, PlayState* play);

static s16 sRupeeCounts[] = { 5, 5, 7, 0 };

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(cullingVolumeDistance, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeScale, 1800, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeDownward, 100, ICHAIN_STOP),
};

void func_8098F040(Actor_ObjMure3* this, PlayState* play) {
    s32 i;
    Vec3f spawnPos;

    Math_Vec3f_Copy(&spawnPos, &this->actor.world.pos);
    for (i = 0; i < 5; i++, spawnPos.y += 20.0f) {
        if (!((this->childrenBits >> i) & 1)) {
            this->children[i] = (Actor_EnItem00*)Item_DropCollectible2(play, &spawnPos, 0x10001);
            if (this->children[i] != NULL) {
                this->children[i]->actor.room = this->actor.room;
            }
        }
    }
}

void func_8098F110(Actor_ObjMure3* this, PlayState* play) {
    s32 i;
    Vec3f spawnPos;
    f32 sin = Math_SinS(this->actor.world.rot.y);
    f32 cos = Math_CosS(this->actor.world.rot.y);
    f32 dist;

    spawnPos.y = this->actor.world.pos.y;

    for (i = 0, dist = -40.0f; i < 5; i++, dist += 20.0f) {
        if (!((this->childrenBits >> i) & 1)) {
            spawnPos.x = this->actor.world.pos.x + (sin * dist);
            spawnPos.z = this->actor.world.pos.z + (cos * dist);
            this->children[i] = (Actor_EnItem00*)Item_DropCollectible2(play, &spawnPos, 0x4000);
            if (this->children[i] != NULL) {
                this->children[i]->actor.room = this->actor.room;
            }
        }
    }
}

void func_8098F220(Actor_ObjMure3* this, PlayState* play) {
    s16 yRot;
    Vec3f spawnPos;
    s32 i;

    spawnPos.y = this->actor.world.pos.y;
    yRot = this->actor.world.rot.y;

    for (i = 0; i < 6; i++) {
        if (!((this->childrenBits >> i) & 1)) {
            spawnPos.x = (Math_SinS(yRot) * 40.0f) + this->actor.world.pos.x;
            spawnPos.z = (Math_CosS(yRot) * 40.0f) + this->actor.world.pos.z;
            this->children[i] = (Actor_EnItem00*)Item_DropCollectible2(play, &spawnPos, 0x4000);
            if (this->children[i] != NULL) {
                this->children[i]->actor.room = this->actor.room;
            }
        }
        yRot += 0x2AAA;
    }

    if (!((this->childrenBits >> 6) & 1)) {
        spawnPos.x = this->actor.world.pos.x;
        spawnPos.z = this->actor.world.pos.z;
        this->children[6] = (Actor_EnItem00*)Item_DropCollectible2(play, &spawnPos, 0x4002);
        if (this->children[6] != NULL) {
            this->children[6]->actor.room = this->actor.room;
        }
    }
}

void func_8098F364(Actor_ObjMure3* this, PlayState* play)
{
    s16 count = sRupeeCounts[OBJMURE3_PARAM_RUPEEINDEX(&this->actor)];
    s32 i;
    Actor_EnItem00** collectible;

    for (i = 0; i < count; i++) {
        collectible = &this->children[i];

        if ((!((this->childrenBits >> i) & 1)) && (*collectible != NULL)) {
            if (Actor_HasParentZ(&(*collectible)->actor) || ((*collectible)->actor.update == NULL)) {
                this->childrenBits |= (1 << i);
            } else {
                Actor_Kill(&(*collectible)->actor);
            }
        }
        *collectible = NULL;
    }
}

void func_8098F438(Actor_ObjMure3* this, PlayState* play) {
    s16 count = sRupeeCounts[OBJMURE3_PARAM_RUPEEINDEX(&this->actor)];
    s32 i;

    for (i = 0; i < count; i++) {
        Actor_EnItem00** collectible = &this->children[i];

        if ((*collectible != NULL) && !((this->childrenBits >> i) & 1)) {
            if (Actor_HasParentZ(&(*collectible)->actor)) {
                Flags_SetSwitch(play, OBJMURE3_GET_SWITCH_FLAG(&this->actor));
            }
            if ((*collectible)->actor.update == NULL) {
                this->childrenBits |= (1 << i);
                this->children[i] = NULL;
            }
        }
    }
}

void ObjMure3_Init(Actor* thisx, PlayState* play) {
    Actor_ObjMure3* this = (Actor_ObjMure3*)thisx;

    if (Flags_GetSwitch(play, OBJMURE3_GET_SWITCH_FLAG(&this->actor))) {
        Actor_Kill(&this->actor);
        return;
    }
    Actor_ProcessInitChain(&this->actor, sInitChain);
    func_8098F598(this);
}

void ObjMure3_Destroy(Actor* thisx, PlayState* play) {
}

void func_8098F598(Actor_ObjMure3* this) {
    this->actionFunc = func_8098F5AC;
}

void func_8098F5AC(Actor_ObjMure3* this, PlayState* play) {
    func_8098F5D0(this);
}

void func_8098F5D0(Actor_ObjMure3* this) {
    this->actionFunc = func_8098F5E4;
}

void func_8098F5E4(Actor_ObjMure3* this, PlayState* play) {
    static Actor_ObjMure3SpawnFunc sSpawnFuncs[] = { func_8098F040, func_8098F110, func_8098F220 };

    if (Math3D_Dist1DSq(this->actor.projectedPos.x, this->actor.projectedPos.z) < SQ(1150.0f)) {
        this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        sSpawnFuncs[OBJMURE3_PARAM_RUPEEINDEX(&this->actor)](this, play);
        func_8098F66C(this);
    }
}

void func_8098F66C(Actor_ObjMure3* this) {
    this->actionFunc = func_8098F680;
}

void func_8098F680(Actor_ObjMure3* this, PlayState* play) {
    func_8098F438(this, play);
    if (Math3D_Dist1DSq(this->actor.projectedPos.x, this->actor.projectedPos.z) >= SQ(1450.0f)) {
        this->actor.flags &= ~ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        func_8098F364(this, play);
        func_8098F5D0(this);
    }
}

void ObjMure3_Update(Actor* thisx, PlayState* play) {
    Actor_ObjMure3* this = (Actor_ObjMure3*)thisx;

    this->actionFunc(this, play);
}

ActorProfile Obj_Mure3_Profile = {
    /**/ ACTOR_OBJ_MURE3,
    /**/ ACTORCAT_BG,
    /**/ FLAGS,
    /**/ OBJECT_GAMEPLAY_KEEP,
    /**/ sizeof(Actor_ObjMure3),
    /**/ ObjMure3_Init,
    /**/ ObjMure3_Destroy,
    /**/ ObjMure3_Update,
    /**/ NULL,
};

OVL_INFO_ACTOR(ACTOR_OBJ_MURE3, Obj_Mure3_Profile);
#endif
