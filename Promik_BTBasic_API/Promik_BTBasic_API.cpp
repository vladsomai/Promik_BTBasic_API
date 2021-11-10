#include "FlashTaskLibrary.h"
#include <stdio.h>
#include <windows.h>
#include "Promik_BTBasic_API.h"
#include <iostream>
#include <string>

IFlashTask* flash_task = nullptr;
static bool debugMode = false;

/////////////////////////////////////////////////////////////////////////
// Function:    BTBasic_DLL_Call
// Description: 
//   Takes call from BT-Basic and parse the function name and parameter passed in,
//   calls the corresponding functions of DLL library 
//   and returns the function result to BT-Basic.
// Return: 
//   Function result. If there is no error, the return value will be EXT_DLL_Result_OK.
//   Otherwise the return value will be EXT_DLL_Result_Error.
// Parameters:
//   functionName - function name specified in dllcall command
//   parameters   - parameters specified in dllcall command
//   returnString - char pointer points to the return sting buffer
//   returnValue  - integer pointer points to the return value buffer
/////////////////////////////////////////////////////////////////////////
DllExport EXT_DLL_Result BTBasic_DLL_Call(char* functionName, char* parameters,
	char* returnString, int* returnValue)
{
	//Spilt the parameters from BTBasic testplan into an array, similar to UNIX command arguments(argc, argv)
	char* next = parameters;
	char* tokens = strtok_s(parameters, ",", &next);
	int argc = 0;//count number of arguments below
	char* argv[16] = { NULL };//allow max 16 items as parameters
	const char* messageToReturn = nullptr;
	size_t sizeOfMessage {};

	while (tokens != NULL && next != NULL)
	{
		argv[argc] = tokens;
		tokens = strtok_s(next, ",", &next);
		argc++;
	}
	//Handle each function here
	if (strcmp(functionName, "LoadEngine") == 0)
	{
		const BOOL engine_started = StartEngine(flash_task, argv[0]);
		if (engine_started)
		{
		    messageToReturn = "Engine loaded!";
			sizeOfMessage = strlen(messageToReturn) + 1;
			strcpy_s(returnString, sizeOfMessage, messageToReturn);

			return EXT_DLL_Result::EXT_DLL_Result_OK;
		}

		messageToReturn = "Engine could not be loaded! Check the path you provided.";
	    sizeOfMessage = strlen(messageToReturn) + 1;
		strcpy_s(returnString, sizeOfMessage, messageToReturn);

		return EXT_DLL_Result::EXT_DLL_Result_Error;
	}
	else if (strcmp(functionName, "CreateFlashTask") == 0)
	{
		flash_task = CreateFlashTask();

		if (flash_task != nullptr)
		{

			messageToReturn = "Flash task created!";
			sizeOfMessage = strlen(messageToReturn) + 1;
			strcpy_s(returnString, sizeOfMessage, messageToReturn);

			return EXT_DLL_Result::EXT_DLL_Result_OK;
		}

		messageToReturn = "Cannot create flash task! Please check if Promik is connected.";
		sizeOfMessage = strlen(messageToReturn) + 1;
		strcpy_s(returnString, sizeOfMessage, messageToReturn);

		return EXT_DLL_Result::EXT_DLL_Result_Error;
	}
	else if (strcmp(functionName, "LoadLicense") == 0)
	{
		const FTLicense* lic_status = LoadLicense(flash_task, argv[0]);
		std::string licenseMessage = lic_status->message;

		messageToReturn = licenseMessage.c_str();
	    sizeOfMessage = strlen(messageToReturn) + 1;
		strcpy_s(returnString, sizeOfMessage, messageToReturn);

		if (lic_status->isLoaded)
		{
			return EXT_DLL_Result::EXT_DLL_Result_OK;
		}

		return EXT_DLL_Result::EXT_DLL_Result_Error;
	}
	else if (strcmp(functionName, "ConfigureProgrammer") == 0)
	{
		size_t programmer_id = atoi(argv[0]);
		ConfigureProgrammer(flash_task, programmer_id, argv[1]);

		std::string tempMessage = "Programmer on slot " + std::to_string(programmer_id) + " configured!";
		messageToReturn = tempMessage.c_str();
		sizeOfMessage = strlen(messageToReturn) + 1;
		strcpy_s(returnString, sizeOfMessage, messageToReturn);

		return EXT_DLL_Result::EXT_DLL_Result_OK;
	}
	else if (strcmp(functionName, "LoadJob") == 0)
	{
		const FTJobResult* job_load_result = LoadJob(flash_task, argv[0]);

		std::string jobResult = job_load_result->message;
		messageToReturn = jobResult.c_str();
		sizeOfMessage = strlen(messageToReturn) + 1;
		strcpy_s(returnString, sizeOfMessage, messageToReturn);

		if (job_load_result->status == FTJobStatus::FT_Job_OK)
		{
			return EXT_DLL_Result::EXT_DLL_Result_OK;

		}

		return EXT_DLL_Result::EXT_DLL_Result_Error;
	}
	else if (strcmp(functionName, "UnloadJob") == 0)
	{
		UnloadJob(flash_task);
	
		messageToReturn = "Job unloaded!";
	    sizeOfMessage = strlen(messageToReturn) + 1;
		strcpy_s(returnString, sizeOfMessage, messageToReturn);
		
		return EXT_DLL_Result::EXT_DLL_Result_OK;
	}
	else if (strcmp(functionName, "PrepareProgramming") == 0)
	{
		const FTFlashResult* prepare_status = PrepareProgramming(flash_task);
		if (prepare_status->status == FTFlashStatus::FT_OK)
		{
			messageToReturn = "Programming prepared!";
			sizeOfMessage = strlen(messageToReturn) + 1;
			strcpy_s(returnString, sizeOfMessage, messageToReturn);

			return EXT_DLL_Result::EXT_DLL_Result_OK;
		}

		messageToReturn = "Preparation failed!";
		sizeOfMessage = strlen(messageToReturn) + 1;
		strcpy_s(returnString, sizeOfMessage, messageToReturn);

		return EXT_DLL_Result::EXT_DLL_Result_Error;
	}
	else if (strcmp(functionName, "ExecuteProgramming") == 0)
	{
	    std::string failure_message = {};
	    std::string Target = {};
		std::string Message = {};

		const FTFlashResult* proc_status = ExecuteProgrammingProcess(flash_task);

		if (proc_status->status == FTFlashStatus::FT_OK)
		{
			messageToReturn = "Programming executed successfully!";
			sizeOfMessage = strlen(messageToReturn) + 1;
			strcpy_s(returnString, sizeOfMessage, messageToReturn);

			return EXT_DLL_Result::EXT_DLL_Result_OK;
		}

		//for getting info about failure - to be returned to BTBasic testplan
		for (UINT32 prog = 0; prog < proc_status->programmerResultArraySize; prog++)
		{
			if(debugMode)
		    	std::cout << "\nprogrammer id: " << proc_status->programmerResultArray[prog].id << std::endl;
			for (UINT32 mcu = 0; mcu < proc_status->programmerResultArray[prog].mcuResultArraySize; mcu++)
			{
				const FTMcuResult* mcu_result = &proc_status->programmerResultArray[prog].mcuResultArray[mcu];
				if (debugMode)
				  std::cout << "\n\nMCU device name: " << mcu_result->name << "\nID: " << mcu_result->channel << "\nSlot: " << mcu_result->slot << std::endl;

				for (UINT32 cmd = 0; cmd < mcu_result->cmdResultArraySize; cmd++)
				{
					const FTCommandResult* cmd_result = &mcu_result->cmdResultArray[cmd];
					if (debugMode)
					  std::cout << "\n\nCommand seq_id: " << cmd_result->sequenceId << "\nCommand id: " << cmd_result->cmdId << "\nName: " << cmd_result->name << std::endl;

					for (UINT32 log = 0; log < cmd_result->logArraySize; ++log) {
						const FTLog * log_result = &cmd_result-> logArray[log];
						if (log_result->level == FTLogLevel::FT_Level_ERR)
						{
							Message = log_result->message;

							if (mcu_result->channel == 0)
							{
								Target = "Target A ";
							}
							else if (mcu_result->channel == 1)
							{
								Target = "Target B ";
							}
							
							failure_message += Target + Message+"\n";
							if (debugMode)
								std::cout << "Failure: " << log_result->message << std::endl;
						}
						else 
						{
							if (debugMode)
							  std::cout << "Info: " << log_result->message << std::endl;
						}
					}
				}
			}
		}

		messageToReturn = failure_message.c_str();
		sizeOfMessage = strlen(messageToReturn) + 1;
		strcpy_s(returnString, sizeOfMessage, messageToReturn);
		
		return EXT_DLL_Result::EXT_DLL_Result_Error;

	}
	else if (strcmp(functionName, "DestroyFlashTask") == 0)
	{
		Destroy(flash_task);
		messageToReturn = "FlashTask destroyed!";
		sizeOfMessage = strlen(messageToReturn) + 1;
		strcpy_s(returnString, sizeOfMessage, messageToReturn);

		return EXT_DLL_Result::EXT_DLL_Result_OK;
	}
	else //in case function does not exist
	{
		messageToReturn = "Function does not exist!";
	    sizeOfMessage = strlen(messageToReturn) + 1;
		strcpy_s(returnString, sizeOfMessage, messageToReturn);

		return EXT_DLL_Result::EXT_DLL_Result_Error;
	}

	return EXT_DLL_Result::EXT_DLL_Result_Error;
}