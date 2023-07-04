#ifndef COMBO_DUNGEON_H
#define COMBO_DUNGEON_H

/* Boss IDs */
#define BOSSID_GOHMA                0x00
#define BOSSID_KING_DODONGO         0x01
#define BOSSID_BARINADE             0x02
#define BOSSID_PHANTOM_GANON        0x03
#define BOSSID_VOLVAGIA             0x04
#define BOSSID_MORPHA               0x05
#define BOSSID_BONGO_BONGO          0x06
#define BOSSID_TWINROVA             0x07
#define BOSSID_ODOLWA               0x08
#define BOSSID_GOHT                 0x09
#define BOSSID_GYORG                0x0a
#define BOSSID_TWINMOLD             0x0b

/* Dungeon IDs */
/* The first values should match the bosses */
#define DUNGEONID_DEKU_TREE                         BOSSID_GOHMA
#define DUNGEONID_DODONGOS_CAVERN                   BOSSID_KING_DODONGO
#define DUNGEONID_JABU_JABU                         BOSSID_BARINADE
#define DUNGEONID_TEMPLE_FOREST                     BOSSID_PHANTOM_GANON
#define DUNGEONID_TEMPLE_FIRE                       BOSSID_VOLVAGIA
#define DUNGEONID_TEMPLE_WATER                      BOSSID_MORPHA
#define DUNGEONID_TEMPLE_SHADOW                     BOSSID_BONGO_BONGO
#define DUNGEONID_TEMPLE_SPIRIT                     BOSSID_TWINROVA
#define DUNGEONID_TEMPLE_WOODFALL                   BOSSID_ODOLWA
#define DUNGEONID_TEMPLE_SNOWHEAD                   BOSSID_GOHT
#define DUNGEONID_TEMPLE_GREAT_BAY                  BOSSID_GYORG
#define DUNGEONID_TEMPLE_STONE_TOWER_INVERTED       BOSSID_TWINMOLD
#define DUNGEONID_TEMPLE_STONE_TOWER                0x0c
#define DUNGEONID_SPIDER_HOUSE_SWAMP                0x0d
#define DUNGEONID_SPIDER_HOUSE_OCEAN                0x0e
#define DUNGEONID_BOTTOM_OF_THE_WELL                0x0f
#define DUNGEONID_ICE_CAVERN                        0x10
#define DUNGEONID_GERUDO_TRAINING_GROUNDS           0x11
#define DUNGEONID_BENEATH_THE_WELL                  0x12
#define DUNGEONID_IKANA_CASTLE                      0x13
#define DUNGEONID_SECRET_SHRINE                     0x14
#define DUNGEONID_BENEATH_THE_WELL_END              0x15
#define DUNGEONID_PIRATE_FORTRESS                   0x16
#define DUNGEONID_GANON_CASTLE                      0x17
#define DUNGEONID_GANON_TOWER                       0x18

/* MQ IDs */
#define MQ_DEKU_TREE                0
#define MQ_DODONGOS_CAVERN          1
#define MQ_JABU_JABU                2
#define MQ_TEMPLE_FOREST            3
#define MQ_TEMPLE_FIRE              4
#define MQ_TEMPLE_WATER             5
#define MQ_TEMPLE_SPIRIT            6
#define MQ_TEMPLE_SHADOW            7
#define MQ_BOTTOM_OF_THE_WELL       8
#define MQ_ICE_CAVERN               9
#define MQ_GERUDO_TRAINING_GROUNDS  10
#define MQ_GANON_CASTLE             11

typedef struct GameState_Play GameState_Play;

int comboCurrentDungeon(GameState_Play *play);
int comboBossDungeon(int dungeonId);

#endif
