#pragma once

#include <windows.h>

typedef enum {
    Trace_OK = 0,
    Trace_INVALID_MCU_ID = 1,
    Trace_KEY_UNAVAILABLE = 2
} FTMcuTraceStatus;

typedef struct FTMcuTrace {
    FTMcuTraceStatus status;
    LPCSTR value;
    LPCSTR description;
} FTMcuTrace;

typedef enum {
    Register_OK = 0,
    Register_INVALID_MCU_ID = 1,
    Register_REGISTER_UNAVAILABLE = 2
} FTMcuRegisterStatus;

typedef struct FTMcuRegister {
    FTMcuRegisterStatus status;
    UINT64 dataSize;
    LPBYTE data;
    LPCSTR description;
} FTMcuRegister;

typedef struct FTMcuDataRegion {
    UINT64 address;
    UINT64 dataSize;
    LPBYTE data;
    LPCSTR description;
} FTMcuDataRegion;

typedef enum {
    Data_OK = 0,
    Data_INVALID_MCU_ID = 1,
    Data_INVALID_RANGE = 2,
    Data_FRAGMENTED_DATA = 3
} FTMcuDataStatus;

typedef struct FTMcuData {
    FTMcuDataStatus status;
    UINT32 dataRegionArraySize;
    FTMcuDataRegion* dataRegionArray;
} FTMcuData;

typedef struct FTMcuChecksum {
    BOOL isAvailable;
    UINT32 value;
} FTMcuChecksum;

typedef struct FTMcuDeviceId {
    BOOL isAvailable;
    LPCSTR valuePtr;
} FTMcuDeviceId;
