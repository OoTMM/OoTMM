#define DUEL_FIXED_FORM DUEL_WART
#include "../../../common/ovl/standalone_miniboss.c"

#if defined(GAME_MM)
static ActorProfile sProfile = {
    ACTOR_BOSS_04,
    ACTORCAT_ENEMY,
    ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED,
    0x15d,
    sizeof(StandaloneWart),
    Duel_Init,
    Duel_Destroy,
    Duel_Update,
    Duel_Draw,
};
OVL_INFO_ACTOR(0x19d, sProfile);
#else
static ActorProfile sProfile = {
    0xd2c,
    ACTORCAT_ENEMY,
    ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED,
    0x115d,
    sizeof(StandaloneWart),
    Duel_Init,
    Duel_Destroy,
    Duel_Update,
    Duel_Draw,
};
OVL_INFO_NATIVE_ENEMY(0x12c, sProfile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_BOSS_04, sProfile, Duel_Init, Duel_Destroy, Duel_Update, Duel_Draw);
static const u32 kOrigin[] = { 0x4e505431, 1, 0x12c, 0x15d, 0x780861a3 };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_BOSS_04_ORIGIN, kOrigin);
#endif
