add_library(libgambatte INTERFACE)

set(LIBGAMBATTE ${CMAKE_CURRENT_LIST_DIR}/gambatte-core/libgambatte)

target_include_directories(libgambatte INTERFACE
	${LIBGAMBATTE}/include
	${LIBGAMBATTE}/src
	${LIBGAMBATTE}/../common
	)

target_sources(libgambatte INTERFACE
	${LIBGAMBATTE}/src/bitmap_font.cpp
	${LIBGAMBATTE}/src/cpu.cpp
	${LIBGAMBATTE}/src/gambatte.cpp
	${LIBGAMBATTE}/src/initstate.cpp
	${LIBGAMBATTE}/src/interrupter.cpp
	${LIBGAMBATTE}/src/interruptrequester.cpp
	${LIBGAMBATTE}/src/loadres.cpp
	${LIBGAMBATTE}/src/memory.cpp
	${LIBGAMBATTE}/src/newstate.cpp
	${LIBGAMBATTE}/src/sound.cpp
	${LIBGAMBATTE}/src/state_osd_elements.cpp
	${LIBGAMBATTE}/src/statesaver.cpp
	${LIBGAMBATTE}/src/tima.cpp
	${LIBGAMBATTE}/src/video.cpp
	${LIBGAMBATTE}/src/file/file.cpp
	${LIBGAMBATTE}/src/mem/camera.cpp
	${LIBGAMBATTE}/src/mem/cartridge.cpp
	${LIBGAMBATTE}/src/mem/huc3.cpp
	${LIBGAMBATTE}/src/mem/memptrs.cpp
	${LIBGAMBATTE}/src/mem/pakinfo.cpp
	${LIBGAMBATTE}/src/mem/rtc.cpp
	${LIBGAMBATTE}/src/mem/sgb.cpp
	${LIBGAMBATTE}/src/mem/time.cpp
	${LIBGAMBATTE}/src/sound/channel1.cpp
	${LIBGAMBATTE}/src/sound/channel2.cpp
	${LIBGAMBATTE}/src/sound/channel3.cpp
	${LIBGAMBATTE}/src/sound/channel4.cpp
	${LIBGAMBATTE}/src/sound/duty_unit.cpp
	${LIBGAMBATTE}/src/sound/envelope_unit.cpp
	${LIBGAMBATTE}/src/sound/length_counter.cpp
	${LIBGAMBATTE}/src/video/ly_counter.cpp
	${LIBGAMBATTE}/src/video/lyc_irq.cpp
	${LIBGAMBATTE}/src/video/next_m0_time.cpp
	${LIBGAMBATTE}/src/video/ppu.cpp
	${LIBGAMBATTE}/src/video/sprite_mapper.cpp
	)
