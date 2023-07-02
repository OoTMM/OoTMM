#include <combo.h>
#include <combo/text.h>

const char* kDungeonNames[] = {
    TEXT_COLOR_GREEN "Deku Tree",
    TEXT_COLOR_RED "Dodongo's Cavern",
    TEXT_COLOR_BLUE "Jabu-Jabu",
    TEXT_COLOR_GREEN "Forest Temple",
    TEXT_COLOR_RED "Fire Temple",
    TEXT_COLOR_BLUE "Water Temple",
    TEXT_COLOR_PINK "Shadow Temple",
    TEXT_COLOR_YELLOW "Spirit Temple",
    TEXT_COLOR_GREEN "Woodfall Temple",
    TEXT_COLOR_RED "Snowhead Temple",
    TEXT_COLOR_BLUE "Great Bay Temple",
    TEXT_COLOR_YELLOW "Stone Tower Temple (Inverted)",
    TEXT_COLOR_YELLOW "Stone Tower Temple",
    TEXT_COLOR_GREEN "Swamp Spider House",
    TEXT_COLOR_BLUE "Ocean Spider House",
    TEXT_COLOR_PINK "Bottom of the Well",
    TEXT_COLOR_BLUE "Ice Cavern",
    TEXT_COLOR_ORANGE "Gerudo Training Grounds",
    TEXT_COLOR_PINK "Beneath the Well",
    TEXT_COLOR_ORANGE "Ikana Castle",
    TEXT_COLOR_YELLOW "Secret Shrine",
    TEXT_COLOR_PINK "Beneath the Well (End)",
    TEXT_COLOR_BLUE "Pirate Fortress",
    TEXT_COLOR_RED "Ganon's Castle",
    TEXT_COLOR_RED "Ganon's Tower",
};

void comboTextAppendDungeonName(char** buffer, u8 dungeonId)
{
    comboTextAppendStr(buffer, kDungeonNames[dungeonId]);
    comboTextAppendClearColor(buffer);
}
