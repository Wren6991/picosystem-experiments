add_library(picosystem_display INTERFACE)

target_sources(picosystem_display INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/picosystem_display.c
	)

target_include_directories(picosystem_display INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/include
	)

target_link_libraries(picosystem_display INTERFACE
	hardware_gpio
	hardware_pio
	hardware_timer
	)

pico_generate_pio_header(picosystem_display ${CMAKE_CURRENT_LIST_DIR}/st7789_lcd.pio)
