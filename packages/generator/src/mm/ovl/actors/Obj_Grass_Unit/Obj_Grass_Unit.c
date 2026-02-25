#include <actors/Obj_Grass_Unit/Obj_Grass_Unit.h>
#include <actors/Obj_Grass/Obj_Grass.h>
#include <actors/Obj_Grass_Carry/Obj_Grass_Carry.h>

#define FLAGS 0x00000000

#define THIS ((ObjGrassUnit*)thisx)

void ObjGrassUnit_Init(Actor* this, PlayState* play2);

ActorProfile Obj_Grass_Unit_Profile = {
    /**/ ACTOR_OBJ_GRASS_UNIT,
    /**/ ACTORCAT_BG,
    /**/ FLAGS,
    /**/ OBJECT_GAMEPLAY_FIELD_KEEP,
    /**/ sizeof(ObjGrassUnit),
    /**/ ObjGrassUnit_Init,
    /**/ Actor_Noop,
    /**/ Actor_Noop,
    /**/ NULL,
};

OVL_INFO_ACTOR(ACTOR_OBJ_GRASS_UNIT, Obj_Grass_Unit_Profile);

// Neat circular pattern with a single bush in the center
static VecPolar sGrassPatternCircle[] = {
    { 0.0f, 0x0000 },  { 80.0f, 0x0000 }, { 80.0f, 0x2000 }, { 80.0f, 0x4000 }, { 80.0f, 0x6000 },
    { 80.0f, 0x8000 }, { 80.0f, 0xA000 }, { 80.0f, 0xC000 }, { 80.0f, 0xE000 },
};

// "Random" looking pattern
static VecPolar sGrassPatternMixed[] = {
    { 40.0f, 0x0666 }, { 40.0f, 0x2CCC }, { 40.0f, 0x5999 }, { 40.0f, 0x8667 }, { 20.0f, 0xC000 }, { 80.0f, 0x1333 },
    { 80.0f, 0x4000 }, { 80.0f, 0x6CCC }, { 80.0f, 0x9334 }, { 80.0f, 0xACCD }, { 80.0f, 0xC667 }, { 60.0f, 0xE000 },
};

typedef struct {
    /* 0x0 */ s32 count;
    /* 0x4 */ VecPolar* positions;
} ObjGrassUnitPattern; // size = 0x8

static ObjGrassUnitPattern sGrassPatterns[2] = {
    { ARRAY_COUNT(sGrassPatternCircle), sGrassPatternCircle },
    { ARRAY_COUNT(sGrassPatternMixed), sGrassPatternMixed },
};

ObjGrass* sGrassManager = NULL;
ObjGrassCarry* sGrassCarry0 = NULL;
ObjGrassCarry* sGrassCarry1 = NULL;
s32 sInitialized = false;

s32 ObjGrassUnit_SpawnObjGrass(Actor* this, PlayState* play) {
    sGrassManager = (ObjGrass*)Actor_Spawn(&play->actorCtx, play, ACTOR_OBJ_GRASS, 0.0f, 0.0f, 0.0f, 0, 0, 0, -1);
    if (sGrassManager != NULL) {
        sGrassManager->actor.room = this->room;
        return true;
    }
    return false;
}

s32 ObjGrassUnit_SpawnObjGrassCarry0(Actor* this, PlayState* play) {
    sGrassCarry0 =
        (ObjGrassCarry*)Actor_Spawn(&play->actorCtx, play, ACTOR_OBJ_GRASS_CARRY, 0.0f, 0.0f, 0.0f, 0, 0, 0, 0);
    if (sGrassCarry0 != NULL) {
        sGrassCarry0->actor.room = this->room;
        return true;
    }
    return false;
}

s32 ObjGrassUnit_SpawnObjGrassCarry1(Actor* this, PlayState* play) {
    sGrassCarry1 =
        (ObjGrassCarry*)Actor_Spawn(&play->actorCtx, play, ACTOR_OBJ_GRASS_CARRY, 0.0f, 0.0f, 0.0f, 0, 0, 0, 1);
    if (sGrassCarry1 != NULL) {
        sGrassCarry1->actor.room = this->room;
        return true;
    }
    return false;
}

s32 ObjGrassUnit_IsUnderwater(PlayState* play, Vec3f* pos) {
    WaterBox* waterBox;
    f32 waterSurface;
    s32 bgId;

    if (WaterBox_GetSurfaceImpl(play, &play->colCtx, pos->x, pos->z, &waterSurface, &waterBox, &bgId) &&
        (pos->y < waterSurface)) {
        return true;
    }
    return false;
}

static void ObjGrassUnit_Alias(Xflag* xflag)
{
    switch (xflag->sceneId)
    {
    case SCE_MM_SOUTHERN_SWAMP_CLEAR:
        xflag->sceneId = SCE_MM_SOUTHERN_SWAMP;
        if (xflag->roomId == 0x00)
            xflag->id += 2;
        break;
    }
}

void ObjGrassUnit_Init(Actor* this, PlayState* play2) {
    PlayState* play = play2;
    ObjGrassGroup* grassGroup;
    ObjGrass* grassManager;
    f32 homePosYSum;
    f32 tmp;
    s32 i;
    VecPolar* grassPos;
    CollisionPoly* poly;
    s32 bgId;
    ObjGrassElement* grassElem;
    ObjGrassUnitPattern* grassPattern;
    Xflag* xf;
    s8 dropTable = OBJGRASSUNIT_GET_DROPTABLE(this);

    if ((sGrassManager == NULL) && !ObjGrassUnit_SpawnObjGrass(this, play)) {
        Actor_Kill(this);
        return;
    }
    if ((sGrassCarry0 == NULL) && ObjGrassUnit_SpawnObjGrassCarry0(this, play)) {
        sGrassCarry0->grassManager = sGrassManager;
    }
    if ((sGrassCarry1 == NULL) && ObjGrassUnit_SpawnObjGrassCarry1(this, play)) {
        sGrassCarry1->grassManager = sGrassManager;
    }
    if (!sInitialized && (sGrassManager != NULL) && (sGrassCarry0 != NULL) && (sGrassCarry1 != NULL)) {
        sInitialized = true;
        sGrassManager->grassCarry[0] = sGrassCarry0;
        sGrassManager->grassCarry[1] = sGrassCarry1;
        sGrassCarry0->grassManager = sGrassManager;
        sGrassCarry1->grassManager = sGrassManager;
    }

    grassManager = sGrassManager;
    if (sGrassManager->activeGrassGroups >= ARRAY_COUNT(sGrassManager->grassGroups)) {
        Actor_Kill(this);
        return;
    }

    homePosYSum = 0.0f;
    grassGroup = &grassManager->grassGroups[sGrassManager->activeGrassGroups];
    grassGroup->count = 0;
    grassPattern = &sGrassPatterns[OBJGRASSUNIT_GET_PATTERN(this)];

    for (i = 0; i < grassPattern->count; i++) {
        grassElem = &grassGroup->elements[grassGroup->count];
        grassPos = &grassPattern->positions[i];

        grassElem->pos.x = this->home.pos.x + (Math_CosS(this->home.rot.y + grassPos->angle) * grassPos->distance);
        grassElem->pos.y = this->home.pos.y + 100.0f;
        grassElem->pos.z = this->home.pos.z + (Math_SinS(this->home.rot.y + grassPos->angle) * grassPos->distance);

        grassElem->pos.y = BgCheck_EntityRaycastFloor5(&play->colCtx, &poly, &bgId, this, &grassElem->pos);
        tmp = grassElem->pos.y - this->home.pos.y;
        if ((fabsf(tmp) < 80.0f) && (grassElem->pos.y > BGCHECK_Y_MIN)) {
            grassGroup->count++;
            grassElem->rotY = (s16)(Rand_Next() >> 0x10);
            grassElem->dropTable = dropTable;
            if (ObjGrassUnit_IsUnderwater(play, &grassElem->pos)) {
                grassElem->flags |= OBJ_GRASS_ELEM_UNDERWATER;
            }
            homePosYSum += grassElem->pos.y;
        }
    }
    if (grassGroup->count > 0) {
        /* Init xflag */
        xf = &grassManager->xflag[grassManager->activeGrassGroups];
        if (comboXflagInit(xf, this, play)) {
            ObjGrassUnit_Alias(xf);
        }

        grassManager->activeGrassGroups++;
        grassGroup->homePos.x = this->home.pos.x;
        grassGroup->homePos.y = (homePosYSum / grassGroup->count);
        grassGroup->homePos.z = this->home.pos.z;
    }
    Actor_Kill(this);
}
