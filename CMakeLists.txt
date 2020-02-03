cmake_minimum_required(VERSION 2.8)

project(HoverScriptTakeOff)

if(NOT MSVC)
    add_definitions("-std=c++11 -Wall -Wextra -Werror")
else()
    add_definitions("-std=c++11 -WX -W2")
endif()

find_package(MAVSDK REQUIRED)
find_package(MAVSDK REQUIRED)
find_package(CUDA REQUIRED)
find_package(jetson-utils)
find_package(jetson-inference)

add_executable(HoverTakeOff HoverTakeOff.cpp)

file(GLOB detectnetCameraSources *.cpp)
file(GLOB detectnetCameraIncludes *.h )

include_directories("${CUDA_INCLUDE_DIRS}")
include_directories(/usr/include/gstreamer-1.0 /usr/lib/aarch64-linux-gnu/gstreamer-1.0/include /usr/include/glib-2.0 /usr/include/libxml2 /usr/lib/aarch64-linux-gnu/glib-2.0/include /usr/local/include)
target_link_libraries(HoverTakeOff
    MAVSDK::mavsdk_action
    MAVSDK::mavsdk_telemetry
    MAVSDK::mavsdk
    jetson-inference
    jetson-utils	
)
