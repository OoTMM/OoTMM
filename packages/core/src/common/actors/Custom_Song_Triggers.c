#include <combo.h>
#include <combo/item.h>
#include <combo/net.h>
#include <combo/player.h>
#include <combo/config.h>
#include <combo/multi.h>
#include <combo/actor.h>

int CustomSongTriggers_HandleSongTriggers(Actor_CustomTriggers* this, PlayState* play, int trigger)
    {
        switch (trigger) {
            case TRIGGER_OOT_SONG_EPONA:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_SONG_EPONA)) {
                    comboAddItemRaw(play, GI_OOT_SONG_EPONA);
                    return 1;
                }
                break;

            case TRIGGER_OOT_SONG_SARIA:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_SONG_SARIA)) {
                    gOotSave.info.inventory.quest.songSaria = 1;
                    return 1;
                }
                break;

            case TRIGGER_OOT_SONG_STORMS:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_SONG_STORMS)) {
                    comboAddItemRaw(play, GI_OOT_SONG_STORMS);
                    return 1;
                }
                break;

            case TRIGGER_OOT_SONG_SUN:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_SONG_SUN)) {
                    comboAddItemRaw(play, GI_OOT_SONG_SUN);
                    return 1;
                }
                break;

            case TRIGGER_OOT_SONG_TIME:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_SONG_TIME)) {
                    comboAddItemRaw(play, GI_OOT_SONG_TIME);
                    return 1;
                }
                break;

            case TRIGGER_OOT_SONG_TP_FIRE:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_SONG_TP_FIRE)) {
                    gOotSave.info.inventory.quest.songTpFire = 1;
                    return 1;
                }
                break;

            case TRIGGER_OOT_SONG_TP_FOREST:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_SONG_TP_FOREST)) {
                    gOotSave.info.inventory.quest.songTpForest = 1;
                    return 1;
                }
                break;

            case TRIGGER_OOT_SONG_TP_LIGHT:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_SONG_TP_LIGHT)) {
                    gOotSave.info.inventory.quest.songTpLight = 1;
                    return 1;
                }
                break;

            case TRIGGER_OOT_SONG_TP_SHADOW:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_SONG_TP_SHADOW)) {
                    gOotSave.info.inventory.quest.songTpShadow = 1;
                    return 1;
                }
                break;

            case TRIGGER_OOT_SONG_TP_SPIRIT:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_SONG_TP_SPIRIT)) {
                    gOotSave.info.inventory.quest.songTpSpirit = 1;
                    return 1;
                }
                break;

            case TRIGGER_OOT_SONG_TP_WATER:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_SONG_TP_WATER)) {
                    gOotSave.info.inventory.quest.songTpWater = 1;
                    return 1;
                }
                break;

            case TRIGGER_OOT_SONG_ZELDA:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_SONG_ZELDA)) {
                    gOotSave.info.inventory.quest.songZelda = 1;
                    return 1;
                }
                break;

            case TRIGGER_OOT_SONG_EMPTINESS:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_SONG_EMPTINESS)) {
                    comboAddItemRaw(play, GI_OOT_SONG_EMPTINESS);
                    return 1;
                }
                break;

            case TRIGGER_MM_SONG_AWAKENING:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_MM_SONG_AWAKENING)) {
                    gMmSave.info.inventory.quest.songAwakening = 1;
                    return 1;
                }
                break;

            case TRIGGER_MM_SONG_EMPTINESS:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_MM_SONG_EMPTINESS)) {
                    comboAddItemRaw(play, GI_MM_SONG_EMPTINESS);
                    return 1;
                }
                break;

            case TRIGGER_MM_SONG_EPONA:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_MM_SONG_EPONA)) {
                    comboAddItemRaw(play, GI_MM_SONG_EPONA);
                    return 1;
                }
                break;

            case TRIGGER_MM_SONG_GORON_HALF:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_MM_SONG_GORON_HALF)) {
                    gMmSave.info.inventory.quest.songLullabyIntro = 1;
                    return 1;
                }
                break;

            case TRIGGER_MM_SONG_GORON:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_MM_SONG_GORON)) {
                    gMmSave.info.inventory.quest.songLullaby = 1;
                    return 1;
                }
                break;

            case TRIGGER_MM_SONG_HEALING:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_MM_SONG_HEALING)) {
                    gMmSave.info.inventory.quest.songHealing = 1;
                    return 1;
                }
                break;

            case TRIGGER_MM_SONG_ORDER:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_MM_SONG_ORDER)) {
                    gMmSave.info.inventory.quest.songOrder = 1;
                    return 1;
                }
                break;

            case TRIGGER_MM_SONG_SOARING:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_MM_SONG_SOARING)) {
                    gMmSave.info.inventory.quest.songSoaring = 1;
                    return 1;
                }
                break;

            case TRIGGER_MM_SONG_STORMS:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_MM_SONG_STORMS)) {
                    comboAddItemRaw(play, GI_MM_SONG_SUN);
                    return 1;
                }
                break;

            case TRIGGER_MM_SONG_SUN:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_MM_SONG_SUN)) {
                    comboAddItemRaw(play, GI_MM_SONG_SUN);
                    return 1;
                }
                break;

            case TRIGGER_MM_SONG_TIME:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_MM_SONG_TIME)) {
                    comboAddItemRaw(play, GI_MM_SONG_TIME);
                    return 1;
                }
                break;

            case TRIGGER_MM_SONG_ZORA:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_MM_SONG_ZORA)) {
                    gMmSave.info.inventory.quest.songNewWave = 1;
                    return 1;
                }
                break;
        }

        return 0;
    }