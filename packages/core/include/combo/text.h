#ifndef COMBO_TEXT_H
#define COMBO_TEXT_H

#include <combo/types.h>

#if defined(GAME_OOT)
# define TEXT_FAST           "\x08"
# define TEXT_CZ             "\x05\x40"
# define TEXT_COLOR_RED      "\x05\x41"
# define TEXT_COLOR_GREEN    "\x05\x42"
# define TEXT_COLOR_BLUE     "\x05\x43"
# define TEXT_COLOR_TEAL     "\x05\x44"
# define TEXT_COLOR_PINK     "\x05\x45"
# define TEXT_COLOR_YELLOW   "\x05\x46"
# define TEXT_COLOR_ORANGE   TEXT_COLOR_YELLOW
# define TEXT_END            "\x02"
# define TEXT_CHOICE2        "\x1b"
# define TEXT_CHOICE3        "\x1c"
# define TEXT_NL             "\x01"
# define TEXT_NOCLOSE        "\x0a"
# define TEXT_SIGNAL         "\x0b"
# define TEXT_ICON           "\x13"
# define TEXT_BB             "\x04"
#else
# define TEXT_FAST           "\x17"
# define TEXT_CZ             "\x00"
# define TEXT_COLOR_RED      "\x01"
# define TEXT_COLOR_GREEN    "\x02"
# define TEXT_COLOR_BLUE     "\x03"
# define TEXT_COLOR_YELLOW   "\x04"
# define TEXT_COLOR_TEAL     "\x05"
# define TEXT_COLOR_PINK     "\x06"
# define TEXT_COLOR_ORANGE   "\x08"
# define TEXT_END            "\xbf"
# define TEXT_CHOICE2        "\xc2"
# define TEXT_CHOICE3        "\xc3"
# define TEXT_BOX_BREAK      "\x10"
# define TEXT_NL             "\x11"
# define TEXT_BOX_BREAK_2    "\x12"
# define TEXT_NOCLOSE        "\x1a"
# define TEXT_SIGNAL         "\xe0"
# define TEXT_ICON           ""
# define TEXT_BB             "\x10"
#endif

#define TEXT_C0   TEXT_COLOR_TEAL
#define TEXT_C1   TEXT_COLOR_RED
#define TEXT_C2   TEXT_COLOR_PINK
#define TEXT_C3   TEXT_COLOR_YELLOW

#define TF_NONE         0
#define TF_CAPITALIZE   (1 << 0)
#define TF_PREPOS       (1 << 1)
#define TF_PROGRESSIVE  (1 << 2)
#define TF_SIGNAL       (1 << 3)
#define TF_PLURAL       (1 << 4)

#define TFS_CONFIRM     (1 << 0)
#define TFS_MUSHROOM    (1 << 1)

typedef struct GameState_Play GameState_Play;
typedef struct ComboItemQuery ComboItemQuery;
typedef struct ComboItemOverride ComboItemOverride;

void comboTextAutoLineBreaks(char* buffer);
void comboTextAppendStr(char** dst, const char* src);
void comboTextAppendHeader(char** b);
void comboTextAppendShopHeader(char** b, s16 price);
void comboTextAppendShopHeader2(char** b, s16 price, s16 price2);
void comboTextAppendClearColor(char** b);
void comboTextAppendNum(char** b, int num);
void comboTextAppendItemName(char** b, s16 gi, int flags);
void comboTextAppendItemNameEx(char** b, s16 gi, int flags, int importance);
void comboTextAppendItemNameQuery(char** b, const ComboItemQuery* q, int flags);
void comboTextAppendItemNameQueryEx(char** b, const ComboItemQuery* q, int flags, int importance);
void comboTextAppendItemNameOverride(char** b, const ComboItemOverride* o, int flags);
void comboTextAppendItemNameOverrideEx(char** b, const ComboItemOverride* o, int flags, int importance);
void comboTextAppendRegionName(char** b, u8 regionId, u8 world, int flags);
int  comboTextAppendCheckName(char** b, u8 checkId, u8 world);
void comboTextAppendNpcReward(char** b, s16 npcId, s16 gi, int importance);
void comboTextMessageCantBuy(GameState_Play* play, int flags);
void comboTextAppendDungeonName(char** buffer, u8 dungeonId);
void comboTextAppendBossName(char** buffer, u8 bossId);
void comboTextAppendOrd(char** b, int num);

#endif
