#ifndef COMBO_PLAY_COLLISION_CONTEXT_H
#define COMBO_PLAY_COLLISION_CONTEXT_H

#include <combo/collision.h>
#include <combo/util.h>
#include <combo/math/vec.h>

typedef struct CollisionPoly
{
    u16 type;
    union
    {
        u16 vtxData[3];
        struct
        {
            u16 flags_vIA;
            u16 flags_vIB;
            u16 vIC;
        };
    };
    Vec3s normal;
    s16 dist;
}
CollisionPoly;

ASSERT_SIZE(CollisionPoly, 0x10);

typedef struct SurfaceType
{
    u32 data[2];
}
SurfaceType;

ASSERT_SIZE(SurfaceType, 0x8);

typedef struct BgCamInfo
{
    u16     setting;
    s16     count;
    Vec3s*  bgCamFuncData;
}
BgCamInfo;

ASSERT_SIZE(BgCamInfo, 0x8);

typedef struct WaterBox
{
    Vec3s   minPos;
    s16     xLength;
    s16     zLength;
    u32     properties;
}
WaterBox;

ASSERT_SIZE(WaterBox, 0x10);

typedef struct CollisionHeader
{
    Vec3s           minBounds;
    Vec3s           maxBounds;
    u16             numVertices;
    Vec3s*          vtxList;
    u16             numPolygons;
    CollisionPoly*  polyList;
    SurfaceType*    surfaceTypeList;
    BgCamInfo*      bgCamList;
    u16             numWaterBoxes;
    WaterBox*       waterBoxes;
}
CollisionHeader;

ASSERT_SIZE(CollisionHeader, 0x2c);
ASSERT_OFFSET(CollisionHeader, minBounds,       0x00);
ASSERT_OFFSET(CollisionHeader, maxBounds,       0x06);
ASSERT_OFFSET(CollisionHeader, numVertices,     0x0c);
ASSERT_OFFSET(CollisionHeader, vtxList,         0x10);
ASSERT_OFFSET(CollisionHeader, numPolygons,     0x14);
ASSERT_OFFSET(CollisionHeader, polyList,        0x18);
ASSERT_OFFSET(CollisionHeader, surfaceTypeList, 0x1c);
ASSERT_OFFSET(CollisionHeader, bgCamList,       0x20);
ASSERT_OFFSET(CollisionHeader, numWaterBoxes,   0x24);
ASSERT_OFFSET(CollisionHeader, waterBoxes,      0x28);

typedef struct
{
    u16 head;
}
SSList;

typedef struct
{
    s16 polyId;
    u16 next;
}
SSNode;

typedef struct
{
    SSList floor;
    SSList wall;
    SSList ceiling;
}
StaticLookup;

ASSERT_SIZE(StaticLookup, 0x6);

typedef struct
{
    u16     max;
    u16     count;
    SSNode* tbl;
    u8*     polyCheckTbl;
}
SSNodeList;

ASSERT_SIZE(SSNodeList, 0xc);

typedef struct
{
    u16     polyStartIndex;
    SSList  ceiling;
    SSList  wall;
    SSList  floor;
}
DynaLookup;

typedef struct
{
    Vec3f scale;
    Vec3s rot;
    Vec3f pos;
}
ScaleRotPos;

ASSERT_SIZE(ScaleRotPos, 0x20);

typedef struct
{
    Actor*              actor;
    CollisionHeader*    colHeader;
    DynaLookup          dynaLookup;
    u16                 vtxStartIndex;
    ScaleRotPos         prevTransform;
    ScaleRotPos         curTransform;
    Sphere16            boundingSphere;
    float               minY;
    float               maxY;
}
BgActor;

ASSERT_SIZE(BgActor, 0x64);

typedef struct
{
    SSNode* tbl;
    s32     count;
    s32     max;
}
DynaSSNodeList;

typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ WaterBox* boxes;
} DynaWaterBoxList; // size = 0x8

typedef struct DynaCollisionContext
{
    u8                  bitFlag;
    BgActor             bgActors[BG_ACTOR_MAX];
    u16                 bgActorFlags[BG_ACTOR_MAX];
    CollisionPoly*      polyList;
    Vec3s*              vtxList;
#if defined(GAME_MM)
    DynaWaterBoxList    waterBoxList;
#endif
    DynaSSNodeList      polyNodes;
    s32                 polyNodesMax;
    s32                 polyListMax;
    s32                 vtxListMax;
}
DynaCollisionContext;

typedef struct CollisionContext
{
    CollisionHeader*        colHeader;
    Vec3f                   minBounds;
    Vec3f                   maxBounds;
    Vec3i                   subdivAmount;
    Vec3f                   subdivLength;
    Vec3f                   subdivLengthInv;
    StaticLookup*           lookupTbl;
    SSNodeList              polyNodes;
    DynaCollisionContext    dyna;
    u32                     memSize;
#if defined(GAME_MM)
    u32                     flags;
#endif
}
CollisionContext;

#endif
