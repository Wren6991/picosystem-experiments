#include "picosystem_display.h"
#include "st7789_lcd.pio.h"
#include "hardware/pio.h"
#include "hardware/timer.h"

#define PIN_DIN PICOSYSTEM_LCD_MOSI_PIN
#define PIN_CLK PICOSYSTEM_LCD_SCLK_PIN
#define PIN_CS PICOSYSTEM_LCD_CSN_PIN
#define PIN_DC PICOSYSTEM_LCD_DC_PIN
#define PIN_RESET PICOSYSTEM_LCD_RESET_PIN
#define PIN_BL PICOSYSTEM_BACKLIGHT_PIN

#define SERIAL_CLK_DIV 1.f

// Format: cmd length (including cmd byte), post delay in units of 5 ms, then cmd payload
// Note the delays have been shortened a little
static const uint8_t st7789_init_seq[] = {
    1, 20,  0x01,                         // Software reset
    1, 10,  0x11,                         // Exit sleep mode
    2, 2,   0x3a, 0x55,                   // Set colour mode to 16 bit
    2, 0,   0x36, 0x00,                   // Set MADCTL: row then column, refresh is bottom to top ????
    5, 0,   0x2a, 0x00, 0x00, 0x00, SCREEN_WIDTH - 1, // CASET: column addresses from 0 to 240 (f0)
    5, 0,   0x2b, 0x00, 0x00, 0x00, SCREEN_HEIGHT - 1, // RASET: row addresses from 0 to 240 (f0)
    1, 2,   0x21,                         // Inversion on, then 10 ms delay (supposedly a hack?)
    1, 2,   0x13,                         // Normal display on, then 10 ms delay
    1, 2,   0x29,                         // Main screen turn on, then wait 500 ms
    0                                     // Terminate list
};

static inline void lcd_set_dc_cs(bool dc, bool cs) {
    busy_wait_us(1);
    gpio_put_masked((1u << PIN_DC) | (1u << PIN_CS), !!dc << PIN_DC | !!cs << PIN_CS);
    busy_wait_us(1);
}

static inline void lcd_write_cmd(PIO pio, uint sm, const uint8_t *cmd, size_t count) {
    st7789_lcd_wait_idle(pio, sm);
    lcd_set_dc_cs(0, 0);
    st7789_lcd_put8(pio, sm, *cmd++);
    if (count >= 2) {
        st7789_lcd_wait_idle(pio, sm);
        lcd_set_dc_cs(1, 0);
        for (size_t i = 0; i < count - 1; ++i)
            st7789_lcd_put8(pio, sm, *cmd++);
    }
    st7789_lcd_wait_idle(pio, sm);
    lcd_set_dc_cs(1, 1);
}

void picosystem_display_init(PIO pio, uint sm) {
    uint offset = pio_add_program(pio, &st7789_lcd_program);
    st7789_lcd_program_init(pio, sm, offset, PIN_DIN, PIN_CLK, SERIAL_CLK_DIV);

    gpio_init(PIN_CS);
    gpio_init(PIN_DC);
    gpio_init(PIN_RESET);
    gpio_init(PIN_BL);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_set_dir(PIN_DC, GPIO_OUT);
    gpio_set_dir(PIN_RESET, GPIO_OUT);
    gpio_set_dir(PIN_BL, GPIO_OUT);

    busy_wait_us(1);
    gpio_put(PIN_CS, 1);
    gpio_put(PIN_RESET, 1);

    const uint8_t *cmd = st7789_init_seq;
    while (*cmd) {
        lcd_write_cmd(pio, sm, cmd + 2, *cmd);
        busy_wait_us(*(cmd + 1) * 5000);
        cmd += *cmd + 2;
    }

    // Send a single RAMWR, we can then continously send pixels without
    // sending any more commands
    uint8_t ramwr = 0x2c;
    lcd_write_cmd(pio, sm, &ramwr, 1);
    lcd_set_dc_cs(1, 0);

    // Finished with byte data/cmds.
    st7789_lcd_set_bitwidth(pio, sm, 16);

    // Clear the screen before turning on the backlight, so we don't show
    // uninitialised display memory
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i)
        picosystem_display_put(pio, sm, 0);

    gpio_put(PIN_BL, 1);
}

void picosystem_display_put(PIO pio, uint sm, uint16_t pix_rgb565) {
    st7789_lcd_put16(pio, sm, pix_rgb565 >> 8 | (pix_rgb565 & 0xff) << 8);
}
