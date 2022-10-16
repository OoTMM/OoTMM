#include <combo.h>

static int ItemGiver_Common(Actor* this, GameState_Play* play, s16 gi, int ev)
{
    if (Actor_HasParent(this))
    {
        SetEventChk(ev);
        ActorDestroy(this);
        return 1;
    }
    else
    {
        gi = comboOverride(OV_NPC, 0, this->variable, gi);
        GiveItem(this, play, gi, 10000.f, 5000.f);
        return 0;
    }
}

static void ItemGiver_ZeldaLightArrow(Actor* this, GameState_Play* play)
{
    ItemGiver_Common(this, play, GI_OOT_ARROW_LIGHT, EV_OOT_CHK_LIGHT_ARROW);
}

static void ItemGiver_SheikDesertColossus(Actor* this, GameState_Play* play)
{
    ItemGiver_Common(this, play, GI_OOT_SONG_TP_SPIRIT, EV_OOT_CHK_SONG_TP_SPIRIT);
}

static void ItemGiver_Init(Actor* this, GameState_Play* play)
{
    switch (this->variable)
    {
    case NPC_OOT_SHEIK_SPIRIT:
        this->update = ItemGiver_SheikDesertColossus;
        break;
    case NPC_OOT_ZELDA_LIGHT_ARROW:
        this->update = ItemGiver_ZeldaLightArrow;
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
    NULL,
    NULL,
};

static void spawnGiver(GameState_Play* play, u16 npcId)
{
    SpawnActor(
        (char*)play + 0x1c24,
        play,
        AC_ITEM_GIVER,
        0, 0, 0,
        0, 0, 0,
        npcId
    );
}

void comboSpawnItemGivers(GameState_Play* play)
{
    /* Sheik in colossus */
    if (gSave.entrance == 0x1e1 && !GetEventChk(EV_OOT_CHK_SONG_TP_SPIRIT))
    {
        spawnGiver(play, NPC_OOT_SHEIK_SPIRIT);
    }

    /* Zelda Light Arrows */
    if (gSave.entrance == 0x0053 && gSave.quest.medallionShadow && gSave.quest.medallionSpirit && gSave.age == AGE_ADULT && !GetEventChk(EV_OOT_CHK_LIGHT_ARROW))
    {
        spawnGiver(play, NPC_OOT_ZELDA_LIGHT_ARROW);
    }
}
