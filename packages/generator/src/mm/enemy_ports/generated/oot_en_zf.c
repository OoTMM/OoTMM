#include <combo/actor_ovl.h>
#include "../lizalfos.c"

OVL_INFO_NATIVE_ENEMY(0x25, EnemyLizalfosProfile);
static const u32 LizalfosNativeOrigin[] = { 0x4e505431, 0, 0x25, 0x1b, 0xa1ec7d77u };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_ZF_ORIGIN, LizalfosNativeOrigin);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_OOT_EN_ZF, EnemyLizalfosProfile, EnZf_Init,
                     EnZf_Destroy, EnZf_Update, EnZf_Draw);
