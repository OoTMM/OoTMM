#include <combo/actor_ovl.h>
#include "../garo.c"

OVL_INFO_NATIVE_ENEMY(0x113, Enemy_Garo_Profile);
static const u32 GaroNativeOrigin[] = { 0x4e505431, 1, 0x113, 0x155, 0xc8d13e73u };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_JSO_ORIGIN, GaroNativeOrigin);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_EN_JSO, Enemy_Garo_Profile, EnJso_Init,
                     EnJso_Destroy, EnJso_Update, EnJso_Draw);
