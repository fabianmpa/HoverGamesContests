#include "Types.hpp"

class MissionHandler
{
public:
  MissionHandler() = default;
  ~MissionHandler() = default;
  void UploadMission(Mission& mission,string qgc_plan);
  void StartMission(Mission& mission);
  auto StartMissionLambda(shared_ptr<promise<Mission::Result>> prom);
  function<void(Mission::MissionProgress)> MissionProgressCallback = [this](Mission::MissionProgress mission_progress)
  {
    currentMisionProgress.current = mission_progress.current;
    currentMisionProgress.total = mission_progress.total;
  };

private:
  missionProgress_s currentMisionProgress;
};