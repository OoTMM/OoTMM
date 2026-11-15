#include <combo.h>
#include <combo/enemy.h>

u32 comboEnemyMmSetupActorFlag(u16 rawId)
{
    return comboIsForeignEnemyActor(rawId & 0x1fff) ? 0 : (rawId & 0x800);
}

EXPORT_SYMBOL(FOREIGN_ENEMY_MM_SCHEDULE_V1, comboEnemyMmSetupActorFlag);

EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_BROB_COLLISION, CollisionHeader_GetVirtual);
