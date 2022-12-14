#include <combo.h>

#if defined(GAME_OOT)
# define FAST           "\x08"
# define CZ             "\x05\x40"
# define COLOR_RED      "\x05\x41"
# define COLOR_GREEN    "\x05\x42"
# define COLOR_BLUE     "\x05\x43"
# define COLOR_TEAL     "\x05\x44"
# define COLOR_PINK     "\x05\x45"
# define COLOR_YELLOW   "\x05\x46"
# define COLOR_ORANGE   COLOR_YELLOW
# define END            "\x02"
# define CHOICE2        "\x1b"
# define CHOICE3        "\x1c"
# define NL             "\x01"
# define NOCLOSE        "\x0a"
# define SIGNAL         "\x0b"
# define ICON           "\x13"
# define BB             "\x04"
#else
# define FAST           "\x17"
# define CZ             "\x00"
# define COLOR_RED      "\x01"
# define COLOR_GREEN    "\x02"
# define COLOR_BLUE     "\x03"
# define COLOR_YELLOW   "\x04"
# define COLOR_TEAL     "\x05"
# define COLOR_PINK     "\x06"
# define COLOR_ORANGE   "\x08"
# define END            "\xbf"
# define CHOICE2        "\xc2"
# define CHOICE3        "\xc3"
# define NL             "\x11"
# define NOCLOSE        "\x1a"
# define ICON           ""
# define BB             ""
#endif

#define C0   COLOR_TEAL
#define C1   COLOR_RED
#define C2   COLOR_PINK
#define C3   COLOR_YELLOW

static const char* const kItemNamesOot[] = {
    "a " C0 "Deku Stick",
    "a " C0 "Deku Nut",
    "a " C0 "Bomb",
    "the " C1 "Fairy Bow",
    "the " C1 "Fire Arrow",
    C1 "Din's Fire",
    "the " C1 "Fairy Slingshot",
    "the " C1 "Fairy Ocarina",
    "the " C1 "Ocarina of Time",
    C0 "10 Bombchu",
    "the " C1 "Hookshot",
    "the " C1 "Longshot",
    "the " C1 "Ice Arrow",
    C1 "Farore's Wind",
    "the " C1 "Boomerang",
    "the " C1 "Lens of Truth",
    "a " C0 "Magic Bean",
    "the " C1 "Megaton Hammer",
    "the " C1 "Light Arrow",
    C1 "Nayru's Love",
    "an " C1 "Empty Bottle",
    "a " C0 "Red Potion",
    "a " C0 "Green Potion",
    "a " C0 "Blue Potion",
    "a " C0 "Fairy",
    "a " C0 "Fish",
    "a bottle of " C0 "Lon Lon Milk",
    C1 "Ruto's Letter",
    "some " C0 "Blue Fire",
    "a " C0 "Bug",
    "a " C0 "Big Poe",
    "half a bottle of " C0 "Lon Lon Milk",
    "a " C0 "Poe",
    "the " C1 "Weird Egg",
    "the " C1 "Chicken",
    C1 "Zelda's Letter",
    "the " C1 "Keaton Mask",
    "the " C1 "Skull Mask",
    "the " C1 "Spooky Mask",
    "the " C1 "Bunny Hood",
    "the " C1 "Goron Mask",
    "the " C1 "Zora Mask",
    "the " C1 "Gerudo Mask",
    "the " C1 "Mask of Truth",
    "", /* SOLD OUT */
    "the " C1 "Pocket Egg",
    "the " C1 "Pocket Cucco",
    C1 "Cojiro",
    "the " C1 "Odd Mushroom",
    "the " C1 "Odd Potion",
    "the " C1 "Poacher's Saw",
    "the " C1 "Broken Goron's Sword",
    "the " C1 "Prescription",
    "the " C1 "Eyeball Frog",
    "the " C1 "Eye Drops",
    "the " C1 "Claim Check",
    "", /* Bow & Fire */
    "", /* Bow & Ice */
    "", /* Bow & Light */
    "the " C1 "Kokiri Sword",
    "the " C1 "Master Sword",
    "the " C1 "Giant's Knife",
    "the " C1 "Deku Shield",
    "the " C1 "Hylian Shield",
    "the " C1 "Mirror Shield",
    "", /* Kokiri Tunic */
    "the " C1 "Goron Tunic",
    "the " C1 "Zora Tunic",
    "", /* Kokiri Boots */
    "the " C1 "Iron Boots",
    "the " C1 "Hover Boots",
    "", /* Bullet bag */
    "the " C1 "Large Bullet Bag",
    "the " C1 "Largest Bullet Bag",
    "", /* Quiver */
    "the " C1 "Big Quiver",
    "the " C1 "Biggest Quiver",
    "the " C1 "Bomb Bag",
    "the " C1 "Big Bomb Bag",
    "the " C1 "Biggest Bomb Bag",
    "the " C1 "Goron's Bracelet",
    "the " C1 "Silver Gauntlets",
    "the " C1 "Golden Gauntlets",
    "the " C1 "Silver Scale",
    "the " C1 "Golden Scale",
    "the " C1 "Giant's Knife",
    "the " C1 "Adult's Wallet",
    "the " C1 "Giant's Wallet",
    C0 "5 Deku Seeds",
    "", /* Fishing pole */
    "the " C2 "Minuet of Forest",
    "the " C2 "Bolero of Fire",
    "the " C2 "Serenade of Water",
    "the " C2 "Requiem of Spirit",
    "the " C2 "Nocturne of Shadow",
    "the " C2 "Prelude of Light",
    C2 "Zelda's Lullaby",
    C2 "Epona's Song",
    C2 "Saria's Song",
    "the " C2 "Sun's Song",
    "the " C2 "Song of Time",
    "the " C2 "Song of Storms",
    "the " C3 "Forest Medallion",
    "the " C3 "Fire Medallion",
    "the " C3 "Water Medallion",
    "the " C3 "Spirit Medallion",
    "the " C3 "Shadow Medallion",
    "the " C3 "Light Medallion",
    "the " C3 "Kokiri's Emerald",
    "the " C3 "Goron's Ruby",
    "the " C3 "Zora's Sapphire",
    "the " C1 "Stone of Agony",
    "the " C1 "Gerudo's Card",
    "a " C0 "Gold Skulltula Token",
    "a " C0 "Heart Container",
    "a " C0 "Piece of Heart",
    "the " C0 "Big Key",
    "the " C0 "Compass",
    "the " C0 "Dungeon Map",
    "a " C0 "Small Key",
    "a " C0 "Small Magic Jar",
    "a " C0 "Large Magic Jar",
    "a " C0 "Piece of Heart",
    "", /* Dummy */
    "", /* Dummy */
    "", /* Dummy */
    "", /* Dummy */
    "", /* Dummy */
    "", /* Dummy */
    "", /* Dummy */
    "some " C0 "Lon Lon Milk",
    "a " C0 "Recovery Heart",
    "a " C0 "Green Rupee",
    "a " C0 "Blue Rupee",
    "a " C0 "Red Rupee",
    "a " C0 "Purple Rupee",
    "a " C0 "Huge Rupee",
    "", /* Dummy */
    C0 "5 Deku Sticks",
    C0 "10 Deku Sticks",
    C0 "5 Deku Nuts",
    C0 "10 Deku Nuts",
    C0 "5 Bombs",
    C0 "10 Bombs",
    C0 "20 Bombs",
    C0 "30 Bombs",
    C0 "5 Arrows",
    C0 "10 Arrows",
    C0 "30 Arrows",
    C0 "30 Deku Seeds",
    C0 "5 Bombchu",
    C0 "20 Bombchu",
    "the " C1 "Deku Stick Upgrade",
    "the " C1 "Second Deku Stick Upgrade",
    "the " C1 "Deku Nut Upgrade",
    "the " C1 "Second Deku Nut Upgrade",
    "the " C1 "Magic Upgrade",
    "the " C1 "Larger Magic Upgrade",
    "the " C1 "Defense Upgrade",
    C1 "Biggoron's Sword",
    "the " C1 "Deku Shield",    /* Progressive */
    "the " C1 "Hylian Shield",  /* Progressive */
};

static const char* const kItemNamesMm[] = {
    "the " C1 "Ocarina of Time",
    "the " C1 "Hero's Bow",
    "the " C1 "Fire Arrow",
    "the " C1 "Ice Arrow",
    "the " C1 "Light Arrow",
    "", /* Fairy Ocarina */
    "the " C1 "Bombs",
    "the " C1 "Bombchu",
    "a " C0 "Deku Stick",
    "a " C0 "Deku Nut",
    "a " C0 "Magic Bean",
    "", /* Fairy Slingshot (JP) */
    "a " C0 "Powder Keg",
    "the " C1 "Pictograph Box",
    "the " C1 "Lens of Truth",
    "the " C1 "Hookshot",
    "the " C1 "Great Fairy's Sword",
    "a " C1 "bottle of Red Potion",
    "an " C1 "Empty Bottle",
    "a " C0 "Red Potion",
    "a " C0 "Green Potion",
    "a " C0 "Blue Potion",
    "a " C0 "Fairy",
    "the " C1 "Deku Princess",
    "a bottle of " C0 "Milk",
    "half a bottle of " C0 "Milk",
    "a " C0 "Fish",
    "a " C0 "Bug",
    "", /* Blue fire */
    "a " C0 "Poe",
    "a " C0 "Big Poe",
    "a bottle of " C0 "Spring Water",
    "a bottle of " C0 "Hot Spring Water",
    "a " C1 "Zora Egg",
    "a bottle of " C1 "Gold Dust",
    "a " C0 "Magic Mushroom",
    "a " C1 "Seahorse",
    "a bottle of " C0 "Chateau Romani",
    "", /* Hylian Loach */
    "", /* Drink */
    "the " C1 "Moon's Tear",
    "the " C1 "Land Title Deed",
    "the " C1 "Swamp Title Deed",
    "the " C1 "Mountain Title Deed",
    "the " C1 "Ocean Title Deed",
    "the " C1 "Room Key",
    "the " C1 "Letter to Mama",
    "the " C1 "Letter to Kafei",
    "the " C1 "Pendant of Memories",
    "", /* Map JP */
    "the " C1 "Deku Mask",
    "the " C1 "Goron Mask",
    "the " C1 "Zora Mask",
    "the " C1 "Fierce Deity's Mask",
    "the " C1 "Mask of Truth",
    "the " C1 "Kafei's Mask",
    "the " C1 "All Night Mask",
    "the " C1 "Bunny Hood",
    "the " C1 "Keaton Mask",
    "the " C1 "Garo's Mask",
    C1 "Romani's Mask",
    "the " C1 "Circus Leader's Mask",
    "the " C1 "Postman's Hat",
    "the " C1 "Couple's Mask",
    "the " C1 "Great Fairy Mask",
    "the " C1 "Gibdo Mask",
    "the " C1 "Don Gero's Mask",
    "the " C1 "Kamaro's Mask",
    "the " C1 "Captain's Hat",
    "the " C1 "Stone Mask",
    "the " C1 "Bremen Mask",
    "the " C1 "Blast Mask",
    "the " C1 "Mask of Scents",
    "the " C1 "Giant's Mask",
    "", /* Medallion */
    "", /* Medallion */
    "", /* Medallion */
    "the " C1 "Kokiri Sword",
    "the " C1 "Razor Sword",
    "the " C1 "Gilded Sword",
    "", /* Fierce Deity's Sword */
    "the " C1 "Hero's Shield",
    "the " C1 "Mirror Shield",
    "", /* Quiver */
    "the " C1 "Big Quiver",
    "the " C1 "Biggest Quiver",
    "the " C1 "Bomb Bag",
    "the " C1 "Big Bomb Bag",
    "the " C1 "Biggest Bomb Bag",
    "", /* JP items */
    "the " C1 "Adult Wallet",
    "the " C1 "Giant Wallet",
    "",
    C3 "Odolwa's Remains",
    C3 "Goht's Remains",
    C3 "Gyorg's Remains",
    C3 "Twinmold's Remains",
    "the " C2 "Sonata of Awakening",
    "the " C2 "Goron Lullaby",
    "the " C2 "New Wave Bossa Nova",
    "the " C2 "Elegy of Emptiness",
    "the " C2 "Oath to Order",
    "", /* JP Song */
    "the " C2 "Song of Time",
    "the " C2 "Song of Healing",
    C2 "Epona's Song",
    "the " C2 "Song of Soaring",
    "the " C2 "Song of Storms",
    "", /* JP Song */
    "the " C1 "Bomber's Notebook",
    "",
    "a " C0 "Heart Container",
    "",
    "",
    "",
    "the " C2 "Lullaby Intro",
    "the " C0 "Big Key",
    "the " C0 "Compass",
    "the " C0 "Dungeon Map",
    "a " C0 "Stray Fairy",
    "a " C0 "Small Key",
    "",
    "",
    "a " C0 "Piece of Heart",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "a " C0 "Recovery Heart",
    "a " C0 "Green Rupee",
    "a " C0 "Blue Rupee",
    "a " C0 "Red Rupee (10)",
    "a " C0 "Red Rupee",
    "a " C0 "Purple Rupee",
    "a " C0 "Silver Rupee",
    "a " C0 "Gold Rupee",
    "",
    "",
    C0 "5 Deku Nuts",
    C0 "10 Deku Nuts",
    C0 "5 Bombs",
    C0 "10 Bombs",
    C0 "20 Bombs",
    C0 "30 Bombs",
    C0 "10 Arrows",
    C0 "30 Arrows",
    C0 "40 Arrows",
    "",
    C0 "20 Bombchu",
    C0 "10 Bombchu",
    "a " C0 "Bombchu",
    C0 "5 Bombchu",
    C0 "20 Deku Sticks",
    C0 "30 Deku Sticks",
    C0 "30 Deku Nuts",
    C0 "40 Deku Nuts",
    "some " C0 "Chateau Romani",
    "some " C0 "Milk",
    "", /* Gold dust refill */
    "", /* ??? */
    "a " C0 "Seahorse",
    "the " C1 "Magic Upgrade",
    "the " C1 "Larger Magic Upgrade",
    "the " C1 "Defense Upgrade",
    "the " C1 "Spin Attack Upgrade",
    "the " C1 "Hero's Shield", /* Progressive */
    "a " C0 "World Map (Clock Town)",
    "a " C0 "World Map (Woodfall)",
    "a " C0 "World Map (Snowhead)",
    "a " C0 "World Map (Romani Ranch)",
    "a " C0 "World Map (Great Bay)",
    "a " C0 "World Map (Stone Tower)",
};

typedef struct
{
    char* prepos;
    char* name;
}
RegionName;

const RegionName kRegionNamesOot[] = {
    { "in",         "the " COLOR_YELLOW "Sacred Realm" },
    { "inside",     "the " COLOR_GREEN "Deku Tree" },
    { "inside",     COLOR_RED "Dodongo's Cavern" },
    { "inside",     COLOR_BLUE "Jabu-Jabu" },
    { "in",         "the " COLOR_GREEN "Forest Temple" },
    { "in",         "the " COLOR_RED "Fire Temple" },
    { "in",         "the " COLOR_BLUE "Water Temple" },
    { "in",         "the " COLOR_ORANGE "Spirit Temple" },
    { "in",         "the " COLOR_PINK "Shadow Temple" },
    { "at",         "the " COLOR_PINK "Bottom of the Well" },
    { "in",         "the " COLOR_TEAL "Ice Caverns" },
    { "in",         COLOR_YELLOW "Gerudo Training Grounds" },
    { "in",         "the "COLOR_YELLOW "Thieve's Hideout" },
    { "inside",     COLOR_RED "Ganon's Castle" },
    { "in",         "the " COLOR_GREEN "Kokiri Forest" },
    { "in",         COLOR_YELLOW "Hyrule Field" },
    { "in",         COLOR_YELLOW "Hyrule's Market" },
    { "in",         COLOR_ORANGE "Lon Lon Ranch" },
    { "in",         COLOR_YELLOW "Hyrule's Castle" },
    { "in",         COLOR_RED "Ganon's Castle Exterior" },
    { "in",         "the " COLOR_GREEN "Lost Woods" },
    { "in",         "the " COLOR_GREEN "Sacred Meadow" },
    { "in",         COLOR_RED "Kakariko" },
    { "inside",     COLOR_PINK "Kakariko's Graveyard" },
    { "on",         COLOR_RED "Death Mountain's Trail" },
    { "in",         COLOR_RED "Death Mountain's Crater" },
    { "in",         COLOR_RED "Goron City" },
    { "in",         COLOR_BLUE "Zora's River" },
    { "in",         COLOR_BLUE "Zora's Domain" },
    { "in",         COLOR_BLUE "Zora's Fountain" },
    { "around",     COLOR_BLUE "Lake Hylia" },
    { "in",         "the " COLOR_YELLOW "Temple of Time" },
    { "in",         COLOR_ORANGE "Gerudo Valley" },
    { "around",     COLOR_ORANGE "Gerudo Fortress" },
    { "in",         "the " COLOR_YELLOW "Haunted Wastelands" },
    { "around",     "the " COLOR_ORANGE "Desert Colossus" },
};

const RegionName kRegionNamesMm[] = {
    { "in",         COLOR_GREEN "Woodfall Temple" },
    { "in",         COLOR_TEAL "Snowhead Temple" },
    { "in",         COLOR_BLUE "Great Bay Temple" },
    { "in",         COLOR_ORANGE "Stone Tower Temple" },
    { "in",         COLOR_YELLOW "South Clock Town" },
    { "in",         COLOR_YELLOW "North Clock Town" },
    { "in",         COLOR_YELLOW "East Clock Town" },
    { "in",         COLOR_YELLOW "West Clock Town" },
    { "in",         "the " COLOR_YELLOW "Laundry Pool" },
    { "inside",     "the " COLOR_PINK "Giants's Dream" },
    { "on",         COLOR_YELLOW "Clock Tower Roof" },
    { "in",         "the " COLOR_RED "Stock Pot Inn" },
    { "in",         COLOR_RED "Termina Field" },
    { "on",         "the " COLOR_GREEN "Road to the Swamp" },
    { "in",         "the " COLOR_GREEN "Southern Swamp" },
    { "in",         COLOR_GREEN "Deku Palace" },
    { "in",         COLOR_GREEN "Woodfall" },
    { "on",         "the " COLOR_RED "Path to Mountain Village" },
    { "in",         "the " COLOR_RED "Mountain Village" },
    { "on",         "the " COLOR_RED "Path to Snowhead" },
    { "around",     COLOR_RED "Twin Islands" },
    { "around",     COLOR_RED "Goron Village" },
    { "in",         COLOR_RED "Snowhead" },
    { "around",     "the " COLOR_PINK "Milk Road" },
    { "around",     COLOR_ORANGE "Romani Ranch" },
    { "around",     COLOR_BLUE "Great Bay Coast" },
    { "in",         "the " COLOR_BLUE "Pirate's Fortress Exterior" },
    { "in",         "the " COLOR_BLUE "Pirate's Fortress Sewers" },
    { "in",         "the " COLOR_BLUE "Pirate's Fortress Interior" },
    { "around",     COLOR_BLUE "Zora Cape" },
    { "in",         COLOR_BLUE "Zora Hall" },
    { "in",         COLOR_BLUE "Pinnacle Rock" },
    { "on",         "the " COLOR_ORANGE "Road to Ikana" },
    { "in",         COLOR_PINK "Ikana's Graveyard" },
    { "in",         COLOR_ORANGE "Ikana Canyon" },
    { "in",         "the " COLOR_ORANGE "Ancient Castle of Ikana" },
    { "somewhere",  COLOR_PINK "Beneath the Well" },
    { "in",         "a " COLOR_YELLOW "Secret Shrine" },
    { "on",         "the " COLOR_ORANGE "Stone Tower" },
    { "on",         "the " COLOR_RED "Moon" },
};

static void autoLineBreaks(char* buffer)
{
    static const int kMaxLineLength = 37;
    int lineLength;
    int i;
    int lastSpace;
    u8 c;

    lastSpace = -1;
    lineLength = 0;
    i = 0;
    for (;;)
    {
        c = (u8)(buffer[i]);
        if (c == END[0])
            break;
        if (c >= ' ' && c <= '~')
        {
            lineLength++;
            if (lineLength == kMaxLineLength)
            {
                lineLength = i - lastSpace;
                buffer[lastSpace] = NL[0];
            }
        }
        if (c == ' ')
        {
            lastSpace = i;
        }
        i += comboMultibyteCharSize(c);
    }
}

static void appendStr(char** dst, const char* src)
{
    size_t len = strlen(src);
    memcpy(*dst, src, len);
    *dst += len;
}

static int isItemAmbiguousOot(u16 itemId)
{
    switch (itemId)
    {
    case ITEM_OOT_STICK:
    case ITEM_OOT_NUT:
    case ITEM_OOT_BOMB:
    case ITEM_OOT_BOW:
    case ITEM_OOT_ARROW_FIRE:
    case ITEM_OOT_OCARINA_TIME:
    case ITEM_OOT_BOMBCHU_10:
    case ITEM_OOT_ARROW_ICE:
    case ITEM_OOT_LENS:
    case ITEM_OOT_MAGIC_BEAN:
    case ITEM_OOT_ARROW_LIGHT:
    case ITEM_OOT_EMPTY_BOTTLE:
    case ITEM_OOT_MILK_BOTTLE:
    case ITEM_OOT_KEATON_MASK:
    case ITEM_OOT_BUNNY_HOOD:
    case ITEM_OOT_GORON_MASK:
    case ITEM_OOT_ZORA_MASK:
    case ITEM_OOT_MASK_OF_TRUTH:
    case ITEM_OOT_KOKIRI_SWORD:
    case ITEM_OOT_SHIELD_HYLIAN:
    case ITEM_OOT_QUIVER:
    case ITEM_OOT_QUIVER2:
    case ITEM_OOT_QUIVER3:
    case ITEM_OOT_BOMB_BAG:
    case ITEM_OOT_BOMB_BAG2:
    case ITEM_OOT_BOMB_BAG3:
    case ITEM_OOT_WALLET2:
    case ITEM_OOT_WALLET3:
    case ITEM_OOT_SONG_EPONA:
    case ITEM_OOT_SONG_TIME:
    case ITEM_OOT_SONG_STORMS:
    case ITEM_OOT_HEART_CONTAINER:
    case ITEM_OOT_HEART_PIECE:
    case ITEM_OOT_HEART_PIECE2:
    case ITEM_OOT_RECOVERY_HEART:
    case ITEM_OOT_RUPEE_GREEN:
    case ITEM_OOT_RUPEE_BLUE:
    case ITEM_OOT_RUPEE_RED:
    case ITEM_OOT_RUPEE_PURPLE:
    case ITEM_OOT_RUPEE_HUGE:
    case ITEM_OOT_STICKS_5:
    case ITEM_OOT_STICKS_10:
    case ITEM_OOT_NUTS_5:
    case ITEM_OOT_NUTS_10:
    case ITEM_OOT_BOMBS_5:
    case ITEM_OOT_BOMBS_10:
    case ITEM_OOT_BOMBS_20:
    case ITEM_OOT_BOMBS_30:
    case ITEM_OOT_ARROWS_5:
    case ITEM_OOT_ARROWS_10:
    case ITEM_OOT_ARROWS_30:
    case ITEM_OOT_BOMBCHU_5:
    case ITEM_OOT_BOMBCHU_20:
    case ITEM_OOT_MAGIC_UPGRADE:
    case ITEM_OOT_MAGIC_UPGRADE2:
    case ITEM_OOT_DEFENSE_UPGRADE:
        return 1;
    default:
        return 0;
    }
}

static int isItemAmbiguousMm(u16 itemId)
{
    switch (itemId)
    {
    case ITEM_MM_OCARINA_OF_TIME:
    case ITEM_MM_BOW:
    case ITEM_MM_ARROW_FIRE:
    case ITEM_MM_ARROW_ICE:
    case ITEM_MM_ARROW_LIGHT:
    case ITEM_MM_BOMB:
    case ITEM_MM_BOMBCHU:
    case ITEM_MM_STICK:
    case ITEM_MM_NUT:
    case ITEM_MM_MAGIC_BEAN:
    case ITEM_MM_LENS_OF_TRUTH:
    case ITEM_MM_RED_POTION_WITH_BOTTLE:
    case ITEM_MM_EMPTY_BOTTLE:
    case ITEM_MM_MASK_GORON:
    case ITEM_MM_MASK_ZORA:
    case ITEM_MM_MASK_TRUTH:
    case ITEM_MM_MASK_BUNNY:
    case ITEM_MM_MASK_KEATON:
    case ITEM_MM_SWORD_KOKIRI:
    case ITEM_MM_SHIELD_HERO:
    case ITEM_MM_QUIVER:
    case ITEM_MM_QUIVER2:
    case ITEM_MM_QUIVER3:
    case ITEM_MM_BOMB_BAG:
    case ITEM_MM_BOMB_BAG2:
    case ITEM_MM_BOMB_BAG3:
    case ITEM_MM_WALLET2:
    case ITEM_MM_WALLET3:
    case ITEM_MM_SONG_TIME:
    case ITEM_MM_SONG_EPONA:
    case ITEM_MM_SONG_STORMS:
    case ITEM_MM_HEART_CONTAINER:
    case ITEM_MM_HEART_PIECE:
    case ITEM_MM_HEART_PIECE2:
    case ITEM_MM_RECOVERY_HEART:
    case ITEM_MM_RUPEE_GREEN:
    case ITEM_MM_RUPEE_BLUE:
    case ITEM_MM_RUPEE_RED10:
    case ITEM_MM_RUPEE_RED:
    case ITEM_MM_RUPEE_PURPLE:
    case ITEM_MM_RUPEE_SILVER:
    case ITEM_MM_RUPEE_GOLD:
    case ITEM_MM_NUTS_5:
    case ITEM_MM_NUTS_10:
    case ITEM_MM_BOMBS_5:
    case ITEM_MM_BOMBS_10:
    case ITEM_MM_BOMBS_20:
    case ITEM_MM_BOMBS_30:
    case ITEM_MM_ARROWS_10:
    case ITEM_MM_ARROWS_30:
    case ITEM_MM_ARROWS_40:
    case ITEM_MM_BOMBCHUS_20:
    case ITEM_MM_BOMBCHUS_10:
    case ITEM_MM_BOMBCHU_ALT:
    case ITEM_MM_BOMBCHUS_5:
    case ITEM_MM_STICKS_20:
    case ITEM_MM_STICKS_30:
    case ITEM_MM_NUTS_30:
    case ITEM_MM_NUTS_40:
    case ITEM_MM_MAGIC_UPGRADE:
    case ITEM_MM_MAGIC_UPGRADE2:
    case ITEM_MM_DEFENSE_UPGRADE:
    case ITEM_MM_BOTTLED_MILK:
        return 1;
    default:
        return 0;
    }
}

static void appendHeader(char** b)
{
#if defined(GAME_MM)
    /* MM has a header */
    memcpy(*b, "\x00\x30\xfe\xff\xff\xff\xff\xff\xff\xff\xff", 11);
    *b += 11;
#endif
    appendStr(b, FAST);
}

static void appendShopHeader(char** b, s16 price)
{
#if defined(GAME_MM)
    memcpy((*b) + 5, &price, 2);
    *b += 11;
#endif
    appendStr(b, FAST);
}

#if defined(GAME_MM)
static void appendBossRewardHeader(char** b, char icon)
{
    memcpy(*b, "\x06\x00\xfe\xff\xff\xff\xff\xff\xff\xff\xff", 11);
    (*b)[2] = icon;
    *b += 11;
    appendStr(b, FAST);
}
#endif

static void appendClearColor(char** b)
{
#if defined(GAME_OOT)
    appendStr(b, CZ);
#else
    /* strlen doesn't like NUL */
    **b = 0;
    *b += 1;
#endif
}

static void appendNum(char** b, int num)
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

static void appendItemName(char** b, u16 itemId, int capitalize)
{
    char* start;
    const char* itemName;
    int ambiguous;

#if defined(GAME_MM)
    itemId ^= MASK_FOREIGN_ITEM;
#endif

    if (itemId & MASK_FOREIGN_ITEM)
    {
        itemName = kItemNamesMm[itemId & 0xff];
        ambiguous = isItemAmbiguousMm(itemId & 0xff);
    }
    else
    {
        itemName = kItemNamesOot[itemId & 0xff];
        ambiguous = isItemAmbiguousOot(itemId & 0xff);
    }

    start = *b;
    appendStr(b, itemName);
    appendClearColor(b);

    if (ambiguous)
    {
        appendStr(b, " (");
        if (itemId & MASK_FOREIGN_ITEM)
        {
            appendStr(b, COLOR_PINK "MM");
        }
        else
        {
            appendStr(b, COLOR_TEAL "OoT");
        }
        appendClearColor(b);
        appendStr(b, ")");
    }

    if (capitalize)
    {
        start[0] = toupper(start[0]);
    }
}

void comboTextHijackItem(GameState_Play* play, u16 itemId)
{
    char* b;

#if defined(GAME_OOT)
    b = play->msgCtx.textBuffer;
#else
    b = play->textBuffer;
#endif
    appendHeader(&b);
    appendStr(&b, "You got ");
    appendItemName(&b, itemId, 0);
    appendStr(&b, "!");
    appendStr(&b, END);
}

void comboTextHijackItemShop(GameState_Play* play, u16 itemId, s16 price, int confirm)
{
    char* b;

#if defined(GAME_OOT)
    b = play->msgCtx.textBuffer;
#else
    b = play->textBuffer;
#endif

    appendShopHeader(&b, price);

    if (itemId == ITEM_NONE)
    {
        appendStr(&b, "SOLD OUT" NOCLOSE END);

    }

    appendItemName(&b, itemId, 1);
    appendStr(&b, NL COLOR_RED);
    appendNum(&b, price);
    appendStr(&b, " Rupees");
    if (confirm)
    {
        appendStr(&b, NL CHOICE2 COLOR_GREEN);
        appendStr(&b, "Buy" NL);
        appendStr(&b, "No thanks");
    }
    else
    {
        appendStr(&b, NL NL NOCLOSE);
    }
    appendStr(&b, END);
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

#if defined(GAME_OOT)
static const char kIcons[] = {
    0x6c,
    0x6d,
    0x6e,
    0x6b,
    0x66,
    0x67,
    0x68,
    0x69,
    0x6a,
};
#endif

static void appendRegionName(char** b, u8 regionId, int prepos, int capitalize)
{
    char* start;
    const RegionName* regName;

    if (regionId & 0x80)
    {
        regName = &kRegionNamesMm[(regionId & 0x7f) - 1];
    }
    else
    {
        regName = &kRegionNamesOot[(regionId & 0x7f) - 1];
    }

    start = *b;
    if (prepos)
    {
        appendStr(b, regName->prepos);
        appendStr(b, " ");
    }
    appendStr(b, regName->name);
    appendClearColor(b);

    if (capitalize)
    {
        start[0] = toupper(start[0]);
    }
}


#if defined(GAME_OOT)
void comboTextHijackDungeonRewardHints(GameState_Play* play, int base, int count)
{
    char* b;
    int index;

    b = play->msgCtx.textBuffer;
    appendHeader(&b);
    for (int i = 0; i < count; ++i)
    {
        index = base + i;
        appendStr(&b, FAST ICON);
        *b++ = kIcons[index];
        appendRegionName(&b, gComboData.dungeonRewards[index], 1, 1);
        appendStr(&b, "...");

        if (i == (count - 1))
        {
            appendStr(&b, SIGNAL END);
        }
        else
        {
            appendStr(&b, BB);
        }

    }
}
#endif

#if defined(GAME_MM)
void comboTextHijackDungeonRewardHints(GameState_Play* play, int hint)
{
    char* b;

    b = play->textBuffer;
    appendBossRewardHeader(&b, 0x55 + hint);
    appendRegionName(&b, gComboData.dungeonRewards[9 + hint], 1, 1);
    appendStr(&b, "...");
    if (hint != 3)
        appendStr(&b, "\x19");
    appendStr(&b, END);
}
#endif

#if defined(GAME_OOT)
void comboTextHijackSkullReward(GameState_Play* play, s16 itemId, int count)
{
    char* b;

    b = play->msgCtx.textBuffer;
    appendHeader(&b);
    appendStr(&b,
        "Yeaaarrgh! I'm cursed!! Please save me by destroying " COLOR_RED
    );
    appendNum(&b, count);
    appendStr(&b,
        " Spiders of the Curse" CZ " and I will give you "
    );
    appendItemName(&b, itemId, 0);
    appendStr(&b, CZ "." END);
    autoLineBreaks(play->msgCtx.textBuffer);
}
#endif
