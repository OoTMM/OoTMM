#ifndef INCLUDED_COMBO_FLAGS_H
#define INCLUDED_COMBO_FLAGS_H

typedef struct PlayState PlayState;

int  Flags_GetPermSwitchOot(PlayState* play, int sceneId, int flag);
void Flags_SetPermSwitchOot(PlayState* play, int sceneId, int flag);

#endif
