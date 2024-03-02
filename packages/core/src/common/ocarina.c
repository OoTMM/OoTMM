#include <combo.h>

void comboCheckSong(const OcarinaSongButtons* songButtons, int songIndex)
{
    u32 pitch;
    u8 j;
    u8 k;

    for (j = 0, k = 0; j < songButtons->numButtons && k == 0 && gOcarinaSongBufferCursor >= songButtons->numButtons;)
    {
        pitch = gOcarinaSongBuffer[(gOcarinaSongBufferCursor - songButtons->numButtons) + j];
        if (pitch == gOcarinaButtonPitchMap[songButtons->buttonIndex[j]])
        {
            j++;
        }
        else
        {
            k++;
        }
    }

    if (j == songButtons->numButtons)
    {
        gIsOcarinaEnabled = 0;
        gOcarinaFlags = 0;

        gPlayedOcarinaSong = songIndex + 0x80 + 1;
    }
}
