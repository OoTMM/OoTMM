#include <combo.h>
#include "combo/inventory.h"

static u8 CollisionCheck_GetDamageTableEntry(const DamageTable* tbl, s32 type)
{
    u8 entry;
    u8 variant;
    s32 damage;

    entry = tbl->attack[type];
    if (type != 8 && type != 22 && type != 25)
        return entry;
    if ((tbl->attack[10] & 0xf) != 4)
        return entry;

    variant = OotChildSword_GetDamageVariant();
    if (!variant)
        return entry;

    damage = 1 + variant;
    if (type == 25)
        damage *= 2;
    if ((tbl->attack[8] & 0xf0) != (tbl->attack[10] & 0xf0))
        entry = tbl->attack[10];

    return (entry & 0xf0) | damage;
}

void CollisionCheck_ApplyDamageCustom(PlayState* play, CollisionCheckContext* colChkCtx, Collider* col, ColliderElement* elem)
{
    DamageTable* tbl;
    f32 damage;

    (void)play;
    (void)colChkCtx;

    if (col->actor == NULL || !(col->acFlags & AC_HIT))
        return;
    if (!(elem->acElemFlags & ACELEM_HIT) || (elem->acElemFlags & ACELEM_NO_DAMAGE))
        return;

    tbl = col->actor->colChkInfo.damageTable;
    if (tbl == NULL)
    {
        damage = (f32)elem->acHitElem->atDmgInfo.damage - elem->acDmgInfo.defense;
        if (damage < 0)
            damage = 0;
    }
    else
    {
        s32 i;
        u32 flags;
        u8 entry;

        flags = elem->acHitElem->atDmgInfo.dmgFlags;
        for (i = 0; i < 32; i++, flags >>= 1)
        {
            if (flags == 1)
                break;
        }

        entry = CollisionCheck_GetDamageTableEntry(tbl, i);
        damage = entry & 0xf;
        col->actor->colChkInfo.damageEffect = (entry >> 4) & 0xf;
    }

    if (!(col->acFlags & AC_HARD))
        col->actor->colChkInfo.damage += damage;
}

PATCH_FUNC(0x800503dc, CollisionCheck_ApplyDamageCustom);

u8 CollisionCheck_GetSwordDamageWrapper(u32 flags)
{
    u8 dmg;
    u8 kokiriLevel;

    dmg = CollisionCheck_GetSwordDamage(flags);
    kokiriLevel = 1 + OotChildSword_GetDamageVariant();
    if (flags & (DMG_SLASH_KOKIRI | DMG_SPIN_KOKIRI))
        dmg = kokiriLevel;
    else if (flags & DMG_JUMP_KOKIRI)
        dmg = kokiriLevel * 2;
    return dmg;
}

PATCH_CALL(0x808a1b28, CollisionCheck_GetSwordDamageWrapper);
PATCH_CALL(0x808ab9b4, CollisionCheck_GetSwordDamageWrapper);
PATCH_CALL(0x808b5914, CollisionCheck_GetSwordDamageWrapper);
PATCH_CALL(0x808f7910, CollisionCheck_GetSwordDamageWrapper);
PATCH_CALL(0x809693f4, CollisionCheck_GetSwordDamageWrapper);
PATCH_CALL(0x809ab998, CollisionCheck_GetSwordDamageWrapper);
PATCH_CALL(0x809d988c, CollisionCheck_GetSwordDamageWrapper);
PATCH_CALL(0x809f882c, CollisionCheck_GetSwordDamageWrapper);
