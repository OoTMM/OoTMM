#include <combo/actor_ovl.h>
#include "../moblin.c"

OVL_INFO_NATIVE_ENEMY(0x4b, sMoblinPortProfile);
static const u32 MoblinNativeOrigin[] = { 0x4e505431, 0, 0x4b, 0x30, 0xbdce5d44u };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_MB_ORIGIN, MoblinNativeOrigin);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_OOT_EN_MB, sMoblinPortProfile, EnMb_Init,
                     EnMb_Destroy, EnMb_Update, EnMb_Draw);
