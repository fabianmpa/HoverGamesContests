#include <functional>
#include <thread>
#include "CameraHandler.hpp"
#include "TelemetryHandler.hpp"
#include "MissionHandler.hpp"
#include "Utilities.hpp"
#include "ActionHandler.hpp"

int main()
{
    Mavsdk mavsdk;
    /*Set the name of the mission plan*/
    string qgc_plan{"mission.plan"};
    /*Set the name of the recording device*/
    string device {"/dev/video2"};
    #ifdef __aarch64__
    std::string connection_url = "serial:///dev/ttymxc2:921600";
    #else
    std::string connection_url = "udp://:14540";
    #endif
    /*Subscribe to a new system*/
    SubscribeNewSystem(mavsdk, connection_url);
    /*Create action, mission and telemetry handlers*/
    auto system = mavsdk.systems().at(0);
    CameraHandler cameraHandler{device};
    TelemetryHandler telemHandler{system};
    MissionHandler missionHandler{system};
    ActionHandler actionHandler{system};
    while (!telemHandler.IsHealthOk())
    {
        std::cout << "Waiting to be ready" << std::endl;
        sleep_for(seconds(1));
    }
    /*Upload mission*/
    missionHandler.UploadMission(qgc_plan);
    /*Set rates of subscriptions*/
    telemHandler.SetSubscriptionRates();
    /*Subscribe to Gps and hovergames messages*/
    telemHandler.StartSubscription();
    /*Initialize VideoCapture and VideoWriter objects*/
    cameraHandler.InitReader();
    cameraHandler.InitWriter();
    /*Initialize StateMachine thread*/
    std::thread StateMachine(&CameraHandler::StateMachine,&cameraHandler);
    /*Arm Vehicle*/
    actionHandler.armVehicle();
    /*Subscribe to the mission progress*/
    missionHandler.SubscribeMissionProgress();
    /*Start the mission*/
    missionHandler.StartMission();
    while (!missionHandler.IsMissionFinished())
    {}
    /*Run Camera State Machine*/
    StateMachine.join();
    actionHandler.returnToLaunch();
    return 0;
}
