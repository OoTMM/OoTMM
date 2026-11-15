#include <combo.h>
#include <combo/enemy.h>
#include <combo/player.h>

typedef struct
{
    Actor* owner;
    ColliderElement* element;
    u32 nativeFlags;
    u32 sourceFlags;
    u8 power;
} EnemyNativeAttack;
static EnemyNativeAttack sNativeAttacks[32];
extern u32 gForeignEnemyDamageTablesVrom;

void comboEnemyAttackRecord(Actor* owner, ColliderElement* element, u32 sourceFlags)
{
    EnemyNativeAttack* available = NULL;
    if (!owner || !element)
        return;
    for (u32 i = 0; i < ARRAY_COUNT(sNativeAttacks); ++i)
    {
        EnemyNativeAttack* entry = &sNativeAttacks[i];
        if (entry->element == element)
        {
            available = entry;
            break;
        }
        if (!entry->element && !available)
            available = entry;
    }
    if (!available)
        return;
    if (!gForeignEnemyDamageTablesVrom || !sourceFlags)
    {
        memset(available, 0, sizeof(*available));
        return;
    }
    available->owner = owner;
    available->element = element;
    available->nativeFlags = element->atDmgInfo.dmgFlags;
    available->power = element->atDmgInfo.damage;
    available->sourceFlags = sourceFlags;
}

u32 comboEnemyAttackProvenance(ColliderElement* element)
{
    if (!element)
        return 0;
    for (u32 i = 0; i < ARRAY_COUNT(sNativeAttacks); ++i)
    {
        EnemyNativeAttack* entry = &sNativeAttacks[i];
        if (entry->element == element && entry->nativeFlags == element->atDmgInfo.dmgFlags && entry->power == element->atDmgInfo.damage)
            return entry->sourceFlags;
    }
    return 0;
}

void comboEnemyAttackForget(Actor* owner)
{
    for (u32 i = 0; i < ARRAY_COUNT(sNativeAttacks); ++i)
        if (sNativeAttacks[i].owner == owner)
            memset(&sNativeAttacks[i], 0, sizeof(sNativeAttacks[i]));
}

void comboEnemyAttackReset(void)
{
    memset(sNativeAttacks, 0, sizeof(sNativeAttacks));
}

u32 comboEnemyMeleeFlags(Player* player, u32 flags, int strong, int spin)
{
    u32 tier = 0;
    static const u8 slash[] = { 8, 9, 10 };
    static const u8 jump[] = { 25, 27, 26 };
    static const u8 spins[] = { 22, 24, 23 };
    if (!player)
        return 0;
    if (flags == (1u << 8))
        return 1u << (strong ? 30 : 6);
    if (flags != (1u << 9))
        return 0;
    switch (player->heldItemAction)
    {
    case PLAYER_CUSTOM_IA_SWORD_MASTER: tier = 1; break;
    case PLAYER_CUSTOM_IA_SWORD_GIANTS_KNIFE:
    case PLAYER_CUSTOM_IA_SWORD_BIGGORON:
    case PLAYER_IA_SWORD_TWO_HANDED: tier = 2; break;
    default:
        if (player->transformation != MM_PLAYER_FORM_HUMAN)
            tier = 2;
        break;
    }
    return 1u << (spin ? spins[tier] : strong ? jump[tier]
                                              : slash[tier]);
}

static u8 enemyForwardHammer(PlayState* play, Collider* collider)
{
    Player* player = GET_PLAYER(play);
    if (!(collider->acFlags & AC_TYPE_PLAYER) || !player->meleeWeaponState)
        return 0;
    if (player->heldItemAction == PLAYER_CUSTOM_IA_HAMMER)
        return player->meleeWeaponAnimation == PLAYER_MWA_FORWARD_SLASH_2H;
    if (player->transformation != MM_PLAYER_FORM_GORON)
        return 0;
    return player->meleeWeaponAnimation == PLAYER_MWA_GORON_PUNCH_LEFT ||
           player->meleeWeaponAnimation == PLAYER_MWA_GORON_PUNCH_RIGHT ||
           player->meleeWeaponAnimation == PLAYER_MWA_GORON_PUNCH_BUTT;
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_FORWARD_HAMMER, enemyForwardHammer);

static u8 enemyOotSwordDamage(u32 flags)
{
    if (flags & ((1u << 22) | (1u << 8)))
        return 1;
    if (flags & ((1u << 25) | (1u << 24) | (1u << 9) | (1u << 6) | (1u << 1)))
        return 2;
    if (flags & ((1u << 30) | (1u << 27) | (1u << 23) | (1u << 10)))
        return 4;
    if (flags & (1u << 26))
        return 8;
    return 0;
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_OOT_SWORD_DAMAGE, enemyOotSwordDamage);

static s32 enemyOotMirrorShield(PlayState* play)
{
    return ((u8(*)(PlayState*))0x8012405c)(play) != 0;
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_OOT_MIRROR_SHIELD, enemyOotMirrorShield);
