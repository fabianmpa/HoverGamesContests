#pragma once
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <mavsdk/plugins/mission/mission.h>
#include <mavsdk/plugins/action/action.h>
#include <iostream>

using namespace std;
using namespace mavsdk;

typedef Mission::MissionProgress missionProgress_s;
typedef Telemetry::GpsInfo gps_s;
typedef Telemetry::HoverGamesStatus_s hovergames_s;

#define ERROR_CONSOLE_TEXT "\033[31m" // Turn text on console red
#define TELEMETRY_CONSOLE_TEXT "\033[34m" // Turn text on console blue
#define NORMAL_CONSOLE_TEXT "\033[0m" // Restore normal console colour