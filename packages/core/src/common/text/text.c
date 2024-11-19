#include <combo.h>
#include <combo/text.h>
#include <combo/item.h>
#include <combo/config.h>

typedef struct
{
    char* prepos;
    char* name;
}
RegionName;

typedef struct
{
    char* name;
    int flags;
}
CheckName;

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
    { "inside",     TEXT_COLOR_RED "Ganon's Castle Tower" },
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
    { "in",         TEXT_COLOR_ORANGE "Inverted Stone Tower Temple" },
    { "inside",     "the " TEXT_COLOR_GREEN "Butler Race" },
    { "inside",     "the " TEXT_COLOR_RED "Goron Racetrack" },
};

static const RegionName kRegionNowhere = { NULL, TEXT_COLOR_PINK "nowhere" };
static const RegionName kRegionLinkPocket = { "inside", TEXT_COLOR_YELLOW "Link's Pocket" };
static const RegionName kRegionNamelessPlace = { "in", "a " TEXT_COLOR_PINK "Nameless Place" };

static const CheckName kCheckNamesOot[] = {
    { "the " TEXT_COLOR_BLUE "Frogs Ocarina Game", TF_NONE },
    { TEXT_COLOR_BLUE "Fishing", TF_NONE },
    { "a " TEXT_COLOR_PINK "Ravaged Village", TF_NONE },
    { TEXT_COLOR_BLUE "King Zora", TF_NONE },
    { "the " TEXT_COLOR_RED "Great Fairy outside of Ganon's Castle", TF_NONE },
    { "the " TEXT_COLOR_RED "Fire Temple Hammer Chest", TF_NONE },
    { "the " TEXT_COLOR_RED "Fire Temple Scarecrow Chest", TF_NONE },
    { "the " TEXT_COLOR_YELLOW "Gerudo Training Grounds Water Room", TF_NONE },
    { "the " TEXT_COLOR_ORANGE "Haunted Wastelands Chest", TF_NONE },
    { "the " TEXT_COLOR_YELLOW "Gerudo Archery", TF_NONE },
    { "the " TEXT_COLOR_GREEN "Cow in Link's house", TF_NONE },
    { TEXT_COLOR_RED "Biggoron", TF_NONE },
    { "the " TEXT_COLOR_TEAL "Ice Cavern Final Chest", TF_NONE },
    { "the " TEXT_COLOR_YELLOW "Market Treasure Game", TF_NONE },
    { TEXT_COLOR_RED "Shooting at the Sun", TF_NONE },
    { "the " TEXT_COLOR_GREEN "Floormaster in the Forest Temple", TF_NONE },
    { TEXT_COLOR_PINK "bombing a fiery skull pot", TF_NONE },
    { "a " TEXT_COLOR_PINK "Stalfos duel near spikes", TF_NONE },
    { "the " TEXT_COLOR_BLUE "Water Temple River chest", TF_NONE },
    { TEXT_COLOR_GREEN "trading a bird and a mixture in the Lost Woods", TF_NONE },
    { TEXT_COLOR_BLUE "Stingers in Jabu-Jabu's Belly", TF_NONE },
    { "playing " TEXT_COLOR_YELLOW "a symphony in Spirit Temple", TF_NONE },
    { "a " TEXT_COLOR_GREEN "chest hidden by a time block in Deku Tree", TF_NONE },
    { "a " TEXT_COLOR_RED "singular scrub in Death Mountain Crater", TF_NONE },
    { "a " TEXT_COLOR_GREEN "spider deep within Deku Tree", TF_NONE },
};

static const CheckName kCheckNamesMm[] = {
    { "the " TEXT_COLOR_ORANGE "Ranch Defense", TF_NONE },
    { "the " TEXT_COLOR_GREEN  "Butler Race", TF_NONE },
    { TEXT_COLOR_PINK "Anju and Kafei", TF_NONE },
    { TEXT_COLOR_BLUE "Don Gero's Choir", TF_NONE },
    { "the " TEXT_COLOR_RED "Goron Race", TF_NONE },
    { "the " TEXT_COLOR_PINK "Beneath the Graveyard Night 3 Chest", TF_NONE },
    { "the " TEXT_COLOR_PINK "Termina Field Musical Stones", TF_PLURAL },
    { "the " TEXT_COLOR_TEAL "Bank's Final Reward", TF_NONE },
    { "the " TEXT_COLOR_TEAL "Milk Bar Performance", TF_NONE },
    { "the " TEXT_COLOR_GREEN "Boat Archery", TF_NONE },
    { "the " TEXT_COLOR_BLUE "Ocean Spider House Chest", TF_NONE },
    { "the " TEXT_COLOR_BLUE "Pinnacle Rock Seahorses", TF_NONE },
    { "the " TEXT_COLOR_BLUE "Fisherman's Game", TF_NONE },
    { TEXT_COLOR_ORANGE "Igos du Ikana", TF_NONE },
    { "the " TEXT_COLOR_YELLOW "Secret Shrine Wart and Final Chest", TF_NONE },
    { "the " TEXT_COLOR_PINK "Cow Beneath The Well", TF_NONE },
    { "the " TEXT_COLOR_RED "Blacksmith", TF_NONE },
    { "the " TEXT_COLOR_PINK "Midnight Meeting", TF_NONE },
    { TEXT_COLOR_BLUE "Madame Aroma in the Bar", TF_NONE },
    { TEXT_COLOR_YELLOW "Marching for Cuccos", TF_NONE },
    { TEXT_COLOR_PINK "Finding Kafei", TF_NONE },
    { "an " TEXT_COLOR_PINK "Invisible Soldier", TF_NONE },
    { "the " TEXT_COLOR_BLUE "Great Bay Temple Wart", TF_NONE },
    { "the " TEXT_COLOR_TEAL "second Snowhead Wizzrobe", TF_NONE },
    { "the " TEXT_COLOR_GREEN "Woodfall Temple Gekko", TF_NONE },
    { TEXT_COLOR_YELLOW "defeating Gomess", TF_NONE },
    { TEXT_COLOR_TEAL "feeding a freezing Goron", TF_NONE },
    { TEXT_COLOR_PINK "healing Kamaro", TF_NONE },
    { "the " TEXT_COLOR_GREEN "Woodfall Temple Dark Room", TF_NONE },
};

static int isItemAmbiguous(s16 gi)
{
    switch (gi)
    {
    case GI_OOT_BOW:
    case GI_OOT_QUIVER2:
    case GI_OOT_QUIVER3:
    case GI_OOT_ARROWS_5:
    case GI_OOT_ARROWS_10:
    case GI_OOT_ARROWS_30:
    case GI_MM_BOW:
    case GI_MM_QUIVER2:
    case GI_MM_QUIVER3:
    case GI_MM_ARROWS_10:
    case GI_MM_ARROWS_30:
    case GI_MM_ARROWS_40:
        return !Config_Flag(CFG_SHARED_BOWS);
    case GI_OOT_BOMB_BAG:
    case GI_OOT_BOMB_BAG2:
    case GI_OOT_BOMB_BAG3:
    case GI_OOT_BOMB:
    case GI_OOT_BOMBS_5:
    case GI_OOT_BOMBS_10:
    case GI_OOT_BOMBS_20:
    case GI_OOT_BOMBS_30:
    case GI_MM_BOMB_BAG:
    case GI_MM_BOMB_BAG2:
    case GI_MM_BOMB_BAG3:
    case GI_MM_BOMB:
    case GI_MM_BOMBS_5:
    case GI_MM_BOMBS_10:
    case GI_MM_BOMBS_20:
    case GI_MM_BOMBS_30:
        return !Config_Flag(CFG_SHARED_BOMB_BAGS);
    case GI_OOT_MAGIC_UPGRADE:
    case GI_OOT_MAGIC_UPGRADE2:
    case GI_OOT_MAGIC_JAR_SMALL:
    case GI_OOT_MAGIC_JAR_LARGE:
    case GI_MM_MAGIC_UPGRADE:
    case GI_MM_MAGIC_UPGRADE2:
    case GI_MM_MAGIC_JAR_SMALL:
    case GI_MM_MAGIC_JAR_LARGE:
        return !Config_Flag(CFG_SHARED_MAGIC);
    case GI_OOT_ARROW_FIRE:
    case GI_MM_ARROW_FIRE:
        return !Config_Flag(CFG_SHARED_MAGIC_ARROW_FIRE);
    case GI_OOT_ARROW_ICE:
    case GI_MM_ARROW_ICE:
        return !Config_Flag(CFG_SHARED_MAGIC_ARROW_ICE);
    case GI_OOT_ARROW_LIGHT:
    case GI_MM_ARROW_LIGHT:
        return !Config_Flag(CFG_SHARED_MAGIC_ARROW_LIGHT);
    case GI_OOT_SONG_EPONA:
    case GI_MM_SONG_EPONA:
        return !Config_Flag(CFG_SHARED_SONG_EPONA);
    case GI_OOT_SONG_STORMS:
    case GI_MM_SONG_STORMS:
        return !Config_Flag(CFG_SHARED_SONG_STORMS);
    case GI_OOT_SONG_TIME:
    case GI_MM_SONG_TIME:
        return !Config_Flag(CFG_SHARED_SONG_TIME);
    case GI_OOT_SONG_SUN:
    case GI_MM_SONG_SUN:
        return !(Config_Flag(CFG_SHARED_SONG_SUN) || !Config_Flag(CFG_MM_SONG_SUN));
    case GI_OOT_SONG_EMPTINESS:
    case GI_MM_SONG_EMPTINESS:
        return !(Config_Flag(CFG_SHARED_SONG_EMPTINESS) || !Config_Flag(CFG_OOT_SONG_EMPTINESS));
    case GI_OOT_STICK_UPGRADE:
    case GI_OOT_STICK_UPGRADE2:
    case GI_MM_STICK_UPGRADE:
    case GI_MM_STICK_UPGRADE2:
    case GI_OOT_NUT_UPGRADE:
    case GI_OOT_NUT_UPGRADE2:
    case GI_MM_NUT_UPGRADE:
    case GI_MM_NUT_UPGRADE2:
        return Config_Flag(CFG_MM_UPGRADES_STICKS_NUTS) && !Config_Flag(CFG_SHARED_NUTS_STICKS);
    case GI_OOT_SPIN_UPGRADE:
    case GI_MM_SPIN_UPGRADE:
        return Config_Flag(CFG_OOT_SPIN_UPGRADE) && !Config_Flag(CFG_SHARED_SPIN_UPGRADE);
    case GI_OOT_STICK:
    case GI_OOT_STICKS_5:
    case GI_OOT_STICKS_10:
    case GI_OOT_NUTS_5:
    case GI_OOT_NUTS_5_ALT:
    case GI_OOT_NUTS_10:
    case GI_MM_STICK:
    case GI_MM_NUT:
    case GI_MM_NUTS_5:
    case GI_MM_NUTS_10:
        return !Config_Flag(CFG_SHARED_NUTS_STICKS);
    case GI_OOT_STONE_OF_AGONY:
    case GI_MM_STONE_OF_AGONY:
        return Config_Flag(CFG_MM_STONE_OF_AGONY) && !Config_Flag(CFG_SHARED_STONE_OF_AGONY);
    case GI_OOT_HOOKSHOT:
    case GI_MM_HOOKSHOT:
    case GI_MM_HOOKSHOT_SHORT:
        return !Config_Flag(CFG_SHARED_HOOKSHOT);
    case GI_OOT_LENS:
    case GI_MM_LENS:
        return !Config_Flag(CFG_SHARED_LENS);
    case GI_OOT_OCARINA_FAIRY:
        return (Config_Flag(CFG_MM_OCARINA_FAIRY) && !Config_Flag(CFG_SHARED_OCARINA));
    case GI_OOT_OCARINA_TIME:
    case GI_MM_OCARINA_FAIRY:
    case GI_MM_OCARINA_OF_TIME:
        return !Config_Flag(CFG_SHARED_OCARINA);
    case GI_OOT_MASK_KEATON:
    case GI_MM_MASK_KEATON:
        return !Config_Flag(CFG_SHARED_MASK_KEATON);
    case GI_OOT_MASK_BUNNY:
    case GI_MM_MASK_BUNNY:
        return !Config_Flag(CFG_SHARED_MASK_BUNNY);
    case GI_OOT_MASK_GORON:
    case GI_MM_MASK_GORON:
        return !Config_Flag(CFG_SHARED_MASK_GORON);
    case GI_OOT_MASK_ZORA:
    case GI_MM_MASK_ZORA:
        return !Config_Flag(CFG_SHARED_MASK_ZORA);
    case GI_OOT_MASK_TRUTH:
    case GI_MM_MASK_TRUTH:
        return !Config_Flag(CFG_SHARED_MASK_TRUTH);
    case GI_OOT_MASK_BLAST:
    case GI_MM_MASK_BLAST:
        return (Config_Flag(CFG_OOT_MASK_BLAST) && !Config_Flag(CFG_SHARED_MASK_BLAST));
    case GI_OOT_MASK_STONE:
    case GI_MM_MASK_STONE:
        return (Config_Flag(CFG_OOT_MASK_STONE) && !Config_Flag(CFG_SHARED_MASK_STONE));
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
    case GI_OOT_TRAP_RUPOOR:
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
    case GI_MM_TRAP_RUPOOR:
        return !Config_Flag(CFG_SHARED_WALLETS);
    case GI_OOT_HEART_CONTAINER:
    case GI_OOT_HEART_CONTAINER2:
    case GI_OOT_HEART_PIECE:
    case GI_OOT_RECOVERY_HEART:
    case GI_OOT_DEFENSE_UPGRADE:
    case GI_MM_HEART_CONTAINER:
    case GI_MM_HEART_PIECE:
    case GI_MM_RECOVERY_HEART:
    case GI_MM_DEFENSE_UPGRADE:
        return !Config_Flag(CFG_SHARED_HEALTH);
    case GI_OOT_SOUL_ENEMY_OCTOROK:
    case GI_OOT_SOUL_ENEMY_WALLMASTER:
    case GI_OOT_SOUL_ENEMY_DODONGO:
    case GI_OOT_SOUL_ENEMY_KEESE:
    case GI_OOT_SOUL_ENEMY_TEKTITE:
    case GI_OOT_SOUL_ENEMY_PEAHAT:
    case GI_OOT_SOUL_ENEMY_LIZALFOS_DINALFOS:
    case GI_OOT_SOUL_ENEMY_SKULLTULA:
    case GI_OOT_SOUL_ENEMY_ARMOS:
    case GI_OOT_SOUL_ENEMY_DEKU_BABA:
    case GI_OOT_SOUL_ENEMY_DEKU_SCRUB:
    case GI_OOT_SOUL_ENEMY_BUBBLE:
    case GI_OOT_SOUL_ENEMY_BEAMOS:
    case GI_OOT_SOUL_ENEMY_REDEAD_GIBDO:
    case GI_OOT_SOUL_ENEMY_SKULLWALLTULA:
    case GI_OOT_SOUL_ENEMY_SHELL_BLADE:
    case GI_OOT_SOUL_ENEMY_LIKE_LIKE:
    case GI_OOT_SOUL_ENEMY_IRON_KNUCKLE:
    case GI_OOT_SOUL_ENEMY_FREEZARD:
    case GI_OOT_SOUL_ENEMY_WOLFOS:
    case GI_OOT_SOUL_ENEMY_GUAY:
    case GI_OOT_SOUL_ENEMY_FLYING_POT:
    case GI_OOT_SOUL_ENEMY_FLOORMASTER:
    case GI_OOT_SOUL_ENEMY_LEEVER:
    case GI_OOT_SOUL_ENEMY_STALCHILD:
    case GI_MM_SOUL_ENEMY_OCTOROK:
    case GI_MM_SOUL_ENEMY_WALLMASTER:
    case GI_MM_SOUL_ENEMY_DODONGO:
    case GI_MM_SOUL_ENEMY_KEESE:
    case GI_MM_SOUL_ENEMY_TEKTITE:
    case GI_MM_SOUL_ENEMY_PEAHAT:
    case GI_MM_SOUL_ENEMY_LIZALFOS_DINALFOS:
    case GI_MM_SOUL_ENEMY_SKULLTULA:
    case GI_MM_SOUL_ENEMY_ARMOS:
    case GI_MM_SOUL_ENEMY_DEKU_BABA:
    case GI_MM_SOUL_ENEMY_DEKU_SCRUB:
    case GI_MM_SOUL_ENEMY_BUBBLE:
    case GI_MM_SOUL_ENEMY_BEAMOS:
    case GI_MM_SOUL_ENEMY_REDEAD_GIBDO:
    case GI_MM_SOUL_ENEMY_SKULLWALLTULA:
    case GI_MM_SOUL_ENEMY_SHELL_BLADE:
    case GI_MM_SOUL_ENEMY_LIKE_LIKE:
    case GI_MM_SOUL_ENEMY_IRON_KNUCKLE:
    case GI_MM_SOUL_ENEMY_FREEZARD:
    case GI_MM_SOUL_ENEMY_WOLFOS:
    case GI_MM_SOUL_ENEMY_GUAY:
    case GI_MM_SOUL_ENEMY_FLYING_POT:
    case GI_MM_SOUL_ENEMY_FLOORMASTER:
    case GI_MM_SOUL_ENEMY_LEEVER:
    case GI_MM_SOUL_ENEMY_STALCHILD:
        return !Config_Flag(CFG_SHARED_SOULS_ENEMY);
    case GI_OOT_SOUL_NPC_SHOOTING_GALLERY_OWNER:
    case GI_OOT_SOUL_NPC_BAZAAR_SHOPKEEPER:
    case GI_OOT_SOUL_NPC_GORON:
    case GI_OOT_SOUL_NPC_GORON_CHILD:
    case GI_OOT_SOUL_NPC_BOMBCHU_SHOPKEEPER:
    case GI_OOT_SOUL_NPC_BOMBERS:
    case GI_OOT_SOUL_NPC_CITIZEN:
    case GI_OOT_SOUL_NPC_COMPOSER_BROS:
    case GI_OOT_SOUL_NPC_DAMPE:
    case GI_OOT_SOUL_NPC_CHEST_GAME_OWNER:
    case GI_OOT_SOUL_NPC_GORON_SHOPKEEPER:
    case GI_OOT_SOUL_NPC_HONEY_DARLING:
    case GI_OOT_SOUL_NPC_RUTO:
    case GI_OOT_SOUL_NPC_MEDIGORON:
    case GI_OOT_SOUL_NPC_BIGGORON:
    case GI_OOT_SOUL_NPC_TALON:
    case GI_OOT_SOUL_NPC_ASTRONOMER:
    case GI_OOT_SOUL_NPC_POE_COLLECTOR:
    case GI_OOT_SOUL_NPC_BOMBCHU_BOWLING_LADY:
    case GI_OOT_SOUL_NPC_FISHING_POND_OWNER:
    case GI_OOT_SOUL_NPC_ROOFTOP_MAN:
    case GI_OOT_SOUL_NPC_ZORA:
    case GI_OOT_SOUL_NPC_ZORA_SHOPKEEPER:
    case GI_OOT_SOUL_NPC_MALON:
    case GI_OOT_SOUL_NPC_BEAN_SALESMAN:
    case GI_OOT_SOUL_NPC_CARPENTERS:
    case GI_OOT_SOUL_NPC_ANJU:
    case GI_OOT_SOUL_NPC_GURU_GURU:
    case GI_OOT_SOUL_NPC_SCIENTIST:
    case GI_OOT_SOUL_NPC_GORMAN:
    case GI_OOT_SOUL_NPC_GROG:
    case GI_OOT_SOUL_NPC_DOG_LADY:
    case GI_OOT_SOUL_NPC_CARPET_MAN:
    case GI_OOT_SOUL_NPC_OLD_HAG:
    case GI_OOT_SOUL_NPC_BANKER:
    case GI_MM_SOUL_NPC_SHOOTING_GALLERY_OWNER:
    case GI_MM_SOUL_NPC_BAZAAR_SHOPKEEPER:
    case GI_MM_SOUL_NPC_GORON:
    case GI_MM_SOUL_NPC_GORON_CHILD:
    case GI_MM_SOUL_NPC_BOMBCHU_SHOPKEEPER:
    case GI_MM_SOUL_NPC_BOMBERS:
    case GI_MM_SOUL_NPC_CITIZEN:
    case GI_MM_SOUL_NPC_COMPOSER_BROS:
    case GI_MM_SOUL_NPC_DAMPE:
    case GI_MM_SOUL_NPC_CHEST_GAME_OWNER:
    case GI_MM_SOUL_NPC_GORON_SHOPKEEPER:
    case GI_MM_SOUL_NPC_HONEY_DARLING:
    case GI_MM_SOUL_NPC_RUTO:
    case GI_MM_SOUL_NPC_MEDIGORON:
    case GI_MM_SOUL_NPC_BIGGORON:
    case GI_MM_SOUL_NPC_TALON:
    case GI_MM_SOUL_NPC_ASTRONOMER:
    case GI_MM_SOUL_NPC_POE_COLLECTOR:
    case GI_MM_SOUL_NPC_BOMBCHU_BOWLING_LADY:
    case GI_MM_SOUL_NPC_FISHING_POND_OWNER:
    case GI_MM_SOUL_NPC_ROOFTOP_MAN:
    case GI_MM_SOUL_NPC_ZORA:
    case GI_MM_SOUL_NPC_ZORA_SHOPKEEPER:
    case GI_MM_SOUL_NPC_MALON:
    case GI_MM_SOUL_NPC_BEAN_SALESMAN:
    case GI_MM_SOUL_NPC_CARPENTERS:
    case GI_MM_SOUL_NPC_ANJU:
    case GI_MM_SOUL_NPC_GURU_GURU:
    case GI_MM_SOUL_NPC_SCIENTIST:
    case GI_MM_SOUL_NPC_GORMAN:
    case GI_MM_SOUL_NPC_GROG:
    case GI_MM_SOUL_NPC_DOG_LADY:
    case GI_MM_SOUL_NPC_CARPET_MAN:
    case GI_MM_SOUL_NPC_OLD_HAG:
    case GI_MM_SOUL_NPC_BANKER:
        return !Config_Flag(CFG_SHARED_SOULS_NPC);
    case GI_OOT_SOUL_MISC_GS:
    case GI_OOT_SOUL_MISC_BUSINESS_SCRUB:
    case GI_MM_SOUL_MISC_GS:
    case GI_MM_SOUL_MISC_BUSINESS_SCRUB:
        return !Config_Flag(CFG_SHARED_SOULS_MISC);
    case GI_OOT_SKELETON_KEY:
    case GI_MM_SKELETON_KEY:
        return !Config_Flag(CFG_SHARED_SKELETON_KEY);
    case GI_OOT_BUTTON_A:
    case GI_OOT_BUTTON_C_RIGHT:
    case GI_OOT_BUTTON_C_LEFT:
    case GI_OOT_BUTTON_C_DOWN:
    case GI_OOT_BUTTON_C_UP:
    case GI_MM_BUTTON_A:
    case GI_MM_BUTTON_C_RIGHT:
    case GI_MM_BUTTON_C_LEFT:
    case GI_MM_BUTTON_C_DOWN:
    case GI_MM_BUTTON_C_UP:
        return !Config_Flag(CFG_SHARED_OCARINA_BUTTONS);
    case GI_OOT_SHIELD_HYLIAN:
    case GI_OOT_SHIELD_MIRROR:
    case GI_MM_SHIELD_HERO:
    case GI_MM_SHIELD_MIRROR:
        return !Config_Flag(CFG_SHARED_SHIELDS);
    case GI_OOT_BOMBCHU_5:
    case GI_OOT_BOMBCHU_10:
    case GI_OOT_BOMBCHU_20:
    case GI_OOT_BOMBCHU_BAG_5:
    case GI_OOT_BOMBCHU_BAG_10:
    case GI_OOT_BOMBCHU_BAG_20:
    case GI_MM_BOMBCHU:
    case GI_MM_BOMBCHU_5:
    case GI_MM_BOMBCHU_10:
    case GI_MM_BOMBCHU_20:
    case GI_MM_BOMBCHU_BAG:
    case GI_MM_BOMBCHU_BAG_5:
    case GI_MM_BOMBCHU_BAG_10:
    case GI_MM_BOMBCHU_BAG_20:
        return !Config_Flag(CFG_SHARED_BOMBCHU);
    case GI_OOT_SPELL_FIRE:
    case GI_MM_SPELL_FIRE:
        return (Config_Flag(CFG_MM_SPELL_FIRE) && !Config_Flag(CFG_SHARED_SPELL_FIRE));
    case GI_OOT_SPELL_WIND:
    case GI_MM_SPELL_WIND:
        return (Config_Flag(CFG_MM_SPELL_WIND) && !Config_Flag(CFG_SHARED_SPELL_WIND));
    case GI_OOT_SPELL_LOVE:
    case GI_MM_SPELL_LOVE:
        return (Config_Flag(CFG_MM_SPELL_LOVE) && !Config_Flag(CFG_SHARED_SPELL_LOVE));
    case GI_OOT_BOOTS_IRON:
    case GI_MM_BOOTS_IRON:
        return (Config_Flag(CFG_MM_BOOTS_IRON) && !Config_Flag(CFG_SHARED_BOOTS_IRON));
    case GI_OOT_BOOTS_HOVER:
    case GI_MM_BOOTS_HOVER:
        return (Config_Flag(CFG_MM_BOOTS_HOVER) && !Config_Flag(CFG_SHARED_BOOTS_HOVER));
    case GI_OOT_TUNIC_GORON:
    case GI_MM_TUNIC_GORON:
        return (Config_Flag(CFG_MM_TUNIC_GORON) && !Config_Flag(CFG_SHARED_TUNIC_GORON));
    case GI_OOT_TUNIC_ZORA:
    case GI_MM_TUNIC_ZORA:
        return (Config_Flag(CFG_MM_TUNIC_ZORA) && !Config_Flag(CFG_SHARED_TUNIC_ZORA));
    case GI_OOT_SCALE_SILVER:
    case GI_OOT_SCALE_GOLDEN:
    case GI_MM_SCALE_SILVER:
    case GI_MM_SCALE_GOLDEN:
        return (Config_Flag(CFG_MM_SCALES) && !Config_Flag(CFG_SHARED_SCALES));
    case GI_OOT_GORON_BRACELET:
    case GI_OOT_SILVER_GAUNTLETS:
    case GI_OOT_GOLDEN_GAUNTLETS:
    case GI_MM_GORON_BRACELET:
    case GI_MM_SILVER_GAUNTLETS:
    case GI_MM_GOLDEN_GAUNTLETS:
        return (Config_Flag(CFG_MM_STRENGTH) && !Config_Flag(CFG_SHARED_STRENGTH));
    case GI_OOT_SWORD_KOKIRI:
    case GI_MM_SWORD_KOKIRI:
        return !Config_Flag(CFG_SHARED_SWORDS);
    case GI_OOT_SWORD_RAZOR:
    case GI_OOT_SWORD_GILDED:
    case GI_MM_SWORD_RAZOR:
    case GI_MM_SWORD_GILDED:
        return Config_Flag(CFG_OOT_EXTRA_CHILD_SWORDS) && !Config_Flag(CFG_SHARED_SWORDS);
    case GI_OOT_HAMMER:
    case GI_MM_HAMMER:
        return Config_Flag(CFG_MM_HAMMER) && !Config_Flag(CFG_SHARED_HAMMER);
    case GI_OOT_MAGIC_BEAN:
    case GI_OOT_BOTTLE_EMPTY:
    case GI_OOT_BOTTLE_MILK:
    case GI_OOT_MILK:
    case GI_OOT_POTION_RED:
    case GI_OOT_POTION_GREEN:
    case GI_OOT_POTION_BLUE:
    case GI_OOT_BOTTLE_POTION_RED:
    case GI_OOT_BOTTLE_POTION_GREEN:
    case GI_OOT_BOTTLE_POTION_BLUE:
    case GI_OOT_BOTTLE_FAIRY:
    case GI_OOT_BOTTLE_POE:
    case GI_OOT_BOTTLE_BIG_POE:
    case GI_OOT_FAIRY:
    case GI_OOT_BUG:
    case GI_OOT_FISH:
    case GI_OOT_POE:
    case GI_OOT_BIG_POE:
    case GI_MM_MAGIC_BEAN:
    case GI_MM_BOTTLE_POTION_RED:
    case GI_MM_BOTTLE_POTION_GREEN:
    case GI_MM_BOTTLE_POTION_BLUE:
    case GI_MM_BOTTLE_FAIRY:
    case GI_MM_BOTTLE_POE:
    case GI_MM_BOTTLE_BIG_POE:
    case GI_MM_BOTTLE_EMPTY:
    case GI_MM_BOTTLE_MILK:
    case GI_MM_MILK:
    case GI_MM_POTION_RED:
    case GI_MM_POTION_GREEN:
    case GI_MM_POTION_BLUE:
    case GI_MM_FAIRY:
    case GI_MM_BUGS:
    case GI_MM_FISH:
    case GI_MM_POE:
    case GI_MM_BIG_POE:
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
    int numTwoDigits;

    numTwoDigits = num % 100;
    if (numTwoDigits >= 11 && numTwoDigits <= 13)
    {
        suffix = "th";
    }
    else
    {
        switch (numTwoDigits % 10)
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
    comboTextAppendItemNameEx(b, gi, flags, -1);
}

static const char* nameOverride(s16 gi)
{
    if (Config_Flag(CFG_SHARED_SOULS_NPC))
    {
        switch (gi)
        {
        case GI_OOT_SOUL_NPC_SHOOTING_GALLERY_OWNER:
        case GI_MM_SOUL_NPC_SHOOTING_GALLERY_OWNER:
            return "the " TEXT_C1 "Soul of the Shooting Gallery/Town Archery Owner";
        case GI_OOT_SOUL_NPC_BAZAAR_SHOPKEEPER:
        case GI_MM_SOUL_NPC_BAZAAR_SHOPKEEPER:
            return "the " TEXT_C1 "Soul of the Bazaar/Swamp Archery Owner";
        case GI_OOT_SOUL_NPC_GORON_CHILD:
        case GI_MM_SOUL_NPC_GORON_CHILD:
            return "the " TEXT_C1 "Soul of the Goron Child/Baby";
        case GI_OOT_SOUL_NPC_BOMBCHU_SHOPKEEPER:
        case GI_MM_SOUL_NPC_BOMBCHU_SHOPKEEPER:
            return "the " TEXT_C1 "Soul of the Bombchu/Bomb Shop Owner";
        case GI_OOT_SOUL_NPC_BOMBERS:
        case GI_MM_SOUL_NPC_BOMBERS:
            return "the " TEXT_C1 "Soul of the Graveyard Kid/Bombers";
        case GI_OOT_SOUL_NPC_CHEST_GAME_OWNER:
        case GI_MM_SOUL_NPC_CHEST_GAME_OWNER:
            return "the " TEXT_C1 "Soul of the Chest Game Owner/Fisherman";
        case GI_OOT_SOUL_NPC_RUTO:
        case GI_MM_SOUL_NPC_RUTO:
            return "the " TEXT_C1 "Soul of Ruto/Lulu";
        case GI_OOT_SOUL_NPC_MEDIGORON:
        case GI_MM_SOUL_NPC_MEDIGORON:
            return "the " TEXT_C1 "Soul of Medigoron/Keg Trial Goron";
        case GI_OOT_SOUL_NPC_TALON:
        case GI_MM_SOUL_NPC_TALON:
            return "the " TEXT_C1 "Soul of Talon/Mr. Barten";
        case GI_OOT_SOUL_NPC_POE_COLLECTOR:
        case GI_MM_SOUL_NPC_POE_COLLECTOR:
            return "the " TEXT_C1 "Soul of Poe Collector/Ghost Hut Owner";
        case GI_OOT_SOUL_NPC_BOMBCHU_BOWLING_LADY:
        case GI_MM_SOUL_NPC_BOMBCHU_BOWLING_LADY:
            return "the " TEXT_C1 "Soul of Bombchu Bowling/Chest Game Lady";
        case GI_OOT_SOUL_NPC_FISHING_POND_OWNER:
        case GI_MM_SOUL_NPC_FISHING_POND_OWNER:
            return "the " TEXT_C1 "Soul of Fishing Pond/Trading Post Owner";
        case GI_OOT_SOUL_NPC_ROOFTOP_MAN:
        case GI_MM_SOUL_NPC_ROOFTOP_MAN:
            return "the " TEXT_C1 "Soul of Rooftop Man/Part-Timer";
        case GI_OOT_SOUL_NPC_MALON:
        case GI_MM_SOUL_NPC_MALON:
            return "the " TEXT_C1 "Soul of Malon/Romani/Cremia";
        case GI_OOT_SOUL_NPC_ANJU:
        case GI_MM_SOUL_NPC_ANJU:
            return "the " TEXT_C1 "Soul of Cucco Lady/Anju";
        case GI_OOT_SOUL_NPC_GORMAN:
        case GI_MM_SOUL_NPC_GORMAN:
            return "the " TEXT_C1 "Soul of Ingo/Gorman & Bros.";
        case GI_OOT_SOUL_NPC_GROG:
        case GI_MM_SOUL_NPC_GROG:
            return "the " TEXT_C1 "Soul of the Punk Kid/Grog";
        case GI_OOT_SOUL_NPC_CARPET_MAN:
        case GI_MM_SOUL_NPC_CARPET_MAN:
            return "the " TEXT_C1 "Soul of the Carpet Man/Swordsman";
        case GI_OOT_SOUL_NPC_OLD_HAG:
        case GI_MM_SOUL_NPC_OLD_HAG:
            return "the " TEXT_C1 "Soul of the Old Hag/Anju's Grandmother";
        case GI_OOT_SOUL_NPC_BANKER:
        case GI_MM_SOUL_NPC_BANKER:
            return "the " TEXT_C1 "Soul of the Beggar/Banker";
        case GI_OOT_SOUL_NPC_THIEVES:
        case GI_MM_SOUL_NPC_THIEVES:
            return "the " TEXT_C1 "Soul of the Patrolling Thieves and their Chief";
        case GI_OOT_SOUL_ENEMY_THIEVES:
        case GI_MM_SOUL_ENEMY_THIEVES:
            return "the " TEXT_C1 "Soul of the Fighting Thieves";
        }
    }

    return NULL;
}

void comboTextAppendItemNameEx(char** b, s16 gi, int flags, int importance)
{
    char* start;
    const char* itemName;
    const char* tmp;
    int ambiguous;

    ambiguous = isItemAmbiguous(gi);
    itemName = kItemNames[gi - 1];

    if (gi == GI_OOT_ARROW_ICE)
    {
        if (Config_Flag(CFG_OOT_BLUE_FIRE_ARROWS))
        {
            itemName = "the " TEXT_C1 "Blue Fire Arrows";
            ambiguous = 0;
        }
    }

    if (gi == GI_OOT_ARROW_LIGHT)
    {
        if (Config_Flag(CFG_OOT_SUNLIGHT_ARROWS))
        {
            itemName = "the " TEXT_C1 "Sunlight Arrows";
            ambiguous = 0;
        }
    }

    if (gi == GI_MM_BOMBER_NOTEBOOK)
    {
        if(Config_Flag(CFG_MENU_NOTEBOOK))
            itemName = "the " TEXT_C1 "Bombers' Tracker";
    }

    tmp = nameOverride(gi);
    if (tmp)
        itemName = tmp;

    if (flags & TF_PROGRESSIVE)
    {
        switch (gi)
        {
        case GI_OOT_SWORD_KOKIRI:
            if (Config_Flag(CFG_OOT_PROGRESSIVE_SWORDS) || Config_Flag(CFG_OOT_EXTRA_CHILD_SWORDS))
            {
                itemName = "a " TEXT_C1 "Progressive Sword";
                ambiguous = !Config_Flag(CFG_SHARED_SWORDS);
            }
            break;
        case GI_OOT_SWORD_KNIFE:
            if (Config_Flag(CFG_OOT_PROGRESSIVE_SWORDS_GORON))
            {
                itemName = "a " TEXT_C1 "Progressive Goron Sword";
            }
            break;
        case GI_MM_SWORD_KOKIRI:
            itemName = "a " TEXT_C1 "Progressive Sword";
            ambiguous = !Config_Flag(CFG_SHARED_SWORDS);
            break;
        case GI_OOT_PROGRESSIVE_SHIELD_DEKU:
        case GI_MM_PROGRESSIVE_SHIELD_HERO:
            itemName = "a " TEXT_C1 "Progressive Shield";
            ambiguous = !Config_Flag(CFG_SHARED_SHIELDS);
            break;
        case GI_OOT_OCARINA_FAIRY:
        case GI_MM_OCARINA_FAIRY:
            itemName = "a " TEXT_C1 "Progressive Ocarina";
            ambiguous = !Config_Flag(CFG_SHARED_OCARINA);
            break;
        case GI_OOT_HOOKSHOT:
        case GI_MM_HOOKSHOT_SHORT:
            itemName = "a " TEXT_C1 "Progressive Hookshot";
            ambiguous = !Config_Flag(CFG_SHARED_HOOKSHOT);
            break;
        case GI_OOT_WALLET:
        case GI_MM_WALLET:
            itemName = "a " TEXT_C1 "Progressive Wallet";
            ambiguous = !Config_Flag(CFG_SHARED_WALLETS);
            break;
        case GI_MM_SONG_GORON_HALF:
            itemName = "a " TEXT_C2 "Progressive Goron Lullaby";
            break;
        case GI_OOT_SCALE_SILVER:
        case GI_MM_SCALE_SILVER:
            itemName = "a " TEXT_C1 "Progressive Scale";
            ambiguous = !Config_Flag(CFG_SHARED_SCALES) && Config_Flag(CFG_MM_SCALES);
            break;
        case GI_OOT_GORON_BRACELET:
        case GI_MM_GORON_BRACELET:
            itemName = "a " TEXT_C1 "Progressive Strength";
            ambiguous = !Config_Flag(CFG_SHARED_STRENGTH) && Config_Flag(CFG_MM_STRENGTH);
            break;
        case GI_OOT_BOW:
        case GI_MM_BOW:
            if (Config_Flag(CFG_SHARED_BOWS))
            {
                itemName = "a " TEXT_C1 "Bow";
                ambiguous = 0;
            }
            break;
        case GI_MM_CLOCK1:
            if (Config_Flag(CFG_MM_CLOCKS_PROGRESSIVE))
                itemName = "a " TEXT_C1 "Progressive Clock";
            break;
        case GI_OOT_BOMBCHU_5:
        case GI_OOT_BOMBCHU_10:
        case GI_OOT_BOMBCHU_20:
            if (Config_Flag(CFG_OOT_BOMBCHU_BAG))
                itemName = TEXT_C1 "Bombchu"; /* Generic term for pack or bag */
            break;
        case GI_MM_BOMBCHU:
        case GI_MM_BOMBCHU_5:
        case GI_MM_BOMBCHU_10:
        case GI_MM_BOMBCHU_20:
            if (Config_Flag(CFG_MM_BOMBCHU_BAG))
                itemName = TEXT_C1 "Bombchu"; /* Generic term for pack or bag */
            break;
        }

    }

    start = *b;
    comboTextAppendStr(b, itemName);
    comboTextAppendClearColor(b);

    if (ambiguous && !Config_Flag(CFG_ONLY_OOT) && !Config_Flag(CFG_ONLY_MM))
    {
        comboTextAppendStr(b, " (");
        if (gi >= GI_MM_RUPEE_GREEN)
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

    comboTextAppendItemImportance(b, gi, importance);

    if (flags & TF_CAPITALIZE)
    {
        start[0] = toupper(start[0]);
    }
}

void comboTextAppendItemNameQuery(char** b, const ComboItemQuery* q, int flags)
{
    comboTextAppendItemNameQueryEx(b, q, flags, -1);
}

void comboTextAppendItemNameQueryEx(char** b, const ComboItemQuery* q, int flags, int importance)
{
    ComboItemOverride o;

    comboItemOverride(&o, q);
    comboTextAppendItemNameOverrideEx(b, &o, flags, importance);
}

void comboTextAppendItemNameOverride(char** b, const ComboItemOverride* o, int flags)
{
    comboTextAppendItemNameOverrideEx(b, o, flags, -1);
}

void comboTextAppendItemNameOverrideEx(char** b, const ComboItemOverride* o, int flags, int importance)
{
    s16 gi;

    if (flags & TF_PROGRESSIVE)
        gi = o->giRaw;
    else
        gi = o->gi;
    comboTextAppendItemNameEx(b, gi, flags, importance);
    if (o->player != PLAYER_SELF && o->player != PLAYER_ALL && o->player != gComboConfig.playerId)
    {
        comboTextAppendStr(b, " for " TEXT_COLOR_YELLOW "Player ");
        comboTextAppendNum(b, o->player);
        comboTextAppendClearColor(b);
    }

    if (o->playerFrom != PLAYER_SELF && o->playerFrom != PLAYER_ALL && o->playerFrom != gComboConfig.playerId)
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

    start = *b;

    if (regionId == 0xff)
        regName = &kRegionLinkPocket;
    else if (regionId == 0x00)
        regName = &kRegionNowhere;
    else if (regionId == 0xfe)
        regName = &kRegionNamelessPlace;
    else if (regionId & 0x80)
        regName = &kRegionNamesMm[(regionId & 0x7f) - 1];
    else
        regName = &kRegionNamesOot[(regionId & 0x7f) - 1];

    if ((flags & TF_PREPOS) && regName->prepos)
    {
        comboTextAppendStr(b, regName->prepos);
        comboTextAppendStr(b, " ");
    }
    comboTextAppendStr(b, regName->name);
    comboTextAppendClearColor(b);

    if (world != 0 && world != 0xff && world != gComboConfig.playerId)
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

int comboTextAppendCheckName(char** b, u8 checkId, u8 world)
{
    const CheckName* cn;

    if (checkId & 0x80)
    {
        cn = &kCheckNamesMm[(checkId & 0x7f) - 1];
    }
    else
    {
        cn = &kCheckNamesOot[(checkId & 0x7f) - 1];
    }

    comboTextAppendStr(b, cn->name);
    comboTextAppendClearColor(b);

    if (world != 0 && world != 0xff && world != gComboConfig.playerId)
    {
        comboTextAppendStr(b, " in " TEXT_COLOR_YELLOW "World ");
        comboTextAppendNum(b, world);
        comboTextAppendClearColor(b);
    }

    return cn->flags;
}

static int isSoldOut(s16 gi)
{
    return (gi == GI_MM_SOLD_OUT);
}

void comboTextHijackItemShop(PlayState* play, const ComboItemOverride* o, s16 price, int flags)
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
    if (flags & TFS_CONFIRM)
    {
        comboTextAppendStr(&b, TEXT_NL TEXT_CHOICE2 TEXT_COLOR_GREEN);
        comboTextAppendStr(&b, "Buy" TEXT_NL);
        comboTextAppendStr(&b, "No thanks");
    }
    else
    {
        comboTextAppendStr(&b, TEXT_NL TEXT_NL);
        if (flags & TFS_MUSHROOM)
            comboTextAppendStr(&b, TEXT_COLOR_PINK "Requires Magic Mushroom");
        comboTextAppendStr(&b, TEXT_NOCLOSE);
    }
    comboTextAppendStr(&b, TEXT_END);
}

#if defined(GAME_OOT)
void comboMessageCancel(PlayState* play)
{
    u8* ctx = (u8*)(void*)&play->msgCtx;

    ctx[0xe3e7] = 2;
    ctx[0xe304] = 0x36;
    ctx[0xe3e4] = 0;

    play->msgCtx.ocarinaMode = 4;
    *(((char*)GET_PLAYER(play)) + 0x141) = 0;
}
#endif

#if defined(GAME_MM)
void comboTextHijackDungeonRewardHints(PlayState* play, int hint)
{
    const ItemHint* ih;
    char* b;

    ih = &gComboConfig.hints.dungeonRewards[9 + hint];
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
void comboTextHijackLightArrows(PlayState* play)
{
    char* b;

    b = play->msgCtx.textBuffer;
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b,
        "Have you found the " TEXT_COLOR_YELLOW "Light Arrows " TEXT_CZ
    );
    comboTextAppendRegionName(&b, gComboConfig.hints.lightArrows.region, gComboConfig.hints.lightArrows.world, TF_PREPOS);
    comboTextAppendStr(&b, "?" TEXT_END);
    comboTextAutoLineBreaks(play->msgCtx.textBuffer);
}
#endif

#if defined(GAME_MM)
void comboTextHijackOathToOrder(PlayState* play)
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
    comboTextAppendRegionName(&b, gComboConfig.hints.oathToOrder.region, gComboConfig.hints.oathToOrder.world, TF_PREPOS);
    comboTextAppendStr(&b, "?" TEXT_END);
    comboTextAutoLineBreaks(start);
}
#endif

void comboTextAppendNpcReward(char** b, s16 npcId, s16 gi, int importance)
{
    ComboItemQuery q = ITEM_QUERY_INIT;

    q.ovType = OV_NPC;
    q.id = npcId;
    q.gi = gi;
    comboTextAppendItemNameQueryEx(b, &q, TF_PREPOS | TF_PROGRESSIVE, importance);
}

void comboTextMessageCantBuy(PlayState* play, int flags)
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

static int shouldItemBeHintedWithImportance(s16 gi)
{
    if (!Config_Flag(CFG_HINT_IMPORTANCE))
        return 0;

    if (gi == GI_NOTHING)
        return 0;

    switch (gi)
    {
    case GI_OOT_BOMBCHU_5:
    case GI_OOT_BOMBCHU_10:
    case GI_OOT_BOMBCHU_20:
    case GI_MM_BOMBCHU:
    case GI_MM_BOMBCHU_5:
    case GI_MM_BOMBCHU_10:
    case GI_MM_BOMBCHU_20:
        return 1;
    default:
        return !isItemFastBuy(gi);
    }
}

void comboTextAppendItemImportance(char** b, s16 gi, int importance)
{
    static const char* kImportanceStrs[] = {
        TEXT_COLOR_RED "unreachable",
        TEXT_COLOR_PINK "not required",
        TEXT_COLOR_TEAL "sometimes required",
        TEXT_COLOR_YELLOW "required"
    };

    if (!shouldItemBeHintedWithImportance(gi))
        return;
    if (importance < 0 || importance > 3)
        return;

    comboTextAppendStr(b, " (");
    comboTextAppendStr(b, kImportanceStrs[importance]);
    comboTextAppendClearColor(b);
    comboTextAppendStr(b, ")");
}

#if defined(GAME_OOT)
void comboTextHijackFishCaught(PlayState* play, const ComboItemOverride* o)
{
    char* b;
    char* start;
    int isSelf;

    isSelf = (o->player == PLAYER_SELF) || (o->player == PLAYER_ALL) || (o->player == gComboConfig.playerId);

    b = play->msgCtx.textBuffer;

    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "You caught ");
    comboTextAppendItemNameOverride(&b, o, TF_NONE);
    comboTextAppendStr(&b, "!");
    if (isSelf)
    {
        comboTextExtra(&b, play, o->gi);
    }
    comboTextAppendStr(&b, TEXT_BB TEXT_CZ);
    comboTextAppendStr(&b, "Do you want to keep it?");
    comboTextAppendStr(&b, TEXT_NL " " TEXT_NL TEXT_CHOICE2 TEXT_COLOR_GREEN);
    comboTextAppendStr(&b, "Yes" TEXT_NL);
    comboTextAppendStr(&b, "No");
    comboTextAppendStr(&b, TEXT_END);
    comboTextAutoLineBreaks(start);
}
#endif
