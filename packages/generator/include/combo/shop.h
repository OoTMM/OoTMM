#ifndef COMBO_SHOP_H
#define COMBO_SHOP_H

#include <combo/types.h>

/* Shop */
#define SC_OK               0x00
#define SC_OK_NOCUTSCENE    0x01
#define SC_ERR_CANNOTBUY    0x02
#define SC_ERR_NORUPEES     0x04

typedef struct PlayState PlayState;
typedef struct Actor_EnGirlA Actor_EnGirlA;

u8   Shop_ItemSlot(PlayState* play, Actor_EnGirlA* girlA);
int  Shop_Precond(PlayState* play, Actor_EnGirlA* girlA);
void Shop_DisplayTextBox(PlayState* play, Actor_EnGirlA* girlA);
void Shop_DisplayTextBoxConfirm(PlayState* play, Actor_EnGirlA* girlA);
void Shop_AfterBuy(PlayState* play, Actor_EnGirlA* girlA);
void Shop_SetupItem(PlayState* play, Actor_EnGirlA* girlA);
void Shop_UpdateItem(PlayState* play, Actor_EnGirlA* girlA);

#endif
