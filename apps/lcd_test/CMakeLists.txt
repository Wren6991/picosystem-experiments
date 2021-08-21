add_executable(lcd_test
	main.c
)

pico_generate_pio_header(lcd_test ${CMAKE_CURRENT_LIST_DIR}/st7789_lcd.pio)

target_link_libraries(lcd_test PRIVATE
	pico_stdlib
	hardware_pio
	hardware_interp
	picosystem_buttons
	# REQUIRED! Virtual BOOTSEL switch when ABXY are all pressed:
	picosystem_bootbutton
)

pico_add_extra_outputs(lcd_test)
