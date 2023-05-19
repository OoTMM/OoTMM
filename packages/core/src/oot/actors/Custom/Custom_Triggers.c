#include <combo.h>

#define TRIGGER_OOT_SHEIK_COLOSSUS          0x100
#define TRIGGER_OOT_SHEIK_KAKARIKO          0x101
#define TRIGGER_OOT_SARIA_OCARINA           0x102
#define TRIGGER_OOT_ZELDA_LIGHT_ARROW       0x103
#define TRIGGER_OOT_WEIRD_EGG               0x104
#define TRIGGER_OOT_POCKET_EGG              0x105

void CustomTriggers_HandleTriggerGame(Actor_CustomTriggers* this, GameState_Play* play)
{
    switch (this->trigger)
    {
    case TRIGGER_OOT_SHEIK_COLOSSUS:
        if (CustomTriggers_GiveItemNpc(this, play, GI_OOT_SONG_TP_SPIRIT, NPC_OOT_SHEIK_SPIRIT))
        {
            SetEventChk(EV_OOT_CHK_SONG_TP_SPIRIT);
            this->trigger = 0;
        }
        break;
    case TRIGGER_OOT_SHEIK_KAKARIKO:
        if (CustomTriggers_GiveItemNpc(this, play, GI_OOT_SONG_TP_SHADOW, NPC_OOT_SHEIK_SHADOW))
        {
            SetEventChk(EV_OOT_CHK_SONG_TP_SHADOW);
            SetEventChk(EV_OOT_CHK_BONGO_ESCAPE);
            this->trigger = 0;
        }
        break;
    case TRIGGER_OOT_SARIA_OCARINA:
        if (CustomTriggers_GiveItemNpc(this, play, GI_OOT_OCARINA_FAIRY, NPC_OOT_SARIA_OCARINA))
        {
            SetEventChk(EV_OOT_CHK_SARIA_OCARINA);
            this->trigger = 0;
        }
        break;
    case TRIGGER_OOT_ZELDA_LIGHT_ARROW:
        if (CustomTriggers_GiveItemNpc(this, play, GI_OOT_ARROW_LIGHT, NPC_OOT_ZELDA_LIGHT_ARROW))
        {
            SetEventChk(EV_OOT_CHK_LIGHT_ARROW);
            this->trigger = 0;
        }
        break;
    case TRIGGER_OOT_WEIRD_EGG:
        if (CustomTriggers_GiveItemNpc(this, play, GI_OOT_CHICKEN, NPC_OOT_WEIRD_EGG))
        {
            comboRemoveTradeItemChild(XITEM_OOT_CHILD_WEIRD_EGG);
            this->trigger = 0;
            this->events.weirdEgg = 0;
        }
        break;
    case TRIGGER_OOT_POCKET_EGG:
        if (CustomTriggers_GiveItemNpc(this, play, GI_OOT_POCKET_CUCCO, NPC_OOT_POCKET_EGG))
        {
            comboRemoveTradeItemAdult(XITEM_OOT_ADULT_POCKET_EGG);
            this->trigger = 0;
            this->events.pocketEgg = 0;
        }
        break;
    }
}

void CustomTriggers_CheckTriggerGame(Actor_CustomTriggers* this, GameState_Play* play)
{
    /* Sheik in colossus */
    if (gSave.entrance == 0x1e1 && !GetEventChk(EV_OOT_CHK_SONG_TP_SPIRIT))
    {
        this->trigger = TRIGGER_OOT_SHEIK_COLOSSUS;
        return;
    }

    /* Sheik in Kakariko */
    if (play->sceneId == SCE_OOT_KAKARIKO_VILLAGE && gSave.inventory.quest.medallionForest && gSave.inventory.quest.medallionFire && gSave.inventory.quest.medallionWater && gSave.age == AGE_ADULT && !GetEventChk(EV_OOT_CHK_SONG_TP_SHADOW))
    {
        this->trigger = TRIGGER_OOT_SHEIK_KAKARIKO;
        return;
    }

    /* Saria's Ocarina */
    if (gSave.entrance == 0x05e0 && !GetEventChk(EV_OOT_CHK_SARIA_OCARINA))
    {
        this->trigger = TRIGGER_OOT_SARIA_OCARINA;
        return;
    }

    /* Zelda Light Arrows */
    if (play->sceneId == SCE_OOT_TEMPLE_OF_TIME && gSave.age == AGE_ADULT && !GetEventChk(EV_OOT_CHK_LIGHT_ARROW))
    {
        int shouldTrigger;
        if (comboConfig(CFG_OOT_LACS_CUSTOM))
            shouldTrigger = comboSpecialCond(SPECIAL_LACS);
        else
            shouldTrigger = (gSave.inventory.quest.medallionSpirit && gSave.inventory.quest.medallionShadow);

        if (shouldTrigger)
            this->trigger = TRIGGER_OOT_ZELDA_LIGHT_ARROW;
        return;
    }

    /* Weird egg */
    if (this->events.weirdEgg)
    {
        this->trigger = TRIGGER_OOT_WEIRD_EGG;
        this->events.weirdEgg = 0;
        return;
    }

    /* Pocket egg */
    if (this->events.pocketEgg)
    {
        this->trigger = TRIGGER_OOT_POCKET_EGG;
        this->events.pocketEgg = 0;
        return;
    }
}

