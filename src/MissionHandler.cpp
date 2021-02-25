#include "MissionHandler.hpp"
#include "ResultHandler.hpp"

void MissionHandler::UploadMission(Mission& mission,string qgc_plan)
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

void MissionHandler::StartMission(Mission& mission)
{
   auto prom = std::make_shared<std::promise<Mission::Result>>();
   auto future_result = prom->get_future();
   mission.start_mission_async(StartMissionLambda(prom));
   const Mission::Result result = future_result.get();
   handle_mission_err_result_exit(result, "Mission start failed: ", "Mission start succeded");
}

