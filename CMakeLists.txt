cmake_minimum_required(VERSION 3.10.2)
project(hovergames)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT MSVC)
    add_definitions("-Wall -Wextra")
else()
    add_definitions("-WX -W2")
endif()

find_package(MAVSDK REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Threads)
include_directories(include)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIB_DIR})
add_executable(${PROJECT_NAME}
    src/hovergames.cpp
    src/GpsHandler.cpp
    src/CameraHandler.cpp
    src/MissionHandler.cpp
    src/ResultHandler.cpp
    src/VideoWriter.cpp
    src/VideoReader.cpp
)

target_link_libraries(${PROJECT_NAME}
    ${OpenCV_LIBS}
    MAVSDK::mavsdk_telemetry
    MAVSDK::mavsdk_action
    MAVSDK::mavsdk
    MAVSDK::mavsdk_mission
    pthread
)
