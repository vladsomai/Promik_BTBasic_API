#pragma once

#include <windows.h>
#include "FTLog.h"

typedef enum {
    DynData_OK = 0,
    DynData_INVALID_ADDR = 1,
    DynData_INVALID_SIZE = 2,
    DynData_INVALID_MCU = 3,
    DynData_PROGRAMMING_PREPARED = 4,
    DynData_NO_JOB = 5
} FTDynDataResult;

typedef struct FTDynDataInfo {
    UINT32 id;
    UINT64 startAddr;
    UINT64 size;
} FTDynDataInfo;

typedef struct FTProgFileInfo {
    LPCSTR fileName;
    UINT32 crc32;
} FTProgFileInfo;

typedef struct FTMcuInfo {
    UINT32 channel;
    UINT32 slot;
    LPCSTR deviceName;
    UINT32 progFileInfoArraySize;
    FTProgFileInfo* progFileInfoArray;
    UINT32 dynDataInfoArraySize;
    FTDynDataInfo* dynDataInfoArray;
    UINT32 logArraySize;
    FTLog* logArray;
} FTMcuInfo;

typedef struct FTProgrammerInfo {
    UINT32 programmerId;
    UINT32 logArraySize;
    FTLog* logArray;
    UINT32 mcuInfoArraySize;
    FTMcuInfo* mcuInfoArray;
} FTProgrammerInfo;

typedef struct FTSetupInfo {
    UINT32 programmerInfoArraySize;
    FTProgrammerInfo* programmerInfoArray;
} FTSetupInfo;
