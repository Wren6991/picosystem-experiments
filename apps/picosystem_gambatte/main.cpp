#include "pico/stdlib.h"
#include "file/file.h"
#include "gambatte.h"

// GB BIOS image (not included in the repo, do not open an issue asking where to find it)
__in_flash("roms") const
#include "roms/gb_bios.bin.h"

// Some open source gameboy/GBC games:
__in_flash("roms") const
#include "roms/2048.gb.h"
// __in_flash("roms") const
// #include "roms/ucity.gbc.h"

// This has external linkage in stdfile.h, gets searched when a file is opened.
extern const __in_flash("romtable") file_entry file_manifest[] = {
	{"gb_bios.bin",     (const char*)__gb_bios_bin,    __gb_bios_bin_len    },
	{"2048.gb",         (const char*)__2048_gb,        __2048_gb_len        },
	// {"ucity.gbc",       (const char*)__ucity_gbc,      __ucity_gbc_len      },
	{0,                 0,                             0 /* end */          }
};


int main() {
	stdio_init_all();

	printf("Starting PicoSystem Gambatte\n");

	gambatte::GB gb;
	gb.loadBios("gb_bios.bin");
	gb.load("2048.gb");

	uint16_t *framebuf = (uint16_t*)malloc(160 * 144 * sizeof(uint16_t));

	// No room for sound buffer

	printf("Attempting to run one frame.\n");

	size_t audio_samples = 0;
	gb.runFor(framebuf, 160, NULL, audio_samples);

	printf("Apparently got %d audio samples\n", audio_samples);

}
