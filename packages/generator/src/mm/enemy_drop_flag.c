#include <combo.h>
#include <combo/enemy.h>

static u8 enemyOotDropFlag(Actor* actor, ColliderElement* hit, s32 freeze)
{
    if (!hit)
        return 0;
    u32 flags = comboEnemyAttackFlags(hit);
    if (freeze && (flags & ((1u << 28) | (1u << 18) | (1u << 17))))
    {
        actor->freezeTimer = comboEnemyAttackDamage(hit);
        return 0;
    }
    static const u8 arrowBits[] = { 11, 12, 14, 15, 16, 13 };
    for (unsigned i = 0; i < sizeof(arrowBits); ++i)
        if (flags & (1u << arrowBits[i]))
            return 1u << i;
    if (flags & (1u << 19))
    {
        if (freeze)
            actor->freezeTimer = comboEnemyAttackDamage(hit);
        return 0x40;
    }
    return 0;
}

void enemySetOotDropFlag(Actor* actor, ColliderElement* element, s32 freeze)
{
    actor->dropFlag = enemyOotDropFlag(actor, element->acHitElem, freeze);
}

void enemySetOotJointDropFlag(Actor* actor, ColliderJntSph* collider, s32 freeze)
{
    u8 flags = 0;
    for (s32 i = collider->count; i > 0; --i)
        flags |= enemyOotDropFlag(actor, collider->elements[i - 1].base.acHitElem, freeze);
    actor->dropFlag = flags;
}

EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_DROP_FLAG, enemySetOotDropFlag);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_DROP_FLAG_JOINT, enemySetOotJointDropFlag);
