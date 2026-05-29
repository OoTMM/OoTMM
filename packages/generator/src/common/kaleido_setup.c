#include <combo.h>
#include <combo/common/kaleido_setup.h>

static int sHoldingStart = false;
int KaleidoSetup_FrameAdvance(Input *input)
{
    if (CHECK_BTN_ALL(input->press.button, BTN_START))
    {
        return true;
    }

    if (CHECK_BTN_ALL(input->cur.button, BTN_START))
    {
        if (sHoldingStart)
        {
            sHoldingStart = false;
            return true;
        }
        sHoldingStart = true;
    }
    else
    {
        sHoldingStart = false;
    }
    return false;
}
