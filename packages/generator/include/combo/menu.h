#ifndef COMBO_MENU_H
#define COMBO_MENU_H

#include <PR/gbi.h>

#define MENU_NONE               0
#define MENU_INFO               1
#define MENU_SONG_NOTES_OOT     2
#define MENU_SONG_NOTES_MM      3
#define MENU_SOULS_OOT_ENEMY    4
#define MENU_SOULS_OOT_BOSS     5
#define MENU_SOULS_OOT_NPC      6
#define MENU_SOULS_OOT_ANIMAL   7
#define MENU_SOULS_OOT_MISC     8
#define MENU_SOULS_MM_ENEMY     9
#define MENU_SOULS_MM_BOSS      10
#define MENU_SOULS_MM_NPC       11
#define MENU_SOULS_MM_ANIMAL    12
#define MENU_SOULS_MM_MISC      13
#define MENU_RUSTY_KEYS_OOT     14
#define MENU_RUSTY_KEYS_MM      15
#define MENU_MAX                16

typedef struct PlayState PlayState;

/* Menu */
void comboMenuTick(void);
void comboMenuUpdate(PlayState* play);
void comboMenuDraw(PlayState* play);
void comboMenuNext(void);

#define MM_EQUIPPED_ITEM_OUTLINE   ((u8*)0x02001360)
#define MM_QUEST_SHIELD_SLOT        0x04
#define MM_QUEST_SWORD_SLOT         0x05
#define MM_PAUSE_QUEST_VTX_OFFSET   0x198
#define MM_PAUSE_NAMED_ITEM_OFFSET  0x25c
#define MM_PAUSE_ITEM_NONE          999

enum
{
    MM_EQ_ICON_MASTER = 0,
    MM_EQ_ICON_BIGGORON,
    MM_EQ_ICON_BIGGORON_BROKEN,
    MM_EQ_ICON_SHIELD_DEKU,
    MM_EQ_ICON_SHIELD_HYLIAN,
};

static const u8 sCustomEquipForeignIcons[] = {
    ITEM_OOT_SWORD_MASTER,
    ITEM_OOT_SWORD_KNIFE_BIGGORON,
    ITEM_OOT_SWORD_KNIFE_BROKEN,
    ITEM_OOT_SHIELD_DEKU,
    ITEM_OOT_SHIELD_HYLIAN,
};

enum
{
    MM_QUEST_EQUIP_SWORD = 0,
    MM_QUEST_EQUIP_SHIELD,
    MM_QUEST_EQUIP_MAX,
};

enum
{
    MM_QUEST_VTX_SMALL = 0,
    MM_QUEST_VTX_PRIMARY_OUTLINE,
    MM_QUEST_VTX_SMALL_OUTLINE,
    MM_QUEST_VTX_MAX,
};

#endif
