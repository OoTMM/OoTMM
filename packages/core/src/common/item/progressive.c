#include <combo.h>
#include <combo/item.h>

s32 comboProgressive(s32 gi)
{
    gi = comboProgressiveOot(gi);
    gi = comboProgressiveMm(gi);
    return gi;
}

s16 comboRenewable(s16 gi, s16 def)
{
    switch (gi)
    {
    case GI_MM_RECOVERY_HEART:
    case GI_MM_STICK:
    case GI_MM_NUT:
    case GI_MM_NUTS_5:
    case GI_MM_NUTS_10:
    case GI_MM_BOMB:
    case GI_MM_BOMBS_5:
    case GI_MM_BOMBS_10:
    case GI_MM_BOMBS_20:
    case GI_MM_BOMBS_30:
    case GI_MM_BOMBCHU:
    case GI_MM_BOMBCHU_5:
    case GI_MM_BOMBCHU_10:
    case GI_MM_BOMBCHU_20:
    case GI_MM_ARROWS_10:
    case GI_MM_ARROWS_30:
    case GI_MM_ARROWS_40:
    case GI_MM_SHIELD_HERO:
    case GI_MM_MILK:
    case GI_MM_CHATEAU:
    case GI_MM_FISH:
    case GI_MM_BUGS:
    case GI_MM_FAIRY:
    case GI_MM_POTION_RED:
    case GI_MM_POTION_GREEN:
    case GI_MM_POTION_BLUE:
    case GI_MM_MAGIC_BEAN:
        break;
    case GI_MM_BOTTLE_POTION_RED:
        gi = GI_MM_POTION_RED;
        break;
    case GI_MM_BOTTLE_POTION_GREEN:
        gi = GI_MM_POTION_GREEN;
        break;
    case GI_MM_BOTTLE_POTION_BLUE:
        gi = GI_MM_POTION_BLUE;
        break;
    case GI_MM_BOTTLE_CHATEAU:
        gi = GI_MM_CHATEAU;
        break;
    case GI_MM_BOTTLE_MILK:
        gi = GI_MM_MILK;
        break;
    case GI_OOT_RECOVERY_HEART:
    case GI_OOT_STICK:
    case GI_OOT_STICKS_5:
    case GI_OOT_STICKS_10:
    case GI_OOT_NUTS_5:
    case GI_OOT_NUTS_5_ALT:
    case GI_OOT_NUTS_10:
    case GI_OOT_BOMB:
    case GI_OOT_BOMBS_5:
    case GI_OOT_BOMBS_10:
    case GI_OOT_BOMBS_20:
    case GI_OOT_BOMBS_30:
    case GI_OOT_ARROWS_5:
    case GI_OOT_ARROWS_10:
    case GI_OOT_ARROWS_30:
    case GI_OOT_DEKU_SEEDS_5:
    case GI_OOT_DEKU_SEEDS_30:
    case GI_OOT_BOMBCHU_5:
    case GI_OOT_BOMBCHU_10:
    case GI_OOT_BOMBCHU_20:
    case GI_OOT_SHIELD_DEKU:
    case GI_OOT_SHIELD_HYLIAN:
    case GI_OOT_MILK:
    case GI_OOT_POTION_RED:
    case GI_OOT_POTION_GREEN:
    case GI_OOT_POTION_BLUE:
    case GI_OOT_FAIRY:
    case GI_OOT_FISH:
    case GI_OOT_BUG:
    case GI_OOT_BLUE_FIRE:
    case GI_OOT_POE:
        break;
    case GI_OOT_BOTTLE_MILK:
        gi = GI_OOT_MILK;
        break;
    case GI_OOT_BOTTLE_POTION_RED:
        gi = GI_OOT_POTION_RED;
        break;
    case GI_OOT_BOTTLE_POTION_GREEN:
        gi = GI_OOT_POTION_GREEN;
        break;
    case GI_OOT_BOTTLE_POTION_BLUE:
        gi = GI_OOT_POTION_BLUE;
        break;
    default:
        return def;
    }

    return gi;
}
