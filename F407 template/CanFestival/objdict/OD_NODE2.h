
/* File generated by gen_cfile.py. Should not be modified. */

#ifndef OD_NODE2_H
#define OD_NODE2_H

#include "data.h"

/* Prototypes of function provided by object dictionnary */
UNS32 NODE2_valueRangeTest (UNS8 typeValue, void * value);
const indextable * NODE2_scanIndexOD (CO_Data *d, UNS16 wIndex, UNS32 * errorCode);

/* Master node data struct */
extern CO_Data NODE2_Data;

//extern UNS8 Node_ID;		/* Mapped at index 0x2000, subindex 0x00*/
//extern UNS16 CAN_Bitrate;		/* Mapped at index 0x2001, subindex 0x00*/
//extern UNS16 RS232_Baudrate;		/* Mapped at index 0x2002, subindex 0x00*/
//extern UNS16 Version_Software_Version;		/* Mapped at index 0x2003, subindex 0x01 */
//extern UNS16 Version_Hardware_Version;		/* Mapped at index 0x2003, subindex 0x02 */
//extern UNS16 Version_Application_Number;		/* Mapped at index 0x2003, subindex 0x03 */
//extern UNS16 Version_Application_Version;		/* Mapped at index 0x2003, subindex 0x04 */
//extern UNS16 Version_Basesector_Version;		/* Mapped at index 0x2003, subindex 0x05 */
//extern UNS64 Serial_Number;		/* Mapped at index 0x2004, subindex 0x00*/
//extern UNS16 RS232_Frame_Timeout;		/* Mapped at index 0x2005, subindex 0x00*/
//extern UNS16 USB_Frame_Timeout;		/* Mapped at index 0x2006, subindex 0x00*/
//extern UNS16 Miscellaneous_Configuration;		/* Mapped at index 0x2008, subindex 0x00*/
//extern UNS16 Internal_Dip_Switch_State;		/* Mapped at index 0x2009, subindex 0x00*/
//extern UNS16 CAN_Bitrate_Display;		/* Mapped at index 0x200A, subindex 0x00*/
//extern UNS32 Custom_persistent_memory_Custom_persistent_memory_1;		/* Mapped at index 0x200C, subindex 0x01 */
//extern UNS32 Custom_persistent_memory_Custom_persistent_memory_2;		/* Mapped at index 0x200C, subindex 0x02 */
//extern UNS32 Custom_persistent_memory_Custom_persistent_memory_3;		/* Mapped at index 0x200C, subindex 0x03 */
//extern UNS32 Custom_persistent_memory_Custom_persistent_memory_4;		/* Mapped at index 0x200C, subindex 0x04 */
//extern UNS16 Internal_DataRecorder_Control;		/* Mapped at index 0x2010, subindex 0x00*/
//extern UNS16 Internal_DataRecorder_Configuration;		/* Mapped at index 0x2011, subindex 0x00*/
//extern UNS16 Internal_DataRecorder_Sampling_Period;		/* Mapped at index 0x2012, subindex 0x00*/
//extern UNS16 Internal_DataRecorder_Number_of_Preceding_Samples;		/* Mapped at index 0x2013, subindex 0x00*/
//extern UNS16 Internal_DataRecorder_Number_of_Sampling_Variables;		/* Mapped at index 0x2014, subindex 0x00*/
//extern UNS16 Internal_DataRecorder_Index_of_Variables_Internal_DataRecorder_Index_of_Variable_1;		/* Mapped at index 0x2015, subindex 0x01 */
//extern UNS16 Internal_DataRecorder_Index_of_Variables_Internal_DataRecorder_Index_of_Variable_2;		/* Mapped at index 0x2015, subindex 0x02 */
//extern UNS16 Internal_DataRecorder_Index_of_Variables_Internal_DataRecorder_Index_of_Variable_3;		/* Mapped at index 0x2015, subindex 0x03 */
//extern UNS16 Internal_DataRecorder_Index_of_Variables_Internal_DataRecorder_Index_of_Variable_4;		/* Mapped at index 0x2015, subindex 0x04 */
//extern UNS16 Internal_DataRecorder_SubIndex_of_Variables_Internal_DataRecorder_SubIndex_of_Variable_1;		/* Mapped at index 0x2016, subindex 0x01 */
//extern UNS16 Internal_DataRecorder_SubIndex_of_Variables_Internal_DataRecorder_SubIndex_of_Variable_2;		/* Mapped at index 0x2016, subindex 0x02 */
//extern UNS16 Internal_DataRecorder_SubIndex_of_Variables_Internal_DataRecorder_SubIndex_of_Variable_3;		/* Mapped at index 0x2016, subindex 0x03 */
//extern UNS16 Internal_DataRecorder_SubIndex_of_Variables_Internal_DataRecorder_SubIndex_of_Variable_4;		/* Mapped at index 0x2016, subindex 0x04 */
//extern UNS16 Internal_DataRecorder_Status;		/* Mapped at index 0x2017, subindex 0x00*/
//extern UNS16 Internal_DataRecorder_Max_Number_of_Samples;		/* Mapped at index 0x2018, subindex 0x00*/
//extern UNS16 Internal_DataRecorder_Number_of_Recorded_Samples;		/* Mapped at index 0x2019, subindex 0x00*/
//extern UNS16 Internal_DataRecorder_Vector_Start_Offset;		/* Mapped at index 0x201A, subindex 0x00*/
//extern UNS8 Internal_DataRecorder_Data_Buffer[1];		/* Mapped at index 0x201B, subindex 0x00*/
//extern UNS32 Incremental_Encoder_1_Counter;		/* Mapped at index 0x2020, subindex 0x00*/
//extern UNS32 Incremental_Encoder_1_Counter_at_Index_Pulse;		/* Mapped at index 0x2021, subindex 0x00*/
//extern UNS16 Hall_Sensor_Pattern;		/* Mapped at index 0x2022, subindex 0x00*/
//extern UNS16 Internal_Object_Actual_Rotor_Angle;		/* Mapped at index 0x2023, subindex 0x00*/
//extern UNS16 Internal_System_State;		/* Mapped at index 0x2024, subindex 0x00*/
//extern UNS32 Internal_Object_Reserved;		/* Mapped at index 0x2025, subindex 0x00*/
//extern UNS32 Internal_Object_ProcessMemory_Internal_Object_MemoryAddress;		/* Mapped at index 0x2026, subindex 0x01 */
//extern UNS16 Internal_Object_ProcessMemory_Internal_Object_MemoryValue;		/* Mapped at index 0x2026, subindex 0x02 */
//extern INTEGER16 Current_Actual_Value_Averaged;		/* Mapped at index 0x2027, subindex 0x00*/
//extern INTEGER32 Velocity_Actual_Value_Averaged;		/* Mapped at index 0x2028, subindex 0x00*/
//extern INTEGER32 Auxiliary_Velocity_Actual_Value_Averaged;		/* Mapped at index 0x2029, subindex 0x00*/
//extern UNS32 Internal_Object_Internal_Parameters_Internal_Object_Main_Position_Sensor_Resolution;		/* Mapped at index 0x202C, subindex 0x01 */
//extern UNS32 Internal_Object_Internal_Parameters_Internal_Object_Maximal_Velocity_Limit;		/* Mapped at index 0x202C, subindex 0x02 */
//extern UNS32 Internal_Object_Internal_Parameters_Internal_Object_Auxiliary_Position_Sensor_Resolution;		/* Mapped at index 0x202C, subindex 0x03 */
//extern INTEGER16 Internal_Object_Internal_Values_Internal_Object_Maximum_measured_angle_difference;		/* Mapped at index 0x202D, subindex 0x01 */
//extern INTEGER16 Internal_Object_Internal_Values_Internal_Object_Minimum_measured_angle_difference;		/* Mapped at index 0x202D, subindex 0x02 */
//extern UNS16 Internal_Object_Internal_Values_Internal_Object_Angle_difference_overflow_counter;		/* Mapped at index 0x202D, subindex 0x03 */
//extern UNS16 Internal_Object_Internal_Values_Internal_Object_Angle_Error_factor_2_pole_motor;		/* Mapped at index 0x202D, subindex 0x04 */
//extern UNS16 Internal_Object_Internal_Values_Internal_Object_Angle_Error_factor_N_pole_motor;		/* Mapped at index 0x202D, subindex 0x05 */
//extern UNS16 Internal_Object_ADC_values_Internal_Object_ADC_NTC_temperature_value;		/* Mapped at index 0x202E, subindex 0x01 */
//extern INTEGER16 Internal_Object_ADC_values_Internal_Object_ADC_current_U_W1;		/* Mapped at index 0x202E, subindex 0x02 */
//extern INTEGER16 Internal_Object_ADC_values_Internal_Object_ADC_current_V_W2;		/* Mapped at index 0x202E, subindex 0x03 */
//extern UNS16 Internal_Object_ADC_values_Internal_Object_ADC_Input_voltage;		/* Mapped at index 0x202E, subindex 0x04 */
//extern UNS16 Internal_Object_ADC_values_Internal_Object_ADC_Gate_driver_voltage;		/* Mapped at index 0x202E, subindex 0x05 */
//extern UNS16 Internal_Object_ADC_values_Internal_Object_ADC_Sine_entry_Voltage;		/* Mapped at index 0x202E, subindex 0x06 */
//extern UNS16 Internal_Object_ADC_values_Internal_Object_ADC_Cosine_entry_Voltage;		/* Mapped at index 0x202E, subindex 0x07 */
//extern UNS16 Internal_Object_ADC_values_Internal_Object_ADC_CAN_ID_Switch_1;		/* Mapped at index 0x202E, subindex 0x08 */
//extern UNS16 Internal_Object_ADC_values_Internal_Object_ADC_CAN_ID_Switch_2;		/* Mapped at index 0x202E, subindex 0x09 */
//extern UNS16 Internal_Object_ADC_values_Internal_Object_ADC_CAN_ID_Switch_3;		/* Mapped at index 0x202E, subindex 0x0A */
//extern INTEGER16 Internal_Object_ADC_values_Internal_Object_ADC_current_W_W3;		/* Mapped at index 0x202E, subindex 0x0B */
//extern INTEGER16 Internal_Object_ADC_values_Internal_Object_ADC_Supply_Voltage_Udd;		/* Mapped at index 0x202E, subindex 0x0C */
//extern INTEGER16 Current_Mode_Setting_Value;		/* Mapped at index 0x2030, subindex 0x00*/
//extern INTEGER16 Current_Demand_Value;		/* Mapped at index 0x2031, subindex 0x00*/
//extern INTEGER32 Position_Mode_Setting_Value;		/* Mapped at index 0x2062, subindex 0x00*/
//extern INTEGER32 Auxiliary_Velocity_Sensor_Actual_Value;		/* Mapped at index 0x2069, subindex 0x00*/
//extern INTEGER32 Velocity_Mode_Setting_Value;		/* Mapped at index 0x206B, subindex 0x00*/
//extern INTEGER32 Auxiliary_Velocity_Actual_Value;		/* Mapped at index 0x206C, subindex 0x00*/
//extern UNS16 Configuration_of_Digital_Inputs_Configuration_of_Digital_Input1;		/* Mapped at index 0x2070, subindex 0x01 */
//extern UNS16 Configuration_of_Digital_Inputs_Configuration_of_Digital_Input2;		/* Mapped at index 0x2070, subindex 0x02 */
//extern UNS16 Configuration_of_Digital_Inputs_Configuration_of_Digital_Input3;		/* Mapped at index 0x2070, subindex 0x03 */
//extern UNS16 Configuration_of_Digital_Inputs_Configuration_of_Digital_Input4;		/* Mapped at index 0x2070, subindex 0x04 */
//extern UNS16 Configuration_of_Digital_Inputs_Configuration_of_Digital_Input5;		/* Mapped at index 0x2070, subindex 0x05 */
//extern UNS16 Configuration_of_Digital_Inputs_Configuration_of_Digital_Input6;		/* Mapped at index 0x2070, subindex 0x06 */
//extern UNS16 Configuration_of_Digital_Inputs_Configuration_of_Digital_Input7;		/* Mapped at index 0x2070, subindex 0x07 */
//extern UNS16 Configuration_of_Digital_Inputs_Configuration_of_Digital_Input8;		/* Mapped at index 0x2070, subindex 0x08 */
//extern UNS16 Configuration_of_Digital_Inputs_Configuration_of_Digital_Input9;		/* Mapped at index 0x2070, subindex 0x09 */
//extern UNS16 Digital_Input_Functionalities_Digital_Input_Functionalities_State;		/* Mapped at index 0x2071, subindex 0x01 */
//extern UNS16 Digital_Input_Functionalities_Digital_Input_Functionalities_Mask;		/* Mapped at index 0x2071, subindex 0x02 */
//extern UNS16 Digital_Input_Functionalities_Digital_Input_Functionalities_Polarity;		/* Mapped at index 0x2071, subindex 0x03 */
//extern UNS16 Digital_Input_Functionalities_Digital_Input_Functionalities_Execution_Mask;		/* Mapped at index 0x2071, subindex 0x04 */
//extern INTEGER32 Position_Marker_Position_Marker_Captured_Position;		/* Mapped at index 0x2074, subindex 0x01 */
//extern UNS8 Position_Marker_Position_Marker_Edge_Type;		/* Mapped at index 0x2074, subindex 0x02 */
//extern UNS8 Position_Marker_Position_Marker_Mode;		/* Mapped at index 0x2074, subindex 0x03 */
//extern UNS16 Position_Marker_Position_Marker_Counter;		/* Mapped at index 0x2074, subindex 0x04 */
//extern INTEGER32 Position_Marker_Position_Marker_History_1;		/* Mapped at index 0x2074, subindex 0x05 */
//extern INTEGER32 Position_Marker_Position_Marker_History_2;		/* Mapped at index 0x2074, subindex 0x06 */
//extern UNS16 Digital_Output_Functionalities_Digital_Output_State;		/* Mapped at index 0x2078, subindex 0x01 */
//extern UNS16 Digital_Output_Functionalities_Digital_Output_Mask;		/* Mapped at index 0x2078, subindex 0x02 */
//extern UNS16 Digital_Output_Functionalities_Digital_Output_Polarity;		/* Mapped at index 0x2078, subindex 0x03 */
//extern UNS16 Configuration_of_Digital_Outputs_Configuration_of_Digital_Output1;		/* Mapped at index 0x2079, subindex 0x01 */
//extern UNS16 Configuration_of_Digital_Outputs_Configuration_of_Digital_Output2;		/* Mapped at index 0x2079, subindex 0x02 */
//extern UNS16 Configuration_of_Digital_Outputs_Configuration_of_Digital_Output3;		/* Mapped at index 0x2079, subindex 0x03 */
//extern UNS16 Configuration_of_Digital_Outputs_Configuration_of_Digital_Output4;		/* Mapped at index 0x2079, subindex 0x04 */
//extern UNS16 Configuration_of_Digital_Outputs_Configuration_of_Digital_Output5;		/* Mapped at index 0x2079, subindex 0x05 */
//extern UNS16 Position_Compare_Position_Compare_Configuration;		/* Mapped at index 0x207A, subindex 0x01 */
//extern INTEGER32 Position_Compare_Position_Compare_Reference_Position;		/* Mapped at index 0x207A, subindex 0x02 */
//extern INTEGER32 Position_Compare_Position_Compare_Interval_Width;		/* Mapped at index 0x207A, subindex 0x03 */
//extern UNS16 Position_Compare_Position_Compare_Interval_Repetitions;		/* Mapped at index 0x207A, subindex 0x04 */
//extern UNS16 Position_Compare_Position_Compare_Pulse_Width;		/* Mapped at index 0x207A, subindex 0x05 */
//extern UNS16 Configuration_of_Analog_Inputs_Configuration_of_Analog_Input1;		/* Mapped at index 0x207B, subindex 0x01 */
//extern UNS16 Configuration_of_Analog_Inputs_Configuration_of_Analog_Input2;		/* Mapped at index 0x207B, subindex 0x02 */
//extern INTEGER16 Analog_Inputs_Analog_Input_1;		/* Mapped at index 0x207C, subindex 0x01 */
//extern INTEGER16 Analog_Inputs_Analog_Input_2;		/* Mapped at index 0x207C, subindex 0x02 */
//extern UNS16 Analog_Input_Functionalities_Execution_Mask;		/* Mapped at index 0x207D, subindex 0x00*/
//extern UNS16 Current_Threshold_for_Homing_Mode;		/* Mapped at index 0x2080, subindex 0x00*/
//extern INTEGER32 Home_Position;		/* Mapped at index 0x2081, subindex 0x00*/
//extern INTEGER32 Home_Position_Displacement;		/* Mapped at index 0x2082, subindex 0x00*/
//extern UNS64 Interpolation_Data_Record;		/* Mapped at index 0x20C1, subindex 0x00*/
//extern UNS16 Interpolation_Buffer_Interpolation_Buffer_Status;		/* Mapped at index 0x20C4, subindex 0x01 */
//extern UNS16 Interpolation_Buffer_Interpolation_Buffer_Underflow_Warning;		/* Mapped at index 0x20C4, subindex 0x02 */
//extern UNS16 Interpolation_Buffer_Interpolation_Buffer_Overflow_Warning;		/* Mapped at index 0x20C4, subindex 0x03 */
//extern INTEGER16 Following_Error_Actual_Value;		/* Mapped at index 0x20F4, subindex 0x00*/
//extern UNS16 Holding_Brake_Configuration_Brake_Reaction_Rise_Time;		/* Mapped at index 0x2100, subindex 0x01 */
//extern UNS16 Holding_Brake_Configuration_Brake_Reaction_Fall_Time;		/* Mapped at index 0x2100, subindex 0x02 */
//extern UNS16 Holding_Brake_Configuration_Brake_Timeout;		/* Mapped at index 0x2100, subindex 0x03 */
//extern UNS16 Standstill_Window_Configuration_Standstill_Window;		/* Mapped at index 0x2101, subindex 0x01 */
//extern UNS16 Standstill_Window_Configuration_Standstill_Window_Time;		/* Mapped at index 0x2101, subindex 0x02 */
//extern UNS16 Standstill_Window_Configuration_Standstill_Timeout;		/* Mapped at index 0x2101, subindex 0x03 */
//extern UNS32 Sensor_Configuration_Pulse_Number_Incremental_Encoder_1;		/* Mapped at index 0x2210, subindex 0x01 */
//extern UNS16 Sensor_Configuration_Position_Sensor_Type;		/* Mapped at index 0x2210, subindex 0x02 */
//extern UNS16 Sensor_Configuration_Internal_Absolute_Position_Offset;		/* Mapped at index 0x2210, subindex 0x03 */
//extern UNS16 Sensor_Configuration_Position_Sensor_Polarity;		/* Mapped at index 0x2210, subindex 0x04 */
//extern UNS16 SSI_Encoder_Configuration_SSI_Encoder_Data_Rate;		/* Mapped at index 0x2211, subindex 0x01 */
//extern UNS16 SSI_Encoder_Configuration_SSI_Encoder_Number_of_Data_Bits;		/* Mapped at index 0x2211, subindex 0x02 */
//extern INTEGER32 SSI_Encoder_Configuration_SSI_Encoder_Actual_Position;		/* Mapped at index 0x2211, subindex 0x03 */
//extern UNS16 SSI_Encoder_Configuration_SSI_Encoder_Encoding_Type;		/* Mapped at index 0x2211, subindex 0x04 */
//extern UNS32 Incremental_Encoder_2_Configuration_Incremental_Encoder_2_Pulse_Number;		/* Mapped at index 0x2212, subindex 0x01 */
//extern UNS32 Incremental_Encoder_2_Configuration_Incremental_Encoder_2_Counter;		/* Mapped at index 0x2212, subindex 0x02 */
//extern UNS32 Incremental_Encoder_2_Configuration_Incremental_Encoder_2_Counter_at_Index_Pulse;		/* Mapped at index 0x2212, subindex 0x03 */
//extern UNS32 Sinus_Incremental_Encoder_2_Configuration_Sinus_Incremental_Encoder_2_Resolution;		/* Mapped at index 0x2213, subindex 0x01 */
//extern INTEGER32 Sinus_Incremental_Encoder_2_Configuration_Sinus_Incremental_Encoder_2_Actual_Position;		/* Mapped at index 0x2213, subindex 0x02 */
//extern UNS16 Sinus_Incremental_Encoder_2_Configuration_Internal_Sinus_Incremental_Encoder_2_Actual_Angle;		/* Mapped at index 0x2213, subindex 0x03 */
//extern UNS16 Controller_Structure;		/* Mapped at index 0x2220, subindex 0x00*/
//extern UNS32 Gear_Configuration_Gear_Ratio_Numerator;		/* Mapped at index 0x2230, subindex 0x01 */
//extern UNS16 Gear_Configuration_Gear_Ratio_Denominator;		/* Mapped at index 0x2230, subindex 0x02 */
//extern UNS32 Gear_Configuration_Gear_Maximal_Speed;		/* Mapped at index 0x2230, subindex 0x03 */
//extern UNS16 Internal_SSI_Encoder_Configuration_Internal_SSI_Encoder_State;		/* Mapped at index 0x2240, subindex 0x01 */
//extern INTEGER32 Internal_SSI_Encoder_Configuration_Internal_SSI_Encoder_Zero_Position_Reference;		/* Mapped at index 0x2240, subindex 0x02 */
//extern INTEGER32 Digital_Position_Input_Digital_Position_Desired_Value;		/* Mapped at index 0x2300, subindex 0x01 */
//extern UNS16 Digital_Position_Input_Digital_Position_Scaling_Numerator;		/* Mapped at index 0x2300, subindex 0x02 */
//extern UNS16 Digital_Position_Input_Digital_Position_Scaling_Denominator;		/* Mapped at index 0x2300, subindex 0x03 */
//extern UNS8 Digital_Position_Input_Digital_Position_Polarity;		/* Mapped at index 0x2300, subindex 0x04 */
//extern INTEGER32 Digital_Position_Input_Digital_Position_Offset;		/* Mapped at index 0x2300, subindex 0x05 */
//extern INTEGER16 Analog_Current_Setpoint_Configuration_Analog_Current_Setpoint_Scaling;		/* Mapped at index 0x2301, subindex 0x01 */
//extern INTEGER16 Analog_Current_Setpoint_Configuration_Analog_Current_Setpoint_Offset;		/* Mapped at index 0x2301, subindex 0x02 */
//extern INTEGER8 Analog_Current_Setpoint_Configuration_Analog_Current_Setpoint_Notation_Index;		/* Mapped at index 0x2301, subindex 0x03 */
//extern INTEGER16 Analog_Current_Setpoint_Configuration_Analog_Current_Setpoint;		/* Mapped at index 0x2301, subindex 0x04 */
//extern INTEGER16 Analog_Velocity_Setpoint_Configuration_Analog_Velocity_Setpoint_Scaling;		/* Mapped at index 0x2302, subindex 0x01 */
//extern INTEGER32 Analog_Velocity_Setpoint_Configuration_Analog_Velocity_Setpoint_Offset;		/* Mapped at index 0x2302, subindex 0x02 */
//extern INTEGER8 Analog_Velocity_Setpoint_Configuration_Analog_Velocity_Setpoint_Notation_Index;		/* Mapped at index 0x2302, subindex 0x03 */
//extern INTEGER32 Analog_Velocity_Setpoint_Configuration_Analog_Velocity_Setpoint;		/* Mapped at index 0x2302, subindex 0x04 */
//extern INTEGER16 Analog_Position_Setpoint_Configuration_Analog_Position_Setpoint_Scaling;		/* Mapped at index 0x2303, subindex 0x01 */
//extern INTEGER32 Analog_Position_Setpoint_Configuration_Analog_Position_Setpoint_Offset;		/* Mapped at index 0x2303, subindex 0x02 */
//extern INTEGER8 Analog_Position_Setpoint_Configuration_Analog_Position_Setpoint_Notation_Index;		/* Mapped at index 0x2303, subindex 0x03 */
//extern INTEGER32 Analog_Position_Setpoint_Configuration_Analog_Position_Setpoint;		/* Mapped at index 0x2303, subindex 0x04 */
//extern UNS16 Internal_P_Autotuning_Parameters_Internal_Cut_off_Frequency_of_Oscillator;		/* Mapped at index 0x2400, subindex 0x01 */
//extern UNS32 Internal_P_Autotuning_Parameters_Internal_Target_Amplitude_of_Oscillation;		/* Mapped at index 0x2400, subindex 0x02 */
//extern UNS16 Internal_P_Autotuning_Parameters_Internal_Oscillation_Control;		/* Mapped at index 0x2400, subindex 0x03 */
//extern UNS32 Internal_P_Autotuning_Values_Internal_Cycle_Duration;		/* Mapped at index 0x2401, subindex 0x01 */
//extern UNS32 Internal_P_Autotuning_Values_Internal_Phase_Delay;		/* Mapped at index 0x2401, subindex 0x02 */
//extern UNS16 Internal_P_Autotuning_Values_Internal_Set_Value_Ramp_Current;		/* Mapped at index 0x2401, subindex 0x03 */
//extern UNS32 Internal_P_Autotuning_Values_Internal_Actual_Position_Peak_Value;		/* Mapped at index 0x2401, subindex 0x04 */
//extern UNS32 Internal_P_Autotuning_Values_Internal_Actual_Value_of_Dynamic_Friction;		/* Mapped at index 0x2401, subindex 0x05 */
//extern UNS16 Internal_P_Autotuning_Values_Internal_Number_of_Oscillation_Cycles;		/* Mapped at index 0x2401, subindex 0x06 */
//extern UNS16 Internal_P_Autotuning_Values_Internal_Maximum_Friction_Current;		/* Mapped at index 0x2401, subindex 0x07 */
//extern UNS32 Internal_P_Autotuning_Values_Internal_Actual_Position_Peak_Value_Aux;		/* Mapped at index 0x2401, subindex 0x08 */
//extern UNS32 Internal_P_Autotuning_Values_Internal_Actual_Position_Peak_Value_Difference;		/* Mapped at index 0x2401, subindex 0x09 */
//extern UNS16 Internal_LR_Identification_Internal_LR_Identification_Control;		/* Mapped at index 0x2410, subindex 0x01 */
//extern UNS16 Internal_LR_Identification_Internal_LR_Identification_Current;		/* Mapped at index 0x2410, subindex 0x02 */
//extern UNS16 Internal_LR_Identification_Internal_Resistance;		/* Mapped at index 0x2411, subindex 0x01 */
//extern UNS16 Internal_LR_Identification_Internal_Inductivity;		/* Mapped at index 0x2411, subindex 0x02 */
//extern UNS16 Internal_LR_Identification_Internal_Frequency;		/* Mapped at index 0x2411, subindex 0x03 */
//extern UNS32 Internal_Firmware_Download_Internal_Download_Start;		/* Mapped at index 0x2FFF, subindex 0x01 */
//extern UNS16 Internal_Firmware_Download_Internal_Download_Number_of_SubSections;		/* Mapped at index 0x2FFF, subindex 0x02 */
//extern UNS8 Internal_Firmware_Download_Internal_Download_Code_SubSection[1];		/* Mapped at index 0x2FFF, subindex 0x03 */
//extern UNS16 Internal_Firmware_Download_Internal_Download_Boot_Code_Version;		/* Mapped at index 0x2FFF, subindex 0x04 */
//extern UNS32 Internal_Firmware_Download_Internal_Download_Serial_Number_HighWord;		/* Mapped at index 0x2FFF, subindex 0x05 */
//extern UNS32 Internal_Firmware_Download_Internal_Download_Serial_Number_LowWord;		/* Mapped at index 0x2FFF, subindex 0x06 */
//extern INTEGER16 Abort_Connection_Option_Code;		/* Mapped at index 0x6007, subindex 0x00*/
//extern UNS16 Controlword;		/* Mapped at index 0x6040, subindex 0x00*/
//extern UNS16 Statusword;		/* Mapped at index 0x6041, subindex 0x00*/
//extern INTEGER16 Shutdown_Option_Code;		/* Mapped at index 0x605B, subindex 0x00*/
//extern INTEGER16 Disable_Operation_Option_Code;		/* Mapped at index 0x605C, subindex 0x00*/
//extern INTEGER16 Fault_Reaction_Option_Code;		/* Mapped at index 0x605E, subindex 0x00*/
//extern INTEGER8 Modes_of_Operation;		/* Mapped at index 0x6060, subindex 0x00*/
//extern INTEGER8 Modes_of_Operation_Display;		/* Mapped at index 0x6061, subindex 0x00*/
//extern INTEGER32 Position_Demand_Value;		/* Mapped at index 0x6062, subindex 0x00*/
//extern INTEGER32 Position_Actual_Value;		/* Mapped at index 0x6064, subindex 0x00*/
//extern UNS32 Max_Following_Error;		/* Mapped at index 0x6065, subindex 0x00*/
//extern UNS32 Position_Window;		/* Mapped at index 0x6067, subindex 0x00*/
//extern UNS16 Position_Window_Time;		/* Mapped at index 0x6068, subindex 0x00*/
//extern INTEGER32 Velocity_Sensor_Actual_Value;		/* Mapped at index 0x6069, subindex 0x00*/
//extern INTEGER32 Velocity_Demand_Value;		/* Mapped at index 0x606B, subindex 0x00*/
//extern INTEGER32 Velocity_Actual_Value;		/* Mapped at index 0x606C, subindex 0x00*/
//extern UNS32 Velocity_Window;		/* Mapped at index 0x606D, subindex 0x00*/
//extern UNS16 Velocity_Window_Time;		/* Mapped at index 0x606E, subindex 0x00*/
//extern INTEGER16 Current_Actual_Value;		/* Mapped at index 0x6078, subindex 0x00*/
//extern INTEGER32 Target_Position;		/* Mapped at index 0x607A, subindex 0x00*/
//extern INTEGER32 Home_Offset;		/* Mapped at index 0x607C, subindex 0x00*/
//extern INTEGER32 Software_Position_Limit_Min_Position_Limit;		/* Mapped at index 0x607D, subindex 0x01 */
//extern INTEGER32 Software_Position_Limit_Max_Position_Limit;		/* Mapped at index 0x607D, subindex 0x02 */
//extern UNS32 Max_Profile_Velocity;		/* Mapped at index 0x607F, subindex 0x00*/
//extern UNS32 Profile_Velocity;		/* Mapped at index 0x6081, subindex 0x00*/
//extern UNS32 Profile_Acceleration;		/* Mapped at index 0x6083, subindex 0x00*/
//extern UNS32 Profile_Deceleration;		/* Mapped at index 0x6084, subindex 0x00*/
//extern UNS32 QuickStop_Deceleration;		/* Mapped at index 0x6085, subindex 0x00*/
//extern INTEGER16 Motion_Profile_Type;		/* Mapped at index 0x6086, subindex 0x00*/
//extern INTEGER8 Position_Notation_Index;		/* Mapped at index 0x6089, subindex 0x00*/
//extern UNS8 Position_Dimension_Index;		/* Mapped at index 0x608A, subindex 0x00*/
//extern INTEGER8 Velocity_Notation_Index;		/* Mapped at index 0x608B, subindex 0x00*/
//extern UNS8 Velocity_Dimension_Index;		/* Mapped at index 0x608C, subindex 0x00*/
//extern INTEGER8 Acceleration_Notation_Index;		/* Mapped at index 0x608D, subindex 0x00*/
//extern UNS8 Acceleration_Dimension_Index;		/* Mapped at index 0x608E, subindex 0x00*/
//extern INTEGER8 Homing_Method;		/* Mapped at index 0x6098, subindex 0x00*/
//extern UNS32 Homing_Speeds_Speed_for_Switch_Search;		/* Mapped at index 0x6099, subindex 0x01 */
//extern UNS32 Homing_Speeds_Speed_for_Zero_Search;		/* Mapped at index 0x6099, subindex 0x02 */
//extern UNS32 Homing_Acceleration;		/* Mapped at index 0x609A, subindex 0x00*/
//extern INTEGER16 Interpolation_Sub_Mode_Selection;		/* Mapped at index 0x60C0, subindex 0x00*/
//extern UNS8 Interpolation_Time_Period_Interpolation_Time_Periode_Value;		/* Mapped at index 0x60C2, subindex 0x01 */
//extern INTEGER8 Interpolation_Time_Period_Interpolation_Time_Index;		/* Mapped at index 0x60C2, subindex 0x02 */
//extern UNS32 Interpolation_Data_Configuration_Maximum_Buffer_Size;		/* Mapped at index 0x60C4, subindex 0x01 */
//extern UNS32 Interpolation_Data_Configuration_Actual_Buffer_Size;		/* Mapped at index 0x60C4, subindex 0x02 */
//extern UNS8 Interpolation_Data_Configuration_Buffer_Organisation;		/* Mapped at index 0x60C4, subindex 0x03 */
//extern UNS16 Interpolation_Data_Configuration_Buffer_Position;		/* Mapped at index 0x60C4, subindex 0x04 */
//extern UNS8 Interpolation_Data_Configuration_Size_Of_Data_Record;		/* Mapped at index 0x60C4, subindex 0x05 */
//extern UNS8 Interpolation_Data_Configuration_Buffer_Clear;		/* Mapped at index 0x60C4, subindex 0x06 */
//extern UNS32 Max_Acceleration;		/* Mapped at index 0x60C5, subindex 0x00*/
//extern INTEGER16 Current_Control_Parameter_Set_Current_Regulator_P_Gain;		/* Mapped at index 0x60F6, subindex 0x01 */
//extern INTEGER16 Current_Control_Parameter_Set_Current_Regulator_I_Gain;		/* Mapped at index 0x60F6, subindex 0x02 */
//extern INTEGER16 Velocity_Control_Parameter_Set_Speed_Regulator_P_Gain;		/* Mapped at index 0x60F9, subindex 0x01 */
//extern INTEGER16 Velocity_Control_Parameter_Set_Speed_Regulator_I_Gain;		/* Mapped at index 0x60F9, subindex 0x02 */
//extern UNS8 Velocity_Control_Parameter_Set_Compatibility_Entry;		/* Mapped at index 0x60F9, subindex 0x03 */
//extern UNS16 Velocity_Control_Parameter_Set_Velocity_Feedforward_Factor_in_Speed_Regulator;		/* Mapped at index 0x60F9, subindex 0x04 */
//extern UNS16 Velocity_Control_Parameter_Set_Acceleration_Feedforward_Factor_in_Speed_Regulator;		/* Mapped at index 0x60F9, subindex 0x05 */
//extern INTEGER16 Position_Control_Parameter_Set_Position_Regulator_P_Gain;		/* Mapped at index 0x60FB, subindex 0x01 */
//extern INTEGER16 Position_Control_Parameter_Set_Position_Regulator_I_Gain;		/* Mapped at index 0x60FB, subindex 0x02 */
//extern INTEGER16 Position_Control_Parameter_Set_Position_Regulator_D_Gain;		/* Mapped at index 0x60FB, subindex 0x03 */
//extern UNS16 Position_Control_Parameter_Set_Velocity_Feedforward_Factor_in_Position_Regulator;		/* Mapped at index 0x60FB, subindex 0x04 */
//extern UNS16 Position_Control_Parameter_Set_Acceleration_Feedforward_Factor_in_Position_Regulator;		/* Mapped at index 0x60FB, subindex 0x05 */
//extern INTEGER32 Target_Velocity;		/* Mapped at index 0x60FF, subindex 0x00*/
//extern UNS16 Motor_Type;		/* Mapped at index 0x6402, subindex 0x00*/
//extern UNS16 Motor_Data_Continuous_Current_Limit;		/* Mapped at index 0x6410, subindex 0x01 */
//extern UNS16 Motor_Data_Output_Current_Limit;		/* Mapped at index 0x6410, subindex 0x02 */
//extern UNS8 Motor_Data_Pole_Pair_Number;		/* Mapped at index 0x6410, subindex 0x03 */
//extern UNS32 Motor_Data_Maximal_Motor_Speed;		/* Mapped at index 0x6410, subindex 0x04 */
//extern UNS16 Motor_Data_Thermal_Time_Constant_Winding;		/* Mapped at index 0x6410, subindex 0x05 */
//extern UNS32 Supported_Drive_Modes;		/* Mapped at index 0x6502, subindex 0x00*/

#endif // OD_NODE2_H
