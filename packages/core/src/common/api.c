#include <combo.h>

extern int Player_AddItemWithIcon[];
typedef u8 (*AddItemWithIconFn)(PlayState*, Actor_Player*, const GetItem*);

u8 AddItemWithIcon(PlayState* play, Actor_Player* link, const GetItem* giEntry)
{
    AddItemWithIconFn fn;

    fn = OverlayAddr((u32)Player_AddItemWithIcon);
    return fn(play, link, giEntry);
}
