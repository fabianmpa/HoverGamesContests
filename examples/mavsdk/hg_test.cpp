//
// Simple example to demonstrate how to use the MAVSDK.
//
// Author: Julian Oes <julian@oes.ch>
/*
#include <chrono>
#include <cstdint>
#include <thread>
#include <mutex>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <iostream>

using namespace mavsdk;
using namespace std::this_thread;
using namespace std::chrono;

#define ERROR_CONSOLE_TEXT "\033[31m" // Turn text on console red
#define TELEMETRY_CONSOLE_TEXT "\033[34m" // Turn text on console blue
#define NORMAL_CONSOLE_TEXT "\033[0m" // Restore normal console colour

void usage(std::string bin_name)
{
    std::cout << NORMAL_CONSOLE_TEXT << "Usage : " << bin_name << " <connection_url>" << std::endl
              << "Connection URL format should be :" << std::endl
              << " For TCP : tcp://[server_host][:server_port]" << std::endl
              << " For UDP : udp://[bind_host][:bind_port]" << std::endl
              << " For Serial : serial:///path/to/serial/dev[:baudrate]" << std::endl
              << "For example, to connect to the simulator use URL: udp://:14540" << std::endl;
}

void component_discovered(ComponentType component_type)
{
    std::cout << NORMAL_CONSOLE_TEXT << "Discovered a component with type "
              << unsigned(component_type) << std::endl;
}

void hg_callback(Telemetry::HoverGamesStatus_s hgStatus)
{
    std::cout << "Global SM: "<< (int)hgStatus.HoverGames_SM << "\n"
              << "Active SM: "<< (int)hgStatus.HoverGames_ActiveSM << std::endl;
}

void gps_callback(Telemetry::GpsInfo gps_info)
{
        std::cout << "Alt: "<< (int)gps_info.alt << "\n"
                  << "Lat: "<< (int)gps_info.lat << "\n"
                  << "Lon: "<< (int)gps_info.lon << std::endl;
}

int main(int argc, char** argv)
{
    Mavsdk mavsdk;
    std::string connection_url;
    ConnectionResult connection_result;

    bool discovered_system = false;
    if (argc == 2) {
        connection_url = argv[1];
        connection_result = mavsdk.add_any_connection(connection_url);
    } else {
        usage(argv[0]);
        return 1;
    }

    if (connection_result != ConnectionResult::Success) {
        std::cout << ERROR_CONSOLE_TEXT << "Connection failed: " << connection_result
                  << NORMAL_CONSOLE_TEXT << std::endl;
        return 1;
    }

    std::cout << "Waiting to discover system..." << std::endl;
    mavsdk.subscribe_on_new_system([&mavsdk, &discovered_system]() {
        const auto system = mavsdk.systems().at(0);

        if (system->is_connected()) {
            std::cout << "Discovered system" << std::endl;
            discovered_system = true;
        }
    });

    // We usually receive heartbeats at 1Hz, therefore we should find a system after around 2
    // seconds.
    sleep_for(seconds(2));

    if (!discovered_system) {
        std::cout << ERROR_CONSOLE_TEXT << "No system found, exiting." << NORMAL_CONSOLE_TEXT
                  << std::endl;
        return 1;
    }

    const auto system = mavsdk.systems().at(0);

    // Register a callback so we get told when components (camera, gimbal) etc
    // are found.
    system->register_component_discovered_callback(component_discovered);

    auto telemetry = Telemetry{system};
    auto action = Action{system};

    const Telemetry::Result set_rate_result_hg = telemetry.set_rate_HoverGamesStatus(1.0);
    if (set_rate_result_hg != Telemetry::Result::Success) {
        std::cout << ERROR_CONSOLE_TEXT << "Setting rate failed:" << set_rate_result_hg
                  << NORMAL_CONSOLE_TEXT << std::endl;
        return 1;
    }

    const Telemetry::Result set_rate_result_hg_gps = telemetry.set_rate_gps_info(1.0);
    if (set_rate_result_hg_gps != Telemetry::Result::Success) {
        std::cout << ERROR_CONSOLE_TEXT << "Setting rate failed:" << set_rate_result_hg
                  << NORMAL_CONSOLE_TEXT << std::endl;
        return 1;
    }

    telemetry.subscribe_HoverGamesStatus(hg_callback);

    telemetry.subscribe_gps_info(gps_callback);

    // Check if vehicle is ready to arm
    while (telemetry.health_all_ok() != true) {
        std::cout << "Vehicle is getting ready to arm" << std::endl;
        sleep_for(seconds(1));
    }

    // Arm vehicle
    std::cout << "Arming..." << std::endl;
    const Action::Result arm_result = action.arm();

    if (arm_result != Action::Result::Success) {
        std::cout << ERROR_CONSOLE_TEXT << "Arming failed:" << arm_result << NORMAL_CONSOLE_TEXT
                  << std::endl;
        return 1;
    }

    // Take off
    std::cout << "Taking off..." << std::endl;
    const Action::Result takeoff_result = action.takeoff();
    if (takeoff_result != Action::Result::Success) {
        std::cout << ERROR_CONSOLE_TEXT << "Takeoff failed:" << takeoff_result
                  << NORMAL_CONSOLE_TEXT << std::endl;
        return 1;
    }

    sleep_for(seconds(30));

    std::cout << "Landing..." << std::endl;
    const Action::Result land_result = action.land();
    if (land_result != Action::Result::Success) {
        std::cout << ERROR_CONSOLE_TEXT << "Land failed:" << land_result << NORMAL_CONSOLE_TEXT
                  << std::endl;
        return 1;
    }

    // Check if vehicle is still in air
    while (telemetry.in_air()) {
        std::cout << "Vehicle is landing..." << std::endl;
        sleep_for(seconds(1));
    }
    std::cout << "Landed!" << std::endl;

    // We are relying on auto-disarming but let's keep watching the telemetry for a bit longer.
    sleep_for(seconds(3));
    std::cout << "Finished..." << std::endl;
    return 0;
}

*/
/**
 * @file fly_qgc_mission.cpp
 *
 * @brief Demonstrates how to import mission items from QGroundControl plan,
 * and fly them using the MAVSDK.
 *
 * Steps to run this example:
 * 1. (a) Create a Mission in QGroundControl and save them to a file (.plan) (OR)
 *    (b) Use a pre-created sample mission plan in "plugins/mission/qgroundcontrol_sample.plan".
 *    Click
 * [here](https://user-images.githubusercontent.com/26615772/31763673-972c5bb6-b4dc-11e7-8ff0-f8b39b6b88c3.png)
 * to see what sample mission plan in QGroundControl looks like.
 * 2. Run the example by passing path of the QGC mission plan as argument (By default, sample
 * mission plan is imported).
 *
 * Example description:
 * 1. Imports QGC mission items from .plan file.
 * 2. Uploads mission items to vehicle.
 * 3. Starts mission from first mission item.
 * 4. Commands RTL once QGC Mission is accomplished.
 *
 * @author Shakthi Prashanth M <shakthi.prashanth.m@intel.com>,
 *         Julian Oes <julian@oes.ch>
 * @date 2018-02-04
 */

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/mission/mission.h>
#include <mavsdk/plugins/telemetry/telemetry.h>

#include <functional>
#include <future>
#include <iostream>
#include <memory>

#define ERROR_CONSOLE_TEXT "\033[31m" // Turn text on console red
#define TELEMETRY_CONSOLE_TEXT "\033[34m" // Turn text on console blue
#define NORMAL_CONSOLE_TEXT "\033[0m" // Restore normal console colour

using namespace mavsdk;
using namespace std::chrono; // for seconds(), milliseconds()
using namespace std::this_thread; // for sleep_for()

// Handles Action's result
inline void handle_action_err_exit(Action::Result result, const std::string& message);
// Handles Mission's result
inline void handle_mission_err_exit(Mission::Result result, const std::string& message);
// Handles Connection result
inline void handle_connection_err_exit(ConnectionResult result, const std::string& message);

void usage(std::string bin_name)
{
    std::cout << NORMAL_CONSOLE_TEXT << "Usage : " << bin_name
              << " <connection_url> [path of QGC Mission plan]" << std::endl
              << "Connection URL format should be :" << std::endl
              << " For TCP : tcp://[server_host][:server_port]" << std::endl
              << " For UDP : udp://[bind_host][:bind_port]" << std::endl
              << " For Serial : serial:///path/to/serial/dev[:baudrate]" << std::endl
              << "For example, to connect to the simulator use URL: udp://:14540" << std::endl;
}

void hg_callback(Telemetry::HoverGamesStatus_s hgStatus)
{
    std::cout << "Global SM: "<< (int)hgStatus.HoverGames_SM << "\n"
              << "Active SM: "<< (int)hgStatus.HoverGames_ActiveSM << std::endl;
}

void gps_callback(Telemetry::GpsInfo gps_info)
{
        std::cout << "Alt: "<< (int)gps_info.alt << "\n"
                  << "Lat: "<< (int)gps_info.lat << "\n"
                  << "Lon: "<< (int)gps_info.lon << std::endl;
}

int main(int argc, char** argv)
{
    Mavsdk mavsdk;
    std::string connection_url;
    ConnectionResult connection_result;

    // Locate path of QGC Sample plan
    std::string qgc_plan = "mission.plan";

    if (argc != 2 && argc != 3) {
        usage(argv[0]);
        return 1;
    }

    connection_url = argv[1];
    if (argc == 3) {
        qgc_plan = argv[2];
    }

    std::cout << "Connection URL: " << connection_url << std::endl;
    std::cout << "Importing mission from mission plan: " << qgc_plan << std::endl;

    {
        auto prom = std::make_shared<std::promise<void>>();
        auto future_result = prom->get_future();

        std::cout << "Waiting to discover system..." << std::endl;
        mavsdk.subscribe_on_new_system([&mavsdk, prom]() {
            const auto system = mavsdk.systems().at(0);

            if (system->is_connected()) {
                std::cout << "Discovered system" << std::endl;
                prom->set_value();
            } else {
                std::cout << "System timed out" << std::endl;
                std::cout << "Exiting." << std::endl;
                exit(0);
            }
        });

        connection_result = mavsdk.add_any_connection(connection_url);
        handle_connection_err_exit(connection_result, "Connection failed: ");

        future_result.get();
    }

    auto system = mavsdk.systems().at(0);
    auto action = Action{system};
    auto mission = Mission{system};
    auto telemetry = Telemetry{system};

    while (!telemetry.health_all_ok()) {
        std::cout << "Waiting for system to be ready" << std::endl;
        sleep_for(seconds(1));
    }

    std::cout << "System ready" << std::endl;

    // Import Mission items from QGC plan
    std::pair<Mission::Result, Mission::MissionPlan> import_res =
        mission.import_qgroundcontrol_mission(qgc_plan);
    handle_mission_err_exit(import_res.first, "Failed to import mission items: ");

    if (import_res.second.mission_items.size() == 0) {
        std::cerr << "No missions! Exiting..." << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Found " << import_res.second.mission_items.size()
              << " mission items in the given QGC plan." << std::endl;

    {
        std::cout << "Uploading mission..." << std::endl;
        // Wrap the asynchronous upload_mission function using std::future.
        auto prom = std::make_shared<std::promise<Mission::Result>>();
        auto future_result = prom->get_future();
        mission.upload_mission_async(
            import_res.second, [prom](Mission::Result result) { prom->set_value(result); });

        const Mission::Result result = future_result.get();
        handle_mission_err_exit(result, "Mission upload failed: ");
        std::cout << "Mission uploaded." << std::endl;
    }
    const Telemetry::Result set_rate_result_hg = telemetry.set_rate_HoverGamesStatus(1.0);
    if (set_rate_result_hg != Telemetry::Result::Success) {
        std::cout << ERROR_CONSOLE_TEXT << "Setting rate failed:" << set_rate_result_hg
                  << NORMAL_CONSOLE_TEXT << std::endl;
        return 1;
    }

    const Telemetry::Result set_rate_result_hg_gps = telemetry.set_rate_gps_info(1.0);
    if (set_rate_result_hg_gps != Telemetry::Result::Success) {
        std::cout << ERROR_CONSOLE_TEXT << "Setting rate failed:" << set_rate_result_hg
                  << NORMAL_CONSOLE_TEXT << std::endl;
        return 1;
    }

    telemetry.subscribe_HoverGamesStatus(hg_callback);

    telemetry.subscribe_gps_info(gps_callback);

    std::cout << "Arming..." << std::endl;
    const Action::Result arm_result = action.arm();
    handle_action_err_exit(arm_result, "Arm failed: ");
    std::cout << "Armed." << std::endl;

    // Before starting the mission subscribe to the mission progress.
    mission.subscribe_mission_progress([](Mission::MissionProgress mission_progress) {
        std::cout << "Mission status update: " << mission_progress.current << " / "
                  << mission_progress.total << std::endl;
    });

    {
        std::cout << "Starting mission." << std::endl;
        auto prom = std::make_shared<std::promise<Mission::Result>>();
        auto future_result = prom->get_future();
        mission.start_mission_async([prom](Mission::Result result) {
            prom->set_value(result);
            std::cout << "Started mission." << std::endl;
        });

        const Mission::Result result = future_result.get();
        handle_mission_err_exit(result, "Mission start failed: ");
    }

    while (!mission.is_mission_finished().second) {
        sleep_for(seconds(1));
    }

    // Wait for some time.
    sleep_for(seconds(5));

    {
        // Mission complete. Command RTL to go home.
        std::cout << "Commanding RTL..." << std::endl;
        const Action::Result result = action.return_to_launch();
        if (result != Action::Result::Success) {
            std::cout << "Failed to command RTL (" << result << ")" << std::endl;
        } else {
            std::cout << "Commanded RTL." << std::endl;
        }
    }

    return 0;
}

inline void handle_action_err_exit(Action::Result result, const std::string& message)
{
    if (result != Action::Result::Success) {
        std::cerr << ERROR_CONSOLE_TEXT << message << result << NORMAL_CONSOLE_TEXT << std::endl;
        exit(EXIT_FAILURE);
    }
}

inline void handle_mission_err_exit(Mission::Result result, const std::string& message)
{
    if (result != Mission::Result::Success) {
        std::cerr << ERROR_CONSOLE_TEXT << message << result << NORMAL_CONSOLE_TEXT << std::endl;
        exit(EXIT_FAILURE);
    }
}

// Handles connection result
inline void handle_connection_err_exit(ConnectionResult result, const std::string& message)
{
    if (result != ConnectionResult::Success) {
        std::cerr << ERROR_CONSOLE_TEXT << message << result << NORMAL_CONSOLE_TEXT << std::endl;
        exit(EXIT_FAILURE);
    }
}
