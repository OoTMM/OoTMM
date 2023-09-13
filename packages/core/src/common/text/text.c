#include <combo.h>
#include <combo/text.h>
#include <combo/item.h>

#define C0   TEXT_COLOR_TEAL
#define C1   TEXT_COLOR_RED
#define C2   TEXT_COLOR_PINK
#define C3   TEXT_COLOR_YELLOW

#define X(a, b, c, d, e, text) text
static const char* const kItemNamesOot[] = {
#include "../data/oot/gi.inc"
};

static const char* const kItemNamesMm[] = {
#include "../data/mm/gi.inc"
};
#undef X

typedef struct
{
    char* prepos;
    char* name;
}
RegionName;

const RegionName kRegionNamesOot[] = {
    { "in",         "the " TEXT_COLOR_YELLOW "Sacred Realm" },
    { "inside",     "the " TEXT_COLOR_GREEN "Deku Tree" },
    { "inside",     TEXT_COLOR_RED "Dodongo's Cavern" },
    { "inside",     TEXT_COLOR_BLUE "Jabu-Jabu" },
    { "in",         "the " TEXT_COLOR_GREEN "Forest Temple" },
    { "in",         "the " TEXT_COLOR_RED "Fire Temple" },
    { "in",         "the " TEXT_COLOR_BLUE "Water Temple" },
    { "in",         "the " TEXT_COLOR_ORANGE "Spirit Temple" },
    { "in",         "the " TEXT_COLOR_PINK "Shadow Temple" },
    { "at",         "the " TEXT_COLOR_PINK "Bottom of the Well" },
    { "in",         "the " TEXT_COLOR_TEAL "Ice Cavern" },
    { "in",         TEXT_COLOR_YELLOW "Gerudo Training Grounds" },
    { "in",         "the " TEXT_COLOR_YELLOW "Thieve's Hideout" },
    { "inside",     TEXT_COLOR_RED "Ganon's Castle" },
    { "in",         "the " TEXT_COLOR_GREEN "Kokiri Forest" },
    { "in",         TEXT_COLOR_YELLOW "Hyrule Field" },
    { "in",         TEXT_COLOR_YELLOW "Hyrule's Market" },
    { "in",         TEXT_COLOR_ORANGE "Lon Lon Ranch" },
    { "in",         TEXT_COLOR_YELLOW "Hyrule's Castle" },
    { "in",         TEXT_COLOR_RED "Ganon's Castle Exterior" },
    { "in",         "the " TEXT_COLOR_GREEN "Lost Woods" },
    { "in",         "the " TEXT_COLOR_GREEN "Sacred Meadow" },
    { "in",         TEXT_COLOR_RED "Kakariko" },
    { "inside",     TEXT_COLOR_PINK "Kakariko's Graveyard" },
    { "on",         TEXT_COLOR_RED "Death Mountain's Trail" },
    { "in",         TEXT_COLOR_RED "Death Mountain's Crater" },
    { "in",         TEXT_COLOR_RED "Goron City" },
    { "in",         TEXT_COLOR_BLUE "Zora's River" },
    { "in",         TEXT_COLOR_BLUE "Zora's Domain" },
    { "in",         TEXT_COLOR_BLUE "Zora's Fountain" },
    { "around",     TEXT_COLOR_BLUE "Lake Hylia" },
    { "in",         "the " TEXT_COLOR_YELLOW "Temple of Time" },
    { "in",         TEXT_COLOR_ORANGE "Gerudo Valley" },
    { "around",     TEXT_COLOR_ORANGE "Gerudo Fortress" },
    { "in",         "the " TEXT_COLOR_YELLOW "Haunted Wastelands" },
    { "around",     "the " TEXT_COLOR_ORANGE "Desert Colossus" },
    { "inside",     "an " TEXT_COLOR_YELLOW "Egg" },
};

const RegionName kRegionNamesMm[] = {
    { "in",         TEXT_COLOR_GREEN "Woodfall Temple" },
    { "in",         TEXT_COLOR_TEAL "Snowhead Temple" },
    { "in",         TEXT_COLOR_BLUE "Great Bay Temple" },
    { "in",         TEXT_COLOR_ORANGE "Stone Tower Temple" },
    { "in",         TEXT_COLOR_YELLOW "South Clock Town" },
    { "in",         TEXT_COLOR_YELLOW "North Clock Town" },
    { "in",         TEXT_COLOR_YELLOW "East Clock Town" },
    { "in",         TEXT_COLOR_YELLOW "West Clock Town" },
    { "in",         "the " TEXT_COLOR_YELLOW "Laundry Pool" },
    { "inside",     "the " TEXT_COLOR_PINK "Giant's Dream" },
    { "on",         TEXT_COLOR_YELLOW "Clock Tower Roof" },
    { "in",         "the " TEXT_COLOR_RED "Stock Pot Inn" },
    { "in",         TEXT_COLOR_RED "Termina Field" },
    { "on",         "the " TEXT_COLOR_GREEN "Road to the Swamp" },
    { "in",         "the " TEXT_COLOR_GREEN "Southern Swamp" },
    { "in",         TEXT_COLOR_GREEN "Deku Palace" },
    { "in",         TEXT_COLOR_GREEN "Woodfall" },
    { "on",         "the " TEXT_COLOR_RED "Path to Mountain Village" },
    { "in",         "the " TEXT_COLOR_RED "Mountain Village" },
    { "on",         "the " TEXT_COLOR_RED "Path to Snowhead" },
    { "around",     TEXT_COLOR_RED "Twin Islands" },
    { "around",     TEXT_COLOR_RED "Goron Village" },
    { "in",         TEXT_COLOR_RED "Snowhead" },
    { "around",     "the " TEXT_COLOR_PINK "Milk Road" },
    { "around",     TEXT_COLOR_ORANGE "Romani Ranch" },
    { "around",     TEXT_COLOR_BLUE "Great Bay Coast" },
    { "in",         "the " TEXT_COLOR_BLUE "Pirate's Fortress Exterior" },
    { "in",         "the " TEXT_COLOR_BLUE "Pirate's Fortress Sewers" },
    { "in",         "the " TEXT_COLOR_BLUE "Pirate's Fortress Interior" },
    { "around",     TEXT_COLOR_BLUE "Zora Cape" },
    { "in",         TEXT_COLOR_BLUE "Zora Hall" },
    { "in",         TEXT_COLOR_BLUE "Pinnacle Rock" },
    { "on",         "the " TEXT_COLOR_ORANGE "Road to Ikana" },
    { "in",         TEXT_COLOR_PINK "Ikana's Graveyard" },
    { "in",         TEXT_COLOR_ORANGE "Ikana Canyon" },
    { "in",         "the " TEXT_COLOR_ORANGE "Ancient Castle of Ikana" },
    { "somewhere",  TEXT_COLOR_PINK "Beneath The Well" },
    { "in",         "a " TEXT_COLOR_YELLOW "Secret Shrine" },
    { "on",         "the " TEXT_COLOR_ORANGE "Stone Tower" },
    { "on",         "the " TEXT_COLOR_RED "Moon" },
    { "in",         "the " TEXT_COLOR_GREEN "Swamp Spider House" },
    { "in",         "the " TEXT_COLOR_BLUE "Ocean Spider House" },
    { "from",       TEXT_COLOR_GREEN "Tingle" },
};

static const char* const kCheckNamesOot[] = {
    "the " TEXT_COLOR_BLUE "Frogs Ocarina Game",
    TEXT_COLOR_BLUE "Fishing",
    "a " TEXT_COLOR_PINK "Ravaged Village",
    TEXT_COLOR_BLUE "King Zora",
    "the " TEXT_COLOR_RED "Great Fairy outside of Ganon's Castle",
    "the " TEXT_COLOR_RED "Fire Temple Hammer Chest",
    "the " TEXT_COLOR_RED "Fire Temple Scarecrow Chest",
    "the " TEXT_COLOR_YELLOW "Gerudo Training Grounds Water Room",
    "the " TEXT_COLOR_ORANGE "Haunted Wastelands Chest",
    "the " TEXT_COLOR_YELLOW "Gerudo Archery",
    "the " TEXT_COLOR_GREEN "Cow in Link's house",
    TEXT_COLOR_RED "Biggoron",
    "the " TEXT_COLOR_TEAL "Ice Cavern Final Chest",
    "the " TEXT_COLOR_YELLOW "Market Treasure Game",
    TEXT_COLOR_RED "Shooting at the Sun",
};

static const char* const kCheckNamesMm[] = {
    "the " TEXT_COLOR_ORANGE "Ranch Defense",
    "the " TEXT_COLOR_GREEN  "Butler Race",
    TEXT_COLOR_PINK "Anju and Kafei",
    TEXT_COLOR_BLUE "Don Gero's Choir",
    "the " TEXT_COLOR_RED "Goron Race",
    "the " TEXT_COLOR_PINK "Graveyard Big Poe",
    "the " TEXT_COLOR_TEAL "Bank's Final Reward",
    "the " TEXT_COLOR_TEAL "Milk Bar Performance",
    "the " TEXT_COLOR_GREEN "Boat Archery",
    "the " TEXT_COLOR_BLUE "Ocean Spider House Chest",
    "the " TEXT_COLOR_BLUE "Pinnacle Rock Seahorses",
    "the " TEXT_COLOR_BLUE "Fisherman's Game",
    TEXT_COLOR_ORANGE "Igos du Ikana",
    "the " TEXT_COLOR_YELLOW "Secret Shrine Wart and Final Chest",
    "the " TEXT_COLOR_PINK "Cow Beneath The Well",
    "the " TEXT_COLOR_RED "Blacksmith",
    "the " TEXT_COLOR_PINK "Midnight Meeting",
    TEXT_COLOR_BLUE "Madame Aroma in the Bar",
    TEXT_COLOR_YELLOW "Marching for Cuccos",
    TEXT_COLOR_PINK "Finding Kafei",
    "an " TEXT_COLOR_PINK "Invisible Soldier",
};

static int isItemAmbiguousOot(s16 gi)
{
    switch (gi)
    {
    case GI_OOT_BOW:
    case GI_OOT_QUIVER2:
    case GI_OOT_QUIVER3:
    case GI_OOT_ARROWS_5:
    case GI_OOT_ARROWS_10:
    case GI_OOT_ARROWS_30:
        return !comboConfig(CFG_SHARED_BOWS);
    case GI_OOT_BOMB_BAG:
    case GI_OOT_BOMB_BAG2:
    case GI_OOT_BOMB_BAG3:
    case GI_OOT_BOMB:
    case GI_OOT_BOMBS_5:
    case GI_OOT_BOMBS_10:
    case GI_OOT_BOMBS_20:
    case GI_OOT_BOMBS_30:
        return !comboConfig(CFG_SHARED_BOMB_BAGS);
    case GI_OOT_MAGIC_UPGRADE:
    case GI_OOT_MAGIC_UPGRADE2:
        return !comboConfig(CFG_SHARED_MAGIC);
    case GI_OOT_ARROW_FIRE:
        return !comboConfig(CFG_SHARED_MAGIC_ARROW_FIRE);
    case GI_OOT_ARROW_ICE:
        return !comboConfig(CFG_SHARED_MAGIC_ARROW_ICE);
    case GI_OOT_ARROW_LIGHT:
        return !comboConfig(CFG_SHARED_MAGIC_ARROW_LIGHT);
    case GI_OOT_SONG_EPONA:
        return !comboConfig(CFG_SHARED_SONG_EPONA);
    case GI_OOT_SONG_STORMS:
        return !comboConfig(CFG_SHARED_SONG_STORMS);
    case GI_OOT_SONG_TIME:
        return !comboConfig(CFG_SHARED_SONG_TIME);
    case GI_OOT_SONG_SUN:
        return !(comboConfig(CFG_SHARED_SONG_SUN) || !comboConfig(CFG_MM_SONG_SUN));
    case GI_OOT_STICK:
    case GI_OOT_STICKS_5:
    case GI_OOT_STICKS_10:
    case GI_OOT_NUTS_5:
    case GI_OOT_NUTS_5_ALT:
    case GI_OOT_NUTS_10:
        return !comboConfig(CFG_SHARED_NUTS_STICKS);
    case GI_OOT_HOOKSHOT:
        return !comboConfig(CFG_SHARED_HOOKSHOT);
    case GI_OOT_LENS:
        return !comboConfig(CFG_SHARED_LENS);
    case GI_OOT_OCARINA_FAIRY:
        return (comboConfig(CFG_MM_OCARINA_FAIRY) && !comboConfig(CFG_SHARED_OCARINA));
    case GI_OOT_OCARINA_TIME:
        return !comboConfig(CFG_SHARED_OCARINA);
    case GI_OOT_MASK_KEATON:
        return !comboConfig(CFG_SHARED_MASK_KEATON);
    case GI_OOT_MASK_BUNNY:
        return !comboConfig(CFG_SHARED_MASK_BUNNY);
    case GI_OOT_MASK_GORON:
        return !comboConfig(CFG_SHARED_MASK_GORON);
    case GI_OOT_MASK_ZORA:
        return !comboConfig(CFG_SHARED_MASK_ZORA);
    case GI_OOT_MASK_TRUTH:
        return !comboConfig(CFG_SHARED_MASK_TRUTH);
    case GI_OOT_WALLET:
    case GI_OOT_WALLET2:
    case GI_OOT_WALLET3:
    case GI_OOT_WALLET4:
    case GI_OOT_WALLET5:
    case GI_OOT_RUPEE_GREEN:
    case GI_OOT_RUPEE_BLUE:
    case GI_OOT_RUPEE_RED:
    case GI_OOT_RUPEE_PURPLE:
    case GI_OOT_RUPEE_HUGE:
        return !comboConfig(CFG_SHARED_WALLETS);
    case GI_OOT_HEART_CONTAINER:
    case GI_OOT_HEART_CONTAINER2:
    case GI_OOT_HEART_PIECE:
    case GI_OOT_RECOVERY_HEART:
    case GI_OOT_DEFENSE_UPGRADE:
        return !comboConfig(CFG_SHARED_HEALTH);
    case GI_OOT_SOUL_OCTOROK:
    case GI_OOT_SOUL_WALLMASTER:
    case GI_OOT_SOUL_DODONGO:
    case GI_OOT_SOUL_KEESE:
    case GI_OOT_SOUL_TEKTITE:
    case GI_OOT_SOUL_PEAHAT:
    case GI_OOT_SOUL_LIZALFOS_DINALFOS:
    case GI_OOT_SOUL_SKULLTULA:
    case GI_OOT_SOUL_ARMOS:
    case GI_OOT_SOUL_DEKU_BABA:
    case GI_OOT_SOUL_DEKU_SCRUB:
    case GI_OOT_SOUL_BUBBLE:
    case GI_OOT_SOUL_BEAMOS:
    case GI_OOT_SOUL_REDEAD_GIBDO:
    case GI_OOT_SOUL_SKULLWALLTULA:
    case GI_OOT_SOUL_SHELL_BLADE:
    case GI_OOT_SOUL_LIKE_LIKE:
    case GI_OOT_SOUL_IRON_KNUCKLE:
    case GI_OOT_SOUL_FREEZARD:
    case GI_OOT_SOUL_WOLFOS:
    case GI_OOT_SOUL_GUAY:
    case GI_OOT_SOUL_FLYING_POT:
    case GI_OOT_SOUL_FLOORMASTER:
    case GI_OOT_SOUL_LEEVER:
    case GI_OOT_SOUL_STALCHILD:
        return !comboConfig(CFG_SHARED_SOULS);
    case GI_OOT_SKELETON_KEY:
        return !comboConfig(CFG_SHARED_SKELETON_KEY);
    case GI_OOT_BUTTON_A:
    case GI_OOT_BUTTON_C_RIGHT:
    case GI_OOT_BUTTON_C_LEFT:
    case GI_OOT_BUTTON_C_DOWN:
    case GI_OOT_BUTTON_C_UP:
        return !comboConfig(CFG_SHARED_OCARINA_BUTTONS);
    case GI_OOT_BOMBCHU_10:
    case GI_OOT_MAGIC_BEAN:
    case GI_OOT_BOTTLE_EMPTY:
    case GI_OOT_BOTTLE_MILK:
    case GI_OOT_SWORD_KOKIRI:
    case GI_OOT_SHIELD_HYLIAN:
    case GI_OOT_BOMBCHU_5:
    case GI_OOT_BOMBCHU_20:
    case GI_OOT_SHIELD_MIRROR:
    case GI_OOT_MILK:
    case GI_OOT_POTION_RED:
    case GI_OOT_POTION_GREEN:
    case GI_OOT_POTION_BLUE:
    case GI_OOT_FAIRY:
    case GI_OOT_BUG:
    case GI_OOT_FISH:
        return 1;
    default:
        return 0;
    }
}

static int isItemAmbiguousMm(s16 gi)
{
    switch (gi)
    {
    case GI_MM_BOW:
    case GI_MM_QUIVER2:
    case GI_MM_QUIVER3:
    case GI_MM_ARROWS_10:
    case GI_MM_ARROWS_30:
    case GI_MM_ARROWS_40:
        return !comboConfig(CFG_SHARED_BOWS);
    case GI_MM_BOMB_BAG:
    case GI_MM_BOMB_BAG2:
    case GI_MM_BOMB_BAG3:
    case GI_MM_BOMB:
    case GI_MM_BOMBS_5:
    case GI_MM_BOMBS_10:
    case GI_MM_BOMBS_20:
    case GI_MM_BOMBS_30:
        return !comboConfig(CFG_SHARED_BOMB_BAGS);
    case GI_MM_MAGIC_UPGRADE:
    case GI_MM_MAGIC_UPGRADE2:
        return !comboConfig(CFG_SHARED_MAGIC);
    case GI_MM_ARROW_FIRE:
        return !comboConfig(CFG_SHARED_MAGIC_ARROW_FIRE);
    case GI_MM_ARROW_ICE:
        return !comboConfig(CFG_SHARED_MAGIC_ARROW_ICE);
    case GI_MM_ARROW_LIGHT:
        return !comboConfig(CFG_SHARED_MAGIC_ARROW_LIGHT);
    case GI_MM_SONG_EPONA:
        return !comboConfig(CFG_SHARED_SONG_EPONA);
    case GI_MM_SONG_STORMS:
        return !comboConfig(CFG_SHARED_SONG_STORMS);
    case GI_MM_SONG_TIME:
        return !comboConfig(CFG_SHARED_SONG_TIME);
    case GI_MM_SONG_SUN:
        return !comboConfig(CFG_SHARED_SONG_SUN);
    case GI_MM_STICK:
    case GI_MM_NUT:
    case GI_MM_NUTS_5:
    case GI_MM_NUTS_10:
        return !comboConfig(CFG_SHARED_NUTS_STICKS);
    case GI_MM_HOOKSHOT:
    case GI_MM_HOOKSHOT_SHORT:
        return !comboConfig(CFG_SHARED_HOOKSHOT);
    case GI_MM_LENS:
        return !comboConfig(CFG_SHARED_LENS);
    case GI_MM_OCARINA_FAIRY:
    case GI_MM_OCARINA_OF_TIME:
        return !comboConfig(CFG_SHARED_OCARINA);
    case GI_MM_MASK_GORON:
        return !comboConfig(CFG_SHARED_MASK_GORON);
    case GI_MM_MASK_ZORA:
        return !comboConfig(CFG_SHARED_MASK_ZORA);
    case GI_MM_MASK_TRUTH:
        return !comboConfig(CFG_SHARED_MASK_TRUTH);
    case GI_MM_MASK_BUNNY:
        return !comboConfig(CFG_SHARED_MASK_BUNNY);
    case GI_MM_MASK_KEATON:
        return !comboConfig(CFG_SHARED_MASK_KEATON);
    case GI_MM_WALLET:
    case GI_MM_WALLET2:
    case GI_MM_WALLET3:
    case GI_MM_WALLET4:
    case GI_MM_WALLET5:
    case GI_MM_RUPEE_GREEN:
    case GI_MM_RUPEE_BLUE:
    case GI_MM_RUPEE_RED:
    case GI_MM_RUPEE_PURPLE:
    case GI_MM_RUPEE_SILVER:
    case GI_MM_RUPEE_GOLD:
        return !comboConfig(CFG_SHARED_WALLETS);
    case GI_MM_HEART_CONTAINER:
    case GI_MM_HEART_PIECE:
    case GI_MM_RECOVERY_HEART:
    case GI_MM_DEFENSE_UPGRADE:
        return !comboConfig(CFG_SHARED_HEALTH);
    case GI_MM_SOUL_OCTOROK:
    case GI_MM_SOUL_WALLMASTER:
    case GI_MM_SOUL_DODONGO:
    case GI_MM_SOUL_KEESE:
    case GI_MM_SOUL_TEKTITE:
    case GI_MM_SOUL_PEAHAT:
    case GI_MM_SOUL_LIZALFOS_DINALFOS:
    case GI_MM_SOUL_SKULLTULA:
    case GI_MM_SOUL_ARMOS:
    case GI_MM_SOUL_DEKU_BABA:
    case GI_MM_SOUL_DEKU_SCRUB:
    case GI_MM_SOUL_BUBBLE:
    case GI_MM_SOUL_BEAMOS:
    case GI_MM_SOUL_REDEAD_GIBDO:
    case GI_MM_SOUL_SKULLWALLTULA:
    case GI_MM_SOUL_SHELL_BLADE:
    case GI_MM_SOUL_LIKE_LIKE:
    case GI_MM_SOUL_IRON_KNUCKLE:
    case GI_MM_SOUL_FREEZARD:
    case GI_MM_SOUL_WOLFOS:
    case GI_MM_SOUL_GUAY:
    case GI_MM_SOUL_FLYING_POT:
    case GI_MM_SOUL_FLOORMASTER:
    case GI_MM_SOUL_LEEVER:
    case GI_MM_SOUL_STALCHILD:
        return !comboConfig(CFG_SHARED_SOULS);
    case GI_MM_SKELETON_KEY:
        return !comboConfig(CFG_SHARED_SKELETON_KEY);
    case GI_MM_BUTTON_A:
    case GI_MM_BUTTON_C_RIGHT:
    case GI_MM_BUTTON_C_LEFT:
    case GI_MM_BUTTON_C_DOWN:
    case GI_MM_BUTTON_C_UP:
        return !comboConfig(CFG_SHARED_OCARINA_BUTTONS);
    case GI_MM_BOMBCHU:
    case GI_MM_MAGIC_BEAN:
    case GI_MM_BOTTLE_POTION_RED:
    case GI_MM_BOTTLE_EMPTY:
    case GI_MM_SWORD_KOKIRI:
    case GI_MM_SHIELD_HERO:
    case GI_MM_BOMBCHU_20:
    case GI_MM_BOMBCHU_10:
    case GI_MM_BOMBCHU_5:
    case GI_MM_BOTTLE_MILK:
    case GI_MM_SHIELD_MIRROR:
    case GI_MM_MILK:
    case GI_MM_POTION_RED:
    case GI_MM_POTION_GREEN:
    case GI_MM_POTION_BLUE:
    case GI_MM_FAIRY:
    case GI_MM_BUGS:
    case GI_MM_FISH:
        return 1;
    default:
        return 0;
    }
}

static size_t comboTextStrlen(const char* buffer)
{
    size_t i;
    for (i = 0; ((u8)buffer[i]) != (u8)(TEXT_END[0]); i++)
        ;
    return i;
}

void comboTextAutoLineBreaks(char* buffer)
{
    static const int kMaxLineLength = 37;
    int lineLength;
    int i;
    int lastSpace;
    int lineCount;
    u8 c;

    lastSpace = -1;
    lineLength = 0;
    lineCount = 0;
    i = 0;
    for (;;)
    {
        c = (u8)(buffer[i]);
        if (c == (u8)(TEXT_END[0]))
            break;
        if (c >= ' ' && c <= '~')
        {
            lineLength++;
            if (lineLength >= kMaxLineLength && lastSpace != -1)
            {
                lineLength = i - lastSpace;
                if (lineCount < 3)
                {
                    buffer[lastSpace] = TEXT_NL[0];
                    lineCount++;
                }
                else
                {
                    /* We need to inject a box break */
                    memmove(buffer + lastSpace + 2, buffer + lastSpace + 1, comboTextStrlen(buffer + lastSpace + 1) + 1);
                    buffer[lastSpace] = TEXT_BB[0];
                    buffer[lastSpace + 1] = TEXT_FAST[0];
                    lineCount = 0;
                }
                lastSpace = -1;
            }
        }
        if (c == ' ')
        {
            lastSpace = i;
        }
        if (c == (u8)(TEXT_NL[0]) || c == (u8)(TEXT_BB[0]))
        {
            lastSpace = -1;
            lineLength = 0;
        }
        i += comboMultibyteCharSize(c);
    }
}

void comboTextAppendStr(char** dst, const char* src)
{
    size_t len = strlen(src);
    memcpy(*dst, src, len);
    *dst += len;
}

void comboTextAppendHeader(char** b)
{
#if defined(GAME_MM)
    /* MM has a header */
    memcpy(*b, "\x00\x30\xfe\xff\xff\xff\xff\xff\xff\xff\xff", 11);
    *b += 11;
#endif
    comboTextAppendStr(b, TEXT_FAST);
}

void comboTextAppendShopHeader(char** b, s16 price)
{
#if defined(GAME_MM)
    memcpy((*b) + 5, &price, 2);
    *b += 11;
#endif
    comboTextAppendStr(b, TEXT_FAST);
}

void comboTextAppendShopHeader2(char** b, s16 price1, s16 price2)
{
#if defined(GAME_MM)
    memcpy((*b) + 5, &price1, 2);
    memcpy((*b) + 7, &price2, 2);
    *b += 11;
#endif
    comboTextAppendStr(b, TEXT_FAST);
}

#if defined(GAME_MM)
static void appendBossRewardHeader(char** b, char icon)
{
    memcpy(*b, "\x06\x00\xfe\xff\xff\xff\xff\xff\xff\xff\xff", 11);
    (*b)[2] = icon;
    *b += 11;
    comboTextAppendStr(b, TEXT_FAST);
}
#endif

void comboTextAppendClearColor(char** b)
{
#if defined(GAME_OOT)
    comboTextAppendStr(b, TEXT_CZ);
#else
    /* strlen doesn't like NUL */
    **b = 0;
    *b += 1;
#endif
}

void comboTextAppendNum(char** b, int num)
{
    int denum;
    int started;
    int div;

    denum = 1000000000;
    started = 0;
    while (denum)
    {
        div = num / denum;
        num = num - div * denum;
        if (div)
            started = 1;
        if (started)
        {
            **b = '0' + div;
            *b += 1;
        }
        denum /= 10;
    }
    if (!started)
    {
        **b = '0';
        *b += 1;
    }
}

void comboTextAppendOrd(char** b, int num)
{
    const char* suffix;

    if (num >= 11 && num <= 13)
    {
        suffix = "th";
    }
    else
    {
        switch (num % 10)
        {
        case 1:
            suffix = "st";
            break;
        case 2:
            suffix = "nd";
            break;
        case 3:
            suffix = "rd";
            break;
        default:
            suffix = "th";
            break;
        }
    }

    comboTextAppendNum(b, num);
    comboTextAppendStr(b, suffix);
}

void comboTextAppendItemName(char** b, s16 gi, int flags)
{
    char* start;
    const char* itemName;
    int ambiguous;

#if defined(GAME_MM)
    gi ^= MASK_FOREIGN_GI;
#endif

    if (gi & MASK_FOREIGN_GI)
    {
        itemName = kItemNamesMm[(gi & ~MASK_FOREIGN_GI) - 1];
        ambiguous = isItemAmbiguousMm(gi & ~MASK_FOREIGN_GI);
    }
    else
    {
        itemName = kItemNamesOot[(gi & ~MASK_FOREIGN_GI) - 1];
        ambiguous = isItemAmbiguousOot(gi & ~MASK_FOREIGN_GI);
    }
    if (gi == GI_OOT_ARROW_ICE)
    {
        if (comboConfig(CFG_OOT_BLUE_FIRE_ARROWS))
        {
            itemName = "the " C1 "Blue Fire Arrows";
            ambiguous = 0;
        }
    }
    if(gi == (GI_MM_BOMBER_NOTEBOOK | MASK_FOREIGN_GI))
    {
        if(comboConfig(CFG_MENU_NOTEBOOK))
            itemName = "the " C1 "Bombers' Tracker";
    }
    if (flags & TF_PROGRESSIVE)
    {
        switch (gi)
        {
        case GI_OOT_SWORD_KOKIRI:
            if (comboConfig(CFG_OOT_PROGRESSIVE_SWORDS))
            {
                itemName = "a " C1 "Progressive Sword";
                ambiguous = 1;
            }
            break;
        case GI_OOT_SWORD_KNIFE:
            if (comboConfig(CFG_OOT_PROGRESSIVE_SWORDS_GORON))
            {
                itemName = "a " C1 "Progressive Goron Sword";
            }
            break;
        case GI_MM_SWORD_KOKIRI | MASK_FOREIGN_GI:
            itemName = "a " C1 "Progressive Sword";
            ambiguous = 1;
            break;
        case GI_OOT_PROGRESSIVE_SHIELD_DEKU:
        case GI_MM_PROGRESSIVE_SHIELD_HERO | MASK_FOREIGN_GI:
            itemName = "a " C1 "Progressive Shield";
            ambiguous = 1;
            break;
        case GI_OOT_OCARINA_FAIRY:
        case GI_MM_OCARINA_FAIRY | MASK_FOREIGN_GI:
            itemName = "a " C1 "Progressive Ocarina";
            ambiguous = !comboConfig(CFG_SHARED_OCARINA);
            break;
        case GI_OOT_HOOKSHOT:
        case GI_MM_HOOKSHOT_SHORT | MASK_FOREIGN_GI:
            itemName = "a " C1 "Progressive Hookshot";
            ambiguous = !comboConfig(CFG_SHARED_HOOKSHOT);
            break;
        case GI_OOT_WALLET:
        case GI_MM_WALLET | MASK_FOREIGN_GI:
            itemName = "a " C1 "Progressive Wallet";
            ambiguous = !comboConfig(CFG_SHARED_WALLETS);
            break;
        case GI_MM_SONG_GORON_HALF | MASK_FOREIGN_GI:
            itemName = "a " C2 "Progressive Goron Lullaby";
            break;
        case GI_OOT_SCALE_SILVER:
            itemName = "a " C1 "Progressive Scale";
            break;
        case GI_OOT_GORON_BRACELET:
            itemName = "a " C1 "Progressive Strength";
            break;
        case GI_OOT_BOW:
        case GI_MM_BOW | MASK_FOREIGN_GI:
            if (comboConfig(CFG_SHARED_BOWS))
            {
                itemName = "a " C1 "Bow";
                ambiguous = 0;
            }
            break;
        }

    }

    start = *b;
    comboTextAppendStr(b, itemName);
    comboTextAppendClearColor(b);

    if (ambiguous)
    {
        comboTextAppendStr(b, " (");
        if (gi & MASK_FOREIGN_GI)
        {
            comboTextAppendStr(b, TEXT_COLOR_PINK "MM");
        }
        else
        {
            comboTextAppendStr(b, TEXT_COLOR_TEAL "OoT");
        }
        comboTextAppendClearColor(b);
        comboTextAppendStr(b, ")");
    }

    if (flags & TF_CAPITALIZE)
    {
        start[0] = toupper(start[0]);
    }
}

void comboTextAppendItemNameQuery(char** b, const ComboItemQuery* q, int flags)
{
    ComboItemOverride o;

    comboItemOverride(&o, q);
    comboTextAppendItemNameOverride(b, &o, flags);
}

void comboTextAppendItemNameOverride(char** b, const ComboItemOverride* o, int flags)
{
    s16 gi;

    if (flags & TF_PROGRESSIVE)
        gi = o->giRaw;
    else
        gi = o->gi;
    comboTextAppendItemName(b, gi, flags);
    if (o->player != PLAYER_SELF && o->player != PLAYER_ALL && o->player != gComboData.playerId)
    {
        comboTextAppendStr(b, " for " TEXT_COLOR_YELLOW "Player ");
        comboTextAppendNum(b, o->player);
        comboTextAppendClearColor(b);
    }

    if (o->playerFrom != PLAYER_SELF && o->playerFrom != PLAYER_ALL && o->playerFrom != gComboData.playerId)
    {
        comboTextAppendStr(b, " from " TEXT_COLOR_YELLOW "Player ");
        comboTextAppendNum(b, o->playerFrom);
        comboTextAppendClearColor(b);
    }
}

void comboTextAppendRegionName(char** b, u8 regionId, u8 world, int flags)
{
    char* start;
    const RegionName* regName;

    if (regionId == 0)
    {
        if (flags & TF_PREPOS)
        {
            comboTextAppendStr(b, "inside ");
        }
        comboTextAppendStr(b, TEXT_COLOR_YELLOW "Link's Pocket");
        comboTextAppendClearColor(b);
        return;
    }

    if (regionId & 0x80)
    {
        regName = &kRegionNamesMm[(regionId & 0x7f) - 1];
    }
    else
    {
        regName = &kRegionNamesOot[(regionId & 0x7f) - 1];
    }

    start = *b;
    if (flags & TF_PREPOS)
    {
        comboTextAppendStr(b, regName->prepos);
        comboTextAppendStr(b, " ");
    }
    comboTextAppendStr(b, regName->name);
    comboTextAppendClearColor(b);

    if (world != 0 && world != 0xff && world != gComboData.playerId)
    {
        comboTextAppendStr(b, " in " TEXT_COLOR_YELLOW "World ");
        comboTextAppendNum(b, world);
        comboTextAppendClearColor(b);
    }

    if (flags & TF_CAPITALIZE)
    {
        start[0] = toupper(start[0]);
    }
}

void comboTextAppendCheckName(char** b, u8 checkId, u8 world)
{
    const char* checkName;

    if (checkId & 0x80)
    {
        checkName = kCheckNamesMm[(checkId & 0x7f) - 1];
    }
    else
    {
        checkName = kCheckNamesOot[(checkId & 0x7f) - 1];
    }

    comboTextAppendStr(b, checkName);
    comboTextAppendClearColor(b);

    if (world != 0 && world != 0xff && world != gComboData.playerId)
    {
        comboTextAppendStr(b, " in " TEXT_COLOR_YELLOW "World ");
        comboTextAppendNum(b, world);
        comboTextAppendClearColor(b);
    }
}

static int isSoldOut(s16 gi)
{
#if defined(GAME_OOT)
    return gi == (GI_MM_SOLD_OUT | MASK_FOREIGN_GI);
#else
    return gi == GI_MM_SOLD_OUT;
#endif
}

void comboTextHijackItemShop(GameState_Play* play, const ComboItemOverride* o, s16 price, int confirm)
{
    char* b;

#if defined(GAME_OOT)
    b = play->msgCtx.textBuffer;
#else
    b = play->msgCtx.font.textBuffer.schar;
#endif

    comboTextAppendShopHeader(&b, price);

    if (isSoldOut(o->gi))
    {
        comboTextAppendStr(&b, "SOLD OUT" TEXT_NOCLOSE TEXT_END);
        return;
    }

    comboTextAppendItemNameOverride(&b, o, TF_CAPITALIZE);
    comboTextAppendStr(&b, TEXT_NL TEXT_COLOR_RED);
    comboTextAppendNum(&b, price);
    comboTextAppendStr(&b, " Rupees");
    if (confirm)
    {
        comboTextAppendStr(&b, TEXT_NL TEXT_CHOICE2 TEXT_COLOR_GREEN);
        comboTextAppendStr(&b, "Buy" TEXT_NL);
        comboTextAppendStr(&b, "No thanks");
    }
    else
    {
        comboTextAppendStr(&b, TEXT_NL TEXT_NL TEXT_NOCLOSE);
    }
    comboTextAppendStr(&b, TEXT_END);
}

#if defined(GAME_OOT)
void comboMessageCancel(GameState_Play* play)
{
    u8* ctx = (u8*)(void*)&play->msgCtx;

    ctx[0xe3e7] = 2;
    ctx[0xe304] = 0x36;
    ctx[0xe3e4] = 0;

    play->msgCtx.ocarinaMode = 4;
    *(((char*)GET_LINK(play)) + 0x141) = 0;
}
#endif

#if defined(GAME_MM)
void comboTextHijackDungeonRewardHints(GameState_Play* play, int hint)
{
    const ItemHint* ih;
    char* b;

    ih = &gComboData.hints.dungeonRewards[9 + hint];
    b = play->msgCtx.font.textBuffer.schar;
    appendBossRewardHeader(&b, 0x55 + hint);
    comboTextAppendRegionName(&b, ih->region, ih->world, TF_PREPOS | TF_CAPITALIZE);
    comboTextAppendStr(&b, "...");
    if (hint != 3)
        comboTextAppendStr(&b, "\x19");
    comboTextAppendStr(&b, TEXT_END);
}
#endif

#if defined(GAME_OOT)
void comboTextHijackLightArrows(GameState_Play* play)
{
    char* b;

    b = play->msgCtx.textBuffer;
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b,
        "Have you found the " TEXT_COLOR_YELLOW "Light Arrows " TEXT_CZ
    );
    comboTextAppendRegionName(&b, gComboData.hints.lightArrows.region, gComboData.hints.lightArrows.world, TF_PREPOS);
    comboTextAppendStr(&b, "?" TEXT_END);
    comboTextAutoLineBreaks(play->msgCtx.textBuffer);
}
#endif

#if defined(GAME_MM)
void comboTextHijackOathToOrder(GameState_Play* play)
{
    char* b;
    char* start;

    b = play->msgCtx.font.textBuffer.schar;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b,
        "Have you found the " TEXT_COLOR_PINK "Oath to Order "
    );
    comboTextAppendClearColor(&b);
    comboTextAppendRegionName(&b, gComboData.hints.oathToOrder.region, gComboData.hints.oathToOrder.world, TF_PREPOS);
    comboTextAppendStr(&b, "?" TEXT_END);
    comboTextAutoLineBreaks(start);
}
#endif

void comboTextAppendNpcReward(char** b, s16 npcId, s16 gi)
{
    ComboItemQuery q = ITEM_QUERY_INIT;

    q.ovType = OV_NPC;
    q.id = npcId;
    q.gi = gi;
    comboTextAppendItemNameQuery(b, &q, TF_PREPOS | TF_PROGRESSIVE);
}

void comboTextMessageCantBuy(GameState_Play* play, int flags)
{
    char* b;
    char* start;

#if defined(GAME_OOT)
    b = play->msgCtx.textBuffer;
#else
    b = play->msgCtx.font.textBuffer.schar;
#endif

    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "You can't buy that right now!");
    if (flags & TF_SIGNAL)
        comboTextAppendStr(&b, TEXT_SIGNAL);
    comboTextAppendStr(&b, TEXT_END);
    comboTextAutoLineBreaks(start);
}
