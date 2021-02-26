#include "Types.hpp"

class ActionHandler
{
public:
  ActionHandler(auto& system): action{system}{};
  ~ActionHandler()=default;
  void handle_action_err_exit(Action::Result result, const std::string& failMessage, const string& successMessage);
  void armVehicle(void);
  void returnToLaunch();
private:
  Action action;
};
