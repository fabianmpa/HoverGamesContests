#include "ActionHandler.hpp"

void ActionHandler::handle_action_err_exit(Action::Result result, const std::string& failMessage, const string& successMessage)
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

void ActionHandler::armVehicle(void)
{
    /*Arm vehicle*/
    const Action::Result arm_result = action.arm();
    handle_action_err_exit(arm_result, "Arm failed: ", "Armed.");
}

void ActionHandler::returnToLaunch(void)
{
    /*When mission is completed, send Return to Land command*/
    const Action::Result result = action.return_to_launch();
    handle_action_err_exit(result, "Failed to command RTL", "Commanded RTL.");
}