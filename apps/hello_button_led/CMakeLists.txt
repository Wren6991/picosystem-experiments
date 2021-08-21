add_executable(hello_button_led
    main.c
)

target_link_libraries(hello_button_led
	pico_stdlib
	picosystem_buttons
	# This one is important (virtual BOOTSEL switch when ABXY are all pressed):
	picosystem_bootbutton
)

pico_add_extra_outputs(hello_button_led)
