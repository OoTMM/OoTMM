#ifndef COMBO_DPAD_H
#define COMBO_DPAD_H

#define DPF_ITEMS      0x01
#define DPF_EQUIP      0x02

typedef struct GameState_Play GameState_Play;

void Dpad_Draw(GameState_Play* play);
void Dpad_Update(GameState_Play* play);
int  Dpad_Use(GameState_Play* play, int flags);

#endif
