#include <combo.h>

#define C0 "\x05\x44"
#define C1 "\x05\x41"
#define C2 "\x05\x45"
#define C3 "\x05\x46"

static const char* const kItemNamesOot[] = {
    "a " C0 "Deku Stick",
    "a " C0 "Deku Nut",
    "the " C1 "Bombs",
    "the " C1 "Fairy Bow",
    "the " C1 "Fire Arrow",
    C1 "Din's Fire",
    "the " C1 "Fairy Slingshot",
    "the " C1 "Fairy Ocarina",
    "the " C1 "Ocarina of Time",
    C0 "10 Bombchus",
    "the " C1 "Hookshot",
    "the " C1 "Longshot",
    "the " C1 "Ice Arrow",
    C1 "Farore's Wind",
    "the " C1 "Boomerang",
    "the " C1 "Lens of Truth",
    "a" C0 "Magic Bean",
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
    C1 "Biggoron's Sword", /* Needs check for Giant Knife */
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
    "", /* Bomb bag */
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
    C0 "5 Bombchus",
    C0 "20 Bombchus",
    "the " C1 "Deku Stick Upgrade",
    "the " C1 "Second Deku Stick Upgrade",
    "the " C1 "Deku Nut Upgrade",
    "the " C1 "Second Deku Nut Upgrade",
};

static void appendStr(char** dst, const char* src)
{
    size_t len = strlen(src);
    memcpy(*dst, src, len);
    *dst += len;
}

void comboTextHijackItem(GameState_Play* play, u16 itemId)
{
    char* b;

#if defined(GAME_MM)
    itemId ^= 0x100;
#endif

    b = play->textBuffer;
    appendStr(&b, "\x08You got ");
    appendStr(&b, kItemNamesOot[itemId]);
    appendStr(&b, "\x05\x40!\x02");
}
