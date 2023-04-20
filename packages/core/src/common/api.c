#include <combo.h>

extern int Player_AddItemWithIcon[];
typedef u8 (*AddItemWithIconFn)(GameState_Play*, Actor_Player*, const GetItem*);

u8 AddItemWithIcon(GameState_Play* play, Actor_Player* link, const GetItem* giEntry)
{
    AddItemWithIconFn fn;

    fn = OverlayAddr((u32)Player_AddItemWithIcon);
    return fn(play, link, giEntry);
}
