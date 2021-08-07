#pragma once

#include <windows.h>

#include "FTSetupInfo.h"

typedef enum class FTJobStatus{
    FT_Job_OK = 0,
    FT_Job_WARN = 1,
    FT_Job_ERR = 2,
    FT_Job_ERR_LICENSE = 3,
    FT_Job_ERR_SETUP = 4
} FTJobStatus;

typedef struct FTJobResult {
    FTJobStatus status;
    LPCSTR fixture;
    LPCSTR message;
    FTSetupInfo setupInfo;
    BOOL isJobCached;
} FTJobResult;
