add_library(picosystem_bootbutton INTERFACE)

target_sources(picosystem_bootbutton INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/bootbutton.c
	)

target_link_libraries(picosystem_bootbutton INTERFACE
	pico_time
	pico_bootrom
	hardware_gpio
	)
