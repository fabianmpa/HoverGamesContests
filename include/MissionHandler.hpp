#include "Types.hpp"

class MissionHandler
{
public:
  MissionHandler() = default;
  ~MissionHandler() = default;
  std::function<void(Mission::MissionProgress)> MissionProgressCallback = [this](Mission::MissionProgress mission_progress) {
    currentMisionProgress.current = mission_progress.current;
    currentMisionProgress.total = mission_progress.total;
  };

private:
  missionProgress_s currentMisionProgress;
};