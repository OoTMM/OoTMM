#include <combo.h>
#include <combo/item.h>

void EnHy_GiveItem(Actor* actor, PlayState* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    if (gi == GI_OOT_HEART_PIECE)
        npc = NPC_OOT_DOG_LADY;
    comboGiveItemNpc(actor, play, gi, npc, a, b);
}

PATCH_CALL(0x80ae5138, EnHy_GiveItem);
PATCH_CALL(0x80ae6e84, EnHy_GiveItem);

static s32 EnHy_PlayerHasSpecificBottle(u8 bottleItem)
{
    s32 slot;

    for (slot = ITS_OOT_BOTTLE; slot <= ITS_OOT_BOTTLE4; slot++) {
        if (gSaveContext.save.info.inventory.items[slot] == bottleItem) {
            return true;
        }
    }

    if (gOotExtraItems.bottleAdultSlot == bottleItem || gOotExtraItems.bottleChildSlot == bottleItem)
    {
        return true;
    }

    return false;
}

static s8 sExchangingItemId = -1;

void EnHy_OfferBuyBottledItem(Actor* this, PlayState* play)
{
    Player* player = GET_PLAYER(play);

    if ((this->params & 0x7f) == 5) /* ENHY_TYPE_BEGGAR */
    {
        if (!EnHy_PlayerHasSpecificBottle(ITEM_OOT_BLUE_FIRE)
            && !EnHy_PlayerHasSpecificBottle(ITEM_OOT_BUGS)
            && !EnHy_PlayerHasSpecificBottle(ITEM_OOT_FISH)
            && !EnHy_PlayerHasSpecificBottle(ITEM_OOT_MAGIC_MUSHROOM)
            && !EnHy_PlayerHasSpecificBottle(ITEM_OOT_CHATEAU)
            /* && !EnHy_PlayerHasSpecificBottle(ITEM_OOT_GOLD_DUST) */
            && !EnHy_PlayerHasSpecificBottle(ITEM_OOT_SPRING_WATER)
            && !EnHy_PlayerHasSpecificBottle(ITEM_OOT_SPRING_WATER_HOT)
            && !EnHy_PlayerHasSpecificBottle(ITEM_OOT_ZORA_EGG))
        {
            switch (Actor_GetPlayerExchangeItemId(play))
            {
            case EXCH_ITEM_BOTTLE_POE:
            case EXCH_ITEM_BOTTLE_BIG_POE:
            case EXCH_ITEM_BOTTLE_RUTOS_LETTER:
            case EXCH_CUSTOM_ITEM_DEKU_PRINCESS:
            case EXCH_CUSTOM_ITEM_SEAHORSE:
            case EXCH_CUSTOM_ITEM_GOLD_DUST: /* TODO can we remove this and force gold dust renewable? */
                this->messageId = 0x70EF;
                break;

            default:
                if (Player_GetMask(play) == 0) /* PLAYER_MASK_NONE */
                {
                    this->messageId = 0x70ED;
                }
                break;
            }
        }
        else
        {
            s8 exchangingItemId = Actor_GetPlayerExchangeItemId(play);
            switch (exchangingItemId)
            {
            /* Set all valid sales to 0x70f0 and handle each later */
            case EXCH_ITEM_BOTTLE_BLUE_FIRE:
            case EXCH_ITEM_BOTTLE_FISH:
            case EXCH_ITEM_BOTTLE_BUG:
            case EXCH_CUSTOM_ITEM_MAGIC_MUSHROOM:
            case EXCH_CUSTOM_ITEM_CHATEAU:
            /* case EXCH_CUSTOM_ITEM_GOLD_DUST: */
            case EXCH_CUSTOM_ITEM_SPRING_WATER:
            case EXCH_CUSTOM_ITEM_SPRING_WATER_HOT:
            case EXCH_CUSTOM_ITEM_ZORA_EGG:
                this->messageId = 0x70F0;
                sExchangingItemId = exchangingItemId;
                break;

            default:
                sExchangingItemId = -1;
                if (Player_GetMask(play) == 0) /* PLAYER_MASK_NONE */
                {
                    this->messageId = 0x700C;
                }
                break;
            }
        }

        player->actor.messageId = this->messageId;
    }
}

PATCH_FUNC(0x80ae6178, EnHy_OfferBuyBottledItem)

void EnHy_ExchangeItemForRupees()
{
    s16 rupees;
    switch (sExchangingItemId)
    {
        case EXCH_ITEM_BOTTLE_BLUE_FIRE:
            rupees = 150;
            break;
        case EXCH_ITEM_BOTTLE_FISH:
            rupees = 100;
            break;
        case EXCH_ITEM_BOTTLE_BUG:
            rupees = 50;
            break;
        case EXCH_CUSTOM_ITEM_MAGIC_MUSHROOM:
            rupees = 5;
            break;
        /* case EXCH_CUSTOM_ITEM_GOLD_DUST:  */
        case EXCH_CUSTOM_ITEM_CHATEAU:
            rupees = 200;
            break;

        case EXCH_CUSTOM_ITEM_SPRING_WATER:
        case EXCH_CUSTOM_ITEM_SPRING_WATER_HOT:
        case EXCH_CUSTOM_ITEM_ZORA_EGG:
            rupees = 20;
            break;
        default:
            rupees = 0;
            break;
    }

    if (rupees)
    {
        AddRupees(rupees);
    }
}

PATCH_CALL(0x80ae5c1c, EnHy_ExchangeItemForRupees)
