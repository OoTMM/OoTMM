#include <combo.h>
#include <combo/dungeon.h>

int comboCurrentDungeon(GameState_Play* play)
{
    switch (play->sceneId)
    {
    /* Dungeons */
    case SCE_OOT_INSIDE_DEKU_TREE:
        return DUNGEONID_DEKU_TREE;
    case SCE_OOT_DODONGO_CAVERN:
        return DUNGEONID_DODONGOS_CAVERN;
    case SCE_OOT_INSIDE_JABU_JABU:
        return DUNGEONID_JABU_JABU;
    case SCE_OOT_TEMPLE_FOREST:
        return DUNGEONID_TEMPLE_FOREST;
    case SCE_OOT_TEMPLE_FIRE:
        return DUNGEONID_TEMPLE_FIRE;
    case SCE_OOT_TEMPLE_WATER:
        return DUNGEONID_TEMPLE_WATER;
    case SCE_OOT_TEMPLE_SHADOW:
        return DUNGEONID_TEMPLE_SHADOW;
    case SCE_OOT_TEMPLE_SPIRIT:
        return DUNGEONID_TEMPLE_SPIRIT;
    case SCE_OOT_BOTTOM_OF_THE_WELL:
        return DUNGEONID_BOTTOM_OF_THE_WELL;
    case SCE_OOT_ICE_CAVERN:
        return DUNGEONID_ICE_CAVERN;
    case SCE_OOT_GERUDO_TRAINING_GROUND:
        return DUNGEONID_GERUDO_TRAINING_GROUNDS;
    case SCE_OOT_INSIDE_GANON_CASTLE:
        return DUNGEONID_GANON_CASTLE;
    case SCE_OOT_GANON_TOWER:
        return DUNGEONID_GANON_TOWER;

    /* Boss lairs */
    case SCE_OOT_LAIR_GOHMA:
        return gComboData.boss[BOSSID_GOHMA];
    case SCE_OOT_LAIR_KING_DODONGO:
        return gComboData.boss[BOSSID_KING_DODONGO];
    case SCE_OOT_LAIR_BARINADE:
        return gComboData.boss[BOSSID_BARINADE];
    case SCE_OOT_LAIR_PHANTOM_GANON:
        return gComboData.boss[BOSSID_PHANTOM_GANON];
    case SCE_OOT_LAIR_VOLVAGIA:
        return gComboData.boss[BOSSID_VOLVAGIA];
    case SCE_OOT_LAIR_MORPHA:
        return gComboData.boss[BOSSID_MORPHA];
    case SCE_OOT_LAIR_BONGO_BONGO:
        return gComboData.boss[BOSSID_BONGO_BONGO];
    case SCE_OOT_LAIR_TWINROVA:
        return gComboData.boss[BOSSID_TWINROVA];

    default:
        return -1;
    }
}
