add_subdirectory(gambatte)

add_executable(picosystem_gambatte
    main.cpp
)

target_include_directories(picosystem_gambatte PRIVATE
	roms/include
)

target_link_libraries(picosystem_gambatte
	pico_stdlib
	hardware_dma
	picosystem_buttons
	picosystem_display
	picosystem_bootbutton
	libgambatte
)

target_compile_definitions(picosystem_gambatte PRIVATE
	PICO_DEBUG_MALLOC=1)

pico_add_extra_outputs(picosystem_gambatte)
