#ifndef COMBO_ENVIRONMENT_H
#define COMBO_ENVIRONMENT_H

#if defined(GAME_OOT)
# include <combo/oot/environment.h>
#endif

#if defined(GAME_MM)
# include <combo/mm/environment.h>
#endif

extern u8 gWeatherMode;
extern u8 gInterruptSongOfStorms;

#endif
