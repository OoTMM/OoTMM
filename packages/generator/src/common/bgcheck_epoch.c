#include <combo.h>

u8 gBgCheckPolyEpoch = 1;
static u8* sBgCheckPolyTable;
static volatile u32 sBgCheckQueries, sBgCheckFullClears, sBgCheckClearedBytes;
typedef __typeof__(((CollisionContext*)0)->polyNodes) EnemyPolyNodes;

void comboBgCheckNodesInit(EnemyPolyNodes* nodes)
{
    nodes->max = nodes->count = 0;
    nodes->tbl = NULL;
    nodes->polyCheckTbl = NULL;
    sBgCheckPolyTable = NULL;
}

void comboBgCheckResetPolyChecks(EnemyPolyNodes* nodes, s32 count)
{
    ++sBgCheckQueries;
    if (sBgCheckPolyTable != nodes->polyCheckTbl || ++gBgCheckPolyEpoch == 0)
    {
        sBgCheckPolyTable = nodes->polyCheckTbl;
        gBgCheckPolyEpoch = 1;
        ++sBgCheckFullClears;
        sBgCheckClearedBytes += count;
        bzero(nodes->polyCheckTbl, count);
    }
}

#if defined(GAME_MM)
PATCH_FUNC(0x800c5aac, comboBgCheckNodesInit);
PATCH_FUNC(0x800c9640, comboBgCheckResetPolyChecks);
#else
PATCH_FUNC(0x80030d94, comboBgCheckNodesInit);
PATCH_FUNC(0x80033ff0, comboBgCheckResetPolyChecks);
#endif
