//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: HG_SM.h
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
#ifndef RTW_HEADER_HG_SM_h_
#define RTW_HEADER_HG_SM_h_
#include "rtwtypes.h"
#include "zero_crossing_types.h"
#include <uORB/topics/hovergames.h>
#include <uORB/topics/battery_status.h>
#include <uORB/topics/vehicle_status.h>
#include <uORB/topics/actuator_armed.h>
#include <uORB/topics/distance_sensor.h>
#include <poll.h>
#include <uORB/uORB.h>
#include "rtwtypes.h"
#include "zero_crossing_types.h"
#include "MW_uORB_Read.h"
#include "MW_uORB_Write.h"

// Model Code Variants

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef struct_tag_56f1cdaGgiZ2ZmtsGAepW
#define struct_tag_56f1cdaGgiZ2ZmtsGAepW

struct tag_56f1cdaGgiZ2ZmtsGAepW
{
  int32_T isInitialized;
  boolean_T isSetupComplete;
  boolean_T TunablePropsChanged;
  real32_T ForgettingFactor;
  real32_T pwN;
  real32_T pmN;
  real32_T plambda;
};

#endif                                 //struct_tag_56f1cdaGgiZ2ZmtsGAepW

#ifndef typedef_g_dsp_private_ExponentialMoving
#define typedef_g_dsp_private_ExponentialMoving

typedef tag_56f1cdaGgiZ2ZmtsGAepW g_dsp_private_ExponentialMoving;

#endif                                 //typedef_g_dsp_private_ExponentialMoving

#ifndef struct_tag_YgA2umAxY1QaUdNJg8JkbB
#define struct_tag_YgA2umAxY1QaUdNJg8JkbB

struct tag_YgA2umAxY1QaUdNJg8JkbB
{
  int32_T __dummy;
};

#endif                                 //struct_tag_YgA2umAxY1QaUdNJg8JkbB

#ifndef typedef_e_px4_internal_block_SampleTime
#define typedef_e_px4_internal_block_SampleTime

typedef tag_YgA2umAxY1QaUdNJg8JkbB e_px4_internal_block_SampleTime;

#endif                                 //typedef_e_px4_internal_block_SampleTime

#ifndef struct_tag_b7Prt6FPM0T5j3THBzNIjG
#define struct_tag_b7Prt6FPM0T5j3THBzNIjG

struct tag_b7Prt6FPM0T5j3THBzNIjG
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  e_px4_internal_block_SampleTime SampleTimeHandler;
  pollfd_t eventStructObj;
  orb_metadata_t * orbMetadataObj;
};

#endif                                 //struct_tag_b7Prt6FPM0T5j3THBzNIjG

#ifndef typedef_px4_internal_block_Subscriber
#define typedef_px4_internal_block_Subscriber

typedef tag_b7Prt6FPM0T5j3THBzNIjG px4_internal_block_Subscriber;

#endif                                 //typedef_px4_internal_block_Subscriber

#ifndef struct_tag_eDcrHY7lOcUxDQxCboL0iC
#define struct_tag_eDcrHY7lOcUxDQxCboL0iC

struct tag_eDcrHY7lOcUxDQxCboL0iC
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  orb_advert_t orbAdvertiseObj;
  orb_metadata_t * orbMetadataObj;
};

#endif                                 //struct_tag_eDcrHY7lOcUxDQxCboL0iC

#ifndef typedef_px4_internal_block_Publisher
#define typedef_px4_internal_block_Publisher

typedef tag_eDcrHY7lOcUxDQxCboL0iC px4_internal_block_Publisher;

#endif                                 //typedef_px4_internal_block_Publisher

#ifndef struct_tag_PMfBDzoakfdM9QAdfx2o6D
#define struct_tag_PMfBDzoakfdM9QAdfx2o6D

struct tag_PMfBDzoakfdM9QAdfx2o6D
{
  uint32_T f1[8];
};

#endif                                 //struct_tag_PMfBDzoakfdM9QAdfx2o6D

#ifndef typedef_cell_wrap
#define typedef_cell_wrap

typedef tag_PMfBDzoakfdM9QAdfx2o6D cell_wrap;

#endif                                 //typedef_cell_wrap

#ifndef struct_tag_m1xTwSPz71iHpsYxepaAlB
#define struct_tag_m1xTwSPz71iHpsYxepaAlB

struct tag_m1xTwSPz71iHpsYxepaAlB
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  boolean_T TunablePropsChanged;
  cell_wrap inputVarSize;
  real32_T ForgettingFactor;
  g_dsp_private_ExponentialMoving *pStatistic;
  int32_T NumChannels;
  g_dsp_private_ExponentialMoving _pobj0;
};

#endif                                 //struct_tag_m1xTwSPz71iHpsYxepaAlB

#ifndef typedef_dsp_simulink_MovingAverage
#define typedef_dsp_simulink_MovingAverage

typedef tag_m1xTwSPz71iHpsYxepaAlB dsp_simulink_MovingAverage;

#endif                                 //typedef_dsp_simulink_MovingAverage

// Class declaration for model HG_SM
class HG_SMModelClass {
  // public data and function members
 public:
  // Block signals and states (default storage) for system '<Root>'
  typedef struct {
    px4_Bus_battery_status In1;        // '<S9>/In1'
    dsp_simulink_MovingAverage obj;    // '<Root>/Moving Average'
    px4_Bus_vehicle_status In1_d;      // '<S10>/In1'
    px4_Bus_distance_sensor In1_o;     // '<S12>/In1'
    px4_internal_block_Subscriber obj_p;// '<S6>/SourceBlock'
    px4_internal_block_Subscriber obj_l;// '<S5>/SourceBlock'
    px4_internal_block_Subscriber obj_e;// '<S4>/SourceBlock'
    px4_internal_block_Subscriber obj_o;// '<S3>/SourceBlock'
    px4_internal_block_Subscriber obj_n;// '<S2>/SourceBlock'
    px4_internal_block_Publisher obj_j;// '<S7>/SinkBlock'
    px4_Bus_actuator_armed In1_l;      // '<S11>/In1'
    px4_Bus_hovergames In1_dr;         // '<S8>/In1'
    uint8_T HoverGames_sm;             // '<Root>/Chart'
    uint8_T HoverGames_Activesm;       // '<Root>/Chart'
    uint8_T is_c3_HG_SM;               // '<Root>/Chart'
    uint8_T is_Active;                 // '<Root>/Chart'
  } DW;

  // Zero-crossing (trigger) state
  typedef struct {
    ZCSigState Chart_Trig_ZCE;         // '<Root>/Chart'
  } PrevZCX;

  // Real-time Model Data Structure
  struct RT_MODEL {
    const char_T * volatile errorStatus;
  };

  // model initialize function
  void initialize();

  // model step function
  void step();

  // Constructor
  HG_SMModelClass();

  // Destructor
  ~HG_SMModelClass();

  // Real-Time Model get method
  HG_SMModelClass::RT_MODEL * getRTM();

  // private data and function members
 private:
  // Block signals and states
  DW rtDW;
  PrevZCX rtPrevZCX;                   // Triggered events

  // Real-Time Model
  RT_MODEL rtM;
};

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S2>/NOT' : Unused code path elimination


//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'HG_SM'
//  '<S1>'   : 'HG_SM/Chart'
//  '<S2>'   : 'HG_SM/PX4 uORB Read'
//  '<S3>'   : 'HG_SM/PX4 uORB Read1'
//  '<S4>'   : 'HG_SM/PX4 uORB Read2'
//  '<S5>'   : 'HG_SM/PX4 uORB Read3'
//  '<S6>'   : 'HG_SM/PX4 uORB Read4'
//  '<S7>'   : 'HG_SM/PX4 uORB Write'
//  '<S8>'   : 'HG_SM/PX4 uORB Read/Enabled Subsystem'
//  '<S9>'   : 'HG_SM/PX4 uORB Read1/Enabled Subsystem'
//  '<S10>'  : 'HG_SM/PX4 uORB Read2/Enabled Subsystem'
//  '<S11>'  : 'HG_SM/PX4 uORB Read3/Enabled Subsystem'
//  '<S12>'  : 'HG_SM/PX4 uORB Read4/Enabled Subsystem'

#endif                                 // RTW_HEADER_HG_SM_h_

//
// File trailer for generated code.
//
// [EOF]
//
