#include "pico/stdlib.h"
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
// I dumped this ROM myself from a real cartridge with only a magnifying glass
// and a steady hand

__in_flash("roms") const
#include "roms/tetris.gb.h"

// ----------------------------------------------------------------------------
// File manifest (poor man's read only filesystem)

extern const __in_flash("romtable") file_entry file_manifest[] = {
	{"gb_bios.bin",     (const char*)__gb_bios_bin,    __gb_bios_bin_len    },

	// {"2048.gb",         (const char*)__2048_gb,        __2048_gb_len        },
	// {"ucity.gbc",       (const char*)__ucity_gbc,      __ucity_gbc_len      },

	{"tetris.gb",       (const char*)__tetris_gb,      __tetris_gb_len      },

	{0,                 0,                             0 /* end */          }
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


int main() {
	stdio_init_all();

	puts(splash);

	gambatte::GB gb;
	gb.loadBios("gb_bios.bin");
	gb.load("tetris.gb");

	uint16_t *framebuf = (uint16_t*)malloc(160 * 144 * sizeof(uint16_t));
	// No room for sound buffer

	printf("Setting up display\n");
    PIO pio = pio0;
    uint sm = 0;
    picosystem_display_init(pio, sm);


	printf("Starting...\n");

	while (1) {
		size_t audio_samples = 35112;
		puts(".");
		gb.runFor(framebuf, 160, NULL, audio_samples);
		for (int y = 0; y < SCREEN_HEIGHT; ++y) {
			for (int x = 0; x < SCREEN_WIDTH; ++x) {
				if (x < 160 && y < 144) {
					picosystem_display_put(pio, sm, framebuf[x + y * 160]);
				} else {
					picosystem_display_put(pio, sm, 0x5555);
				}
			}
		}
	}
}
