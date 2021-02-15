#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/mission/mission.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include "hovergames.h"
#include "navqcontrol.h"

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

    /*Impor mission from this directory*/
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
        auto prom = std::make_shared<std::promise<Mission::Result>>();
        auto future_result = prom->get_future();
        mission.upload_mission_async(
            import_res.second, [prom](Mission::Result result) { prom->set_value(result); });

        const Mission::Result result = future_result.get();
        handle_mission_err_exit(result, "Mission upload failed: ");
        std::cout << "Mission uploaded." << std::endl;
    }

    /*Listen to the hovergames topic with a frequency of 1 Hz*/
    const Telemetry::Result set_rate_result_hg = telemetry.set_rate_HoverGamesStatus(1.0);
    if (set_rate_result_hg != Telemetry::Result::Success) {
        std::cout << ERROR_CONSOLE_TEXT << "Setting rate failed:" << set_rate_result_hg
                  << NORMAL_CONSOLE_TEXT << std::endl;
        return 1;
    }
    
    /*Listen to the vehicle GPS topic with a frequency of 1 Hz*/
    const Telemetry::Result set_rate_result_hg_gps = telemetry.set_rate_gps_info(1.0);
    if (set_rate_result_hg_gps != Telemetry::Result::Success) {
        std::cout << ERROR_CONSOLE_TEXT << "Setting rate failed:" << set_rate_result_hg
                  << NORMAL_CONSOLE_TEXT << std::endl;
        return 1;
    }

    /*every time we get a new message, StateMachine is executed*/
    telemetry.subscribe_HoverGamesStatus(StateMachine);

    /*Currently Gps information is only displayed*/
    telemetry.subscribe_gps_info(gps_callback);

    std::cout << "Arming..." << std::endl;
    const Action::Result arm_result = action.arm();
    handle_action_err_exit(arm_result, "Arm failed: ");
    std::cout << "Armed." << std::endl;

    /*Subscribe to the mission progress*/
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

    /*Wait for 20 sec*/
    sleep_for(seconds(20));

    {
        /*When mission is completed, send Return to Land command*/
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

inline void handle_connection_err_exit(ConnectionResult result, const std::string& message)
{
    if (result != ConnectionResult::Success) {
        std::cerr << ERROR_CONSOLE_TEXT << message << result << NORMAL_CONSOLE_TEXT << std::endl;
        exit(EXIT_FAILURE);
    }
}
