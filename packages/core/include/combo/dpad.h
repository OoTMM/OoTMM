#ifndef COMBO_DPAD_H
#define COMBO_DPAD_H

#define DPF_ITEMS      0x01
#define DPF_EQUIP      0x02

typedef struct PlayState PlayState;

void Dpad_Draw(PlayState* play);
void Dpad_Update(PlayState* play);
int  Dpad_Use(PlayState* play, int flags);

#endif
