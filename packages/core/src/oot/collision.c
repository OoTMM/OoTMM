#include <combo.h>

#define DMG_DEKU_NUT     (1 << 0)
#define DMG_DEKU_STICK   (1 << 1)
#define DMG_SLINGSHOT    (1 << 2)
#define DMG_EXPLOSIVE    (1 << 3)
#define DMG_BOOMERANG    (1 << 4)
#define DMG_ARROW_NORMAL (1 << 5)
#define DMG_HAMMER_SWING (1 << 6)
#define DMG_HOOKSHOT     (1 << 7)
#define DMG_SLASH_KOKIRI (1 << 8)
#define DMG_SLASH_MASTER (1 << 9)
#define DMG_SLASH_GIANT  (1 << 10)
#define DMG_ARROW_FIRE   (1 << 11)
#define DMG_ARROW_ICE    (1 << 12)
#define DMG_ARROW_LIGHT  (1 << 13)
#define DMG_ARROW_UNK1   (1 << 14)
#define DMG_ARROW_UNK2   (1 << 15)
#define DMG_ARROW_UNK3   (1 << 16)
#define DMG_MAGIC_FIRE   (1 << 17)
#define DMG_MAGIC_ICE    (1 << 18)
#define DMG_MAGIC_LIGHT  (1 << 19)
#define DMG_SHIELD       (1 << 20)
#define DMG_MIR_RAY      (1 << 21)
#define DMG_SPIN_KOKIRI  (1 << 22)
#define DMG_SPIN_GIANT   (1 << 23)
#define DMG_SPIN_MASTER  (1 << 24)
#define DMG_JUMP_KOKIRI  (1 << 25)
#define DMG_JUMP_GIANT   (1 << 26)
#define DMG_JUMP_MASTER  (1 << 27)
#define DMG_UNKNOWN_1    (1 << 28)
#define DMG_UNBLOCKABLE  (1 << 29)
#define DMG_HAMMER_JUMP  (1 << 30)
#define DMG_UNKNOWN_2    (1 << 31)

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
