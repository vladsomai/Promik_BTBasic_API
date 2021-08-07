#pragma once

#include "FTLog.h"
#include "windows.h"

typedef enum {
    FT_Type_NOT_INITIALIZED = 0,
    FT_Type_IDLE = 1,
    FT_Type_RUNNING = 2,
    FT_Type_OK = 3,
    FT_Type_WARN = 4,
    FT_Type_ERR = 5,
    FT_Type_FATAL = 6,
    FT_Type_SKIPPED = 7
} FTResultType;

typedef struct FTCommandResult {
    LPCSTR name;
    INT32 sequenceId;
    UINT32 cmdId;
    FTResultType state;
    UINT32 logArraySize;
    FTLog* logArray;
    UINT64 durationMs;
} FTCommandResult;

typedef struct FTMcuResult {
    LPCSTR name;
    UINT32 channel;
    UINT32 slot;
    UINT32 cmdResultArraySize;
    FTCommandResult* cmdResultArray;
    LPCSTR driverVersion;
} FTMcuResult;

typedef struct FTProgrammerResult {
    UINT32 id;
    UINT32 mcuResultArraySize;
    FTMcuResult* mcuResultArray;
} FTProgrammerResult;

typedef enum class FTFlashStatus{
    FT_OK = 0,
    FT_ERR = 1,
    FT_REPL_ERR = 2,
    FT_PRECOND_ERR = 3
} FTFlashStatus;

typedef struct FTFlashResult {
    FTFlashStatus status;
    LPCSTR version;
    UINT64 programmingCycle;
    UINT64 cycleDuration;
    UINT32 programmerResultArraySize;
    FTProgrammerResult* programmerResultArray;
} FTFlashResult;
