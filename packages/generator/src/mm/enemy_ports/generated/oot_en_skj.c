#include <combo/actor_ovl.h>
#include "../skull_kid.c"

OVL_INFO_NATIVE_ENEMY(0x115, EnemySkullKidProfile);
static const u32 SkullKidNativeOrigin[] = { 0x4e505431, 0, 0x115, 0x10a, 0x976f6a72u };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_SKJ_ORIGIN, SkullKidNativeOrigin);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_OOT_EN_SKJ, EnemySkullKidProfile, EnSkj_Init,
                     EnSkj_Destroy, EnSkj_Update, EnSkj_Draw);
