#ifndef COMBO_TRAPS_H
#define COMBO_TRAPS_H

#define TRAP_ICE    0x00
#define TRAP_FIRE   0x01
#define TRAP_SHOCK  0x02
#define TRAP_MAX    0x03

typedef struct PlayState PlayState;
void Play_UpdateTraps(PlayState* play);

#endif
