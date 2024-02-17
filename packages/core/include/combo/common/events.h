#ifndef COMBO_COMMON_EVENTS_H
#define COMBO_COMMON_EVENTS_H

#define OOT_EV(a, b)        (((a) << 4) | (b))
#define MM_EV(a, b)         (((a) << 3) | (b))

#define EV_OOT_CHK_DEKU_MIDO_MEET           OOT_EV(0,  2)
#define EV_OOT_CHK_DEKU_MIDO_COMPLAIN       OOT_EV(0,  3)
#define EV_OOT_CHK_DEKU_MIDO_SWORD_SHIELD   OOT_EV(0,  4)
#define EV_OOT_CHK_DEKU_TREE_OPEN           OOT_EV(0,  5)
#define EV_OOT_CHK_EMERALD_TREE_DEAD        OOT_EV(0,  7)
#define EV_OOT_CHK_GOHMA                    OOT_EV(0,  9)
#define EV_OOT_CHK_DEKU_TREE_MEET           OOT_EV(0, 12)
#define EV_OOT_CHK_MALON_SPOKEN_HYRULE      OOT_EV(1,  0)
#define EV_OOT_CHK_MALON_EGG                0x12
#define EV_OOT_CHK_CHILD_TALON_WOKEN        0x13
#define EV_OOT_CHK_CHILD_TALON_FLED         0x14
#define EV_OOT_CHK_TREE_DEAD                0x17
#define EV_OOT_CHK_EPONA                    0x18
#define EV_OOT_CHK_STONE_EMERALD            0x19
#define EV_OOT_CHK_MIDO_TREE_DEAD           0x1c
#define EV_OOT_CHK_BOMBCHU_BOWLING          0x22
#define EV_OOT_CHK_STONE_RUBY               0x25
#define EV_OOT_CHK_RUTO_LETTER              0x31
#define EV_OOT_CHK_KING_ZORA_MOVED          0x33
#define EV_OOT_CHK_STONE_SAPPHIRE           0x37
#define EV_OOT_CHK_ZELDA_LETTER             0x40
#define EV_OOT_CHK_OCARINA_OF_TIME          0x43
#define EV_OOT_CHK_MASTER_SWORD_PULLED      0x45
#define EV_OOT_CHK_MEDALLION_FOREST         0x48
#define EV_OOT_CHK_MEDALLION_FIRE           0x49
#define EV_OOT_CHK_MEDALLION_WATER          0x4a
#define EV_OOT_CHK_DOOR_TIME                0x4b
#define EV_OOT_CHK_RAINBOW_BRIDGE           0x4d
#define EV_OOT_CHK_MASTER_SWORD_CHAMBER     0x4f
#define EV_OOT_CHK_SONG_TP_FOREST           0x50
#define EV_OOT_CHK_SONG_TP_FIRE             0x51
#define EV_OOT_CHK_SONG_TP_WATER            0x52
#define EV_OOT_CHK_SONG_TP_SHADOW           0x54
#define EV_OOT_CHK_SONG_TP_LIGHT            0x55
#define EV_OOT_CHK_SONG_ZELDA               0x59
#define EV_OOT_CHK_SONG_SUN                 0x5a
#define EV_OOT_CHK_SONG_STORMS              0x5b
#define EV_OOT_CHK_WINDMILL_FAST            0x65
#define EV_OOT_CHK_WELL_DRAINED             0x67
#define EV_OOT_CHK_LAKE_HYLIA_WATER         0x69
#define EV_OOT_CHK_ZELDA_FLED               0x80
#define EV_OOT_CHK_ZELDA_FLED_BRIDGE        0x82
#define EV_OOT_CHK_SONG_SCARECROW_ADULT     0x9c
#define EV_OOT_CHK_CARPENTER_1              OOT_EV(0x9, 0)
#define EV_OOT_CHK_CARPENTER_2              OOT_EV(0x9, 1)
#define EV_OOT_CHK_CARPENTER_3              OOT_EV(0x9, 2)
#define EV_OOT_CHK_CARPENTER_4              OOT_EV(0x9, 3)
#define EV_OOT_CHK_SONG_TIME                0xa9
#define EV_OOT_CHK_BONGO_ESCAPE             0xaa
#define EV_OOT_CHK_SONG_TP_SPIRIT           0xac
#define EV_OOT_CHK_SARIA_OCARINA            0xc1
#define EV_OOT_CHK_LIGHT_ARROW              0xc4
#define EV_OOT_CHK_MEDALLION_SPIRIT         0xc8
#define EV_OOT_CHK_FROGS_GAME               0xd0

#define EV_OOT_CHK_TRIAL_LIGHT              0xbf
#define EV_OOT_CHK_TRIAL_FIRE               0xbe
#define EV_OOT_CHK_TRIAL_SHADOW             0xbd
#define EV_OOT_CHK_TRIAL_WATER              0xbc
#define EV_OOT_CHK_TRIAL_FOREST             0xbb
#define EV_OOT_CHK_TRIAL_SPIRIT             0xad
#define EN_OOT_CHK_GANON_BARRIER            0xc3

#define EV_OOT_ITEM_DEKU_SEEDS              0x13
#define EV_OOT_ITEM_DAMPE_GRAVEYARD_HP      0x1c
#define EV_OOT_ITEM_ANJU_POCKET_EGG         0x2c
#define EV_OOT_ITEM_HBA_1500                OOT_EV(0, 15)

#define EV_OOT_INF_KAKARIKO_GATE_OPEN       0x076
#define EV_OOT_INF_KING_ZORA_THAWED         0x138
#define EV_OOT_INF_MAGIC_JAR                0x198
#define EV_OOT_INF_GORON_LINK               OOT_EV(16, 12)
#define EV_OOT_INF_RUTO_ON_BLUE_SWITCH      OOT_EV(20, 0)
#define EV_OOT_INF_RUTO_MET                 OOT_EV(20, 1)
#define EV_OOT_INF_RUTO_TALKED              OOT_EV(20, 2)
#define EV_OOT_INF_RUTO_ESCORT              OOT_EV(20, 3)
#define EV_OOT_INF_RUTO_ASK_SAPPHIRE        OOT_EV(20, 4)
#define EV_OOT_INF_RUTO_GOT_SAPPHIRE        OOT_EV(20, 5)
#define EV_OOT_INF_RUTO_KIDNAPPED           OOT_EV(20, 6)
#define EV_OOT_INF_RUTO_TOP_FLOOR           OOT_EV(20, 7)
#define EV_OOT_INF_HBA_1000                 OOT_EV(25, 0)

/* Custom flags that seems unused */
#define EV_OOT_CHK_SONG_SARIA               0x58
#define EV_OOT_CHK_MEDALLION_SHADOW         0x61
#define EV_OOT_CHK_SONG_EPONA               0x62
#define EV_OOT_ITEM_GORON_BRACELET          0x20
#define EV_OOT_INF_MEDIGORON                0xb2

#define EV_MM_WEEK_ENTERED_TERMINA              MM_EV(0x00, 1)
#define EV_MM_WEEK_ENTERED_CLOCK_TOWN_EAST      MM_EV(0x02, 3)
#define EV_MM_WEEK_ENTERED_CLOCK_TOWN_WEST      MM_EV(0x02, 4)
#define EV_MM_WEEK_ENTERED_CLOCK_TOWN_NORTH     MM_EV(0x02, 5)
#define EV_MM_WEEK_MONKEY_PUNISHED              MM_EV(0x09, 7)
#define EV_MM_WEEK_TOWN_GUARDS                  MM_EV(0x0c, 5)
#define EV_MM_WEEK_FOREST_WITCH                 MM_EV(0x10, 4)
#define EV_MM_WEEK_SPIN_UPGRADE                 MM_EV(0x17, 1)
#define EV_MM_WEEK_FIRST_CYCLE                  MM_EV(0x3b, 2)
#define EV_MM_WEEK_BEAVER_RACE1_DONE            MM_EV(0x5d, 0)
#define EV_MM_WEEK_KEETA_DEAD                   MM_EV(23,   2)
#define EV_MM_WEEK_PRINCESS_RETURNED            MM_EV(23,   5)
#define EV_MM_WEEK_ARCHERY_SWAMP_HP             MM_EV(32,   1)
#define EV_MM_WEEK_BEAVER_RACE_ALT              MM_EV(24,   2)
#define EV_MM_WEEK_ARCHERY_SWAMP_QUIVER         MM_EV(59,   4)
#define EV_MM_WEEK_ARCHERY_TOWN_QUIVER          MM_EV(59,   5)
#define EV_MM_WEEK_PAMELA_FATHER                MM_EV(75,   5)
#define EV_MM_WEEK_TOWN_FAIRY                   MM_EV(8,    7)
#define EV_MM_WEEK_DRANK_CHATEAU_ROMANI         MM_EV(14,   3)

#define EV_MM_WEEK_WOODFALL_TEMPLE_RISE         MM_EV(20,   0)
#define EV_MM_WEEK_DUNGEON_WF                   MM_EV(20,   1)
#define EV_MM_WEEK_CURIOSITY_SHOP_BOUGHT        MM_EV(33,   2)
#define EV_MM_WEEK_SNOWHEAD_BLIZZARD            MM_EV(30,   0)
#define EV_MM_WEEK_DUNGEON_SH                   MM_EV(33,   7)
#define EV_MM_WEEK_GREAT_BAY_TURTLE             MM_EV(53,   5)
#define EV_MM_WEEK_DUNGEON_GB                   MM_EV(55,   7)
#define EV_MM_WEEK_DUNGEON_ST                   MM_EV(52,   5)
#define EV_MM_WEEK_WITCH_MUSHROOM               MM_EV(53,   3)

#define EV_MM_WEEK_NOTEBOOK_MET_ANJU                        MM_EV(66, 0)
#define EV_MM_WEEK_NOTEBOOK_MET_KAFEI                       MM_EV(66, 1)
#define EV_MM_WEEK_NOTEBOOK_MET_CURIOSITY_SHOP_MAN          MM_EV(66, 2)
#define EV_MM_WEEK_NOTEBOOK_MET_BOMB_SHOP_LADY              MM_EV(66, 3)
#define EV_MM_WEEK_NOTEBOOK_MET_ROMANI                      MM_EV(66, 4)
#define EV_MM_WEEK_NOTEBOOK_MET_CREMIA                      MM_EV(66, 5)
#define EV_MM_WEEK_NOTEBOOK_MET_MAYOR_DOTOUR                MM_EV(66, 6)
#define EV_MM_WEEK_NOTEBOOK_MET_MADAME_AROMA                MM_EV(66, 7)
#define EV_MM_WEEK_NOTEBOOK_MET_TOTO                        MM_EV(67, 0)
#define EV_MM_WEEK_NOTEBOOK_MET_GORMAN                      MM_EV(67, 1)
#define EV_MM_WEEK_NOTEBOOK_MET_POSTMAN                     MM_EV(67, 2)
#define EV_MM_WEEK_NOTEBOOK_MET_ROSA_SISTERS                MM_EV(67, 3)
#define EV_MM_WEEK_NOTEBOOK_MET_TOILET_HAND                 MM_EV(67, 4)
#define EV_MM_WEEK_NOTEBOOK_MET_ANJUS_GRANDMOTHER           MM_EV(67, 5)
#define EV_MM_WEEK_NOTEBOOK_MET_KAMARO                      MM_EV(67, 6)
#define EV_MM_WEEK_NOTEBOOK_MET_GROG                        MM_EV(67, 7)
#define EV_MM_WEEK_NOTEBOOK_MET_GORMAN_BROTHERS             MM_EV(68, 0)
#define EV_MM_WEEK_NOTEBOOK_MET_SHIRO                       MM_EV(68, 1)
#define EV_MM_WEEK_NOTEBOOK_MET_GURU_GURU                   MM_EV(68, 2)
#define EV_MM_WEEK_NOTEBOOK_MET_BOMBERS                     MM_EV(68, 3)
#define EV_MM_WEEK_NOTEBOOK_RECEIVED_ROOM_KEY               MM_EV(68, 4)
#define EV_MM_WEEK_NOTEBOOK_PROMISED_MIDNIGHT_MEETING       MM_EV(68, 5)
#define EV_MM_WEEK_NOTEBOOK_PROMISED_TO_MEET_KAFEI          MM_EV(68, 6)
#define EV_MM_WEEK_NOTEBOOK_RECEIVED_LETTER_TO_KAFEI        MM_EV(68, 7)
#define EV_MM_WEEK_NOTEBOOK_DEPOSITED_LETTER_TO_KAFEI       MM_EV(69, 0)
#define EV_MM_WEEK_NOTEBOOK_RECEIVED_PENDANT_OF_MEMORIES    MM_EV(69, 1)
#define EV_MM_WEEK_NOTEBOOK_DELIVERED_PENDANT_OF_MEMORIES   MM_EV(69, 2)
#define EV_MM_WEEK_NOTEBOOK_ESCAPED_SAKONS_HIDEOUT          MM_EV(69, 3)
#define EV_MM_WEEK_NOTEBOOK_PROMISED_TO_HELP_WITH_THEM      MM_EV(69, 4)
#define EV_MM_WEEK_NOTEBOOK_DEFENDED_AGAINST_THEM           MM_EV(69, 5)
#define EV_MM_WEEK_NOTEBOOK_RECEIVED_MILK_BOTTLE            MM_EV(69, 6)
#define EV_MM_WEEK_NOTEBOOK_ESCORTED_CREMIA                 MM_EV(69, 7)
#define EV_MM_WEEK_NOTEBOOK_RECEIVED_ROMANIS_MASK           MM_EV(70, 0)
#define EV_MM_WEEK_NOTEBOOK_RECEIVED_KEATON_MASK            MM_EV(70, 1)
#define EV_MM_WEEK_NOTEBOOK_RECEIVED_PRIORITY_MAIL          MM_EV(70, 2)
#define EV_MM_WEEK_NOTEBOOK_DELIVERED_PRIORITY_MAIL         MM_EV(70, 3)
#define EV_MM_WEEK_NOTEBOOK_LEARNED_SECRET_CODE             MM_EV(70, 4)
#define EV_MM_WEEK_NOTEBOOK_RECEIVED_BOMBERS_NOTEBOOK       MM_EV(70, 5)
#define EV_MM_WEEK_NOTEBOOK_RECEIVED_MAYOR_HP               MM_EV(70, 6)
#define EV_MM_WEEK_NOTEBOOK_RECEIVED_ROSA_SISTERS_HP        MM_EV(70, 7)
#define EV_MM_WEEK_NOTEBOOK_RECEIVED_TOILET_HAND_HP         MM_EV(71, 0)
#define EV_MM_WEEK_NOTEBOOK_RECEIVED_GRANDMA_SHORT_STORY_HP MM_EV(71, 1)
#define EV_MM_WEEK_NOTEBOOK_RECEIVED_GRANDMA_LONG_STORY_HP  MM_EV(71, 2)
#define EV_MM_WEEK_NOTEBOOK_RECEIVED_POSTMAN_HP             MM_EV(71, 3)
#define EV_MM_WEEK_NOTEBOOK_RECEIVED_KAFEIS_MASK            MM_EV(71, 4)
#define EV_MM_WEEK_NOTEBOOK_RECEIVED_ALL_NIGHT_MASK         MM_EV(71, 5)
#define EV_MM_WEEK_NOTEBOOK_RECEIVED_BUNNY_HOOD             MM_EV(71, 6)
#define EV_MM_WEEK_NOTEBOOK_RECEIVED_GAROS_MASK             MM_EV(71, 7)
#define EV_MM_WEEK_NOTEBOOK_RECEIVED_CIRCUS_LEADERS_MASK    MM_EV(72, 0)
#define EV_MM_WEEK_NOTEBOOK_RECEIVED_POSTMANS_HAT           MM_EV(72, 1)
#define EV_MM_WEEK_NOTEBOOK_RECEIVED_COUPLES_MASK           MM_EV(72, 2)
#define EV_MM_WEEK_NOTEBOOK_RECEIVED_BLAST_MASK             MM_EV(72, 3)
#define EV_MM_WEEK_NOTEBOOK_RECEIVED_KAMAROS_MASK           MM_EV(72, 4)
#define EV_MM_WEEK_NOTEBOOK_RECEIVED_STONE_MASK             MM_EV(72, 5)
#define EV_MM_WEEK_NOTEBOOK_RECEIVED_BREMEN_MASK            MM_EV(72, 6)

#if !defined(__ASSEMBLER__)
# define MM_SET_EVENT_WEEK(x)       (gMmSave.weekEventReg[(x) / 8] |= (1 << ((x) % 8)))
# define MM_CLEAR_EVENT_WEEK(x)     (gMmSave.weekEventReg[(x) / 8] &= ~(1 << ((x) % 8)))
# define MM_GET_EVENT_WEEK(x)       (gMmSave.weekEventReg[(x) / 8] & (1 << ((x) % 8)))
# define MM_CHECK_EVENT_INF(x)      (gMmSave.eventInf[(x) >> 4] & (1 << ((x) & 0xf)))
int  GetEventChk(int index);
void SetEventChk(int index);
#endif

#endif
