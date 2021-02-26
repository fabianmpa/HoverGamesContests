#include "MissionHandler.hpp"

void MissionHandler::UploadMission(string qgc_plan)
{
  /*Import mission from this directory*/
  std::pair<Mission::Result, Mission::MissionPlan> import_res = mission.import_qgroundcontrol_mission(qgc_plan);
  handle_mission_err_exit(import_res, "Failed to import mission: ");

   auto prom = std::make_shared<std::promise<Mission::Result>>();
   auto future_result = prom->get_future();
   mission.upload_mission_async(import_res.second, [prom](Mission::Result result) { prom->set_value(result); });
   const Mission::Result result = future_result.get();
   handle_mission_err_result_exit(result, "Mission upload failed: ", "Mission uploaded.");
}

auto  MissionHandler::StartMissionLambda(shared_ptr<promise<Mission::Result>> prom)
{
  return [prom](Mission::Result result)
  {
      prom->set_value(result);
      std::cout << "Started mission." << std::endl;
  };
}

void MissionHandler::StartMission(void)
{
   auto prom = std::make_shared<std::promise<Mission::Result>>();
   auto future_result = prom->get_future();
   mission.start_mission_async(StartMissionLambda(prom));
   const Mission::Result result = future_result.get();
   handle_mission_err_result_exit(result, "Mission start failed: ", "Mission start succeded");
}

void MissionHandler::SubscribeMissionProgress(void)
{
  mission.subscribe_mission_progress(MissionProgressCallback);
}

bool MissionHandler::IsMissionFinished(void)
{
    return mission.is_mission_finished().second;
}

void MissionHandler::handle_mission_err_result_exit(Mission::Result result, const string& failMessage, const string& succesMessage)
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

void MissionHandler::handle_mission_err_exit(std::pair<Mission::Result, Mission::MissionPlan> import_res, const string& message)
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