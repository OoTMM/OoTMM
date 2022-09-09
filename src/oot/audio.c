#include <combo.h>

void PlaySound6(u16 soundId, void*, int, void*, void*, void*);

void PlaySound(u16 soundId)
{
    PlaySound6(soundId, (void*)0x80104394, 4, (void*)0x801043a0, (void*)0x801043a0, (void*)0x801043a8);
}
