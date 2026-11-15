#define DUEL_FIXED_FORM DUEL_SNAPPER
#include "../../../common/ovl/standalone_miniboss.c"

#if defined(GAME_MM)
static ActorProfile sProfile = {
    ACTOR_EN_PAMETFROG,
    ACTORCAT_ENEMY,
    ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED,
    0x128,
    sizeof(StandaloneSnapper),
    Duel_Init,
    Duel_Destroy,
    Duel_Update,
    Duel_Draw,
};
OVL_INFO_ACTOR(0x1b2, sProfile);
#else
static ActorProfile sProfile = {
    0xc07,
    ACTORCAT_ENEMY,
    ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED,
    0x1128,
    sizeof(StandaloneSnapper),
    Duel_Init,
    Duel_Destroy,
    Duel_Update,
    Duel_Draw,
};
OVL_INFO_NATIVE_ENEMY(0x7, sProfile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_EN_PAMETFROG, sProfile, Duel_Init, Duel_Destroy, Duel_Update, Duel_Draw);
static const u32 kOrigin[] = { 0x4e505431, 1, 0x7, 0x128, 0x4ca5a6ff };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_PAMETFROG_ORIGIN, kOrigin);
#endif
