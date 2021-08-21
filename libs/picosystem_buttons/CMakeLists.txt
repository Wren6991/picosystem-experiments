add_library(picosystem_buttons INTERFACE)

target_sources(picosystem_buttons INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/picosystem_buttons.c
	)

target_include_directories(picosystem_buttons INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/include
	)

target_link_libraries(picosystem_buttons INTERFACE
	hardware_gpio
	)
