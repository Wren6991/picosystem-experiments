/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <math.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/gpio.h"
#include "hardware/interp.h"
#include "picosystem_buttons.h"
#include "picosystem_display.h"

#include "st7789_lcd.pio.h"
#include "raspberry_256x256_rgb565.h"

#define IMAGE_SIZE 256
#define LOG_IMAGE_SIZE 8

int main() {
    setup_default_uart();

    PIO pio = pio0;
    uint sm = 0;
    picosystem_display_init(pio, sm);

    // Lane 0 will be u coords (bits 8:1 of addr offset), lane 1 will be v
    // coords (bits 16:9 of addr offset), and we'll represent coords with
    // 16.16 fixed point. ACCUM0,1 will contain current coord, BASE0/1 will
    // contain increment vector, and BASE2 will contain image base pointer
    #define UNIT_LSB 16
    interp_config lane0_cfg = interp_default_config();
    interp_config_set_shift(&lane0_cfg, UNIT_LSB - 1); // -1 because 2 bytes per pixel
    interp_config_set_mask(&lane0_cfg, 1, 1 + (LOG_IMAGE_SIZE - 1));
    interp_config_set_add_raw(&lane0_cfg, true); // Add full accumulator to base with each POP
    interp_config lane1_cfg = interp_default_config();
    interp_config_set_shift(&lane1_cfg, UNIT_LSB - (1 + LOG_IMAGE_SIZE));
    interp_config_set_mask(&lane1_cfg, 1 + LOG_IMAGE_SIZE, 1 + (2 * LOG_IMAGE_SIZE - 1));
    interp_config_set_add_raw(&lane1_cfg, true);

    interp_set_config(interp0_hw, 0, &lane0_cfg);
    interp_set_config(interp0_hw, 1, &lane1_cfg);
    interp0_hw->base[2] = (uint32_t)raspberry_256x256;

    float theta = 0.f;
    float theta_max = 2.f * (float)M_PI;
    float dtheta = 0.02f;
    while (1) {
        if (picosystem_button_pressed(PICOSYSTEM_SW_UP_PIN))
            dtheta += 0.001f;
        if (picosystem_button_pressed(PICOSYSTEM_SW_DOWN_PIN))
            dtheta -= 0.001f;
        if (!picosystem_button_pressed(PICOSYSTEM_SW_A_PIN))
            theta += dtheta;
        if (theta > theta_max)
            theta -= theta_max;
        else if (theta < 0)
            theta += theta_max;
        int32_t rotate[4] = {
            cosf(theta) * (1 << UNIT_LSB), -sinf(theta) * (1 << UNIT_LSB),
            sinf(theta) * (1 << UNIT_LSB),  cosf(theta) * (1 << UNIT_LSB)
        };
        interp0_hw->base[0] = rotate[0];
        interp0_hw->base[1] = rotate[2];
        for (int y = 0; y < SCREEN_HEIGHT; ++y) {
            interp0_hw->accum[0] = rotate[1] * y;
            interp0_hw->accum[1] = rotate[3] * y;
            for (int x = 0; x < SCREEN_WIDTH; ++x) {
                uint16_t colour = *(uint16_t*)(interp0_hw->pop[2]);
                picosystem_display_put(pio, sm, colour >> 8 | (colour & 0xff) << 8);
            }
        }
    }
}
