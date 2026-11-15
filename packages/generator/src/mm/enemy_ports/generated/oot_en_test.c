#include "../stalfos.c"
OVL_INFO_NATIVE_ENEMY(2, StalfosPort_Profile);

static const u32 StalfosNativeOrigin[] = { 0x4e505431, 0, 2, 0x32, 0x894944b3u };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_TEST_ORIGIN, StalfosNativeOrigin);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_OOT_EN_TEST, StalfosPort_Profile, StalfosPort_EnTest_Init,
                     StalfosPort_EnTest_Destroy, StalfosPort_EnTest_Update, StalfosPort_EnTest_Draw);
