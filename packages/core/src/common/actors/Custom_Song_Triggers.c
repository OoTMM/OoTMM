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
                    return 1;
                }
                break;

            case TRIGGER_OOT_SONG_SARIA:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_SONG_SARIA)) {
                    return 1;
                }
                break;

            case TRIGGER_OOT_SONG_STORMS:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_SONG_STORMS)) {

                    return 1;
                }
                break;

            case TRIGGER_OOT_SONG_SUN:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_SONG_SUN)) {
                    return 1;
                }
                break;

            case TRIGGER_OOT_SONG_TIME:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_SONG_TIME)) {
                    return 1;
                }
                break;

            case TRIGGER_OOT_SONG_TP_FIRE:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_SONG_TP_FIRE)) {
                    return 1;
                }
                break;

            case TRIGGER_OOT_SONG_TP_FOREST:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_SONG_TP_FOREST)) {
                    return 1;
                }
                break;

            case TRIGGER_OOT_SONG_TP_LIGHT:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_SONG_TP_LIGHT)) {
                    return 1;
                }
                break;

            case TRIGGER_OOT_SONG_TP_SHADOW:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_SONG_TP_SHADOW)) {
                    return 1;
                }
                break;

            case TRIGGER_OOT_SONG_TP_SPIRIT:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_SONG_TP_SPIRIT)) {
                    return 1;
                }
                break;

            case TRIGGER_OOT_SONG_TP_WATER:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_SONG_TP_WATER)) {
                    return 1;
                }
                break;

            case TRIGGER_OOT_SONG_ZELDA:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_SONG_ZELDA)) {
                    return 1;
                }
                break;

            case TRIGGER_OOT_SONG_EMPTINESS:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_SONG_EMPTINESS)) {
                    return 1;
                }
                break;

            case TRIGGER_MM_SONG_AWAKENING:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_MM_SONG_AWAKENING)) {
                    return 1;
                }
                break;

            case TRIGGER_MM_SONG_EMPTINESS:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_MM_SONG_EMPTINESS)) {
                    return 1;
                }
                break;

            case TRIGGER_MM_SONG_EPONA:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_MM_SONG_EPONA)) {
                    return 1;
                }
                break;

            case TRIGGER_MM_SONG_GORON_HALF:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_MM_SONG_GORON_HALF)) {
                    return 1;
                }
                break;

            case TRIGGER_MM_SONG_GORON:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_MM_SONG_GORON)) {
                    return 1;
                }
                break;

            case TRIGGER_MM_SONG_HEALING:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_MM_SONG_HEALING)) {
                    return 1;
                }
                break;

            case TRIGGER_MM_SONG_ORDER:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_MM_SONG_ORDER)) {
                    return 1;
                }
                break;

            case TRIGGER_MM_SONG_SOARING:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_MM_SONG_SOARING)) {
                    return 1;
                }
                break;

            case TRIGGER_MM_SONG_STORMS:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_MM_SONG_STORMS)) {
                    return 1;
                }
                break;

            case TRIGGER_MM_SONG_SUN:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_MM_SONG_SUN)) {
                    return 1;
                }
                break;

            case TRIGGER_MM_SONG_TIME:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_MM_SONG_TIME)) {
                    return 1;
                }
                break;

            case TRIGGER_MM_SONG_ZORA:
                if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_MM_SONG_ZORA)) {
                    return 1;
                }
                break;
        }

        return 0;
    }