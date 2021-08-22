#ifndef _PICOSYSTEM_DISPLAY_H
#define _PICOSYSTEM_DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hardware/pio.h"

// FIXME, hack for gameboy stuff
#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144

void picosystem_display_init(PIO pio, uint sm);

void picosystem_display_put(PIO pio, uint sm, uint16_t pix_rgb565);

static inline io_rw_32 *picosystem_display_dma_addr(PIO pio, uint sm) {
	return &pio->txf[sm];
}

static inline uint picosystem_display_dma_dreq(PIO pio, uint sm) {
	return pio_get_dreq(pio, sm, true);
}

#ifdef __cplusplus
}
#endif


#endif
