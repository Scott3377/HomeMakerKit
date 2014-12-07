#import <Foundation/Foundation.h>

typedef enum {
    ThermostatTemperatureUnits_Farenheight,
    ThermostatTemperatureUnits_Celcius,
} ThermostatTemperatureUnits;

typedef NS_OPTIONS(NSUInteger, ThermostatSystemCapability) {
    ThermostatSystemCapability_None = 0,
    ThermostatSystemCapability_Off  = 1 << 0,
    ThermostatSystemCapability_Heat = 1 << 1,
    ThermostatSystemCapability_Cool = 1 << 2,
    ThermostatSystemCapability_Aux  = 1 << 3,
    ThermostatSystemCapability_Auto = 1 << 4,
};

typedef enum {
    ThermostatRealtimeResponseType_Online,
    ThermostatRealtimeResponseType_Offline,
    ThermostatRealtimeResponseType_Update,
    ThermostatRealtimeResponseType_Status,
    ThermostatRealtimeResponseType_Error,
} ThermostatRealtimeResponseType;

typedef enum {
    ThermostatSystemSetting_Off,
    ThermostatSystemSetting_Heat,
    ThermostatSystemSetting_Cool,
    ThermostatSystemSetting_Aux,
    ThermostatSystemSetting_Auto
} ThermostatSystemSetting;

typedef enum {
    ThermostatFanMode_Auto,
    ThermostatFanMode_On,
    ThermostatFanMode_Smart
} ThermostatFanMode;

typedef enum {
    ThermostatOperatingMode_Off,
    ThermostatOperatingMode_Aux,
    ThermostatOperatingMode_Heat,
    ThermostatOperatingMode_Cool,
    ThermostatOperatingMode_AutoCool,
    ThermostatOperatingMode_AutoHeat
} ThermostatOperatingMode;

typedef enum {
    ThermostatRunMode_Off,
    ThermostatRunMode_Aux,
    ThermostatRunMode_Heat,
    ThermostatRunMode_Cool,
} ThermostatRunMode;

typedef enum {
    ThermostatHoldMode_Off,
    ThermostatHoldMode_Temporary,
    ThermostatHoldMode_Permanant
} ThermostatHoldMode;