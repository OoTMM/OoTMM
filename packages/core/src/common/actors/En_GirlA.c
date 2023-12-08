#include <combo.h>
#include <combo/item.h>

int shopReadFlag(int);

void EnGirlA_ItemQuery(ComboItemQuery* q, Actor_EnGirlA* girlA)
{
    bzero(q, sizeof(*q));

    q->ovType = OV_SHOP;
    q->id = girlA->shopId;
    q->ovFlags = OVF_PRECOND;
    q->gi = girlA->gi;
    q->giRenew = GI_MM_SOLD_OUT;

    if (shopReadFlag(girlA->shopId))
    {
        q->ovFlags |= OVF_RENEW;

#if defined(GAME_MM)
        switch (girlA->shopId)
        {
        case 0x02:
        case 0x03:
        case 0x04:
            bzero(q, sizeof(*q));
            q->ovType = OV_NONE;
            q->gi = GI_MM_SOLD_OUT;
            break;
        }
#endif
    }
}

void EnGirlA_ItemOverride(ComboItemOverride* o, Actor_EnGirlA* girlA)
{
    ComboItemQuery q;

    EnGirlA_ItemQuery(&q, girlA);
    comboItemOverride(o, &q);
}
