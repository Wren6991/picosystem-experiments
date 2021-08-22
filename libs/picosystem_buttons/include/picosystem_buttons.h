#ifndef _PICOSYSTEM_BUTTONS
#define _PICOSYSTEM_BUTTONS

#include "pico/types.h"

#ifdef __cplusplus
extern "C" {
#endif

bool picosystem_button_pressed(uint pin);

#ifdef __cplusplus
}
#endif

#endif
