#include <combo.h>
#include <combo/player.h>
#include <combo/item.h>

void EnToto_UpdateWrapper(Actor* this, GameState_Play* play)
{
    int shouldGiveItem;
    ActorFunc EnToto_Update;

    shouldGiveItem = 0;

    if (shouldGiveItem)
    {

    }
    else
    {
        EnToto_Update = actorAddr(AC_EN_TOTO, 0x80ba4e2c);
        EnToto_Update(this, play);
    }
}

void EnToto_SpawnItemGiver(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    comboGiveItemNpc(this, play, gi, NPC_MM_MASK_TROUPE_LEADER, a, b);
    //comboSpawnItemGiver(play, NPC_MM_MASK_TROUPE_LEADER);
    //this->parent = &GET_LINK(play)->base;
}

PATCH_CALL(0x80ba4e10, EnToto_SpawnItemGiver);
