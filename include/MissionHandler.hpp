#include "Types.hpp"

class MissionHandler
{
public:
  MissionHandler(auto& system): mission{system}{};
  ~MissionHandler() = default;
  void UploadMission(string qgc_plan);
  void StartMission(void);
  void SubscribeMissionProgress(void);
  bool IsMissionFinished(void);
private:
  auto StartMissionLambda(shared_ptr<promise<Mission::Result>> prom);
  void handle_mission_err_result_exit(Mission::Result result, const string& failMessage, const string& succesMessage);
  void handle_mission_err_exit(std::pair<Mission::Result, Mission::MissionPlan> import_res, const string& message);
  function<void(Mission::MissionProgress)> MissionProgressCallback = [this](Mission::MissionProgress mission_progress)
  {
    currentMisionProgress.current = mission_progress.current;
    currentMisionProgress.total = mission_progress.total;
  };
  missionProgress_s currentMisionProgress;
  Mission mission;
};