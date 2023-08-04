#include <combo.h>
#include <combo/item.h>

static void EnSi_ItemQuery(ComboItemQuery* q, Actor* this)
{
    u16 key;

    /* Poor man's ctz */
    switch (this->variable & 0xff)
    {
    case 0x01: key = 0x00; break;
    case 0x02: key = 0x01; break;
    case 0x04: key = 0x02; break;
    case 0x08: key = 0x03; break;
    case 0x10: key = 0x04; break;
    case 0x20: key = 0x05; break;
    case 0x40: key = 0x06; break;
    case 0x80: key = 0x07; break;
    default:
        UNREACHABLE();
    }
    key += ((((this->variable + 0x100) >> 8) & 0x1f) * 8);

    bzero(q, sizeof(*q));
    q->ovType = OV_GS;
    q->id = key;
    q->gi = GI_OOT_GS_TOKEN;
}

static void EnSi_ItemOverride(ComboItemOverride* o, Actor* this)
{
    ComboItemQuery q;

    EnSi_ItemQuery(&q, this);
    comboItemOverride(o, &q);
}

void EnSi_Draw(Actor* this, GameState_Play* play)
{
    ComboItemOverride o;

    EnSi_ItemOverride(&o, this);
    comboDrawGI(play, this, o.gi, 0);
}

PATCH_CALL(0x80b4b3f8, EnSi_Draw);

void EnSi_GiveItem(GameState_Play* play, Actor* this)
{
    ComboItemQuery q;

    EnSi_ItemQuery(&q, this);
    PlayerDisplayTextBox(play, 0xb4, NULL);
    comboAddItemEx(play, &q, 1);
}

PATCH_CALL(0x80b4b190, EnSi_GiveItem);
PATCH_CALL(0x80b4b280, EnSi_GiveItem);
