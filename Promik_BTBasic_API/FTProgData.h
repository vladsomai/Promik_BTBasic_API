#pragma once

#include <windows.h>

typedef struct FTProgDataRegion {
    UINT64 address;
    UINT64 dataSize;
    LPBYTE data;
} FTProgDataRegion;

typedef enum {
    ProgData_OK = 0,
    ProgData_INVALID_ID = 1,
    ProgData_INVALID_RANGE = 2
} FTProgDataStatus;

typedef struct FTProgData {
    FTProgDataStatus status;
    LPCSTR message;
    UINT32 progDataRegionArraySize;
    FTProgDataRegion* progDataRegionArray;
} FTProgData;
