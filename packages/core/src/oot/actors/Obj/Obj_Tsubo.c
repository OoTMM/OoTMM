#include <combo.h>

void ObjTsubo_InitWrapper(Actor_ObjTsubo* this, GameState_Play* play)
{
    ActorFunc init;

    /* Set the extended properties */
    this->extendedRoomId = this->base.room;
    this->extendedId = g.actorIndex;

    /* Forward init */
    init = actorAddr(AC_OBJ_TSUBO, 0x80a653a8);
    init(&this->base, play);
}

void ObjTsubo_SpawnShuffledDrop(Actor_ObjTsubo* this, GameState_Play* play)
{
    u16 var;
    Actor_EnItem00* item;

    if (comboXflagsGet(play->sceneId, g.sceneSetupId, this->extendedRoomId, this->extendedId))
    {
        /* Already spawned */
        var = this->base.variable;
        if ((var & 0xff) < 0x1a)
        {
            Item_DropCollectible(play, &this->base.position, (var & 0xff) | (((var >> 9) & 0x3f) << 8));
        }
        return;
    }

    /* Spawn an extended item */
    g.spawnExtended = 1;
    item = Item_DropCollectible(play, &this->base.position, 0x0000);
    g.spawnExtended = 0;

    item->extendedRoomId = this->extendedRoomId;
    item->extendedId = this->extendedId;
    item->extendedGi = GI_OOT_RUPEE_GREEN;
}
