zephyr_library()

zephyr_library_sources(
    src/ctimer.c
    src/dlist.c
    src/etimer.c
    src/hash.c
    src/itempool.c
    src/myos.c
    src/process.c
    src/ptimer.c
    src/rtimer.c
    src/rtimer_arch.c
    src/slist.c
    src/timer.c
    src/timestamp.c
)

# timestamp_arch Backend wählen
if(CONFIG_ARCH_POSIX)
  # native_sim / native_posix: Host-Zeit über time.h
  zephyr_library_sources(
    src/arch/timestamp_arch_nativesim.c
  )
else()
  # alle anderen Boards: Zephyr k_uptime
  zephyr_library_sources(
    src/arch/timestamp_arch_others.c
  )
endif()

zephyr_include_directories(include)
zephyr_include_directories(include/myos)
zephyr_include_directories(include/myos/arch)
