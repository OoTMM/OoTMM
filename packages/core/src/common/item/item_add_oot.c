#include <combo.h>
#include <combo/item.h>
#include <combo/sr.h>
#include <combo/souls.h>

void comboAddItemSharedOot(s16 gi, int noEffect)
{
    if (comboConfig(CFG_SHARED_SOULS_ENEMY))
    {
        switch (gi)
        {
        case GI_OOT_SOUL_ENEMY_OCTOROK:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_OCTOROK);
            break;
        case GI_OOT_SOUL_ENEMY_WALLMASTER:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_WALLMASTER);
            break;
        case GI_OOT_SOUL_ENEMY_DODONGO:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_DODONGO);
            break;
        case GI_OOT_SOUL_ENEMY_KEESE:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_KEESE);
            break;
        case GI_OOT_SOUL_ENEMY_TEKTITE:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_TEKTITE);
            break;
        case GI_OOT_SOUL_ENEMY_PEAHAT:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_PEAHAT);
            break;
        case GI_OOT_SOUL_ENEMY_LIZALFOS_DINALFOS:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_LIZALFOS_DINALFOS);
            break;
        case GI_OOT_SOUL_ENEMY_SKULLTULA:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_SKULLTULA);
            break;
        case GI_OOT_SOUL_ENEMY_ARMOS:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_ARMOS);
            break;
        case GI_OOT_SOUL_ENEMY_DEKU_BABA:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_DEKU_BABA);
            break;
        case GI_OOT_SOUL_ENEMY_DEKU_SCRUB:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_DEKU_SCRUB);
            break;
        case GI_OOT_SOUL_ENEMY_BUBBLE:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_BUBBLE);
            break;
        case GI_OOT_SOUL_ENEMY_BEAMOS:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_BEAMOS);
            break;
        case GI_OOT_SOUL_ENEMY_REDEAD_GIBDO:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_REDEAD_GIBDO);
            break;
        case GI_OOT_SOUL_ENEMY_SKULLWALLTULA:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_SKULLWALLTULA);
            break;
        case GI_OOT_SOUL_ENEMY_SHELL_BLADE:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_SHELL_BLADE);
            break;
        case GI_OOT_SOUL_ENEMY_LIKE_LIKE:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_LIKE_LIKE);
            break;
        case GI_OOT_SOUL_ENEMY_IRON_KNUCKLE:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_IRON_KNUCKLE);
            break;
        case GI_OOT_SOUL_ENEMY_FREEZARD:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_FREEZARD);
            break;
        case GI_OOT_SOUL_ENEMY_WOLFOS:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_WOLFOS);
            break;
        case GI_OOT_SOUL_ENEMY_GUAY:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_GUAY);
            break;
        case GI_OOT_SOUL_ENEMY_FLYING_POT:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_FLYING_POT);
            break;
        case GI_OOT_SOUL_ENEMY_FLOORMASTER:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_FLOORMASTER);
            break;
        case GI_OOT_SOUL_ENEMY_LEEVER:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_LEEVER);
            break;
        case GI_OOT_SOUL_ENEMY_STALCHILD:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_STALCHILD);
            break;
        }
    }

    if (comboConfig(CFG_SHARED_OCARINA_BUTTONS))
    {
        switch (gi)
        {
        case GI_OOT_BUTTON_A:
            gSharedCustomSave.ocarinaButtonMaskMm |= A_BUTTON;
            break;
        case GI_OOT_BUTTON_C_RIGHT:
            gSharedCustomSave.ocarinaButtonMaskMm |= R_CBUTTONS;
            break;
        case GI_OOT_BUTTON_C_LEFT:
            gSharedCustomSave.ocarinaButtonMaskMm |= L_CBUTTONS;
            break;
        case GI_OOT_BUTTON_C_UP:
            gSharedCustomSave.ocarinaButtonMaskMm |= U_CBUTTONS;
            break;
        case GI_OOT_BUTTON_C_DOWN:
            gSharedCustomSave.ocarinaButtonMaskMm |= D_CBUTTONS;
            break;
        }
    }
}

int comboAddItemOot(s16 gi, int noEffect)
{
    int count;
    u16 dungeonId;

    count = 0;
    (void)dungeonId;
    switch (gi)
    {
    case GI_OOT_TRIFORCE:
    case GI_OOT_TRIFORCE_POWER:
    case GI_OOT_TRIFORCE_COURAGE:
    case GI_OOT_TRIFORCE_WISDOM:
        count = ++gTriforceCount;
        break;
    case GI_OOT_COIN_RED:
        count = ++gSharedCustomSave.coins[0];
        break;
    case GI_OOT_COIN_GREEN:
        count = ++gSharedCustomSave.coins[1];
        break;
    case GI_OOT_COIN_BLUE:
        count = ++gSharedCustomSave.coins[2];
        break;
    case GI_OOT_COIN_YELLOW:
        count = ++gSharedCustomSave.coins[3];
        break;
    case GI_OOT_SOUL_ENEMY_STALFOS:
    case GI_OOT_SOUL_ENEMY_OCTOROK:
    case GI_OOT_SOUL_ENEMY_WALLMASTER:
    case GI_OOT_SOUL_ENEMY_DODONGO:
    case GI_OOT_SOUL_ENEMY_KEESE:
    case GI_OOT_SOUL_ENEMY_TEKTITE:
    case GI_OOT_SOUL_ENEMY_LEEVER:
    case GI_OOT_SOUL_ENEMY_PEAHAT:
    case GI_OOT_SOUL_ENEMY_LIZALFOS_DINALFOS:
    case GI_OOT_SOUL_ENEMY_GOHMA_LARVA:
    case GI_OOT_SOUL_ENEMY_SHABOM:
    case GI_OOT_SOUL_ENEMY_BABY_DODONGO:
    case GI_OOT_SOUL_ENEMY_DARK_LINK:
    case GI_OOT_SOUL_ENEMY_BIRI_BARI:
    case GI_OOT_SOUL_ENEMY_TAILPASARN:
    case GI_OOT_SOUL_ENEMY_PARASITE:
    case GI_OOT_SOUL_ENEMY_SKULLTULA:
    case GI_OOT_SOUL_ENEMY_TORCH_SLUG:
    case GI_OOT_SOUL_ENEMY_MOBLIN:
    case GI_OOT_SOUL_ENEMY_ARMOS:
    case GI_OOT_SOUL_ENEMY_DEKU_BABA:
    case GI_OOT_SOUL_ENEMY_DEKU_SCRUB:
    case GI_OOT_SOUL_ENEMY_BUBBLE:
    case GI_OOT_SOUL_ENEMY_BEAMOS:
    case GI_OOT_SOUL_ENEMY_FLOORMASTER:
    case GI_OOT_SOUL_ENEMY_REDEAD_GIBDO:
    case GI_OOT_SOUL_ENEMY_SKULLWALLTULA:
    case GI_OOT_SOUL_ENEMY_FLARE_DANCER:
    case GI_OOT_SOUL_ENEMY_DEAD_HAND:
    case GI_OOT_SOUL_ENEMY_SHELL_BLADE:
    case GI_OOT_SOUL_ENEMY_LIKE_LIKE:
    case GI_OOT_SOUL_ENEMY_SPIKE:
    case GI_OOT_SOUL_ENEMY_ANUBIS:
    case GI_OOT_SOUL_ENEMY_IRON_KNUCKLE:
    case GI_OOT_SOUL_ENEMY_SKULL_KID:
    case GI_OOT_SOUL_ENEMY_FLYING_POT:
    case GI_OOT_SOUL_ENEMY_FREEZARD:
    case GI_OOT_SOUL_ENEMY_STINGER:
    case GI_OOT_SOUL_ENEMY_WOLFOS:
    case GI_OOT_SOUL_ENEMY_STALCHILD:
    case GI_OOT_SOUL_ENEMY_GUAY:
    case GI_OOT_SOUL_BOSS_QUEEN_GOHMA:
    case GI_OOT_SOUL_BOSS_KING_DODONGO:
    case GI_OOT_SOUL_BOSS_BARINADE:
    case GI_OOT_SOUL_BOSS_PHANTOM_GANON:
    case GI_OOT_SOUL_BOSS_VOLVAGIA:
    case GI_OOT_SOUL_BOSS_MORPHA:
    case GI_OOT_SOUL_BOSS_BONGO_BONGO:
    case GI_OOT_SOUL_BOSS_TWINROVA:
    case GI_OOT_SOUL_NPC_SARIA:
    case GI_OOT_SOUL_NPC_DARUNIA:
    case GI_OOT_SOUL_NPC_RUTO:
    case GI_OOT_SOUL_NPC_KING_ZORA:
    case GI_OOT_SOUL_NPC_ZELDA:
    case GI_OOT_SOUL_NPC_SHEIK:
    case GI_OOT_SOUL_NPC_CUCCO_LADY:
    case GI_OOT_SOUL_NPC_CARPENTERS:
    case GI_OOT_SOUL_NPC_GURU_GURU:
    case GI_OOT_SOUL_NPC_MIDO:
    case GI_OOT_SOUL_NPC_KOKIRI:
    case GI_OOT_SOUL_NPC_KOKIRI_SHOPKEEPER:
    case GI_OOT_SOUL_NPC_HYLIAN_GUARD:
    case GI_OOT_SOUL_NPC_HYLIAN_CITIZEN:
    case GI_OOT_SOUL_NPC_MALON:
    case GI_OOT_SOUL_NPC_TALON:
    case GI_OOT_SOUL_NPC_FISHING_POND_OWNER:
    case GI_OOT_SOUL_NPC_GORON:
    case GI_OOT_SOUL_NPC_MEDIGORON:
    case GI_OOT_SOUL_NPC_BIGGORON:
    case GI_OOT_SOUL_NPC_GORON_CHILD:
    case GI_OOT_SOUL_NPC_GORON_SHOPKEEPER:
    case GI_OOT_SOUL_NPC_ZORA:
    case GI_OOT_SOUL_NPC_ZORA_SHOPKEEPER:
    case GI_OOT_SOUL_NPC_BAZAAR_SHOPKEEPER:
    case GI_OOT_SOUL_NPC_BOMBCHU_SHOPKEEPER:
    case GI_OOT_SOUL_NPC_POTION_SHOPKEEPER:
    case GI_OOT_SOUL_NPC_BOMBCHU_BOWLING_LADY:
    case GI_OOT_SOUL_NPC_TRASURE_CHEST_GAME_OWNER:
    case GI_OOT_SOUL_NPC_SHOOTING_GALLERY_OWNER:
    case GI_OOT_SOUL_NPC_DAMPE:
    case GI_OOT_SOUL_NPC_GRAVEYARD_KID:
    case GI_OOT_SOUL_NPC_POE_COLLECTOR:
        comboAddSoulOot(gi);
        break;
    case GI_OOT_BUTTON_A:
        gSharedCustomSave.ocarinaButtonMaskOot |= A_BUTTON;
        break;
    case GI_OOT_BUTTON_C_RIGHT:
        gSharedCustomSave.ocarinaButtonMaskOot |= R_CBUTTONS;
        break;
    case GI_OOT_BUTTON_C_LEFT:
        gSharedCustomSave.ocarinaButtonMaskOot |= L_CBUTTONS;
        break;
    case GI_OOT_BUTTON_C_UP:
        gSharedCustomSave.ocarinaButtonMaskOot |= U_CBUTTONS;
        break;
    case GI_OOT_BUTTON_C_DOWN:
        gSharedCustomSave.ocarinaButtonMaskOot |= D_CBUTTONS;
        break;
    case GI_OOT_FISHING_POND_CHILD_FISH_2LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_3LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_4LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_5LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_6LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_7LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_8LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_9LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_10LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_11LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_12LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_13LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_14LBS:
    {
        u8 caughtListLength = gSharedCustomSave.caughtChildFishWeight[0];
        if (caughtListLength < ARRAY_SIZE(gSharedCustomSave.caughtChildFishWeight) - 1) {
            s16 pounds = gi - GI_OOT_FISHING_POND_CHILD_FISH_2LBS + 2;
            gSharedCustomSave.caughtChildFishWeight[++gSharedCustomSave.caughtChildFishWeight[0]] = pounds & 0x7F;
        }
    }
        break;
    case GI_OOT_FISHING_POND_CHILD_LOACH_14LBS:
    case GI_OOT_FISHING_POND_CHILD_LOACH_15LBS:
    case GI_OOT_FISHING_POND_CHILD_LOACH_16LBS:
    case GI_OOT_FISHING_POND_CHILD_LOACH_17LBS:
    case GI_OOT_FISHING_POND_CHILD_LOACH_18LBS:
    case GI_OOT_FISHING_POND_CHILD_LOACH_19LBS:
    {
        u8 caughtListLength = gSharedCustomSave.caughtChildFishWeight[0];
        if (caughtListLength < ARRAY_SIZE(gSharedCustomSave.caughtChildFishWeight) - 1) {
            s16 pounds = gi - GI_OOT_FISHING_POND_CHILD_LOACH_14LBS + 14;
            gSharedCustomSave.caughtChildFishWeight[++gSharedCustomSave.caughtChildFishWeight[0]] = (pounds & 0x7F) | 0x80;
        }
    }
        break;
    case GI_OOT_FISHING_POND_ADULT_FISH_4LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_5LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_6LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_7LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_8LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_9LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_10LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_11LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_12LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_13LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_14LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_15LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_16LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_17LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_18LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_19LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_20LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_21LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_22LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_23LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_24LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_25LBS:
    {
        u8 caughtListLength = gSharedCustomSave.caughtAdultFishWeight[0];
        if (caughtListLength < ARRAY_SIZE(gSharedCustomSave.caughtAdultFishWeight) - 1) {
            s16 pounds = gi - GI_OOT_FISHING_POND_ADULT_FISH_4LBS + 4;
            gSharedCustomSave.caughtAdultFishWeight[++gSharedCustomSave.caughtAdultFishWeight[0]] = pounds & 0x7F;
        }
    }
        break;
    case GI_OOT_FISHING_POND_ADULT_LOACH_29LBS:
    case GI_OOT_FISHING_POND_ADULT_LOACH_30LBS:
    case GI_OOT_FISHING_POND_ADULT_LOACH_31LBS:
    case GI_OOT_FISHING_POND_ADULT_LOACH_32LBS:
    case GI_OOT_FISHING_POND_ADULT_LOACH_33LBS:
    case GI_OOT_FISHING_POND_ADULT_LOACH_34LBS:
    case GI_OOT_FISHING_POND_ADULT_LOACH_35LBS:
    case GI_OOT_FISHING_POND_ADULT_LOACH_36LBS:
    {
        u8 caughtListLength = gSharedCustomSave.caughtAdultFishWeight[0];
        if (caughtListLength < ARRAY_SIZE(gSharedCustomSave.caughtAdultFishWeight) - 1) {
            s16 pounds = gi - GI_OOT_FISHING_POND_ADULT_LOACH_29LBS + 29;
            gSharedCustomSave.caughtAdultFishWeight[++gSharedCustomSave.caughtAdultFishWeight[0]] = (pounds & 0x7F) | 0x80;
        }
    }
        break;
    }

    return count;
}
