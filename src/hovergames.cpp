#include <functional>
#include <future>
#include <memory>
#include "ResultHandler.hpp"
#include "CameraHandler.hpp"
#include "GpsHandler.hpp"
#include "MissionHandler.hpp"

using namespace std::this_thread;
using namespace std::chrono;

int main()
{
    Mavsdk mavsdk;
    std::string connection_url;
    ConnectionResult connection_result;
    CameraHandler camera_h;
    GpsHandler gps_h;
    MissionHandler mission_h;

    // Locate path of QGC Sample plan
    std::string qgc_plan = "mission.plan";

    #ifdef __aarch64__
    connection_url = "serial:///dev/ttymxc2:921600";
    #else
    connection_url = "udp://:14540";
    #endif

    {
        auto prom = std::make_shared<std::promise<void>>();
        auto future_result = prom->get_future();

        mavsdk.subscribe_on_new_system([&mavsdk, prom]()
        {
            const auto system = mavsdk.systems().at(0);

            if (system->is_connected())
            {
                std::cout << "Discovered system" << std::endl;
                prom->set_value();
            }
            else
            {
                std::cout << "System timed out, exiting" << std::endl;
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

    while (!telemetry.health_all_ok())
    {
        std::cout << "Waiting to be ready" << std::endl;
        sleep_for(seconds(1));
    }

    /*Import mission from this directory*/
    std::pair<Mission::Result, Mission::MissionPlan> import_res =
        mission.import_qgroundcontrol_mission(qgc_plan);
    handle_mission_err_exit(import_res.first, "Failed to import mission items: ");

    if (import_res.second.mission_items.size() == 0)
    {
        std::cerr << "No missions! Exiting..." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Found " << import_res.second.mission_items.size()
              << " mission items in the given QGC plan." << std::endl;

    {
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
    if (set_rate_result_hg != Telemetry::Result::Success)
    {
        std::cout << ERROR_CONSOLE_TEXT << "Setting rate failed:" << set_rate_result_hg
                  << NORMAL_CONSOLE_TEXT << std::endl;
        return 1;
    }

    /*Listen to the vehicle GPS topic with a frequency of 1 Hz*/
    const Telemetry::Result set_rate_result_hg_gps = telemetry.set_rate_gps_info(1.0);
    if (set_rate_result_hg_gps != Telemetry::Result::Success)
    {
        std::cout << ERROR_CONSOLE_TEXT << "Setting rate failed:" << set_rate_result_hg
                  << NORMAL_CONSOLE_TEXT << std::endl;
        return 1;
    }

    /*every time we get a new message, StateMachine is executed*/
    telemetry.subscribe_HoverGamesStatus(camera_h.StatusCallback);
    /*Currently Gps information is only displayed*/
    telemetry.subscribe_gps_info(gps_h.GpsCallback);

    std::cout << "Arming..." << std::endl;
    const Action::Result arm_result = action.arm();
    handle_action_err_exit(arm_result, "Arm failed: ");
    std::cout << "Armed." << std::endl;

    /*Subscribe to the mission progress*/
    mission.subscribe_mission_progress(mission_h.MissionProgressCallback);

    {
        std::cout << "Starting mission." << std::endl;
        auto prom = std::make_shared<std::promise<Mission::Result>>();
        auto future_result = prom->get_future();
        mission.start_mission_async([prom](Mission::Result result)
        {
            prom->set_value(result);
            std::cout << "Started mission." << std::endl;
        });

        const Mission::Result result = future_result.get();
        handle_mission_err_exit(result, "Mission start failed: ");
    }

    while (!mission.is_mission_finished().second)
    {
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
