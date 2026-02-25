#include <combo.h>
#include <combo/text.h>

typedef struct
{
    const char* color;
    const char* name;
}
DungeonNameDef;

const DungeonNameDef kDungeonNameDefs[] = {
    { TEXT_COLOR_GREEN, "Deku Tree" },
    { TEXT_COLOR_RED, "Dodongo's Cavern" },
    { TEXT_COLOR_BLUE, "Jabu-Jabu" },
    { TEXT_COLOR_GREEN, "Forest Temple" },
    { TEXT_COLOR_RED, "Fire Temple" },
    { TEXT_COLOR_BLUE, "Water Temple" },
    { TEXT_COLOR_PINK, "Shadow Temple" },
    { TEXT_COLOR_YELLOW, "Spirit Temple" },
    { TEXT_COLOR_GREEN, "Woodfall Temple" },
    { TEXT_COLOR_RED, "Snowhead Temple" },
    { TEXT_COLOR_BLUE, "Great Bay Temple" },
    { TEXT_COLOR_YELLOW, "Stone Tower Temple (Inverted)" },
    { TEXT_COLOR_YELLOW, "Stone Tower Temple" },
    { TEXT_COLOR_GREEN, "Swamp Spider House" },
    { TEXT_COLOR_BLUE, "Ocean Spider House" },
    { TEXT_COLOR_PINK, "Bottom of the Well" },
    { TEXT_COLOR_BLUE, "Ice Cavern" },
    { TEXT_COLOR_ORANGE, "Gerudo Training Grounds" },
    { TEXT_COLOR_PINK, "Beneath The Well" },
    { TEXT_COLOR_ORANGE, "Ikana Castle" },
    { TEXT_COLOR_YELLOW, "Secret Shrine" },
    { TEXT_COLOR_PINK, "Beneath The Well (End)" },
    { TEXT_COLOR_BLUE, "Pirate Fortress" },
    { TEXT_COLOR_RED, "Ganon's Castle" },
    { TEXT_COLOR_RED, "Ganon's Tower" },
    { TEXT_COLOR_YELLOW, "Clock Tower Roof" },
};

const char* const kBossColors[] = {
    TEXT_COLOR_GREEN,
    TEXT_COLOR_RED,
    TEXT_COLOR_BLUE,
    TEXT_COLOR_GREEN,
    TEXT_COLOR_RED,
    TEXT_COLOR_BLUE,
    TEXT_COLOR_PINK,
    TEXT_COLOR_ORANGE,
    TEXT_COLOR_GREEN,
    TEXT_COLOR_RED,
    TEXT_COLOR_BLUE,
    TEXT_COLOR_ORANGE,
};

const char* const kBossNames[] = {
    "Gohma",
    "King Dodongo",
    "Barinade",
    "Phantom Ganon",
    "Volvagia",
    "Morpha",
    "Bongo Bongo",
    "Twinrova",
    "Odolwa",
    "Goht",
    "Gyorg",
    "Twinmold",
};

void comboTextAppendDungeonName(char** buffer, u8 dungeonId, const char* prefix)
{
    const DungeonNameDef* def;

    def = kDungeonNameDefs + dungeonId;
    comboTextAppendStr(buffer, def->color);
    if (prefix)
        comboTextAppendStr(buffer, prefix);
    comboTextAppendStr(buffer, def->name);
    comboTextAppendClearColor(buffer);
}

void comboTextAppendBossName(char** buffer, u8 bossId, const char* prefix)
{
    comboTextAppendStr(buffer, kBossColors[bossId]);
    if (prefix)
        comboTextAppendStr(buffer, prefix);
    comboTextAppendStr(buffer, kBossNames[bossId]);
    comboTextAppendClearColor(buffer);
}
