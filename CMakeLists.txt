cmake_minimum_required(VERSION 3.5.0)
project(zeus)

find_package(
    Qt6
    COMPONENTS
        Widgets
    REQUIRED
)
find_package(PkgConfig REQUIRED)
pkg_check_modules(
    PULSE
        REQUIRED
    libpulse>=5.0
    libpulse-mainloop-glib>=0.9.16
)

if(WITH_DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -O0")
endif()

set(CMAKE_AUTOMOC ON)
set(MOCUIC_DIR ${CMAKE_CURRENT_BINARY_DIR}/.mocuic)

add_subdirectory(src)
