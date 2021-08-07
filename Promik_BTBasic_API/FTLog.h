#pragma once

#include <windows.h>

typedef enum class FTLogLevel{
    FT_Level_INFO = 0,
    FT_Level_WARN = 1,
    FT_Level_ERR = 2
} FTLogLevel;

typedef struct FTLog {
    FTLogLevel level;
    LPCSTR message;
} FTLog;
