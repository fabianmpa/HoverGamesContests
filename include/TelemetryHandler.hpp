#include "Types.hpp"

class TelemetryHandler
{
public:
  TelemetryHandler(auto &system) : telemetry{system} {};
  ~TelemetryHandler() = default;
  void SetSubscriptionRates(void);
  void StartSubscription(void);
  bool IsHealthOk(void);

private:
  void handle_telemetry_result(Telemetry::Result set_rate_result);
  function<void(Telemetry::GpsInfo)> GpsCallback = [this](Telemetry::GpsInfo gps_info) {
    currentGps.lon = gps_info.lon;
    currentGps.lat = gps_info.lat;
    currentGps.alt = gps_info.alt;
  };
  function<void(Telemetry::HoverGamesStatus_s)> HovergamesCallback = [this](Telemetry::HoverGamesStatus_s hgStatus) {
    currentHovergames.HoverGames_SM = hgStatus.HoverGames_SM;
    currentHovergames.HoverGames_ActiveSM = hgStatus.HoverGames_ActiveSM;
  };
  Telemetry telemetry;
  hovergames_s currentHovergames;
  gps_s currentGps;
};