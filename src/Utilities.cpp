#include "Utilities.hpp"
#include "ResultHandler.hpp"

auto SubscribeSystemLambda(Mavsdk& mavsdk, shared_ptr<promise<void>> prom)
{
  return [&mavsdk,prom]
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
  };
}

void SubscribeNewSystem(Mavsdk& mavsdk, string& connection_url)
{
    auto prom = std::make_shared<std::promise<void>>();
    auto future_result = prom->get_future();
    mavsdk.subscribe_on_new_system(SubscribeSystemLambda(mavsdk,prom));
    ConnectionResult connection_result = mavsdk.add_any_connection(connection_url);
    handle_connection_err_exit(connection_result, "Connection failed: ");
    future_result.get();
}

