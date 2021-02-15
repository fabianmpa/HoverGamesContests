//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: HG_SM.cpp
//
// Code generated for Simulink model 'HG_SM'.
//
// Model version                  : 1.2
// Simulink Coder version         : 9.4 (R2020b) 29-Jul-2020
// C/C++ source code generated on : Tue Jan 26 21:36:23 2021
//
// Target selection: ert.tlc
// Embedded hardware selection: NXP->Cortex-M4
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
// Validation result: Not run
//
#include "HG_SM.h"

// Named constants for Chart: '<Root>/Chart'
const uint8_T ACTIVE_STATE = 2U;
const uint8_T FAULTED_STATE = 3U;
const uint8_T IMAGE_CAPTURE_STATE = 1U;
const uint8_T IN_Active = 1U;
const uint8_T IN_Faulted = 2U;
const uint8_T IN_ImageCapture = 1U;
const uint8_T IN_NO_ACTIVE_CHILD = 0U;
const uint8_T IN_Paused = 2U;
const uint8_T IN_PreActive = 3U;
const uint8_T IN_Recording = 3U;
const uint8_T IN_Standby = 4U;
const uint8_T PAUSED_STATE = 2U;
const uint8_T PREACTIVE_STATE = 1U;
const uint8_T RECORDING_STATE = 0U;
const uint8_T STANDBY_STATE = 0U;

#ifndef UCHAR_MAX
#include <limits.h>
#endif

#if ( UCHAR_MAX != (0xFFU) ) || ( SCHAR_MAX != (0x7F) )
#error Code was generated for compiler with different sized uchar/char. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( USHRT_MAX != (0xFFFFU) ) || ( SHRT_MAX != (0x7FFF) )
#error Code was generated for compiler with different sized ushort/short. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( UINT_MAX != (0xFFFFFFFFU) ) || ( INT_MAX != (0x7FFFFFFF) )
#error Code was generated for compiler with different sized uint/int. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( ULONG_MAX != (0xFFFFFFFFU) ) || ( LONG_MAX != (0x7FFFFFFF) )
#error Code was generated for compiler with different sized ulong/long. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

// Skipping ulong_long/long_long check: insufficient preprocessor integer range. 

// Model step function
void HG_SMModelClass::step()
{
  g_dsp_private_ExponentialMoving *obj;
  px4_Bus_actuator_armed b_varargout_2_1;
  px4_Bus_battery_status b_varargout_2;
  px4_Bus_distance_sensor b_varargout_2_2;
  px4_Bus_hovergames rtb_BusAssignment;
  px4_Bus_vehicle_status b_varargout_2_0;
  real32_T lambda;
  real32_T pmLocal;
  real32_T pwLocal;
  boolean_T b_varargout_1;
  boolean_T b_varargout_1_0;
  boolean_T b_varargout_1_1;
  boolean_T flag;
  boolean_T rtb_LogicalOperator;

  // MATLABSystem: '<S2>/SourceBlock' incorporates:
  //   Inport: '<S8>/In1'

  rtb_LogicalOperator = uORB_read_step(rtDW.obj_n.orbMetadataObj,
    &rtDW.obj_n.eventStructObj, &rtb_BusAssignment, false, 1.0);

  // Outputs for Enabled SubSystem: '<S2>/Enabled Subsystem' incorporates:
  //   EnablePort: '<S8>/Enable'

  if (rtb_LogicalOperator) {
    rtDW.In1_dr = rtb_BusAssignment;
  }

  // End of MATLABSystem: '<S2>/SourceBlock'
  // End of Outputs for SubSystem: '<S2>/Enabled Subsystem'

  // MATLABSystem: '<S3>/SourceBlock' incorporates:
  //   Inport: '<S9>/In1'

  rtb_LogicalOperator = uORB_read_step(rtDW.obj_o.orbMetadataObj,
    &rtDW.obj_o.eventStructObj, &b_varargout_2, false, 1.0);

  // Outputs for Enabled SubSystem: '<S3>/Enabled Subsystem' incorporates:
  //   EnablePort: '<S9>/Enable'

  if (rtb_LogicalOperator) {
    rtDW.In1 = b_varargout_2;
  }

  // End of Outputs for SubSystem: '<S3>/Enabled Subsystem'

  // MATLABSystem: '<S4>/SourceBlock' incorporates:
  //   Inport: '<S10>/In1'

  b_varargout_1 = uORB_read_step(rtDW.obj_e.orbMetadataObj,
    &rtDW.obj_e.eventStructObj, &b_varargout_2_0, false, 1.0);

  // Outputs for Enabled SubSystem: '<S4>/Enabled Subsystem' incorporates:
  //   EnablePort: '<S10>/Enable'

  if (b_varargout_1) {
    rtDW.In1_d = b_varargout_2_0;
  }

  // End of Outputs for SubSystem: '<S4>/Enabled Subsystem'

  // MATLABSystem: '<S5>/SourceBlock' incorporates:
  //   Inport: '<S11>/In1'

  b_varargout_1_0 = uORB_read_step(rtDW.obj_l.orbMetadataObj,
    &rtDW.obj_l.eventStructObj, &b_varargout_2_1, false, 1.0);

  // Outputs for Enabled SubSystem: '<S5>/Enabled Subsystem' incorporates:
  //   EnablePort: '<S11>/Enable'

  if (b_varargout_1_0) {
    rtDW.In1_l = b_varargout_2_1;
  }

  // End of Outputs for SubSystem: '<S5>/Enabled Subsystem'

  // MATLABSystem: '<S6>/SourceBlock' incorporates:
  //   Inport: '<S12>/In1'

  b_varargout_1_1 = uORB_read_step(rtDW.obj_p.orbMetadataObj,
    &rtDW.obj_p.eventStructObj, &b_varargout_2_2, false, 1.0);

  // Outputs for Enabled SubSystem: '<S6>/Enabled Subsystem' incorporates:
  //   EnablePort: '<S12>/Enable'

  if (b_varargout_1_1) {
    rtDW.In1_o = b_varargout_2_2;
  }

  // End of Outputs for SubSystem: '<S6>/Enabled Subsystem'

  // MATLABSystem: '<Root>/Moving Average'
  if (rtDW.obj.ForgettingFactor != 0.9F) {
    flag = (rtDW.obj.isInitialized == 1);
    if (flag) {
      rtDW.obj.TunablePropsChanged = true;
    }

    rtDW.obj.ForgettingFactor = 0.9F;
  }

  if (rtDW.obj.TunablePropsChanged) {
    rtDW.obj.TunablePropsChanged = false;
    obj = rtDW.obj.pStatistic;
    flag = (obj->isInitialized == 1);
    if (flag) {
      obj->TunablePropsChanged = true;
    }

    rtDW.obj.pStatistic->ForgettingFactor = rtDW.obj.ForgettingFactor;
  }

  obj = rtDW.obj.pStatistic;
  if (obj->isInitialized != 1) {
    obj->isSetupComplete = false;
    obj->isInitialized = 1;
    obj->pwN = 1.0F;
    obj->pmN = 0.0F;
    obj->plambda = obj->ForgettingFactor;
    obj->isSetupComplete = true;
    obj->TunablePropsChanged = false;
    obj->pwN = 1.0F;
    obj->pmN = 0.0F;
  }

  if (obj->TunablePropsChanged) {
    obj->TunablePropsChanged = false;
    obj->plambda = obj->ForgettingFactor;
  }

  pwLocal = obj->pwN;
  pmLocal = obj->pmN;
  lambda = obj->plambda;
  pmLocal = (1.0F - 1.0F / pwLocal) * pmLocal + 1.0F / pwLocal *
    rtDW.In1_o.current_distance;
  obj->pwN = lambda * pwLocal + 1.0F;
  obj->pmN = pmLocal;

  // Logic: '<Root>/Logical Operator' incorporates:
  //   Logic: '<S3>/NOT'
  //   Logic: '<S4>/NOT'
  //   Logic: '<S5>/NOT'
  //   Logic: '<S6>/NOT'
  //   MATLABSystem: '<S3>/SourceBlock'
  //   MATLABSystem: '<S4>/SourceBlock'
  //   MATLABSystem: '<S5>/SourceBlock'
  //   MATLABSystem: '<S6>/SourceBlock'

  rtb_LogicalOperator = ((!rtb_LogicalOperator) || (!b_varargout_1) ||
    (!b_varargout_1_0) || (!b_varargout_1_1));

  // Chart: '<Root>/Chart' incorporates:
  //   TriggerPort: '<S1>/execute'

  if (rtb_LogicalOperator && (rtPrevZCX.Chart_Trig_ZCE != POS_ZCSIG)) {
    switch (rtDW.is_c3_HG_SM) {
     case IN_Active:
      rtDW.HoverGames_sm = ACTIVE_STATE;
      switch (rtDW.is_Active) {
       case IN_ImageCapture:
        rtDW.HoverGames_Activesm = IMAGE_CAPTURE_STATE;
        break;

       case IN_Paused:
        rtDW.HoverGames_Activesm = PAUSED_STATE;
        break;

       default:
        // case IN_Recording:
        rtDW.HoverGames_Activesm = RECORDING_STATE;
        break;
      }
      break;

     case IN_Faulted:
      rtDW.HoverGames_sm = FAULTED_STATE;
      break;

     case IN_PreActive:
      rtDW.HoverGames_sm = PREACTIVE_STATE;
      break;

     default:
      // case IN_Standby:
      rtDW.HoverGames_sm = STANDBY_STATE;
      break;
    }

    switch (rtDW.is_c3_HG_SM) {
     case IN_Active:
      if (rtDW.In1_d.engine_failure || (rtDW.In1.warning > 0) ||
          rtDW.In1_d.failsafe || (rtDW.In1.remaining < 0.4)) {
        rtDW.is_Active = IN_NO_ACTIVE_CHILD;
        rtDW.is_c3_HG_SM = IN_Faulted;
      } else if (!rtDW.In1_l.armed) {
        rtDW.is_Active = IN_NO_ACTIVE_CHILD;
        rtDW.is_c3_HG_SM = IN_PreActive;
      } else {
        switch (rtDW.is_Active) {
         case IN_ImageCapture:
          // DataTypeConversion: '<Root>/Data Type Conversion' incorporates:
          //   MATLABSystem: '<Root>/Moving Average'

          if (static_cast<uint32_T>(pmLocal) >= 1000U) {
            rtDW.is_Active = IN_Recording;
          } else {
            if ((static_cast<uint32_T>(pmLocal) < 100U) ||
                (rtDW.In1.voltage_filtered_v < 13.0F)) {
              rtDW.is_Active = IN_Paused;
            }
          }
          break;

         case IN_Paused:
          // DataTypeConversion: '<Root>/Data Type Conversion' incorporates:
          //   MATLABSystem: '<Root>/Moving Average'

          if (static_cast<uint32_T>(pmLocal) >= 100U) {
            rtDW.is_Active = IN_ImageCapture;
          }
          break;

         default:
          // DataTypeConversion: '<Root>/Data Type Conversion' incorporates:
          //   MATLABSystem: '<Root>/Moving Average'

          // case IN_Recording:
          if ((static_cast<uint32_T>(pmLocal) < 100U) ||
              (rtDW.In1.voltage_filtered_v < 13.0F)) {
            rtDW.is_Active = IN_Paused;
          } else {
            if (static_cast<uint32_T>(pmLocal) < 1000U) {
              rtDW.is_Active = IN_ImageCapture;
            }
          }
          break;
        }
      }
      break;

     case IN_Faulted:
      if ((!rtDW.In1_d.engine_failure) || (rtDW.In1.warning == 0) ||
          (!rtDW.In1_d.failsafe)) {
        rtDW.is_c3_HG_SM = IN_Standby;
      }
      break;

     case IN_PreActive:
      if (rtDW.In1_l.armed && (rtDW.In1_d.arming_state == 2)) {
        rtDW.is_c3_HG_SM = IN_Active;
        rtDW.is_Active = IN_Paused;
      } else if (!rtDW.In1_l.armed) {
        rtDW.is_c3_HG_SM = IN_Standby;
      } else {
        if (rtDW.In1_d.engine_failure || (rtDW.In1.warning > 0) ||
            rtDW.In1_d.failsafe || (rtDW.In1.remaining < 0.4)) {
          rtDW.is_c3_HG_SM = IN_Faulted;
        }
      }
      break;

     default:
      // case IN_Standby:
      if (rtDW.In1_l.ready_to_arm) {
        rtDW.is_c3_HG_SM = IN_PreActive;
      } else {
        if (rtDW.In1_d.engine_failure || (rtDW.In1.warning > 0) ||
            rtDW.In1_d.failsafe || (rtDW.In1.remaining < 0.4)) {
          rtDW.is_c3_HG_SM = IN_Faulted;
        }
      }
      break;
    }
  }

  rtPrevZCX.Chart_Trig_ZCE = rtb_LogicalOperator;

  // BusAssignment: '<Root>/Bus Assignment' incorporates:
  //   Inport: '<S8>/In1'

  rtb_BusAssignment = rtDW.In1_dr;
  rtb_BusAssignment.hovergames_sm = rtDW.HoverGames_sm;
  rtb_BusAssignment.hovergames_activesm = rtDW.HoverGames_Activesm;

  // MATLABSystem: '<S7>/SinkBlock' incorporates:
  //   BusAssignment: '<Root>/Bus Assignment'

  uORB_write_step(rtDW.obj_j.orbMetadataObj, &rtDW.obj_j.orbAdvertiseObj,
                  &rtb_BusAssignment);
}

// Model initialize function
void HG_SMModelClass::initialize()
{
  {
    dsp_simulink_MovingAverage *obj;
    g_dsp_private_ExponentialMoving *obj_0;
    px4_Bus_hovergames rtb_BusAssignment;
    real32_T varargin_2;
    boolean_T flag;
    rtPrevZCX.Chart_Trig_ZCE = POS_ZCSIG;

    // Chart: '<Root>/Chart'
    rtDW.is_c3_HG_SM = IN_Standby;

    // Start for MATLABSystem: '<S2>/SourceBlock'
    rtDW.obj_n.matlabCodegenIsDeleted = false;
    rtDW.obj_n.isSetupComplete = false;
    rtDW.obj_n.isInitialized = 1;
    rtDW.obj_n.orbMetadataObj = ORB_ID(hovergames);
    uORB_read_initialize(rtDW.obj_n.orbMetadataObj, &rtDW.obj_n.eventStructObj);
    rtDW.obj_n.isSetupComplete = true;

    // Start for MATLABSystem: '<S3>/SourceBlock'
    rtDW.obj_o.matlabCodegenIsDeleted = false;
    rtDW.obj_o.isSetupComplete = false;
    rtDW.obj_o.isInitialized = 1;
    rtDW.obj_o.orbMetadataObj = ORB_ID(battery_status);
    uORB_read_initialize(rtDW.obj_o.orbMetadataObj, &rtDW.obj_o.eventStructObj);
    rtDW.obj_o.isSetupComplete = true;

    // Start for MATLABSystem: '<S4>/SourceBlock'
    rtDW.obj_e.matlabCodegenIsDeleted = false;
    rtDW.obj_e.isSetupComplete = false;
    rtDW.obj_e.isInitialized = 1;
    rtDW.obj_e.orbMetadataObj = ORB_ID(vehicle_status);
    uORB_read_initialize(rtDW.obj_e.orbMetadataObj, &rtDW.obj_e.eventStructObj);
    rtDW.obj_e.isSetupComplete = true;

    // Start for MATLABSystem: '<S5>/SourceBlock'
    rtDW.obj_l.matlabCodegenIsDeleted = false;
    rtDW.obj_l.isSetupComplete = false;
    rtDW.obj_l.isInitialized = 1;
    rtDW.obj_l.orbMetadataObj = ORB_ID(actuator_armed);
    uORB_read_initialize(rtDW.obj_l.orbMetadataObj, &rtDW.obj_l.eventStructObj);
    rtDW.obj_l.isSetupComplete = true;

    // Start for MATLABSystem: '<S6>/SourceBlock'
    rtDW.obj_p.matlabCodegenIsDeleted = false;
    rtDW.obj_p.isSetupComplete = false;
    rtDW.obj_p.isInitialized = 1;
    rtDW.obj_p.orbMetadataObj = ORB_ID(distance_sensor);
    uORB_read_initialize(rtDW.obj_p.orbMetadataObj, &rtDW.obj_p.eventStructObj);
    rtDW.obj_p.isSetupComplete = true;

    // Start for MATLABSystem: '<Root>/Moving Average'
    rtDW.obj.matlabCodegenIsDeleted = true;
    rtDW.obj.isInitialized = 0;
    rtDW.obj.NumChannels = -1;
    rtDW.obj.matlabCodegenIsDeleted = false;
    flag = (rtDW.obj.isInitialized == 1);
    if (flag) {
      rtDW.obj.TunablePropsChanged = true;
    }

    rtDW.obj.ForgettingFactor = 0.9F;
    obj = &rtDW.obj;
    rtDW.obj.isSetupComplete = false;
    rtDW.obj.isInitialized = 1;
    rtDW.obj.NumChannels = 1;
    varargin_2 = rtDW.obj.ForgettingFactor;
    obj->_pobj0.isInitialized = 0;
    flag = (obj->_pobj0.isInitialized == 1);
    if (flag) {
      obj->_pobj0.TunablePropsChanged = true;
    }

    obj->_pobj0.ForgettingFactor = varargin_2;
    rtDW.obj.pStatistic = &obj->_pobj0;
    rtDW.obj.isSetupComplete = true;
    rtDW.obj.TunablePropsChanged = false;

    // End of Start for MATLABSystem: '<Root>/Moving Average'

    // InitializeConditions for MATLABSystem: '<Root>/Moving Average'
    obj_0 = rtDW.obj.pStatistic;
    if (obj_0->isInitialized == 1) {
      obj_0->pwN = 1.0F;
      obj_0->pmN = 0.0F;
    }

    // End of InitializeConditions for MATLABSystem: '<Root>/Moving Average'

    // Start for MATLABSystem: '<S7>/SinkBlock' incorporates:
    //   BusAssignment: '<Root>/Bus Assignment'

    rtDW.obj_j.matlabCodegenIsDeleted = false;
    rtDW.obj_j.isSetupComplete = false;
    rtDW.obj_j.isInitialized = 1;
    rtDW.obj_j.orbMetadataObj = ORB_ID(hovergames);
    uORB_write_initialize(rtDW.obj_j.orbMetadataObj, &rtDW.obj_j.orbAdvertiseObj,
                          &rtb_BusAssignment, 1);
    rtDW.obj_j.isSetupComplete = true;
  }
}

// Constructor
HG_SMModelClass::HG_SMModelClass() :
  rtDW(),
  rtPrevZCX(),
  rtM()
{
  // Currently there is no constructor body generated.
}

// Destructor
HG_SMModelClass::~HG_SMModelClass()
{
  // Currently there is no destructor body generated.
}

// Real-Time Model get method
HG_SMModelClass::RT_MODEL * HG_SMModelClass::getRTM()
{
  return (&rtM);
}

//
// File trailer for generated code.
//
// [EOF]
//
