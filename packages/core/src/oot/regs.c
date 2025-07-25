#include <combo.h>

static u8 sColorCButtons[3] = { 255, 160, 0 };
EXPORT_SYMBOL(COLOR_C_BUTTONS, sColorCButtons);

void Regs_AfterInit(void)
{
    R_C_BTN_COLOR(0) = sColorCButtons[0];
    R_C_BTN_COLOR(1) = sColorCButtons[1];
    R_C_BTN_COLOR(2) = sColorCButtons[2];
}
