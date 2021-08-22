add_executable(lcd_test
	main.c
)

target_link_libraries(lcd_test PRIVATE
	pico_stdlib
	hardware_pio
	hardware_interp
	picosystem_display
	picosystem_buttons
	# REQUIRED! Virtual BOOTSEL switch when ABXY are all pressed:
	picosystem_bootbutton
)

pico_add_extra_outputs(lcd_test)
