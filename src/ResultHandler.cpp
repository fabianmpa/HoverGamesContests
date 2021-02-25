#include "ResultHandler.hpp"

void handle_action_err_exit(Action::Result result, const std::string& failMessage, const string& successMessage)
{
    if (result != Action::Result::Success) {
        cerr << ERROR_CONSOLE_TEXT << failMessage << result << NORMAL_CONSOLE_TEXT << endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << successMessage << result << endl;
    }
}

void handle_mission_err_result_exit(Mission::Result result, const string& failMessage, const string& succesMessage)
{
    if (result != Mission::Result::Success) {
        cerr << ERROR_CONSOLE_TEXT << failMessage << result << NORMAL_CONSOLE_TEXT << endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << succesMessage << endl;
    }
}

void handle_mission_err_exit(std::pair<Mission::Result, Mission::MissionPlan> import_res, const string& message)
{
    if (import_res.first != Mission::Result::Success) {
        std::cerr << ERROR_CONSOLE_TEXT << message << import_res.first << NORMAL_CONSOLE_TEXT << std::endl;
        exit(EXIT_FAILURE);
    }
    else if (import_res.second.mission_items.size() == 0)
    {
        std::cerr << "No missions! Exiting..." << std::endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        std::cout << "Found " << import_res.second.mission_items.size() << " mission items in the QGC plan." << std::endl;
    }
}

void handle_connection_err_exit(ConnectionResult result, const string& message)
{
    if (result != ConnectionResult::Success) {
        std::cerr << ERROR_CONSOLE_TEXT << message << result << NORMAL_CONSOLE_TEXT << std::endl;
        exit(EXIT_FAILURE);
    }
}

void handle_telemetry_result(Telemetry::Result set_rate_result)
{
    if (set_rate_result != Telemetry::Result::Success)
    {
        std::cout << ERROR_CONSOLE_TEXT << "Setting rate failed:" << set_rate_result  << NORMAL_CONSOLE_TEXT << std::endl;
        exit(EXIT_FAILURE);
    }
}