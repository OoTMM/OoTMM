#define DUEL_FIXED_FORM DUEL_SLIME
#include "../../../common/ovl/standalone_miniboss.c"

#if defined(GAME_MM)
static ActorProfile sProfile = {
    ACTOR_EN_BIGSLIME,
    ACTORCAT_ENEMY,
    ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED,
    0x128,
    sizeof(StandaloneSlime),
    Duel_Init,
    Duel_Destroy,
    Duel_Update,
    Duel_Draw,
};
OVL_INFO_ACTOR(0x1bf, sProfile);
#else
static ActorProfile sProfile = {
    0xc65,
    ACTORCAT_ENEMY,
    ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED,
    0x1128,
    sizeof(StandaloneSlime),
    Duel_Init,
    Duel_Destroy,
    Duel_Update,
    Duel_Draw,
};
OVL_INFO_NATIVE_ENEMY(0x65, sProfile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_EN_BIGSLIME, sProfile, Duel_Init, Duel_Destroy, Duel_Update, Duel_Draw);
static const u32 kOrigin[] = { 0x4e505431, 1, 0x65, 0x128, 0xb0ae9cde };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_BIGSLIME_ORIGIN, kOrigin);
#endif
