#include <combo.h>
#include <combo/audio.h>

void comboPlayFanfare(int fanfare)
{
    switch (fanfare)
    {
    case FF_NONE:
        break;
    case FF_ITEM_MASK:
#if defined(GAME_MM)
        Audio_PlayFanfare(0x37);
        break;
#else
    /* fallthrough */
#endif
    case FF_ITEM_MAJOR:
        Audio_PlayFanfare(0x922);
        break;
    case FF_ITEM_MINOR:
        PlaySound(0x4831);
        break;
    case FF_ITEM_HEART_CONTAINER:
        Audio_PlayFanfare(0x924);
        break;
    case FF_ITEM_HEART_PIECE:
        Audio_PlayFanfare(0x39);
        break;
    }
}
