#ifndef COMBO_MAGIC_H
#define COMBO_MAGIC_H

#if defined(GAME_OOT)
# define MAGIC_STATE_IDLE             0x0
# define MAGIC_STATE_CONSUME_SETUP    0x1
# define MAGIC_STATE_CONSUME          0x2
# define MAGIC_STATE_METER_FLASH_1    0x3
# define MAGIC_STATE_METER_FLASH_2    0x4
# define MAGIC_STATE_RESET            0x5
# define MAGIC_STATE_METER_FLASH_3    0x6
# define MAGIC_STATE_CONSUME_LENS     0x7
# define MAGIC_STATE_STEP_CAPACITY    0x8
# define MAGIC_STATE_FILL             0x9
# define MAGIC_STATE_ADD              0xa

# define MAGIC_CONSUME_NOW                  0x00
# define MAGIC_CONSUME_WAIT_NO_PREVIEW      0x01
# define MAGIC_CONSUME_NOW_ALT              0x02
# define MAGIC_CONSUME_LENS                 0x03
# define MAGIC_CONSUME_WAIT_PREVIEW         0x04
# define MAGIC_ADD                          0x05
#endif

#if defined(GAME_MM)
# define MAGIC_STATE_IDLE                           0x00
# define MAGIC_STATE_CONSUME_SETUP                  0x01
# define MAGIC_STATE_CONSUME                        0x02
# define MAGIC_STATE_METER_FLASH_1                  0x03
# define MAGIC_STATE_METER_FLASH_2                  0x04
# define MAGIC_STATE_RESET                          0x05
# define MAGIC_STATE_METER_FLASH_3                  0x06
# define MAGIC_STATE_CONSUME_LENS                   0x07
# define MAGIC_STATE_STEP_CAPACITY                  0x08
# define MAGIC_STATE_FILL                           0x09
# define MAGIC_STATE_CONSUME_GORON_ZORA_SETUP       0x0a
# define MAGIC_STATE_CONSUME_GORON_ZORA             0x0b
# define MAGIC_STATE_CONSUME_GIANTS_MASK            0x0c

# define MAGIC_CONSUME_NOW              0x00
# define MAGIC_CONSUME_WAIT_NO_PREVIEW  0x01
# define MAGIC_CONSUME_NOW_ALT          0x02
# define MAGIC_CONSUME_LENS             0x03
# define MAGIC_CONSUME_WAIT_PREVIEW     0x04
# define MAGIC_CONSUME_GORON_ZORA       0x05
# define MAGIC_CONSUME_GIANTS_MASK      0x06
# define MAGIC_CONSUME_DEITY_BEAM       0x07
#endif

typedef struct GameState_Play GameState_Play;

void Magic_Reset(GameState_Play* play);
void Magic_Update(GameState_Play* play);
void Magic_Refill(GameState_Play* play);
int Magic_RequestChange(GameState_Play* play, s16 amount, s16 type);

#endif
