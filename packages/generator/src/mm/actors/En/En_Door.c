#include <combo.h>

int EnDoor_GetID(PlayState* play, int id)
{
    switch (play->sceneId)
    {
    case SCE_MM_SOUTHERN_SWAMP:
    case SCE_MM_SOUTHERN_SWAMP_CLEAR:
        switch (id)
        {
        case 3: return DOORID_MM_TOURIST_INFORMATION;
        case 4: return DOORID_MM_POTION_SHOP;
        }
        break;
    case SCE_MM_TOURIST_INFORMATION:
        return DOORID_MM_TOURIST_INFORMATION;
    case SCE_MM_POTION_SHOP:
        return DOORID_MM_POTION_SHOP;
    case SCE_MM_CLOCK_TOWN_WEST:
        switch (id)
        {
        case 0: return DOORID_MM_POST_OFFICE;
        case 1: return DOORID_MM_SWORDSMAN_SCHOOL;
        case 2: return DOORID_MM_LOTTERY;
        case 3: return DOORID_MM_BOMB_SHOP;
        case 4: return DOORID_MM_TRADING_POST;
        case 5: return DOORID_MM_CURIOSITY_SHOP;
        }
        break;
    case SCE_MM_BOMB_SHOP:
        return DOORID_MM_BOMB_SHOP;
    case SCE_MM_POST_OFFICE:
        return DOORID_MM_POST_OFFICE;
    case SCE_MM_LOTTERY:
        return DOORID_MM_LOTTERY;
    case SCE_MM_SWORDSMAN_SCHOOL:
        return DOORID_MM_SWORDSMAN_SCHOOL;
    case SCE_MM_TRADING_POST:
        return DOORID_MM_TRADING_POST;
    case SCE_MM_CURIOSITY_SHOP:
        switch (id)
        {
        case 0: return DOORID_MM_CURIOSITY_SHOP;
        case 1: return DOORID_MM_KAFEI_HIDEOUT;
        }
        break;
    case SCE_MM_LAUNDRY_POOL:
        return DOORID_MM_KAFEI_HIDEOUT;
    case SCE_MM_CLOCK_TOWN_SOUTH:
    case SCE_MM_SHOOTING_GALLERY:
        return DOORID_MM_TOWN_ARCHERY;
    case SCE_MM_ROAD_SOUTHERN_SWAMP:
    case SCE_MM_SHOOTING_GALLERY_SWAMP:
        return DOORID_MM_SWAMP_ARCHERY;
    case SCE_MM_OBSERVATORY:
    case SCE_MM_TERMINA_FIELD:
        return DOORID_MM_OBSERVATORY;
    case SCE_MM_MOUNTAIN_VILLAGE_SPRING:
    case SCE_MM_MOUNTAIN_VILLAGE_WINTER:
    case SCE_MM_BLACKSMITH:
        return DOORID_MM_BLACKSMITH;
    case SCE_MM_IKANA_CANYON:
    case SCE_MM_MUSIC_BOX_HOUSE:
        return DOORID_MM_MUSIC_HOUSE;
    case SCE_MM_GREAT_BAY_COAST:
    case SCE_MM_LABORATORY:
        return DOORID_MM_LABORATORY;
    case SCE_MM_IKANA_GRAVEYARD:
    case SCE_MM_DAMPE_HOUSE:
        return DOORID_MM_DAMPE_HOUSE;
    case SCE_MM_BENEATH_THE_GRAVEYARD:
        return DOORID_MM_BENEATH_GRAVEYARD;
    case SCE_MM_MAYOR_HOUSE:
        switch (id)
        {
        case 0: return DOORID_MM_MAYOR_RESIDENCE_OFFICE;
        case 1: return DOORID_MM_MAYOR_RESIDENCE_SALON;
        case 2: return DOORID_MM_MAYOR_RESIDENCE_KAFEI;
        case 3: return DOORID_MM_MAYOR_RESIDENCE;
        }
        break;
    case SCE_MM_MILK_BAR:
        return DOORID_MM_MILK_BAR;
    case SCE_MM_HONEY_DARLING:
        return DOORID_MM_HONEY_DARLING;
    case SCE_MM_TREASURE_SHOP:
        return DOORID_MM_TREASURE_CHEST_GAME;
    case SCE_MM_DOG_RACETRACK:
        return DOORID_MM_DOG_RACETRACK;
    case SCE_MM_CUCCO_SHACK:
        return DOORID_MM_CUCCO_SHACK;
    case SCE_MM_ROMANI_RANCH:
        switch (id)
        {
        case 0: return DOORID_MM_RANCH_HOUSE;
        case 1: return DOORID_MM_RANCH_BARN;
        case 2: return DOORID_MM_DOG_RACETRACK;
        case 3: return DOORID_MM_CUCCO_SHACK;
        }
        break;
    case SCE_MM_RANCH_HOUSE_BARN:
        switch (id)
        {
        case 0: return DOORID_MM_RANCH_BARN;
        case 1: return DOORID_MM_RANCH_HOUSE;
        case 2: return DOORID_MM_RANCH_HOUSE_ROOM;
        }
        break;
    case SCE_MM_ZORA_HALL_ROOMS:
        switch (id)
        {
        case 0: return DOORID_MM_ZORA_SHOP;
        case 1: return DOORID_MM_ZORA_JAPAS_ROOM;
        case 2: return DOORID_MM_ZORA_TIJO_ROOM;
        case 3: return DOORID_MM_ZORA_LULU_ROOM;
        case 4: return DOORID_MM_ZORA_EVAN_ROOM;
        }
        break;
    case SCE_MM_ZORA_HALL:
        switch (id)
        {
        case 0: return DOORID_MM_ZORA_LULU_ROOM;
        case 1: return DOORID_MM_ZORA_EVAN_ROOM;
        case 2: return DOORID_MM_ZORA_JAPAS_ROOM;
        case 3: return DOORID_MM_ZORA_TIJO_ROOM;
        case 4: return DOORID_MM_ZORA_SHOP;
        }
        break;
    case SCE_MM_STOCK_POT_INN:
        switch (id)
        {
        case 0: return DOORID_MM_GRANDMA_ROOM;
        case 1: return DOORID_MM_STOCK_POT_INN_STAFF_ROOM;
        case 3: return DOORID_MM_STOCK_POT_INN_DORMITORY;
        case 4: return DOORID_MM_STOCK_POT_INN_ROOF;
        case 5: return DOORID_MM_STOCK_POT_INN;
        }
        break;
    case SCE_MM_CLOCK_TOWN_EAST:
        switch (id)
        {
        case 0: return DOORID_MM_STOCK_POT_INN;
        case 1: return DOORID_MM_STOCK_POT_INN_ROOF;
        case 2: return DOORID_MM_TREASURE_CHEST_GAME;
        case 3: return DOORID_MM_TOWN_ARCHERY;
        case 4: return DOORID_MM_HONEY_DARLING;
        case 5: return DOORID_MM_MAYOR_RESIDENCE;
        case 6: return DOORID_MM_MILK_BAR;
        }
        break;
    }

    return -1;
}

int EnDoor_IsRustyLocked(PlayState* play, Actor* this)
{
    int id;

    id = EnDoor_GetID(play, (this->params >> 10));
    if (id < 0)
        return 0;
    return !BITMAP8_GET(gSharedCustomSave.rustyKeysMm, id);
}

void EnDoor_DrawLock(PlayState* play, Actor* this)
{
    u8 timer;
    s32 yaw;

    /* Fix rotation */
    yaw = Math_Vec3f_Yaw(&play->view.eye, &this->world.pos);
    if (ABS((s16)(this->shape.rot.y - yaw)) < 0x4000) {
        Matrix_RotateY(M_PI, MTXMODE_APPLY);
    }

    timer = *(u8*)((char*)this + 0x1a6);
    if (timer) {
        Actor_DrawDoorLock(play, timer, 0);
    } else if (EnDoor_IsRustyLocked(play, this)) {
        Door_DrawRustyLock(play);
    }
}
