#include <combo.h>

/* C buttons for Ocarina */
EXPORT_ADDR(COLOR_C_BUTTONS_R, 0x801131f9);
EXPORT_ADDR(COLOR_C_BUTTONS_G, 0x801131fb);
EXPORT_ADDR(COLOR_C_BUTTONS_B, 0x801131fd);
EXPORT_ADDR(COLOR_C_BUTTONS_HIGHLIGHT_R, 0x801131ff);
EXPORT_ADDR(COLOR_C_BUTTONS_HIGHLIGHT_G, 0x80113201);
EXPORT_ADDR(COLOR_C_BUTTONS_HIGHLIGHT_B, 0x80113203);

static u8 sColorCButtons[] = { 0xff, 0xff, 0x32 };
EXPORT_SYMBOL(COLOR_C_BUTTONS, sColorCButtons);

void Message_AfterResetOcarinaNoteState(void)
{
    /* C button colors while playing */
    *((u16*)0x8012be1c) = sColorCButtons[0];
    *((u16*)0x8012be20) = sColorCButtons[1];
    *((u16*)0x8012be1e) = sColorCButtons[2];
}
