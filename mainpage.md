@startuml
title HoverGames State Machine Module
[*] --> StandbyState
StandbyState   --> PreActiveState : <i>arming_state == ARMING_STATE_ARMED</i>
PreActiveState --> ActiveState    : <i>actuator.armed == hovergamesArmingStates.armed</i> **AND** \n <i>battery.voltage_filtered_v >= minimumVoltageFiltered</i> **AND** \n  <i>battery.remaining >= minimumBatteryRemaining </i>
StandbyState   --> FaultedState   : <i>Faulted transition condition</i>
PreActiveState --> FaultedState   : <i>Faulted transition condition</i>
ActiveState    --> FaultedState   : <i>Faulted transition condition</i>
FaultedState   --> StandbyState   : <i>Standby transiton condition</i>

state ActiveState {
 PausedState        --> RecordingState        : <i>distance.current_distance >= fiveMeters</i>   **AND** \n <i>distance.current_distance <= twentyMeters</i>
 PausedState        --> ImageCamptureState    : <i>distance.current_distance > twentyMeters</i>  **AND** \n <i>distance.current_distance <= thirtyFourMeters</i>
 RecordingState     --> PausedState           : <i>distance.current_distance < fiveMeters</i>
 RecordingState     --> ImageCamptureState    : <i>distance.current_distance > twentyMeters</i>  **AND** \n <i>distance.current_distance <= thirtyFourMeters</i>
 ImageCamptureState --> PausedState           : <i>distance.current_distance < fiveMeters</i>
 ImageCamptureState --> RecordingState        : <i>distance.current_distance >= fiveMeters</i>   **AND** \n <i>distance.current_distance <= twentyMeters</i>
}

legend
======
Faulted transition condition
----
""<i>battery.warning</i>                          != BATTERY_WARNING_NONE""
""**OR** <i>vehStatus.engine_failure</i>          == hovergamesFailures.engineFailure""
""**OR** <i>vehStatus.failsafe</i>                == hovergamesFailures.failSafe""
""**OR** <i>vehStatus.failure_detector_status</i> != FAILURE_NONE""
""**OR** <i>vehStatus.mission_failure</i>         == hovergamesFailures.missionFailure""
======
Standby transition condition
----
""<i>battery.warning</i>                          == battery_status_s::BATTERY_WARNING_NONE""
""**OR** <i>vehStatus.engine_failure</i>          != _hovergamesFailures.engineFailure""
""**OR** <i>vehStatus.failsafe</i>                != _hovergamesFailures.failSafe""
""**OR** <i>vehStatus.failure_detector_status</i> == vehicle_status_s::FAILURE_NONE""
""**OR** <i>vehStatus.mission_failure</i>         != _hovergamesFailures.missionFailure""
end legend
@enduml