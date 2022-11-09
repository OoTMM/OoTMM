#ifndef COMBO_SHADER_H
#define COMBO_SHADER_H

#include <combo/game_state.h>
#include <combo/types.h>

typedef void (*ShaderFunc)(GameState* gs, s16 index);

typedef struct PACKED ALIGNED(4)
{
    ShaderFunc  func;
    u32         lists[8];
}
Shader;

#define DRAWGI_OOT_EMPTY_BOTTLE             0x01
#define DRAWGI_OOT_SMALL_KEY                0x02
#define DRAWGI_OOT_NOTE_GREEN               0x03
#define DRAWGI_OOT_NOTE_RED                 0x04
#define DRAWGI_OOT_NOTE_BLUE                0x05
#define DRAWGI_OOT_NOTE_ORANGE              0x06
#define DRAWGI_OOT_NOTE_PURPLE              0x07
#define DRAWGI_OOT_NOTE_YELLOW              0x08
#define DRAWGI_OOT_RECOVERY_HEART           0x09
#define DRAWGI_OOT_BOSS_KEY                 0x0a
#define DRAWGI_OOT_COMPASS                  0x0b
#define DRAWGI_OOT_MEDALLION_FOREST         0x0c
#define DRAWGI_OOT_MEDALLION_FIRE           0x0d
#define DRAWGI_OOT_MEDALLION_WATER          0x0e
#define DRAWGI_OOT_MEDALLION_SPIRIT         0x0f
#define DRAWGI_OOT_MEDALLION_SHADOW         0x10
#define DRAWGI_OOT_MEDALLION_LIGHT          0x11
#define DRAWGI_OOT_DEKU_NUT                 0x12
#define DRAWGI_OOT_HEART_CONTAINER          0x13
#define DRAWGI_OOT_HEART_PIECE              0x14
#define DRAWGI_OOT_QUIVER                   0x15
#define DRAWGI_OOT_QUIVER2                  0x16
#define DRAWGI_OOT_QUIVER3                  0x17
#define DRAWGI_OOT_BOMB_BAG                 0x18
#define DRAWGI_OOT_BOMB_BAG2                0x19
#define DRAWGI_OOT_BOMB_BAG3                0x1a
#define DRAWGI_OOT_DEKU_STICK               0x1b
#define DRAWGI_OOT_MAP                      0x1c
#define DRAWGI_OOT_SHIELD_DEKU              0x1d
#define DRAWGI_OOT_MAGIC_SMALL              0x1e
#define DRAWGI_OOT_MAGIC_LARGE              0x1f
#define DRAWGI_OOT_BOMB                     0x20
#define DRAWGI_OOT_STONE_OF_AGONY           0x21
#define DRAWGI_OOT_WALLET2                  0x22
#define DRAWGI_OOT_WALLET3                  0x23
#define DRAWGI_OOT_GERUDO_CARD              0x24
#define DRAWGI_OOT_ARROW                    0x25
#define DRAWGI_OOT_ARROW2                   0x26
#define DRAWGI_OOT_ARROW3                   0x27
#define DRAWGI_OOT_BOMBCHU                  0x28
#define DRAWGI_OOT_EGG                      0x29
#define DRAWGI_OOT_SCALE_SILVER             0x2a
#define DRAWGI_OOT_SCALE_GOLD               0x2b
#define DRAWGI_OOT_SHIELD_HYLIAN            0x2c
#define DRAWGI_OOT_HOOKSHOT                 0x2d
#define DRAWGI_OOT_LONGSHOT                 0x2e
#define DRAWGI_OOT_OCARINA_TIME             0x2f
#define DRAWGI_OOT_MILK                     0x30
#define DRAWGI_OOT_MASK_KEATON              0x31
#define DRAWGI_OOT_MASK_SPOOKY              0x32
#define DRAWGI_OOT_SLINGSHOT                0x33
#define DRAWGI_OOT_BOOMERANG                0x34
#define DRAWGI_OOT_BOW                      0x35
#define DRAWGI_OOT_LENS                     0x36
#define DRAWGI_OOT_POTION_GREEN             0x37
#define DRAWGI_OOT_POTION_RED               0x38
#define DRAWGI_OOT_POTION_BLUE              0x39
#define DRAWGI_OOT_SHIELD_MIRROR            0x3a
#define DRAWGI_OOT_ZELDA_LETTER             0x3b
#define DRAWGI_OOT_TUNIC_GORON              0x3c
#define DRAWGI_OOT_TUNIC_ZORA               0x3d
#define DRAWGI_OOT_MAGIC_BEAN               0x3e
#define DRAWGI_OOT_FISH                     0x3f
#define DRAWGI_OOT_POACHER_SAW              0x40
#define DRAWGI_OOT_MEGATON_HAMMER           0x41
#define DRAWGI_OOT_BETA_WHISTLE             0x42
#define DRAWGI_OOT_BIGGORON_SWORD           0x43
#define DRAWGI_OOT_CHICKEN                  0x44
#define DRAWGI_OOT_RUTO_LETTER              0x45
#define DRAWGI_OOT_OCARINA_FAIRY            0x46
#define DRAWGI_OOT_BOOTS_IRON               0x47
#define DRAWGI_OOT_SEEDS                    0x48
#define DRAWGI_OOT_GAUNTLETS_SILVER         0x49
#define DRAWGI_OOT_GAUNTLETS_GOLD           0x4a
#define DRAWGI_OOT_BETA_COIN_YELLOW         0x4b
#define DRAWGI_OOT_BETA_COIN_RED            0x4c
#define DRAWGI_OOT_BETA_COIN_GREEN          0x4d
#define DRAWGI_OOT_BETA_COIN_BLUE           0x4e
#define DRAWGI_OOT_MASK_SKULL               0x4f
#define DRAWGI_OOT_MASK_BUNNY               0x50
#define DRAWGI_OOT_MASK_TRUTH               0x51
#define DRAWGI_OOT_EYE_DROPS                0x52
#define DRAWGI_OOT_ODD_POTION               0x53
#define DRAWGI_OOT_ODD_MUSHROOM             0x54
#define DRAWGI_OOT_CLAIM_CHECK              0x55
#define DRAWGI_OOT_BROKEN_GORON_SWORD       0x56
#define DRAWGI_OOT_PRESCRIPTION             0x57
#define DRAWGI_OOT_GORON_BRACELET           0x58
#define DRAWGI_OOT_SOLD_OUT                 0x59
#define DRAWGI_OOT_EYEBALL_FROG             0x5a
#define DRAWGI_OOT_MASK_GORON               0x5b
#define DRAWGI_OOT_MASK_ZORA                0x5c
#define DRAWGI_OOT_MASK_GERUDO              0x5d
#define DRAWGI_OOT_COJIRO                   0x5e
#define DRAWGI_OOT_BOOTS_HOVER              0x5f
#define DRAWGI_OOT_ARROW_FIRE               0x60
#define DRAWGI_OOT_ARROW_ICE                0x61
#define DRAWGI_OOT_ARROW_LIGHT              0x62
#define DRAWGI_OOT_GS_TOKEN                 0x63
#define DRAWGI_OOT_SPELL_FIRE               0x64
#define DRAWGI_OOT_SPELL_WIND               0x65
#define DRAWGI_OOT_SPELL_LOVE               0x66
#define DRAWGI_OOT_BLUE_FIRE                0x67
#define DRAWGI_OOT_BUG                      0x68
#define DRAWGI_OOT_BUTTERFLY                0x69
#define DRAWGI_OOT_POE                      0x6a
#define DRAWGI_OOT_FAIRY                    0x6b
#define DRAWGI_OOT_BULLET_BAG2              0x6c
#define DRAWGI_OOT_RUPEE_GREEN              0x6d
#define DRAWGI_OOT_RUPEE_BLUE               0x6e
#define DRAWGI_OOT_RUPEE_RED                0x6f
#define DRAWGI_OOT_BIG_POE                  0x70
#define DRAWGI_OOT_RUPEE_PURPLE             0x71
#define DRAWGI_OOT_RUPEE_HUGE               0x72
#define DRAWGI_OOT_BULLET_BAG3              0x73
#define DRAWGI_OOT_SWORD_KOKIRI             0x74
#define DRAWGI_OOT_GS_TOKEN2                0x75
#define DRAWGI_OOT_SWORD_MASTER             0x76
#define DRAWGI_OOT_NOTE_GRAY                0x77
#define DRAWGI_OOT_STONE_EMERALD            0x78
#define DRAWGI_OOT_STONE_RUBY               0x79
#define DRAWGI_OOT_STONE_SAPPHIRE           0x7a
#define DRAWGI_OOT_DEFENSE_UPGRADE          0x7b

extern const Shader kShaders[256];

#endif
