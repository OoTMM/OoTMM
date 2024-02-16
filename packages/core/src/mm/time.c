#include <combo.h>

u32 Time_Game2Linear(u8 day, u16 time)
{
    u32 linear;

    linear = day * 0x10000;
    time -= 0x4000;
    linear |= time;
    return linear;
}

void Time_Linear2Game(u8* day, u16* time, u32 linear)
{
    u8 d;
    u16 t;

    d = linear >> 16;
    t = linear & 0xffff;
    t += 0x4000;

    *day = d;
    *time = t;
}

u32 Time_LinearMoonCrash(void)
{
    return 0x10000 + 0x8000 * gSharedCustomSave.mm.halfDays;
}

int Time_IsMoonCrashLinear(u32 time)
{
    u32 linearMoonCrash;
    linearMoonCrash = Time_LinearMoonCrash();
    if (time >= linearMoonCrash && time < 0x40000)
        return 1;
    return 0;
}

int Time_IsMoonCrash(u8 day, u16 time)
{
    return Time_IsMoonCrashLinear(Time_Game2Linear(day, time));
}
