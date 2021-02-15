#include <thread>
#include <mutex>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <iostream>

using namespace mavsdk;
using namespace std::this_thread;
using namespace std::chrono;


void usage(std::string bin_name);

void component_discovered(ComponentType component_type);

void worker(int index);