#include "pico/bootrom.h"
#include "pico/time.h"
#include "hardware/gpio.h"

// All must be pressed at power up to enter USB boot mode:
static const int boot_buttons[] = {
    PICOSYSTEM_SW_Y_PIN,
    PICOSYSTEM_SW_X_PIN,
    PICOSYSTEM_SW_A_PIN,
    PICOSYSTEM_SW_B_PIN
};

// Registered to run before main:
void __attribute__((constructor)) boot_button_check() {
    for (int i = 0; i < count_of(boot_buttons); ++i) {
        gpio_init(boot_buttons[i]);
        gpio_pull_up(boot_buttons[i]);
    }
    sleep_us(100);
    bool all_pressed = true;
    for (int i = 0; i < count_of(boot_buttons); ++i)
        all_pressed = all_pressed && !gpio_get(boot_buttons[i]);

    // Use green LED as bootloader activity light
    if (all_pressed)
        reset_usb_boot((1u << PICOSYSTEM_LED_G_PIN) | (1u << PICOSYSTEM_LED_R_PIN), 0);
}
