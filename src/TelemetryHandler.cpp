#include "TelemetryHandler.hpp"

void TelemetryHandler::SetSubscriptionRates(void)
{
    /*Listen to the hovergames topic with a frequency of 1 Hz*/
    const Telemetry::Result set_rate_result_hg = telemetry.set_rate_HoverGamesStatus(1.0);
    handle_telemetry_result(set_rate_result_hg);

    /*Listen to the vehicle GPS topic with a frequency of 1 Hz*/
    const Telemetry::Result set_rate_result_hg_gps = telemetry.set_rate_gps_info(1.0);
    handle_telemetry_result(set_rate_result_hg_gps);
}

void TelemetryHandler::StartSubscription(void)
{
  /*every time we get a new message, StateMachine is executed*/
  telemetry.subscribe_HoverGamesStatus(HovergamesCallback);
  /*Read Gps information*/
  telemetry.subscribe_gps_info(GpsCallback);
}

void TelemetryHandler::handle_telemetry_result(Telemetry::Result set_rate_result)
{
    if (set_rate_result != Telemetry::Result::Success)
    {
        std::cout << ERROR_CONSOLE_TEXT << "Setting rate failed:" << set_rate_result  << NORMAL_CONSOLE_TEXT << std::endl;
        exit(EXIT_FAILURE);
    }
}

bool TelemetryHandler::IsHealthOk(void)
{
    return telemetry.health_all_ok();
}