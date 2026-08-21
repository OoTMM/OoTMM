#include <combo.h>
#include <combo/sr.h>
#include <combo/dungeon.h>
#include <combo/player.h>
#include <combo/config.h>
#include <combo/global.h>
#include <combo/flags.h>

ComboSilverRupeeData gSilverRupeeData[SR_MAX] = {
    { SCE_OOT_DODONGO_CAVERN,           0x00, 0 }, /* SR_DC */
    { SCE_OOT_BOTTOM_OF_THE_WELL,       0x1f, 5 }, /* SR_BOTW */
    { SCE_OOT_TEMPLE_SPIRIT,            0x05, 5 }, /* SR_SPIRIT1 */
    { SCE_OOT_TEMPLE_SPIRIT,            0x0a, 5 }, /* SR_SPIRIT2 */
    { SCE_OOT_TEMPLE_SPIRIT,            0x02, 5 }, /* SR_SPIRIT3 */
    { SCE_OOT_TEMPLE_SHADOW,            0x01, 5 }, /* SR_SHADOW1 */
    { SCE_OOT_TEMPLE_SHADOW,            0x00, 0 }, /* SR_SHADOW2 */
    { SCE_OOT_TEMPLE_SHADOW,            0x09, 5 }, /* SR_SHADOW3 */
    { SCE_OOT_TEMPLE_SHADOW,            0x08, 5 }, /* SR_SHADOW4 */
    { SCE_OOT_ICE_CAVERN,               0x08, 5 }, /* SR_IC1 */
    { SCE_OOT_ICE_CAVERN,               0x09, 5 }, /* SR_IC2 */
    { SCE_OOT_GERUDO_TRAINING_GROUND,   0x1c, 5 }, /* SR_GTG1 */
    { SCE_OOT_GERUDO_TRAINING_GROUND,   0x0c, 5 }, /* SR_GTG2 */
    { SCE_OOT_GERUDO_TRAINING_GROUND,   0x1b, 5 }, /* SR_GTG3 */
    { SCE_OOT_INSIDE_GANON_CASTLE,      0x0b, 5 }, /* SR_GANON1 */
    { SCE_OOT_INSIDE_GANON_CASTLE,      0x12, 5 }, /* SR_GANON2 */
    { SCE_OOT_INSIDE_GANON_CASTLE,      0x09, 5 }, /* SR_GANON3 */
    { SCE_OOT_INSIDE_GANON_CASTLE,      0x0e, 5 }, /* SR_GANON4 */
};

static void SilverRupees_SetData(int id, int count, int flag)
{
    gSilverRupeeData[id].count = count;
    gSilverRupeeData[id].flag = flag;
}

void SilverRupees_Init(void)
{
    if (Config_IsMq(MQ_DODONGOS_CAVERN))
    {
        SilverRupees_SetData(SR_DC, 5, 0x25);
    }

    if (Config_IsMq(MQ_BOTTOM_OF_THE_WELL))
    {
        SilverRupees_SetData(SR_BOTW, 0, 0);
    }

    if (Config_IsMq(MQ_TEMPLE_SPIRIT))
    {
        SilverRupees_SetData(SR_SPIRIT1, 5, 0x37);
        SilverRupees_SetData(SR_SPIRIT2, 5, 0x00);
        SilverRupees_SetData(SR_SPIRIT3, 0, 0);
    }

    if (Config_IsMq(MQ_TEMPLE_SHADOW))
    {
        SilverRupees_SetData(SR_SHADOW2, 10, 0x03);
        SilverRupees_SetData(SR_SHADOW3,  5, 0x11);
        SilverRupees_SetData(SR_SHADOW4, 10, 0x08);
    }

    if (Config_IsMq(MQ_ICE_CAVERN))
    {
        SilverRupees_SetData(SR_IC1, 0, 0);
        SilverRupees_SetData(SR_IC2, 0, 0);
    }

    if (Config_IsMq(MQ_GERUDO_TRAINING_GROUNDS))
    {
        SilverRupees_SetData(SR_GTG1, 5, 0x1c);
        SilverRupees_SetData(SR_GTG2, 6, 0x0c);
        SilverRupees_SetData(SR_GTG3, 3, 0x1b);
    }

    if (Config_IsMq(MQ_GANON_CASTLE))
    {
        SilverRupees_SetData(SR_GANON1, 5, 0x0b);
        SilverRupees_SetData(SR_GANON2, 5, 0x02);
        SilverRupees_SetData(SR_GANON3, 5, 0x01);
        SilverRupees_SetData(SR_GANON4, 0, 0);
    }
}

int SilverRupees_GetCount(int id)
{
    return ((gSharedCustomSave.silverRupees[id >> 1]) >> ((id & 1) * 4)) & 0xf;
}

static void SilverRupees_SetCount(int id, int count)
{
    u8* p;

    p = &gSharedCustomSave.silverRupees[id >> 1];
    *p &= ~(0xf << ((id & 1) * 4));
    *p |= (count << ((id & 1) * 4));
}

int SilverRupees_IncCount(PlayState* play, int id)
{
    const ComboSilverRupeeData* data;
    int tmp;

    data = &gSilverRupeeData[id];
    tmp = SilverRupees_GetCount(id) + 1;
    if (tmp > data->count)
        tmp = data->count;
    SilverRupees_SetCount(id, tmp);

    if (data->count && (tmp == data->count) && data->flag < 0x20
#if defined(GAME_OOT)
        && !(play && play->sceneId == data->sceneId)
#endif
    )
    {
        gOotSave.info.perm[data->sceneId].switches |= (1 << data->flag);
    }

    return tmp;
}

#if defined(GAME_OOT)
void SilverRupees_Update(PlayState* play)
{
    const ComboSilverRupeeData* data;
    Player* link;

    /* Sanity check */
    if (gSaveContext.gameMode != GAMEMODE_NORMAL)
        return;
    link = GET_PLAYER(play);
    if ((Message_GetState(&play->msgCtx) != TEXT_STATE_NONE) || (link->stateFlags1 & (PLAYER_ACTOR_STATE_CLIMB | PLAYER_ACTOR_STATE_CLIMB2)))
        return;

    /* Trigger obtained silver rupees for the current scene */
    for (int sr = 0; sr < SR_MAX; ++sr)
    {
        data = &gSilverRupeeData[sr];
        if (!data->count)
            continue;
        if (data->sceneId != play->sceneId)
            continue;
        if (SilverRupees_GetCount(sr) < data->count)
            continue;
        Flags_SetSwitch(play, data->flag);
    }
}
#endif
