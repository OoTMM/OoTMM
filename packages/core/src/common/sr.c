#include <combo.h>
#include <combo/sr.h>
#include <combo/dungeon.h>

static u32* const kSilverRupeeCounts[] = {
    &gOotSilverRupeeCounts1,
    &gOotSilverRupeeCounts2,
    &gOotSilverRupeeCounts3,
    &gOotSilverRupeeCounts4,
    &gOotSilverRupeeCounts5,
};

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

static void updateSR(int id, int count, int flag)
{
    gSilverRupeeData[id].count = count;
    gSilverRupeeData[id].flag = flag;
}

void comboSilverRupeesInit(void)
{
    if (gComboData.mq & (1 << MQ_DODONGOS_CAVERN))
    {
        updateSR(SR_DC, 5, 0x25);
    }

    if (gComboData.mq & (1 << MQ_BOTTOM_OF_THE_WELL))
    {
        updateSR(SR_BOTW, 0, 0);
    }

    if (gComboData.mq & (1 << MQ_TEMPLE_SPIRIT))
    {
        /* Need logic */
        updateSR(SR_SPIRIT1, 5, 0x37);
        updateSR(SR_SPIRIT2, 5, 0x00);
        updateSR(SR_SPIRIT3, 0, 0);
    }

    if (gComboData.mq & (1 << MQ_TEMPLE_SHADOW))
    {
        updateSR(SR_SHADOW2, 10, 0x03);
        updateSR(SR_SHADOW3,  5, 0x11); /* Double check */
        updateSR(SR_SHADOW4, 10, 0x08);
    }

    if (gComboData.mq & (1 << MQ_ICE_CAVERN))
    {
        updateSR(SR_IC1, 0, 0);
        updateSR(SR_IC2, 0, 0);
    }

    if (gComboData.mq & (1 << MQ_GERUDO_TRAINING_GROUNDS))
    {
        updateSR(SR_GTG1, 5, 0x1c);
        updateSR(SR_GTG2, 6, 0x0c);
        updateSR(SR_GTG3, 3, 0x1b);
    }

    if (gComboData.mq & (1 << MQ_GANON_CASTLE))
    {
        updateSR(SR_GANON1, 5, 0x0b);
        updateSR(SR_GANON2, 5, 0x02);
        updateSR(SR_GANON3, 5, 0x01);
        updateSR(SR_GANON4, 0, 0);
    }
}

int comboSilverRupeesGetCount(int id)
{
    return ((*(kSilverRupeeCounts[id >> 2])) >> ((id & 3) * 8)) & 0xff;
}

int comboSilverRupeesIncCount(GameState_Play* play, int id)
{
    const ComboSilverRupeeData* data;
    int tmp;

    data = &gSilverRupeeData[id];
    tmp = comboSilverRupeesGetCount(id) + 1;
    if (tmp > data->count)
        tmp = data->count;
    (*(kSilverRupeeCounts[id >> 2])) &= ~(0xff << ((id & 3) * 8));
    (*(kSilverRupeeCounts[id >> 2])) |= (tmp << ((id & 3) * 8));

    if (tmp >= data->count)
    {
        /* Special-cased */
        if (data->flag >= 0x20)
            return tmp;

        /* Set switch flag */
        gOotSave.perm[data->sceneId].switches |= (1 << data->flag);

#if defined(GAME_OOT)
        Actor_Player* link;

        if (play && play->sceneId == data->sceneId)
        {
            link = GET_LINK(play);
            if (link->state & (PLAYER_ACTOR_STATE_CLIMB | PLAYER_ACTOR_STATE_CLIMB2))
            {
                g.delayedSwitchFlag = data->flag;
            }
            else
                SetSwitchFlag(play, data->flag);
        }
#endif
    }

    return tmp;
}

/* Handle temp flags */
void comboSrUpdate(GameState_Play* play)
{
    const ComboSilverRupeeData* data;

    for (int sr = 0; sr < SR_MAX; ++sr)
    {
        data = &gSilverRupeeData[sr];
        if (data->flag < 0x20)
            continue;
        if (play->sceneId != data->sceneId)
            continue;
        if (comboSilverRupeesGetCount(sr) < data->count)
            continue;

        SetSwitchFlag(play, data->flag);
    }
}
