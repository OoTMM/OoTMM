#include "Obj_Mure.h"
#include <combo/global.h>

#define FLAGS 0

void ObjMure_Init(Actor_ObjMure* this, GameState_Play* play);
void ObjMure_Destroy(Actor_ObjMure* this, GameState_Play* play);
void ObjMure_Update(Actor_ObjMure* this, GameState_Play* play);

void ObjMure_InitialAction(Actor_ObjMure* this, GameState_Play* play);
void ObjMure_CulledState(Actor_ObjMure* this, GameState_Play* play);
void ObjMure_ActiveState(Actor_ObjMure* this, GameState_Play* play);
void ObjMure_KillActors(Actor_ObjMure* this, GameState_Play* play);
void ObjMure_CheckChildren(Actor_ObjMure* this, GameState_Play* play);

static f32 sZClip[OBJMURE_TYPE_MAX] = {
    1600.0f,
    1600.0f,
    1000.0f,
    1000.0f,
    1000.0f,
};

static s32 sMaxChildSpawns[] = {
    12,
    9,
    8,
    0,
};

static s16 sSpawnActorIds[OBJMURE_TYPE_MAX] = {
    AC_EN_KUSA,
    AC_PLAYER,
    AC_EN_FISH,
    AC_EN_INSECT,
    AC_EN_BUTTE,
};

static s16 sSpawnParams[] = { 0, 2, -1, 0, -1 };

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 1200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1200, ICHAIN_STOP),
};

typedef enum {
    /* 0 */ OBJMURE_CHILD_STATE_0,
    /* 1 */ OBJMURE_CHILD_STATE_DEAD,
    /* 2 */ OBJMURE_CHILD_STATE_2
} ObjMureChildState;

s32 func_808D78D0(Actor_ObjMure* this, GameState_Play* play)
{
    if ((this->type == OBJMURE_TYPE_FISH) || (this->type == OBJMURE_TYPE_BUGS) || (this->type == OBJMURE_TYPE_BUTTERFLY)) {
        Actor_ProcessInitChain(&this->actor, sInitChain);
    } else {
        return FALSE;
    }
    return TRUE;
}

s32 func_808D7928(Actor_ObjMure* this, GameState_Play* play)
{
    if (!func_808D78D0(this, play)) {
        return FALSE;
    }
    return TRUE;
}

void ObjMure_AliasButterfly(Actor_ObjMure* this)
{
    Xflag* xf;

    xf = &this->xflag;
    switch (xf->sceneId)
    {
    case SCE_MM_GREAT_BAY_COAST:
        if (xf->setupId == 0)
        {
            xf->setupId = 1;
            xf->id += 69;
        }
        break;
    }
}

void ObjMure_Alias(Actor_ObjMure* this)
{
    switch (OBJ_MURE_GET_TYPE(&this->actor))
    {
    case OBJMURE_TYPE_BUTTERFLY:
        ObjMure_AliasButterfly(this);
        break;
    }
}

void ObjMure_Init(Actor_ObjMure* this, GameState_Play* play)
{
    comboXflagInit(&this->xflag, &this->actor, play);
    if (!g.xflagOverride)
        ObjMure_Alias(this);

    this->chNum = OBJ_MURE_GET_CHNUM(&this->actor);
    this->ptn = OBJ_MURE_GET_PTN(&this->actor);
    this->svNum = OBJ_MURE_GET_SVNUM(&this->actor);
    this->type = OBJ_MURE_GET_TYPE(&this->actor);
    if (this->ptn >= 4) {
        Actor_Kill(&this->actor);
        return;
    }
    if (this->type >= OBJMURE_TYPE_MAX) {
        Actor_Kill(&this->actor);
        return;
    }
    if (!func_808D7928(this, play)) {
        Actor_Kill(&this->actor);
        return;
    }
    this->actionFunc = ObjMure_InitialAction;
}

void ObjMure_Destroy(Actor_ObjMure* this, GameState_Play* play) {
}

s32 ObjMure_GetMaxChildSpawns(Actor_ObjMure* this) {
    if (this->chNum == 0) {
        return sMaxChildSpawns[this->ptn];
    }
    return this->chNum;
}

void ObjMure_GetSpawnPos(Vec3f* outPos, Vec3f* inPos, s32 ptn, s32 idx)
{
    *outPos = *inPos;
}

static Actor* ObjMure_SpawnActor(Actor_ObjMure* this, GameState_Play* play, s16 actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable, int ex1, int ex2, int ex3, int index)
{
    Actor* tmp;

    memcpy(&g.xflag, &this->xflag, sizeof(Xflag));
    g.xflag.sliceId = (u8)index;
    g.xflagOverride = TRUE;
    tmp = Actor_SpawnAsChildAndCutscene(&play->actorCtx, play, actorId, x, y, z, rx, ry, rz, variable, ex1, ex2, ex3);
    g.xflagOverride = FALSE;

    return tmp;
}

void ObjMure_SpawnActors0(Actor_ObjMure* this, GameState_Play* play)
{
    s32 i;
    Vec3f pos;
    s32 maxChildren = ObjMure_GetMaxChildSpawns(this);

    for (i = 0; i < maxChildren; i++) {
        switch (this->childrenStates[i]) {
            case OBJMURE_CHILD_STATE_DEAD:
                break;

            case OBJMURE_CHILD_STATE_2:
                ObjMure_GetSpawnPos(&pos, &this->actor.world.pos, this->ptn, i);
                this->children[i] = ObjMure_SpawnActor(
                    this, play, sSpawnActorIds[this->type], pos.x, pos.y, pos.z, this->actor.world.rot.x,
                    this->actor.world.rot.y, this->actor.world.rot.z, sSpawnParams[this->type], this->actor.csId,
                    this->actor.halfDaysBits, NULL, i);
                if (this->children[i] != NULL) {
                    if (this->type == 0x90) {
                        ((ObjMureChild*)this->children[i])->unk_197 = 1;
                    }
                    this->children[i]->room = this->actor.room;
                }
                break;

            default:
                ObjMure_GetSpawnPos(&pos, &this->actor.world.pos, this->ptn, i);
                this->children[i] = ObjMure_SpawnActor(
                    this, play, sSpawnActorIds[this->type], pos.x, pos.y, pos.z, this->actor.world.rot.x,
                    this->actor.world.rot.y, this->actor.world.rot.z, sSpawnParams[this->type], this->actor.csId,
                    this->actor.halfDaysBits, NULL, i);
                if (this->children[i] != NULL) {
                    this->children[i]->room = this->actor.room;
                }
                break;
        }
    }
}

void ObjMure_SpawnActors1(Actor_ObjMure* this, GameState_Play* play)
{
    Actor* actor = &this->actor;
    Vec3f spawnPos;
    s32 maxChildren = ObjMure_GetMaxChildSpawns(this);
    s32 i;

    for (i = 0; i < maxChildren; i++) {
        ObjMure_GetSpawnPos(&spawnPos, &actor->world.pos, this->ptn, i);
        this->children[i] = ObjMure_SpawnActor(
            this, play, sSpawnActorIds[this->type], spawnPos.x, spawnPos.y, spawnPos.z, actor->world.rot.x,
            actor->world.rot.y, actor->world.rot.z,
            (this->type == OBJMURE_TYPE_BUTTERFLY && i == 0) ? 1 : sSpawnParams[this->type], this->actor.csId,
            this->actor.halfDaysBits, NULL, i);
        if (this->children[i] != NULL) {
            this->childrenStates[i] = OBJMURE_CHILD_STATE_0;
            this->children[i]->room = actor->room;
        } else {
            this->childrenStates[i] = OBJMURE_CHILD_STATE_DEAD;
        }
    }
}

void ObjMure_SpawnActors(Actor_ObjMure* this, GameState_Play* play) {
    switch (this->svNum)
    {
        case 0:
            ObjMure_SpawnActors0(this, play);
            break;

        case 1:
            ObjMure_SpawnActors1(this, play);
            break;

        default:
            break;
    }
}

void ObjMure_KillActorsImpl(Actor_ObjMure* this, GameState_Play* play) {
    s32 maxChildren = ObjMure_GetMaxChildSpawns(this);
    s32 i;

    for (i = 0; i < maxChildren; i++) {
        switch (this->childrenStates[i]) {
            case OBJMURE_CHILD_STATE_DEAD:
                this->children[i] = NULL;
                break;

            case OBJMURE_CHILD_STATE_2:
                if (this->children[i] != NULL) {
                    Actor_Kill(this->children[i]);
                    this->children[i] = NULL;
                }
                break;

            default:
                if (this->children[i] != NULL) {
                    if (Actor_HasParent(this->children[i], play)) {
                        this->children[i] = NULL;
                    } else {
                        Actor_Kill(this->children[i]);
                        this->children[i] = NULL;
                    }
                }
                break;
        }
    }
}

void ObjMure_KillActors(Actor_ObjMure* this, GameState_Play* play) {
    ObjMure_KillActorsImpl(this, play);
}

void ObjMure_CheckChildren(Actor_ObjMure* this, GameState_Play* play) {
    s32 maxChildren = ObjMure_GetMaxChildSpawns(this);
    s32 i;

    for (i = 0; i < maxChildren; i++) {
        if (this->children[i] != NULL) {
            if (this->childrenStates[i] == OBJMURE_CHILD_STATE_0) {
                if (this->children[i]->update != NULL) {
                    if ((this->type == 0x90) && (((ObjMureChild*)this->children[i])->unk_197 != 0)) {
                        this->childrenStates[i] = OBJMURE_CHILD_STATE_2;
                    }
                } else {
                    this->childrenStates[i] = OBJMURE_CHILD_STATE_DEAD;
                    this->children[i] = NULL;
                }
            } else if ((this->childrenStates[i] == OBJMURE_CHILD_STATE_2) && (this->children[i]->update == NULL)) {
                this->childrenStates[i] = OBJMURE_CHILD_STATE_DEAD;
                this->children[i] = NULL;
            }
        }
    }
}

void ObjMure_InitialAction(Actor_ObjMure* this, GameState_Play* play) {
    this->actionFunc = ObjMure_CulledState;
}

void ObjMure_CulledState(Actor_ObjMure* this, GameState_Play* play) {
    if (fabsf(this->actor.projectedPos.z) < sZClip[this->type]) {
        this->actionFunc = ObjMure_ActiveState;
        this->actor.flags |= ACTOR_FLAG_MM_10;
        ObjMure_SpawnActors(this, play);
    }
}

void ObjMure_SetFollowTargets(Actor_ObjMure* this, f32 randMax) {
    s32 index;
    s32 maxChildren = ObjMure_GetMaxChildSpawns(this);
    s32 i;

    for (i = 0; i < maxChildren; i++) {
        if (this->children[i] != NULL) {
            this->children[i]->child = NULL;
            if (Rand_ZeroOne() <= randMax) {
                index = Rand_ZeroOne() * (maxChildren - 0.5f);
                if (i != index) {
                    this->children[i]->child = this->children[index];
                }
            }
        }
    }
}

/**
 * Selects a child that will follow after the player
 * `idx1` is the index + 1 of the child that will follow the player. If `idx1` is zero, no actor will follow the player
 */
void ObjMure_SetChildToFollowPlayer(Actor_ObjMure* this, s32 idx1) {
    s32 maxChildren = ObjMure_GetMaxChildSpawns(this);
    s32 i;
    s32 i2;
    s32 j;

    for (i = 0, i2 = 0; i < maxChildren; i++) {
        if (this->children[i] != NULL) {
            if (i2 < idx1) {
                i2++;
                this->children[i]->child = this->children[i];
                for (j = 0; j < maxChildren; j++) {
                    if ((i != j) && (this->children[j]->child == this->children[i])) {
                        this->children[j]->child = NULL;
                    }
                }
            } else if (this->children[i]->child == this->children[i]) {
                this->children[i]->child = NULL;
            }
        }
    }
}

void ObjMure_GroupBehavior0(Actor_ObjMure* this, GameState_Play* play) {
    if (this->unk_19C <= 0) {
        if (this->unk_19E) {
            this->unk_19E = FALSE;
            ObjMure_SetFollowTargets(this, (Rand_ZeroOne() * 0.5f) + 0.1f);
            if (this->actor.xzDistToPlayer < 60.0f) {
                this->unk_19C = (s32)(Rand_ZeroOne() * 5.5f) + 4;
            } else {
                this->unk_19C = (s32)(Rand_ZeroOne() * 40.5f) + 4;
            }
        } else {
            this->unk_19E = TRUE;
            if (this->actor.xzDistToPlayer < 60.0f) {
                this->unk_19C = (s32)(Rand_ZeroOne() * 10.5f) + 4;
                ObjMure_SetFollowTargets(this, (Rand_ZeroOne() * 0.2f) + 0.8f);
            } else {
                this->unk_19C = (s32)(Rand_ZeroOne() * 10.5f) + 4;
                ObjMure_SetFollowTargets(this, (Rand_ZeroOne() * 0.2f) + 0.6f);
            }
        }
    }
    if (this->actor.xzDistToPlayer < 120.0f) {
        this->unk_1A0++;
    } else {
        this->unk_1A0 = 0;
    }
    if (this->unk_1A0 >= 80) {
        ObjMure_SetChildToFollowPlayer(this, 1);
    } else {
        ObjMure_SetChildToFollowPlayer(this, 0);
    }
}

void ObjMure_GroupBehavior1(Actor_ObjMure* this, GameState_Play* play)
{
    s32 maxChildren;
    s32 i;

    if (this->unk_19C <= 0) {
        if (this->unk_19E) {
            this->unk_19E = FALSE;
            ObjMure_SetFollowTargets(this, Rand_ZeroOne() * 0.2f);
            if (this->actor.xzDistToPlayer < 60.0f) {
                this->unk_19C = (s32)(Rand_ZeroOne() * 5.5f) + 4;
            } else {
                this->unk_19C = (s32)(Rand_ZeroOne() * 40.5f) + 4;
            }
        } else {
            this->unk_19E = TRUE;
            ObjMure_SetFollowTargets(this, Rand_ZeroOne() * 0.7f);
            this->unk_19C = (s32)(Rand_ZeroOne() * 10.5f) + 4;
        }
    }

    maxChildren = ObjMure_GetMaxChildSpawns(this);
    for (i = 0; i < maxChildren; i++) {
        if (this->children[i] != NULL) {
            if ((this->children[i]->child != NULL) && (this->children[i]->child->update == NULL)) {
                this->children[i]->child = NULL;
            }
        }
    }
}

static Actor_ObjMureFunc sTypeGroupBehaviorFunc[] = {
    NULL, NULL, ObjMure_GroupBehavior0, ObjMure_GroupBehavior0, ObjMure_GroupBehavior1,
};

void ObjMure_ActiveState(Actor_ObjMure* this, GameState_Play* play) {
    ObjMure_CheckChildren(this, play);
    if ((sZClip[this->type] + 40.0f) <= fabsf(this->actor.projectedPos.z)) {
        this->actionFunc = ObjMure_CulledState;
        this->actor.flags &= ~ACTOR_FLAG_MM_10;
        ObjMure_KillActors(this, play);
    } else if (sTypeGroupBehaviorFunc[this->type] != NULL) {
        sTypeGroupBehaviorFunc[this->type](this, play);
    }
}

void ObjMure_Update(Actor_ObjMure* this, GameState_Play* play)
{
    if (this->unk_19C > 0) {
        this->unk_19C--;
    }
    this->actionFunc(this, play);
}

static ActorProfile ObjMure_ActorProfile =
{
    AC_OBJ_MURE,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(Actor_ObjMure),
    (ActorFunc)ObjMure_Init,
    (ActorFunc)ObjMure_Destroy,
    (ActorFunc)ObjMure_Update,
    NULL,
};

OVL_ACTOR_INFO(AC_OBJ_MURE, ObjMure_ActorProfile);
