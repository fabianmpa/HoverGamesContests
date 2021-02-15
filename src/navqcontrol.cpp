#include "include/navqcontrol.h"

void StateMachine(Telemetry::HoverGamesStatus_s hgStatus)
{
    /*Here goes the state machine hanling the Coral Camera*/
}

void gps_callback(Telemetry::GpsInfo gps_info)
{
        std::cout << "Alt: "<< (int)gps_info.alt << "\n"
                  << "Lat: "<< (int)gps_info.lat << "\n"
                  << "Lon: "<< (int)gps_info.lon << std::endl;
}
