#include "hardware/gpio.h"

static const uint8_t all_picosystem_buttons[] = {
	PICOSYSTEM_SW_Y_PIN,
	PICOSYSTEM_SW_X_PIN,
	PICOSYSTEM_SW_A_PIN,
	PICOSYSTEM_SW_B_PIN,
	PICOSYSTEM_SW_DOWN_PIN,
	PICOSYSTEM_SW_RIGHT_PIN,
	PICOSYSTEM_SW_LEFT_PIN,
	PICOSYSTEM_SW_UP_PIN
};

void __attribute__((constructor)) init_all_picosystem_buttons() {
	for (int i = 0; i < count_of(all_picosystem_buttons); ++i) {
		gpio_init(i);
		gpio_pull_up(i);
	}
}

bool picosystem_button_pressed(uint pin) {
    return !gpio_get(pin);
}
