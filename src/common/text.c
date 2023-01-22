#include <combo.h>

#define C0   TEXT_COLOR_TEAL
#define C1   TEXT_COLOR_RED
#define C2   TEXT_COLOR_PINK
#define C3   TEXT_COLOR_YELLOW

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
    "the " C0 "Boss Key",
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
    "a " C0 "Small Key (Forest)",
    "a " C0 "Small Key (Fire)",
    "a " C0 "Small Key (Water)",
    "a " C0 "Small Key (Spirit)",
    "a " C0 "Small Key (Shadow)",
    "a " C0 "Small Key (Ganon)",
    "a " C0 "Small Key (Well)",
    "a " C0 "Small Key (Hideout)",
    "a " C0 "Small Key (GTG)",
    "the " C0 "Boss Key (Forest)",
    "the " C0 "Boss Key (Fire)",
    "the " C0 "Boss Key (Water)",
    "the " C0 "Boss Key (Spirit)",
    "the " C0 "Boss Key (Shadow)",
    "the " C0 "Boss Key (Ganon)",
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
    "the " C0 "Boss Key",
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
    "a " C0 "Swamp Skulltula Token",
    "an " C0 "Ocean Skulltula Token",
    "a " C0 "Small Key (Woodfall)",
    "a " C0 "Small Key (Snowhead)",
    "a " C0 "Small Key (Great Bay)",
    "a " C0 "Small Key (Stone Tower)",
    "the " C0 "Boss Key (Woodfall)",
    "the " C0 "Boss Key (Snowhead)",
    "the " C0 "Boss Key (Great Bay)",
    "the " C0 "Boss Key (Stone Tower)",
    "a " C0 "Stray Fairy (Woodfall)",
    "a " C0 "Stray Fairy (Snowhead)",
    "a " C0 "Stray Fairy (Great Bay)",
    "a " C0 "Stray Fairy (Stone Tower)",
    "a " C0 "Stray Fairy (Town)",
};

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
    { "inside",     "the " TEXT_COLOR_PINK "Giants's Dream" },
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
    { "somewhere",  TEXT_COLOR_PINK "Beneath the Well" },
    { "in",         "a " TEXT_COLOR_YELLOW "Secret Shrine" },
    { "on",         "the " TEXT_COLOR_ORANGE "Stone Tower" },
    { "on",         "the " TEXT_COLOR_RED "Moon" },
    { "in",         "the " TEXT_COLOR_GREEN "Swamp Spider House" },
    { "in",         "the " TEXT_COLOR_BLUE "Ocean Spider House" },
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
};

static const char* const kCheckNamesMm[] = {
    "the " TEXT_COLOR_ORANGE "Ranch Defense",
    "the " TEXT_COLOR_GREEN  "Butler Race",
    TEXT_COLOR_PINK "Anju and Kafei",
    TEXT_COLOR_BLUE "Don Gero's Choir",
    "the " TEXT_COLOR_RED "Goron Race",
    "the " TEXT_COLOR_PINK "Graveyard Big Poe",
    "the " TEXT_COLOR_TEAL "Bank's Final Reward",
    "the " TEXT_COLOR_TEAL "Sound Check",
    "the " TEXT_COLOR_GREEN "Boat Archery",
    "the " TEXT_COLOR_BLUE "Ocean Spider House Chest",
    "the " TEXT_COLOR_BLUE "Pinnacle Rock Seahorses",
    "the " TEXT_COLOR_BLUE "Fisherman's Game",
    TEXT_COLOR_ORANGE "Igos du Ikana",
    "the " TEXT_COLOR_YELLOW "Secret Shrine Final Chest",
};

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
    case ITEM_OOT_HOOKSHOT:
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
    case ITEM_MM_HOOKSHOT:
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

void comboTextAutoLineBreaks(char* buffer)
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
        if (c == (u8)(TEXT_END[0]))
            break;
        if (c >= ' ' && c <= '~')
        {
            lineLength++;
            if (lineLength >= kMaxLineLength && lastSpace != -1)
            {
                lineLength = i - lastSpace;
                buffer[lastSpace] = TEXT_NL[0];
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

void comboTextAppendItemName(char** b, u16 itemId, int flags)
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

    if (flags & TF_PROGRESSIVE)
    {
        switch (itemId)
        {
        case ITEM_OOT_KOKIRI_SWORD:
        case ITEM_MM_SWORD_KOKIRI | MASK_FOREIGN_ITEM:
            itemName = "a " C1 "Progressive Sword";
            ambiguous = 1;
            break;
        case ITEM_OOT_PROGRESSIVE_SHIELD_DEKU:
        case ITEM_MM_PROGRESSIVE_SHIELD_HERO | MASK_FOREIGN_ITEM:
            itemName = "a " C1 "Progressive Shield";
            ambiguous = 1;
            break;
        case ITEM_OOT_OCARINA_FAIRY:
            itemName = "a " C1 "Progressive Ocarina";
            ambiguous = 1;
            break;
        case ITEM_OOT_HOOKSHOT:
            itemName = "a " C1 "Progressive Hookshot";
            ambiguous = 1;
            break;
        case ITEM_OOT_WALLET2:
        case ITEM_MM_WALLET2 | MASK_FOREIGN_ITEM:
            itemName = "a " C1 "Progressive Wallet";
            ambiguous = 1;
            break;
        case ITEM_MM_SONG_GORON_HALF | MASK_FOREIGN_ITEM:
            itemName = "a " C2 "Progressive Goron Lullaby";
            break;
        case ITEM_OOT_SILVER_SCALE:
            itemName = "a " C1 "Progressive Scale";
            break;
        case ITEM_OOT_GORON_BRACELET:
            itemName = "a " C1 "Progressive Strength";
            break;
        }
    }

    start = *b;
    comboTextAppendStr(b, itemName);
    comboTextAppendClearColor(b);

    if (ambiguous)
    {
        comboTextAppendStr(b, " (");
        if (itemId & MASK_FOREIGN_ITEM)
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

void comboTextAppendRegionName(char** b, u8 regionId, int flags)
{
    char* start;
    const RegionName* regName;

    if (regionId == 0)
    {
        if (flags & TF_PREPOS)
        {
            comboTextAppendStr(b, "in ");
        }
        comboTextAppendStr(b, "the " TEXT_COLOR_RED "Void");
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

    if (flags & TF_CAPITALIZE)
    {
        start[0] = toupper(start[0]);
    }
}

void comboTextAppendCheckName(char** b, u8 checkId)
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
}

void comboTextHijackItem(GameState_Play* play, u16 itemId)
{
    char* b;

#if defined(GAME_OOT)
    b = play->msgCtx.textBuffer;
#else
    b = play->textBuffer;
#endif
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b, "You got ");
    comboTextAppendItemName(&b, itemId, 0);
    comboTextAppendStr(&b, "!");
    comboTextAppendStr(&b, TEXT_END);
}

void comboTextHijackItemShop(GameState_Play* play, u16 itemId, s16 price, int confirm)
{
    char* b;

#if defined(GAME_OOT)
    b = play->msgCtx.textBuffer;
#else
    b = play->textBuffer;
#endif

    comboTextAppendShopHeader(&b, price);

    if (itemId == ITEM_NONE)
    {
        comboTextAppendStr(&b, "SOLD OUT" TEXT_NOCLOSE TEXT_END);

    }

    comboTextAppendItemName(&b, itemId, TF_CAPITALIZE);
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
    char* b;

    b = play->textBuffer;
    appendBossRewardHeader(&b, 0x55 + hint);
    comboTextAppendRegionName(&b, gComboData.hints.dungeonRewards[9 + hint], TF_PREPOS | TF_CAPITALIZE);
    comboTextAppendStr(&b, "...");
    if (hint != 3)
        comboTextAppendStr(&b, "\x19");
    comboTextAppendStr(&b, TEXT_END);
}
#endif

#if defined(GAME_OOT)
void comboTextHijackSkullReward(GameState_Play* play, s16 itemId, int count)
{
    char* b;

    b = play->msgCtx.textBuffer;
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b,
        "Yeaaarrgh! I'm cursed!! Please save me by destroying " TEXT_COLOR_RED
    );
    comboTextAppendNum(&b, count);
    comboTextAppendStr(&b,
        " Spiders of the Curse" TEXT_CZ " and I will give you "
    );
    comboTextAppendItemName(&b, itemId, TF_PROGRESSIVE);
    comboTextAppendStr(&b, TEXT_CZ "." TEXT_END);
    comboTextAutoLineBreaks(play->msgCtx.textBuffer);
}

void comboTextHijackLightArrows(GameState_Play* play)
{
    char* b;

    b = play->msgCtx.textBuffer;
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b,
        "Have you found the " TEXT_COLOR_YELLOW "Light Arrows " TEXT_CZ
    );
    comboTextAppendRegionName(&b, gComboData.hints.lightArrows, TF_PREPOS);
    comboTextAppendStr(&b, "?" TEXT_END);
    comboTextAutoLineBreaks(play->msgCtx.textBuffer);
}
#endif

#if defined(GAME_MM)
void comboTextHijackOathToOrder(GameState_Play* play)
{
    char* b;
    char* start;

    b = play->textBuffer;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b,
        "Have you found the " TEXT_COLOR_PINK "Oath to Order "
    );
    comboTextAppendClearColor(&b);
    comboTextAppendRegionName(&b, gComboData.hints.oathToOrder, TF_PREPOS);
    comboTextAppendStr(&b, "?" TEXT_END);
    comboTextAutoLineBreaks(start);
}
#endif

void comboTextAppendNpcReward(char** b, s16 npcId, s16 gi)
{
    s16 itemId;

    gi = comboOverrideEx(OV_NPC, 0, npcId, gi, 0);
    itemId = comboItemFromGI(gi);
    comboTextAppendItemName(b, itemId, TF_PREPOS | TF_PROGRESSIVE);
}
