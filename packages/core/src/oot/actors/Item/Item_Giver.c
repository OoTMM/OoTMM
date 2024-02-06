#include <combo.h>
#include <combo/item.h>

static int ItemGiver_Common(Actor* this, GameState_Play* play, s16 gi, int ev)
{
    if (GetEventChk(ev))
    {
        ActorDestroy(this);
        return 0;
    }

    if (Actor_HasParent(this))
    {
        SetEventChk(ev);
        ActorDestroy(this);
        return 1;
    }

    comboGiveItemNpc(this, play, gi, this->variable, 10000.f, 5000.f);
    return 0;
}

static void ItemGiver_Init(Actor* this, GameState_Play* play)
{
}

static void ItemGiver_Update(Actor* this, GameState_Play* play)
{
    switch (this->variable)
    {
    case NPC_OOT_BLUE_WARP_GOHMA:
        ItemGiver_Common(this, play, GI_OOT_STONE_EMERALD, EV_OOT_CHK_STONE_EMERALD);
        break;
    case NPC_OOT_BLUE_WARP_KING_DODONGO:
        ItemGiver_Common(this, play, GI_OOT_STONE_RUBY, EV_OOT_CHK_STONE_RUBY);
        break;
    case NPC_OOT_BLUE_WARP_BARINADE:
        ItemGiver_Common(this, play, GI_OOT_STONE_SAPPHIRE, EV_OOT_CHK_STONE_SAPPHIRE);
        break;
    case NPC_OOT_BLUE_WARP_PHANTOM_GANON:
        ItemGiver_Common(this, play, GI_OOT_MEDALLION_FOREST, EV_OOT_CHK_MEDALLION_FOREST);
        break;
    case NPC_OOT_BLUE_WARP_VOLVAGIA:
        ItemGiver_Common(this, play, GI_OOT_MEDALLION_FIRE, EV_OOT_CHK_MEDALLION_FIRE);
        break;
    case NPC_OOT_BLUE_WARP_MORPHA:
        ItemGiver_Common(this, play, GI_OOT_MEDALLION_WATER, EV_OOT_CHK_MEDALLION_WATER);
        break;
    case NPC_OOT_BLUE_WARP_BONGO_BONGO:
        ItemGiver_Common(this, play, GI_OOT_MEDALLION_SHADOW, EV_OOT_CHK_MEDALLION_SHADOW);
        break;
    case NPC_OOT_BLUE_WARP_TWINROVA:
        ItemGiver_Common(this, play, GI_OOT_MEDALLION_SPIRIT, EV_OOT_CHK_MEDALLION_SPIRIT);
        break;
    default:
        ActorDestroy(this);
        break;
    }
}

ActorInit ItemGiver_gActorInit = {
    AC_ITEM_GIVER,
    0x6,
    0x10,
    0x1,
    sizeof(Actor),
    (ActorFunc)ItemGiver_Init,
    NULL,
    (ActorFunc)ItemGiver_Update,
    NULL,
};

void comboSpawnItemGiver(GameState_Play* play, u16 npcId)
{
    SpawnActor(
        &play->actorCtx,
        play,
        AC_ITEM_GIVER,
        0, 0, 0,
        0, 0, 0,
        npcId
    );
}
