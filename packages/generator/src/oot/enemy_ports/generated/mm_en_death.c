#define DUEL_FIXED_FORM DUEL_GOMESS
#include "../../../common/ovl/standalone_miniboss.c"

#if defined(GAME_MM)
static ActorProfile sProfile = {
    ACTOR_EN_DEATH,
    ACTORCAT_ENEMY,
    ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED |
        ACTOR_FLAG_DRAW_CULLING_DISABLED,
    0x52,
    sizeof(StandaloneGomess),
    Duel_Init,
    Duel_Destroy,
    Duel_Update,
    Duel_Draw,
};
OVL_INFO_ACTOR(0x18a, sProfile);
#else
static ActorProfile sProfile = {
    0xc43,
    ACTORCAT_ENEMY,
    ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED |
        ACTOR_FLAG_DRAW_CULLING_DISABLED,
    0x1052,
    sizeof(StandaloneGomess),
    Duel_Init,
    Duel_Destroy,
    Duel_Update,
    Duel_Draw,
};
OVL_INFO_NATIVE_ENEMY(0x43, sProfile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_EN_DEATH, sProfile, Duel_Init, Duel_Destroy, Duel_Update, Duel_Draw);
static const u32 kOrigin[] = { 0x4e505431, 1, 0x43, 0x52, 0x5112de68 };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_DEATH_ORIGIN, kOrigin);
#endif
