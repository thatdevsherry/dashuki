cmake_minimum_required(VERSION 3.20)

project(suzuki_dashboard C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

include(ProcessorCount)
ProcessorCount(N)
if(N EQUAL 0)
    set(N 4)
endif()
set(CMAKE_BUILD_PARALLEL_LEVEL ${N})

include(FetchContent)

set(FETCHCONTENT_QUIET OFF)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 6.0
)
FetchContent_Declare(
    suzuki_sdl
    GIT_REPOSITORY https://github.com/thatdevsherry/libsuzuki.git
    GIT_TAG 0.1.0
)

FetchContent_MakeAvailable(raylib suzuki_sdl)

target_include_directories(raylib PRIVATE ${CMAKE_SYSROOT}/usr/include/libdrm)

add_executable(
    ${PROJECT_NAME}
    src/main.c
    src/comms.c
    src/constants.c
    src/interface.c
    src/menu.c
    src/save.c
    src/state.c
    src/utils.c
    src/gui/dashboard.c
    src/gui/diag_params.c
    src/gui/flags.c
    src/gui/fuel.c
    src/gui/guidelines.c
    src/gui/menu.c
    src/gui/rpm.c
    src/gui/speed.c
    src/gui/temps.c
    src/gui/throttle.c
    src/gui/odometer.c
)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib m pthread suzuki_sdl)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src
)

set(PROJECT_COMPILE_OPTIONS
    -Wall
    -Wextra
    #-Wpedantic
    -Werror
    -Wshadow
    -Wformat=2
    -Wundef
    -Wnull-dereference
    -Wwrite-strings
    -Wcast-qual
    -Wcast-align
    #-Wstrict-prototypes
    #-Wmissing-prototypes
    -Wdouble-promotion
    #-Wconversion
)

target_compile_options(${PROJECT_NAME} PRIVATE ${PROJECT_COMPILE_OPTIONS})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(
        ${PROJECT_NAME}
        PRIVATE
            DEBUG_MODE
            DATA_FILE_PATH="$ENV{DATA_FILE_PATH}"
            DATA_FILE_PATH_TMP="$ENV{DATA_FILE_PATH_TMP}"
            SETTINGS_FILE_PATH="$ENV{SETTINGS_FILE_PATH}"
            SETTINGS_FILE_PATH_TMP="$ENV{SETTINGS_FILE_PATH_TMP}"
    )
else()
    target_compile_definitions(
        ${PROJECT_NAME}
        PRIVATE
            DATA_FILE_PATH="/opt/dashuki_data.txt"
            DATA_FILE_PATH_TMP="/opt/dashuki_data.txt.tmp"
            SETTINGS_FILE_PATH="/opt/dashuki_settings.txt"
            SETTINGS_FILE_PATH_TMP="/opt/dashuki_settings.txt.tmp"
    )
endif()

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND
        ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_STRIP} $<TARGET_FILE:${PROJECT_NAME}>
    )
endif()
