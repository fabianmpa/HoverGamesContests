#include "Types.hpp"

void SubscribeNewSystem(Mavsdk& mavsdk,string& connection_url);
void handle_connection_err_exit(ConnectionResult result, const std::string& message);
