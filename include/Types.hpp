#pragma once
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <mavsdk/plugins/mission/mission.h>
#include <mavsdk/plugins/action/action.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <cstdint>

using namespace std;
using namespace mavsdk;
using namespace std::chrono;
using namespace std::this_thread;

typedef Mission::MissionProgress missionProgress_s;
typedef Telemetry::GpsInfo gps_s;
typedef Telemetry::HoverGamesStatus_s hovergames_s;

#define ERROR_CONSOLE_TEXT "\033[31m" // Turn text on console red
#define TELEMETRY_CONSOLE_TEXT "\033[34m" // Turn text on console blue
#define NORMAL_CONSOLE_TEXT "\033[0m" // Restore normal console colour

struct Size
{

    uint32_t m_Width;
    uint32_t m_Height;

    Size() : Size(0, 0) {}

    Size(uint32_t width, uint32_t height) :
            m_Width{width}, m_Height{height} {}

    Size(const Size& other)
            : Size(other.m_Width, other.m_Height) {}

    ~Size() = default;

    Size &operator=(const Size& other) = default;
};

struct BBoxColor
{
    std::tuple<int, int, int> colorCode;
};