#include <combo.h>
#include <combo/checks.h>

u32 Checks_MakeOverrideKey(const ComboItemQuery* q)
{
    u32 key;

    if (q->ovType == OV_NONE)
        return 0;

    key = 0;
    key |= (((u32)(q->ovType & 0x7f)) << 24);
    key |= (((u32)(q->sceneId & 0xff)) << 16);
    key |= (((u32)(q->roomId & 0xff)) << 8);
    key |= (((u32)(q->id & 0xff)) << 0);

#if defined(GAME_MM)
    key |= 0x80000000;
#endif

    return key;
}

u32 Checks_MakeNpcOverrideKey(int npc)
{
    ComboItemQuery q = ITEM_QUERY_INIT;
    q.ovType = OV_NPC;
    q.id = npc;
    return Checks_MakeOverrideKey(&q);
}
