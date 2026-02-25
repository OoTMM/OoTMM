#include <combo.h>

u8 CollisionCheck_GetSwordDamageWrapper(u32 flags)
{
    u8 dmg;
    u8 kokiriLevel;

    dmg = CollisionCheck_GetSwordDamage(flags);
    kokiriLevel = 1 + (u8)gSharedCustomSave.extraSwordsOot;
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
