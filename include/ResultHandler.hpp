#pragma once
#include "Types.hpp"

// Handles Action's result
void handle_action_err_exit(Action::Result result, const std::string& failMessage, const string& sccessMessage);
// Handles Mission's result
void handle_mission_err_result_exit(Mission::Result result, const std::string& failMessage, const string& succesMessage);

void handle_mission_err_exit(std::pair<Mission::Result, Mission::MissionPlan> import_res, const std::string& message);
// Handles Connection result
void handle_connection_err_exit(ConnectionResult result, const std::string& message);

void handle_telemetry_result(Telemetry::Result set_rate_result);