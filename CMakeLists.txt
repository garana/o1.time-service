
cmake_minimum_required(VERSION 3.22)

project(o1time
        VERSION 1.0
        DESCRIPTION "Simple Time Library"
        LANGUAGES C
)

set(FILES_READER "src/reader/o1-time-reader.h" "src/reader/o1-time-reader.c"
        src/o1-time.h
        src/o1-time.c)
set(FILES_FEEDER "src/feeder/o1-time-feeder.c" "src/feeder/o1-time-feeder.h"
        src/o1-time.h
        src/o1-time.c)

add_library(o1timereader ${FILES_READER})
add_library(o1timefeeder ${FILES_FEEDER})

add_executable(o1-time-reader ${FILES_READER}
        src/reader/main.c
        o1.shm/src/o1shm.h
        o1.shm/src/o1shm.c
        src/cli-common.c
        src/cli-common.h
)

add_executable(o1-time-feeder ${FILES_FEEDER}
        src/feeder/main.c
        o1.shm/src/o1shm.h
        o1.shm/src/o1shm.c
        src/cli-common.c
        src/cli-common.h
)

target_include_directories(o1timereader PRIVATE o1.shm/src)
target_include_directories(o1timefeeder PRIVATE o1.shm/src)

target_include_directories(o1-time-reader PRIVATE o1.shm/src)
target_include_directories(o1-time-feeder PRIVATE o1.shm/src)

install(TARGETS o1timereader ARCHIVE DESTINATION lib COMPONENT libraries)
install(TARGETS o1timefeeder ARCHIVE DESTINATION lib COMPONENT libraries)

install(FILES ${FILES_READER} DESTINATION include COMPONENT headers)
install(FILES ${FILES_FEEDER} DESTINATION include COMPONENT headers)

include(CPack)
