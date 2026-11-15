#define DUEL_FIXED_FORM DUEL_IGOS
#include "../../../common/ovl/standalone_miniboss.c"

#if defined(GAME_MM)
static ActorProfile sProfile = {
    ACTOR_EN_KNIGHT,
    ACTORCAT_ENEMY,
    ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED,
    0x156,
    sizeof(StandaloneIgos),
    Duel_Init,
    Duel_Destroy,
    Duel_Update,
    Duel_Draw,
};
OVL_INFO_ACTOR(0x1c6, sProfile);
#else
static ActorProfile sProfile = {
    0xd15,
    ACTORCAT_ENEMY,
    ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED,
    0x1156,
    sizeof(StandaloneIgos),
    Duel_Init,
    Duel_Destroy,
    Duel_Update,
    Duel_Draw,
};
OVL_INFO_NATIVE_ENEMY(0x115, sProfile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_EN_KNIGHT, sProfile, Duel_Init, Duel_Destroy, Duel_Update, Duel_Draw);
static const u32 kOrigin[] = { 0x4e505431, 1, 0x115, 0x156, 0xb50d8078 };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_KNIGHT_ORIGIN, kOrigin);
#endif
