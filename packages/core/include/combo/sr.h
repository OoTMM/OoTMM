#ifndef INCLUDED_COMBO_SR_H
#define INCLUDED_COMBO_SR_H

#include <combo.h>

/* Silver rupees sets */
#define SR_DC           0x00
#define SR_BOTW         0x01
#define SR_SPIRIT1      0x02
#define SR_SPIRIT2      0x03
#define SR_SPIRIT3      0x04
#define SR_SHADOW1      0x05
#define SR_SHADOW2      0x06
#define SR_SHADOW3      0x07
#define SR_SHADOW4      0x08
#define SR_IC1          0x09
#define SR_IC2          0x0a
#define SR_GTG1         0x0b
#define SR_GTG2         0x0c
#define SR_GTG3         0x0d
#define SR_GANON1       0x0e
#define SR_GANON2       0x0f
#define SR_GANON3       0x10
#define SR_GANON4       0x11
#define SR_MAX          0x12

typedef struct
{
    u16 sceneId;
    u8  flag;
    u8  count;
}
ComboSilverRupeeData;

extern ComboSilverRupeeData gSilverRupeeData[SR_MAX];

void comboSilverRupeesInit(void);
int comboSilverRupeesGetCount(int id);
int comboSilverRupeesIncCount(GameState_Play* play, int id);

#endif
