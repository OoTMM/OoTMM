#ifndef COMBO_ENVIRONMENT_H
#define COMBO_ENVIRONMENT_H

typedef struct EnvironmentContext EnvironmentContext;

#if defined(GAME_MM)
void Environment_NewDay(EnvironmentContext* ctxt);
#endif

#endif
