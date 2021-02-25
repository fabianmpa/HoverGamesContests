#include <functional>
#include <thread>
#include "ResultHandler.hpp"
#include "CameraHandler.hpp"
#include "GpsHandler.hpp"
#include "MissionHandler.hpp"
#include "Utilities.hpp"

int main()
{
    Mavsdk mavsdk;
    CameraHandler camera_h;
    GpsHandler gps_h;
    MissionHandler mission_h;

    // Locate path of QGC Sample plan
    std::string qgc_plan = "mission.plan";
    #ifdef __aarch64__
    std::string connection_url = "serial:///dev/ttymxc2:921600";
    #else
    std::string connection_url = "udp://:14540";
    #endif

    SubscribeNewSystem(mavsdk, connection_url);

    /*Create action, mission and telemetry handlers*/
    auto system = mavsdk.systems().at(0);
    auto action = Action{system};
    auto mission = Mission{system};
    auto telemetry = Telemetry{system};



    while (!telemetry.health_all_ok())
    {
        std::cout << "Waiting to be ready" << std::endl;
        sleep_for(seconds(1));
    }

    mission_h.UploadMission(mission,qgc_plan);

    /*Listen to the hovergames topic with a frequency of 1 Hz*/
    const Telemetry::Result set_rate_result_hg = telemetry.set_rate_HoverGamesStatus(1.0);
    handle_telemetry_result(set_rate_result_hg);

    /*Listen to the vehicle GPS topic with a frequency of 1 Hz*/
    const Telemetry::Result set_rate_result_hg_gps = telemetry.set_rate_gps_info(1.0);
    handle_telemetry_result(set_rate_result_hg_gps);

    camera_h.InitReader();
    camera_h.InitWriter();
    /*every time we get a new message, StateMachine is executed*/
    telemetry.subscribe_HoverGamesStatus(camera_h.StatusCallback);
    /*Currently Gps information is only displayed*/
    telemetry.subscribe_gps_info(gps_h.GpsCallback);

    const Action::Result arm_result = action.arm();
    handle_action_err_exit(arm_result, "Arm failed: ", "Armed.");

    /*Subscribe to the mission progress*/
    mission.subscribe_mission_progress(mission_h.MissionProgressCallback);

    std::thread StateMachine(&CameraHandler::StateMachine,&camera_h);

    mission_h.StartMission(mission);

    while (!mission.is_mission_finished().second)
    {}

    /*Run Camera State Machine*/
    StateMachine.join();

    {
        /*When mission is completed, send Return to Land command*/
        const Action::Result result = action.return_to_launch();
        handle_action_err_exit(result, "Failed to command RTL", "Commanded RTL.");
    }

    return 0;
}
