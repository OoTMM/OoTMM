#include <combo.h>

static int ItemGiver_Common(Actor* this, GameState_Play* play, s16 gi, int ev)
{
    if (GetEventChk(ev))
    {
        ActorDestroy(this);
        return 0;
    }
    else if (Actor_HasParent(this))
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

static void ItemGiver_Init(Actor* this, GameState_Play* play)
{
}

static void ItemGiver_Update(Actor* this, GameState_Play* play)
{
    switch (this->variable)
    {
    case NPC_OOT_SARIA_OCARINA:
        ItemGiver_Common(this, play, GI_OOT_OCARINA_FAIRY, EV_OOT_CHK_SARIA_OCARINA);
        break;
    case NPC_OOT_SHEIK_SHADOW:
        if (ItemGiver_Common(this, play, GI_OOT_SONG_TP_SHADOW, EV_OOT_CHK_SONG_TP_SHADOW))
        {
            SetEventChk(EV_OOT_CHK_BONGO_ESCAPE);
        }
        break;
    case NPC_OOT_SHEIK_SPIRIT:
        ItemGiver_Common(this, play, GI_OOT_SONG_TP_SPIRIT, EV_OOT_CHK_SONG_TP_SPIRIT);
        break;
    case NPC_OOT_ZELDA_LIGHT_ARROW:
        ItemGiver_Common(this, play, GI_OOT_ARROW_LIGHT, EV_OOT_CHK_LIGHT_ARROW);
        break;
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
    /* Saria's Ocarina */
    if (gSave.entrance == 0x05e0 && !GetEventChk(EV_OOT_CHK_SARIA_OCARINA))
    {
        comboSpawnItemGiver(play, NPC_OOT_SARIA_OCARINA);
    }

    /* Sheik in Kakariko */
    if (play->sceneId == SCE_OOT_KAKARIKO_VILLAGE && gSave.inventory.quest.medallionForest && gSave.inventory.quest.medallionFire && gSave.inventory.quest.medallionWater && gSave.age == AGE_ADULT && !GetEventChk(EV_OOT_CHK_SONG_TP_SHADOW))
    {
        comboSpawnItemGiver(play, NPC_OOT_SHEIK_SHADOW);
    }

    /* Sheik in colossus */
    if (gSave.entrance == 0x1e1 && !GetEventChk(EV_OOT_CHK_SONG_TP_SPIRIT))
    {
        comboSpawnItemGiver(play, NPC_OOT_SHEIK_SPIRIT);
    }

    /* Zelda Light Arrows */
    if (play->sceneId == SCE_OOT_TEMPLE_OF_TIME && gSave.inventory.quest.medallionShadow && gSave.inventory.quest.medallionSpirit && gSave.age == AGE_ADULT && !GetEventChk(EV_OOT_CHK_LIGHT_ARROW))
    {
        comboSpawnItemGiver(play, NPC_OOT_ZELDA_LIGHT_ARROW);
    }
}
