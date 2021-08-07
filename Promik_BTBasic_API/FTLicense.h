#pragma once

#include <windows.h>

typedef struct FTLicensedMcuDriver {
    UINT64 expirationDate;
    LPCSTR family;
    UINT32 majorVersion;
    UINT32 minorVersion;
    UINT32 revisionVersion;
} FTLicensedMcuDriver;

typedef struct FTLicenseFileContent {
    UINT32 masksArraySize;
    LPCSTR* masksArray;
    UINT32 serialsArraySize;
    LPCSTR* serialsArray;
    UINT32 mcuDriversArraySize;
    FTLicensedMcuDriver* mcuDriversArray;
} FTLicenseFileContent;

typedef struct FTLicense {
    BOOL isLoaded;
    LPCSTR message;
    FTLicenseFileContent* licenseFile;
} FTLicense;
