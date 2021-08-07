#pragma once

#include "FTLicense.h"
#include "FTJobResult.h"
#include "FTProgData.h"
#include "FTFlashResult.h"
#include "FTMcuMetadata.h"

#ifdef PROMIKFT_EXPORT
#define PROMIKFT_API __declspec(dllexport)
#else
#define PROMIKFT_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct IFlashTask IFlashTask;

/* Creates a handle to FlashTask that must be used as input to other functions
 * @return pointer to FlashTask interface
 */
PROMIKFT_API IFlashTask* CreateFlashTask();

/* Starts the FlashTask Engine
 * @param handle Handle to FlashTask created with a call to CreateFlashTask()
 * @param executablePath Path to FTEngine.exe
 * @return true if process successfully started, false otherwise
 */
PROMIKFT_API BOOL StartEngine(IFlashTask* handle, LPCSTR executablePath);

/* Retrieves FlashTask version
 * @param handle Handle to FlashTask created with a call to CreateFlashTask()
 * @return String representation of the version
 */
PROMIKFT_API LPCSTR GetFTVersion(IFlashTask* handle);

/* Loads the specified license file
 * @param handle Handle to FlashTask created with a call to CreateFlashTask()
 * @param licenseLocation Full path to the license file
 * @return License verification status
 */
PROMIKFT_API const FTLicense* LoadLicense(IFlashTask* handle, LPCSTR licenseLocation);

/* Assigns IP-address properties to programmers specified in the job-file
 * @param handle Handle to FlashTask created with a call to CreateFlashTask()
 * @param programmerId Configuration identifier of the programmer to assign the IP-address to
 * @param programmerIp IP-address of the programmer
 */
PROMIKFT_API VOID ConfigureProgrammer(IFlashTask* handle, UINT32 programmerId, LPCSTR programmerIp);

/* Assigns a physical file containing data to be programmed to a placeholder defined in a job file
 * @param handle Handle to FlashTask created with a call to CreateFlashTask()
 * @param datasetId Identifier of a programmable dataset to use the file in
 * @param fileId Numerical identifier of a placeholder in a dataset to substitute with a file
 * @param filePath Filesystem path to a file to use instead of a placeholder defined in a job file
 */
PROMIKFT_API VOID ComplementProgrammableData(IFlashTask* handle, UINT32 datasetId, UINT32 fileId, LPCSTR filePath);

/* Assigns a physical file containing data to be programmed to a placeholder defined in a job file
 * @param handle Handle to FlashTask created with a call to CreateFlashTask()
 * @param datasetId Identifier of a programmable dataset to use the file in
 * @param placeholderName Alphanumerical identifier of a placeholder in a dataset to substitute with a file
 * @param filePath Filesystem path to a file to use instead of a placeholder defined in a job file
 */
PROMIKFT_API VOID ComplementProgrammableDataByName(IFlashTask* handle, UINT32 datasetId, LPCSTR placeholderName,
                                                   LPCSTR filePath);

/* Loads the job
 * @param handle Handle to FlashTask created with a call to CreateFlashTask()
 * @param jobLocation Full path to the job file
 * @return Status of the load job operation
 */
PROMIKFT_API const FTJobResult* LoadJob(IFlashTask* handle, LPCSTR jobLocation);

/* Loads the job
 * @param handle Handle to FlashTask created with a call to CreateFlashTask()
 * @param programmableDataId Identifier of programmable data to retrieve a region of, as defined in the job file
 * @param startAddress Start address of a memory region to retrieve
 * @param size Size of a memory region to retrieve
 * @return Status of the load job operation
 */
PROMIKFT_API const FTProgData* GetProgData(IFlashTask* handle, UINT32 programmableDataId, UINT64 startAddress,
                                           UINT64 size);

/* Unloads the job in case one is loaded
 * @param handle Handle to FlashTask created with a call to CreateFlashTask()
 */
PROMIKFT_API VOID UnloadJob(IFlashTask* handle);

/* Excludes a specified MCU from the programming process for a duration of a programming cycle
 * @param handle Handle to FlashTask created with a call to CreateFlashTask()
 * @param programmerId Programmer the driver is assigned to
 * @param channelId Programmer channel the driver is assigned to
 * @param slotId Slot on the channel of a programmer the driver is assigned to
 * @return True if the driver identifier to disable has been stored, false if programming has already been prepared
 *         and the driver cannot be disabled any more
 */
PROMIKFT_API BOOL DisableMcu(IFlashTask* handle, UINT32 programmerId, UINT32 channelId, UINT32 slotId);

/* Verifies dynamic replacement metadata and sets the data for a specified MCU for a duration of a cycle
 * @param handle Handle to FlashTask created with a call to CreateFlashTask()
 * @param programmer_id Programmer the driver is assigned to
 * @param channel_id Programmer channel the driver is assigned to
 * @param slot_id Slot on the channel of a programmer the driver is assigned to
 * @param dynamic_replacement_id Identifier of a dynamic replacement as specified in the job file
 * @param address Start address of a dynamic data region
 * @param data Content for a dynamic replacement
 * @param dataSize Size in bytes of data pointed to by data member
 * @return Descriptive enumeration signifying whether the arguments provided are correct and whether
 *         they can be applied for the subseqent execution of a programming process
 */
PROMIKFT_API FTDynDataResult SetDynamicData(IFlashTask* handle, UINT32 programmerId, UINT32 channelId, UINT32 slotId,
                                            UINT32 dynamicReplacementId, UINT64 address, const PBYTE data, UINT64 dataSize);

/* Prepares the execution of a programming cycle: has to be invoked before every cycle
 * @param handle Handle to FlashTask created with a call to CreateFlashTask()
 * @return Tree-like representation of the flow execution status
 */
PROMIKFT_API const FTFlashResult* PrepareProgramming(IFlashTask* handle);

/* Prepares the execution of a programming cycle on a specified programmer channel: has to be invoked before every
 *     programming cycle on the channel
 * @param handle Handle to FlashTask created with a call to CreateFlashTask()
 * @param channelId Identifier of a channel that is to be prepared for a programming process
 * @return Tree-like representation of the flow execution status
 */
PROMIKFT_API const FTFlashResult* PrepareProgrammingOnChannel(IFlashTask* handle, UINT32 channelId);

/* Executes all the command sequences configured in the job file. If some of the sequences have already been executed
 *     using the execute_flow_step(), runs the rest of command sequences till the end
 * @param handle Handle to FlashTask created with a call to CreateFlashTask()
 * @return Tree-like representation of the flow execution status
 */
PROMIKFT_API const FTFlashResult* ExecuteProgrammingProcess(IFlashTask* handle);

/* Executes all the command sequences configured in the job file on a specified programmer channel.
 * @param handle Handle to FlashTask created with a call to CreateFlashTask()
 * @param channelId Identifier of a channel the programming process is to be executed on
 * @return Tree-like representation of the flow execution status
 */
PROMIKFT_API const FTFlashResult* ExecuteProgrammingOnChannel(IFlashTask* handle, UINT32 channelId);

/* Executes one step of the flow
 * @param handle Handle to FlashTask created with a call to CreateFlashTask()
 * @return Tree-like representation of the flow execution status
 */
PROMIKFT_API const FTFlashResult* ExecuteProgrammingStep(IFlashTask* handle);

/* Retrieves traces of commands' execution for a specified MCU
 * @param handle Handle to FlashTask created with a call to CreateFlashTask()
 * @param programmerId Programmer identifier the MCU is assigned to
 * @param channelId Programmer channel identifier the MCU is assigned to
 * @param slotId Identifier of a slot on a channel of a programmer the MCU is assigned to
 * @param key Trace key to retrieve the content for
 * @return Request status along with the trace data if available
 */
PROMIKFT_API const FTMcuTrace* GetMcuTrace(IFlashTask* handle, UINT32 programmerId, UINT32 channelId, UINT32 slotId,
                                           LPCSTR key);

/* Retrieves register data of commands' execution for a specified MCU
 * @param handle Handle to FlashTask created with a call to CreateFlashTask()
 * @param programmerId Programmer identifier the MCU is assigned to
 * @param channelId Programmer channel identifier the MCU is assigned to
 * @param slotId Identifier of a slot on a channel of a programmer the MCU is assigned to
 * @param address Register address to retrieve the content for
 * @return Request status along with the trace data if available
 */
PROMIKFT_API const FTMcuRegister* GetMcuRegister(IFlashTask* handle, UINT32 programmerId, UINT32 channelId,
                                                 UINT32 slotId, UINT64 address);

/* Retrieves data after commands' execution for a specified MCU
 * @param handle Handle to FlashTask created with a call to CreateFlashTask()
 * @param programmerId Programmer identifier the MCU is assigned to
 * @param channelId Programmer channel identifier the MCU is assigned to
 * @param slotId Identifier of a slot on a channel of a programmer the MCU is assigned to
 * @param address Start address of a memory region to retrieve
 * @param size Size of a memory region to retrieve
 * @return Request status along with the MCU data if available
 */
PROMIKFT_API const FTMcuData* GetMcuData(IFlashTask* handle, UINT32 programmerId, UINT32 channelId, UINT32 slotId,
                                         UINT64 address, UINT64 size);

/* Stores FlashTask logs to the provided location
 * @param handle Handle to FlashTask created with a call to CreateFlashTask()
 * @param path File system path to store logs in
 * @return true if logs have been successfully uploaded, false otherwise
 */
PROMIKFT_API BOOL StoreLogFiles(IFlashTask* handle, LPCSTR path);

/* Convenience function for retrieval of a checksum for a given MCU
 * @param handle Handle to FlashTask created with a call to CreateFlashTask()
 * @param programmerId Programmer identifier the MCU is assigned to
 * @param channelId Programmer channel identifier the MCU is assigned to
 * @param slotId Slot identifier of the MCU
 * @return FTMcuChecksum::isAvailable A boolean flag signifying whether the checksum could be retrieved for a given MCU.
 *         FTMcuChecksum::value Checksum, calculated after programming process for a given MCU,
 *                                 valid only if isAvailable is TRUE.
 */
PROMIKFT_API FTMcuChecksum GetMcuChecksum(IFlashTask* handle, UINT32 programmerId, UINT32 channelId, UINT32 slotId);

/* Function for retrieval of a device specific device id for a given MCU
 * @param handle Handle to FlashTask created with a call to CreateFlashTask()
 * @param programmerId Programmer identifier the MCU is assigned to
 * @param channelId Programmer channel identifier the MCU is assigned to
 * @param slotId Slot identifier of the MCU
 * @return FTMcuPartId::isAvailable A boolean flag signifying whether the device id could be retrieved for a given MCU.
 *         FTMcuPartId::valuePtr null terminated device_id string of 8-bit characters for a given MCU,
 *                               valid only if isAvailable is TRUE.
 */
PROMIKFT_API FTMcuDeviceId GetMcuDeviceId(IFlashTask* handle, UINT32 programmerId, UINT32 channelId, UINT32 slotId);

/* Destroys the FlashTask instance created with CreateFlashTask(). Any use of the handle after this function
 *     has been called and have returned, is illegal.
 * @param handle Handle to FlashTask created with a call to CreateFlashTask()
 */
PROMIKFT_API VOID Destroy(IFlashTask* handle);

#ifdef __cplusplus
}
#endif
