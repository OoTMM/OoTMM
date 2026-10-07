#ifndef COMBO_AGE_H
#define COMBO_AGE_H

#define AGE_ADULT 0
#define AGE_CHILD 1

typedef struct PlayState PlayState;

int Age_GetStarting(void);
void Age_SetOot(PlayState* play, int age);
void Age_SwapOot(PlayState* play);
void Age_SetMm(PlayState* play, int age);
void Age_SwapMm(PlayState* play);
void Age_SetRawOot(PlayState* play, int age);
void Age_SetRawMm(PlayState* play, int age);

typedef enum ComboAgeReq
{
    COMBO_AGE_REQ_NONE = 0,
    COMBO_AGE_REQ_CHILD = 1,
    COMBO_AGE_REQ_ADULT = 2,
} ComboAgeReq;

int comboCheckAgeReq(ComboAgeReq req);
ComboAgeReq comboGetItemAgeReqMm(u8 item);
int comboCheckItemAgeReqMm(u8 item);
ComboAgeReq comboGetItemAgeReqOot(u8 item);
int comboCheckItemAgeReqOot(u8 item);
// adding oot side now for future use
#endif
