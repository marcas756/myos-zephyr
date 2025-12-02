zephyr_library()

set(STM32_BOARDS
    nucleo_l073rz
    nucleo_f0401re
)


zephyr_library_sources(
  # Arch specific files
    src/arch/${BOARD}/timestamp_arch.c
    src/arch/${BOARD}/rtimer_arch.c    

  # Common files 
    src/ctimer.c
    src/dlist.c
    src/etimer.c
    src/hash.c
    src/itempool.c
    src/mutex.c
    src/myos.c
    src/process.c
    src/ptimer.c
    src/rtimer.c
    src/slist.c
    src/timer.c
    src/timestamp.c
)


zephyr_include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/arch/${BOARD}
)


