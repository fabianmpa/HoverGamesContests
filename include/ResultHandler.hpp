#include "Types.hpp"

// Handles Action's result
void handle_action_err_exit(Action::Result result, const std::string& message);
// Handles Mission's result
void handle_mission_err_exit(Mission::Result result, const std::string& message);
// Handles Connection result
void handle_connection_err_exit(ConnectionResult result, const std::string& message);