#ifndef COMBO_MASK_H
#define COMBO_MASK_H

#include <combo/types.h>

#if defined(GAME_OOT)
# define MASK_NONE      0x00
# define MASK_KEATON    0x01
# define MASK_SKULL     0x02
# define MASK_SPOOKY    0x03
# define MASK_BUNNY     0x04
# define MASK_GORON     0x05
# define MASK_ZORA      0x06
# define MASK_GERUDO    0x07
# define MASK_TRUTH     0x08
# define MASK_BLAST     0x09
# define MASK_STONE     0x0a
#endif

#if defined(GAME_MM)
# define MASK_NONE               0x00
# define MASK_TRUTH              0x01
# define MASK_KAFEI              0x02
# define MASK_ALL_NIGHT          0x03
# define MASK_BUNNY              0x04
# define MASK_KEATON             0x05
# define MASK_GARO               0x06
# define MASK_ROMANI             0x07
# define MASK_TROUPE_LEADER      0x08
# define MASK_POSTMAN            0x09
# define MASK_COUPLE             0x0a
# define MASK_GREAT_FAIRY        0x0b
# define MASK_GIBDO              0x0c
# define MASK_DON_GERO           0x0d
# define MASK_KAMARO             0x0e
# define MASK_CAPTAIN            0x0f
# define MASK_STONE              0x10
# define MASK_BREMEN             0x11
# define MASK_BLAST              0x12
# define MASK_SCENTS             0x13
# define MASK_GIANT              0x14
# define MASK_FIERCE_DEITY       0x15
# define MASK_GORON              0x16
# define MASK_ZORA               0x17
# define MASK_DEKU               0x18
#endif

typedef struct GameState_Play GameState_Play;

#if defined(GAME_OOT)
# define MASK_REACTION_SET_CARPENTER_BOSS                 0x00
# define MASK_REACTION_SET_CARPENTER_1                    0x01
# define MASK_REACTION_SET_CARPENTER_2                    0x02
# define MASK_REACTION_SET_CARPENTER_3                    0x03
# define MASK_REACTION_SET_CARPENTER_4                    0x04
# define MASK_REACTION_SET_HYRULIAN_GUARD                 0x05
# define MASK_REACTION_SET_HEISHI4_1                      0x06
# define MASK_REACTION_SET_HEISHI4_2                      0x07
# define MASK_REACTION_SET_CUCCO_LADY                     0x08
# define MASK_REACTION_SET_CARPENTERS_SON                 0x09
# define MASK_REACTION_SET_KAKARIKO_ROOF_MAN              0x0a
# define MASK_REACTION_SET_WINDMILL_MAN                   0x0b
# define MASK_REACTION_SET_12                             0x0c
# define MASK_REACTION_SET_CURSED_SKULLTULA_MAN           0x0d
# define MASK_REACTION_SET_DAMPE                          0x0e
# define MASK_REACTION_SET_GRAVEYARD_KID                  0x0f
# define MASK_REACTION_SET_SARIA                          0x10
# define MASK_REACTION_SET_MIDO                           0x11
# define MASK_REACTION_SET_FADO                           0x12
# define MASK_REACTION_SET_KOKIRI_1                       0x13
# define MASK_REACTION_SET_KOKIRI_2                       0x14
# define MASK_REACTION_SET_SKULL_KID                      0x15
# define MASK_REACTION_SET_ZELDA                          0x16
# define MASK_REACTION_SET_MALON                          0x17
# define MASK_REACTION_SET_TALON                          0x18
# define MASK_REACTION_SET_INGO                           0x19
# define MASK_REACTION_SET_LAKESIDE_PROFESSOR             0x1a
# define MASK_REACTION_SET_MAGIC_BEAN_SALESMAN            0x1b
# define MASK_REACTION_SET_RUNNING_MAN                    0x1c
# define MASK_REACTION_SET_ZORA                           0x1d
# define MASK_REACTION_SET_KING_ZORA                      0x1e
# define MASK_REACTION_SET_RUTO                           0x1f
# define MASK_REACTION_SET_GORON                          0x20
# define MASK_REACTION_SET_DARUNIA                        0x21
# define MASK_REACTION_SET_GERUDO_WHITE                   0x22
# define MASK_REACTION_SET_NABOORU                        0x23
# define MASK_REACTION_SET_DANCING_COUPLE                 0x24
# define MASK_REACTION_SET_37                             0x25
# define MASK_REACTION_SET_38                             0x26
# define MASK_REACTION_SET_39                             0x27
# define MASK_REACTION_SET_40                             0x28
# define MASK_REACTION_SET_41                             0x29
# define MASK_REACTION_SET_42                             0x2a
# define MASK_REACTION_SET_43                             0x2b
# define MASK_REACTION_SET_44                             0x2c
# define MASK_REACTION_SET_45                             0x2d
# define MASK_REACTION_SET_46                             0x2e
# define MASK_REACTION_SET_47                             0x2f
# define MASK_REACTION_SET_48                             0x30
# define MASK_REACTION_SET_49                             0x31
# define MASK_REACTION_SET_50                             0x32
# define MASK_REACTION_SET_51                             0x33
# define MASK_REACTION_SET_52                             0x34
# define MASK_REACTION_SET_53                             0x35
# define MASK_REACTION_SET_54                             0x36
# define MASK_REACTION_SET_55                             0x37
# define MASK_REACTION_SET_56                             0x38
# define MASK_REACTION_SET_57                             0x39
# define MASK_REACTION_SET_HAGGLING_TOWNSPEOPLE_1         0x3a
# define MASK_REACTION_SET_HAGGLING_TOWNSPEOPLE_2         0x3b

u16 MaskReaction_GetTextId(GameState_Play* play, int slot);
#endif


#endif
