#include "Obj_Mure3.h"

#define FLAGS 0

#if defined(GAME_OOT)
# define ARG_RUPEE_GREEN (0x4000 | ITEM00_RUPEE_GREEN)
# define ARG_RUPEE_BLUE  (0x4000 | ITEM00_RUPEE_BLUE)
# define ARG_RUPEE_RED   (0x4000 | ITEM00_RUPEE_RED)
#endif

#if defined(GAME_MM)
# define ARG_RUPEE_GREEN (0x4000)
# define ARG_RUPEE_BLUE  (0x10001)
# define ARG_RUPEE_RED   (0x4002)
#endif

static s16 sRupeeCounts[] = { 5, 5, 7, 0 };

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(cullingVolumeDistance, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeScale, 1800, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeDownward, 100, ICHAIN_STOP),
};

void ObjMure3_SetActionInit2(Actor_ObjMure3* this);
void ObjMure3_Init2(Actor_ObjMure3* this, PlayState* play);
void ObjMure3_SetActionWatchSpawn(Actor_ObjMure3* this);
void ObjMure3_WatchSpawn(Actor_ObjMure3* this, PlayState* play);
void ObjMure3_DespawnChildren(Actor_ObjMure3* this, PlayState* play);
void ObjMure3_SetActionWatchDespawn(Actor_ObjMure3* this);
void ObjMure3_WatchDespawn(Actor_ObjMure3* this, PlayState* play);

static void ObjMure3_Alias(Xflag* xf)
{
    switch (xf->sceneId)
    {
#if defined(GAME_OOT)
    case SCE_OOT_KOKIRI_FOREST:
        xf->setupId = 2;
        xf->id = 11;
        break;
#endif
    }
}

int ObjMure3_IsAnyShuffled(Actor_ObjMure3* this, PlayState* play)
{
    s16 count;
    Xflag xf;
    Xflag xf2;

    if (comboXflagInit(&xf, &this->actor, play))
        ObjMure3_Alias(&xf);
    count = sRupeeCounts[OBJMURE3_PARAM_RUPEEINDEX(&this->actor)];

    for (s16 i = 0; i < count; ++i)
    {
        memcpy(&xf2, &xf, sizeof(Xflag));
        xf2.sliceId = i;
        if (Xflag_IsShuffled(&xf2))
        {
            return TRUE;
        }
    }

    return FALSE;
}

void ObjMure3_SpawnRupee(Actor_ObjMure3* this, PlayState* play, Vec3f* pos, int index, int params)
{
    Actor_EnItem00* item;

    if (this->childrenBits & (1 << index))
        return;

    /* Get the matching xflag */
    if (comboXflagInit(&g.xflag, &this->actor, play))
        ObjMure3_Alias(&g.xflag);
    g.xflag.sliceId = (u8)index;

    /* Spawn the item */
    g.xflagOverride = TRUE;
    item = (Actor_EnItem00*)Item_DropCollectible2(play, pos, params);
    g.xflagOverride = FALSE;

    /* Original stuff */
    this->children[index] = item;
    if (item)
        item->actor.room = this->actor.room;
}

void ObjMure3_SpawnFunc0(Actor_ObjMure3* this, PlayState* play) {
    s32 i;
    Vec3f spawnPos;

    Math_Vec3f_Copy(&spawnPos, &this->actor.world.pos);
    for (i = 0; i < 5; i++, spawnPos.y += 20.0f) {
        ObjMure3_SpawnRupee(this, play, &spawnPos, i, ARG_RUPEE_BLUE);
    }
}

void ObjMure3_SpawnFunc1(Actor_ObjMure3* this, PlayState* play) {
    s32 i;
    Vec3f spawnPos;
    f32 sin = Math_SinS(this->actor.world.rot.y);
    f32 cos = Math_CosS(this->actor.world.rot.y);
    f32 dist;

    spawnPos.y = this->actor.world.pos.y;

    for (i = 0, dist = -40.0f; i < 5; i++, dist += 20.0f) {
        spawnPos.x = this->actor.world.pos.x + (sin * dist);
        spawnPos.z = this->actor.world.pos.z + (cos * dist);
        ObjMure3_SpawnRupee(this, play, &spawnPos, i, ARG_RUPEE_GREEN);
    }
}

void ObjMure3_SpawnFunc2(Actor_ObjMure3* this, PlayState* play) {
    s16 yRot;
    Vec3f spawnPos;
    s32 i;

    spawnPos.y = this->actor.world.pos.y;
    yRot = this->actor.world.rot.y;

    for (i = 0; i < 6; i++) {
        spawnPos.x = (Math_SinS(yRot) * 40.0f) + this->actor.world.pos.x;
        spawnPos.z = (Math_CosS(yRot) * 40.0f) + this->actor.world.pos.z;
        ObjMure3_SpawnRupee(this, play, &spawnPos, i, ARG_RUPEE_GREEN);
        yRot += 0x2AAA;
    }

    spawnPos.x = this->actor.world.pos.x;
    spawnPos.z = this->actor.world.pos.z;
    ObjMure3_SpawnRupee(this, play, &spawnPos, 6, ARG_RUPEE_RED);
}

void ObjMure3_MonitorChildren(Actor_ObjMure3* this, PlayState* play) {
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

void ObjMure3_WatchDespawn(Actor_ObjMure3* this, PlayState* play) {
    ObjMure3_MonitorChildren(this, play);
    if (Math3D_Dist1DSq(this->actor.projectedPos.x, this->actor.projectedPos.z) >= SQ(1450.0f)) {
        this->actor.flags &= ~ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        ObjMure3_DespawnChildren(this, play);
        ObjMure3_SetActionWatchSpawn(this);
    }
}

void ObjMure3_SetActionWatchDespawn(Actor_ObjMure3* this) {
    this->actionFunc = ObjMure3_WatchDespawn;
}

void ObjMure3_DespawnChildren(Actor_ObjMure3* this, PlayState* play)
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

void ObjMure3_Init(Actor* thisx, PlayState* play) {
    Actor_ObjMure3* this = (Actor_ObjMure3*)thisx;

    if (!ObjMure3_IsAnyShuffled(this, play) && Flags_GetSwitch(play, OBJMURE3_GET_SWITCH_FLAG(&this->actor))) {
        Actor_Kill(&this->actor);
        return;
    }
    Actor_ProcessInitChain(&this->actor, sInitChain);
    ObjMure3_SetActionInit2(this);
}

void ObjMure3_SetActionInit2(Actor_ObjMure3* this) {
    this->actionFunc = ObjMure3_Init2;
}

void ObjMure3_Init2(Actor_ObjMure3* this, PlayState* play) {
    ObjMure3_SetActionWatchSpawn(this);
}

void ObjMure3_SetActionWatchSpawn(Actor_ObjMure3* this) {
    this->actionFunc = ObjMure3_WatchSpawn;
}

void ObjMure3_WatchSpawn(Actor_ObjMure3* this, PlayState* play) {
    static Actor_ObjMure3SpawnFunc sSpawnFuncs[] = { ObjMure3_SpawnFunc0, ObjMure3_SpawnFunc1, ObjMure3_SpawnFunc2 };

    if (Math3D_Dist1DSq(this->actor.projectedPos.x, this->actor.projectedPos.z) < SQ(1150.0f)) {
        this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        sSpawnFuncs[OBJMURE3_PARAM_RUPEEINDEX(&this->actor)](this, play);
        ObjMure3_SetActionWatchDespawn(this);
    }
}

void ObjMure3_Destroy(Actor* thisx, PlayState* play) {
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
