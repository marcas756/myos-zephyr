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
    src/timestamp_arch.c
)

zephyr_include_directories(include)
zephyr_include_directories(include/myos)
