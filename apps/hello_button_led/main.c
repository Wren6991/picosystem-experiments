#include "hardware/gpio.h"
#include "picosystem_buttons.h"

// Connect A, B, X buttons to R, G, B LEDs, so you can blink the LEDs by
// pressing buttons.

static const uint8_t led_pins[] = {
    PICOSYSTEM_LED_R_PIN,
    PICOSYSTEM_LED_G_PIN,
    PICOSYSTEM_LED_B_PIN
};

static const uint8_t button_pins[] = {
    PICOSYSTEM_SW_A_PIN,
    PICOSYSTEM_SW_B_PIN,
    PICOSYSTEM_SW_X_PIN
};

int main() {
    for (int i = 0; i < count_of(led_pins); ++i) {
        gpio_init(led_pins[i]);
        gpio_set_dir(led_pins[i], GPIO_OUT);
    }
    while (1) {
        for (int i = 0; i < count_of(led_pins); ++i) {
            gpio_put(led_pins[i], picosystem_button_pressed(button_pins[i]));
        }
    }
}

