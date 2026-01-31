#ifndef OCARINA_H
#define OCARINA_H

#include <combo.h>

u8 Ocarina_CheckSongEventSong(u16 ocarinaSong, int songEventId);
void Ocarina_ClearLastPlayedSong(PlayState* play);
void comboSpawnCustomSongTags(PlayState* play);

#endif
