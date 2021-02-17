#include "Types.hpp"

class GpsHandler
{
public:
  GpsHandler() = default;
  ~GpsHandler() = default;
  std::function<void(Telemetry::GpsInfo)> GpsCallback = [this](Telemetry::GpsInfo gps_info) {
    currentGps.lon = gps_info.lon;
    currentGps.lat = gps_info.lat;
    currentGps.alt = gps_info.alt;
  };

private:
  gps_s currentGps;
};