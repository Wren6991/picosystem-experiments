#include "pico/stdlib.h"
#include "hardware/dma.h"

#include "file/file.h"
#include "gambatte.h"
#include "picosystem_display.h"

// ----------------------------------------------------------------------------
// GB BIOS image (not included, do not open an issue asking where to find it)
__in_flash("roms") const
#include "roms/gb_bios.bin.h"

// ----------------------------------------------------------------------------
// Some open source gameboy/GBC games:

// __in_flash("roms") const
// #include "roms/2048.gb.h"

// __in_flash("roms") const
// #include "roms/ucity.gbc.h"

// ----------------------------------------------------------------------------
// I dumped these ROMs myself from a real cartridge with only a magnifying
// glass and a steady hand. You should do the same

__in_flash("roms") const
#include "roms/tetris.gb.h"

// __in_flash("roms") const
// #include "roms/pokemon_yellow.gbc.h"

// ----------------------------------------------------------------------------
// File manifest (poor man's read only filesystem)

extern const __in_flash("romtable") file_entry file_manifest[] = {
	{"gb_bios.bin",            (const char*)__gb_bios_bin,            __gb_bios_bin_len         },

	// {"2048.gb",                (const char*)__2048_gb,                __2048_gb_len             },
	// {"ucity.gbc",              (const char*)__ucity_gbc,              __ucity_gbc_len           },

	{"tetris.gb",              (const char*)__tetris_gb,              __tetris_gb_len           },
	// {"pokemon_yellow.gbc",     (const char*)__pokemon_yellow_gbc,     __pokemon_yellow_gbc_len  },

	{0,                        0,                                          0 /* end */          }
};

// ----------------------------------------------------------------------------

// https://patorjk.com/software/taag/#p=display&f=Big&t=PicoSystem%20Gambatte
const char *splash = "\n"
" _____ _           _____           _\n"
"|  __ (_)         / ____|         | |\n"
"| |__) |  ___ ___| (___  _   _ ___| |_ ___ _ __ ___\n"
"|  ___/ |/ __/ _ \\\\___ \\| | | / __| __/ _ \\ '_ ` _ \\\n"
"| |   | | (_| (_) |___) | |_| \\__ \\ ||  __/ | | | | |\n"
"|_|   |_|\\___\\___/_____/ \\__, |___/\\__\\___|_| |_| |_|\n"
"                          __/ |\n"
"                         |___/\n"
"\n"
"    _____                 _           _   _\n"
"   / ____|               | |         | | | |\n"
"  | |  __  __ _ _ __ ___ | |__   __ _| |_| |_ ___\n"
"  | | |_ |/ _` | '_ ` _ \\| '_ \\ / _` | __| __/ _ \\\n"
"  | |__| | (_| | | | | | | |_) | (_| | |_| ||  __/\n"
"   \\_____|\\__,_|_| |_| |_|_.__/ \\__,_|\\__|\\__\\___|\n"
"\n";

#define FRAME_WIDTH 160
#define FRAME_HEIGHT 144

int main() {
	stdio_init_all();

	puts(splash);

	gambatte::GB gb;
	gb.loadBios("gb_bios.bin");
	gb.load("tetris.gb");

	uint16_t *framebuf = (uint16_t*)malloc(FRAME_WIDTH * FRAME_HEIGHT * sizeof(uint16_t));
	// No room for sound buffer

	printf("Setting up display\n");
    PIO pio = pio0;
    uint sm = 0;
    picosystem_display_init(pio, sm);

    // Configure, but do not start, a DMA transfer from framebuf to LCD.
    uint chan_lcd = dma_claim_unused_channel(true);
    dma_channel_config c = dma_channel_get_default_config(chan_lcd);
   	channel_config_set_dreq(&c, picosystem_display_dma_dreq(pio, sm));
   	channel_config_set_transfer_data_size(&c, DMA_SIZE_16);
   	channel_config_set_bswap(&c, true);
   	dma_channel_configure(chan_lcd, &c,
   		picosystem_display_dma_addr(pio, sm),
   		framebuf,
   		FRAME_WIDTH * FRAME_HEIGHT,
   		false
   	);

	printf("Starting...\n");

	while (1) {
		size_t audio_samples = 35112;
		puts(".");
		gb.runFor(framebuf, FRAME_WIDTH, NULL, audio_samples);
		// Start a new frame transfer. Should transfer faster than we render,
		// so stays ahead of the beam whilst we emulate the next frame. In theory :D
		dma_channel_wait_for_finish_blocking(chan_lcd);
		dma_channel_set_read_addr(chan_lcd, framebuf, true);
	}
}
