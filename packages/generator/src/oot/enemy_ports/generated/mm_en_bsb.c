#define DUEL_FIXED_FORM DUEL_KEETA
#include "../../../common/ovl/standalone_miniboss.c"

#if defined(GAME_MM)
static ActorProfile sProfile = {
    ACTOR_EN_BSB,
    ACTORCAT_ENEMY,
    ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED,
    0x268,
    sizeof(StandaloneKeeta),
    Duel_Init,
    Duel_Destroy,
    Duel_Update,
    Duel_Draw,
};
OVL_INFO_ACTOR(0x1cb, sProfile);
#else
static ActorProfile sProfile = {
    0xe8f,
    ACTORCAT_ENEMY,
    ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED,
    0x1268,
    sizeof(StandaloneKeeta),
    Duel_Init,
    Duel_Destroy,
    Duel_Update,
    Duel_Draw,
};
OVL_INFO_NATIVE_ENEMY(0x28f, sProfile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_EN_BSB, sProfile, Duel_Init, Duel_Destroy, Duel_Update, Duel_Draw);
static const u32 kOrigin[] = { 0x4e505431, 1, 0x28f, 0x268, 0x67992926 };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_BSB_ORIGIN, kOrigin);
#endif
