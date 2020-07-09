/*++

Copyright (c) 2005 Future Technology Devices International Ltd.

Module Name:

    ftcjtag.h

Abstract:

    API DLL for FT2232C Dual Device setup to simulate the Joint Test Action Group(JTAG) synchronous protocol.
    FTCJTAG library definitions

Environment:

    kernel & user mode

Revision History:

    07/02/05    kra     Created.
    24/08/05    kra     Added new function JTAG_GenerateClockPulses and new error code FTC_INVALID_NUMBER_CLOCK_PULSES
	
--*/


#ifndef FTCJTAG_H
#define FTCJTAG_H


// The following ifdef block is the standard way of creating macros
// which make exporting from a DLL simpler.  All files within this DLL
// are compiled with the FTCJTAG_EXPORTS symbol defined on the command line.
// This symbol should not be defined on any project that uses this DLL.
// This way any other project whose source files include this file see
// FTCJTAG_API functions as being imported from a DLL, whereas this DLL
// sees symbols defined with this macro as being exported.

#ifdef FTCJTAG_EXPORTS
#define FTCJTAG_API __declspec(dllexport)
#else
#define FTCJTAG_API __declspec(dllimport)
#endif

typedef DWORD FTC_HANDLE;
typedef ULONG FTC_STATUS;

#define TEST_LOGIC_STATE 1
#define RUN_TEST_IDLE_STATE 2
#define PAUSE_TEST_DATA_REGISTER_STATE 3
#define PAUSE_INSTRUCTION_REGISTER_STATE 4
#define SHIFT_TEST_DATA_REGISTER_STATE 5
#define SHIFT_INSTRUCTION_REGISTER_STATE 6

#define FTC_SUCCESS 0 // FTC_OK
#define FTC_INVALID_HANDLE 1 // FTC_INVALID_HANDLE
#define FTC_DEVICE_NOT_FOUND 2 //FTC_DEVICE_NOT_FOUND
#define FTC_DEVICE_NOT_OPENED 3 //FTC_DEVICE_NOT_OPENED
#define FTC_IO_ERROR 4 //FTC_IO_ERROR
#define FTC_INSUFFICIENT_RESOURCES 5 // FTC_INSUFFICIENT_RESOURCES

#define FTC_FAILED_TO_COMPLETE_COMMAND 20          // cannot change, error code mapped from FT2232c classes
#define FTC_FAILED_TO_SYNCHRONIZE_DEVICE_MPSSE 21  // cannot change, error code mapped from FT2232c classes
#define FTC_INVALID_DEVICE_NAME_INDEX 22           // cannot change, error code mapped from FT2232c classes
#define FTC_NULL_DEVICE_NAME_BUFFER_POINTER 23     // cannot change, error code mapped from FT2232c classes 
#define FTC_DEVICE_NAME_BUFFER_TOO_SMALL 24        // cannot change, error code mapped from FT2232c classes
#define FTC_INVALID_DEVICE_NAME 25                 // cannot change, error code mapped from FT2232c classes
#define FTC_INVALID_LOCATION_ID 26                 // cannot change, error code mapped from FT2232c classes
#define FTC_DEVICE_IN_USE 27                       // cannot change, error code mapped from FT2232c classes
#define FTC_TOO_MANY_DEVICES 28                    // cannot change, error code mapped from FT2232c classes
#define FTC_INVALID_CLOCK_DIVISOR 29
#define FTC_NULL_INPUT_OUTPUT_BUFFER_POINTER 30
#define FTC_INVALID_NUMBER_BITS 31
#define FTC_NULL_WRITE_DATA_BUFFER_POINTER 32
#define FTC_INVALID_NUMBER_BYTES 33
#define FTC_NUMBER_BYTES_TOO_SMALL 34
#define FTC_INVALID_TAP_CONTROLLER_STATE 35
#define FTC_NULL_READ_DATA_BUFFER_POINTER 36
#define FTC_COMMAND_SEQUENCE_BUFFER_FULL 37
#define FTC_NULL_READ_CMDS_DATA_BUFFER_POINTER 38
#define FTC_NO_COMMAND_SEQUENCE 39
#define FTC_INVALID_NUMBER_CLOCK_PULSES 40
#define FTC_NULL_DLL_VERSION_BUFFER_POINTER 41
#define FTC_DLL_VERSION_BUFFER_TOO_SMALL 42
#define FTC_NULL_LANGUAGE_CODE_BUFFER_POINTER 43
#define FTC_NULL_ERROR_MESSAGE_BUFFER_POINTER 44
#define FTC_ERROR_MESSAGE_BUFFER_TOO_SMALL 45
#define FTC_INVALID_LANGUAGE_CODE 46
#define FTC_INVALID_STATUS_CODE 47

#ifdef __cplusplus
extern "C" {
#endif

FTCJTAG_API
FTC_STATUS WINAPI JTAG_GetNumDevices(LPDWORD lpdwNumDevices);

FTCJTAG_API
FTC_STATUS WINAPI JTAG_GetDeviceNameLocID(DWORD dwDeviceNameIndex, LPSTR lpDeviceNameBuffer, DWORD dwBufferSize, LPDWORD lpdwLocationID);

FTCJTAG_API
FTC_STATUS WINAPI JTAG_OpenEx(LPSTR lpDeviceName, DWORD dwLocationID, FTC_HANDLE *pftHandle);

FTCJTAG_API
FTC_STATUS WINAPI JTAG_Open(FTC_HANDLE *pftHandle);

FTCJTAG_API
FTC_STATUS WINAPI JTAG_Close(FTC_HANDLE ftHandle);

FTCJTAG_API
FTC_STATUS WINAPI JTAG_InitDevice(FTC_HANDLE ftHandle, DWORD dwClockDivisor);

FTCJTAG_API
FTC_STATUS WINAPI JTAG_GetClock(DWORD dwClockDivisor, LPDWORD lpdwClockFrequencyHz);

FTCJTAG_API
FTC_STATUS WINAPI JTAG_SetClock(FTC_HANDLE ftHandle, DWORD dwClockDivisor, LPDWORD lpdwClockFrequencyHz);

FTCJTAG_API
FTC_STATUS WINAPI JTAG_SetLoopback(FTC_HANDLE ftHandle, BOOL bLoopbackState);

typedef struct Ft_Input_Output_Pins{
  BOOL  bPin1InputOutputState;
  BOOL  bPin1LowHighState;
  BOOL  bPin2InputOutputState;
  BOOL  bPin2LowHighState;
  BOOL  bPin3InputOutputState;
  BOOL  bPin3LowHighState;
  BOOL  bPin4InputOutputState;
  BOOL  bPin4LowHighState;
}FTC_INPUT_OUTPUT_PINS, *PFTC_INPUT_OUTPUT_PINS;

FTCJTAG_API
FTC_STATUS WINAPI JTAG_SetGPIOs(FTC_HANDLE ftHandle, BOOL bControlLowInputOutputPins,
                                PFTC_INPUT_OUTPUT_PINS pLowInputOutputPinsData,
                                BOOL bControlHighInputOutputPins,
                                PFTC_INPUT_OUTPUT_PINS pHighInputOutputPinsData);

typedef struct Ft_Low_High_Pins{
  BOOL  bPin1LowHighState;
  BOOL  bPin2LowHighState;
  BOOL  bPin3LowHighState;
  BOOL  bPin4LowHighState;
}FTC_LOW_HIGH_PINS, *PFTC_LOW_HIGH_PINS;

FTCJTAG_API
FTC_STATUS WINAPI JTAG_GetGPIOs(FTC_HANDLE ftHandle, BOOL bControlLowInputOutputPins,
                                PFTC_LOW_HIGH_PINS pLowPinsInputData,
                                BOOL bControlHighInputOutputPins,
                                PFTC_LOW_HIGH_PINS pHighPinsInputData);

#define MAX_WRITE_DATA_BYTES_BUFFER_SIZE 65536    // 64k bytes

typedef BYTE WriteDataByteBuffer[MAX_WRITE_DATA_BYTES_BUFFER_SIZE];
typedef WriteDataByteBuffer *PWriteDataByteBuffer;

FTCJTAG_API
FTC_STATUS WINAPI JTAG_Write(FTC_HANDLE ftHandle, BOOL bInstructionTestData, DWORD dwNumBitsToWrite,
                             PWriteDataByteBuffer pWriteDataBuffer, DWORD dwNumBytesToWrite,
                             DWORD dwTapControllerState);

#define MAX_READ_DATA_BYTES_BUFFER_SIZE 65536    // 64k bytes

typedef BYTE ReadDataByteBuffer[MAX_READ_DATA_BYTES_BUFFER_SIZE];
typedef ReadDataByteBuffer *PReadDataByteBuffer;

FTCJTAG_API
FTC_STATUS WINAPI JTAG_Read(FTC_HANDLE ftHandle, BOOL bInstructionTestData, DWORD dwNumBitsToRead,
                            PReadDataByteBuffer pReadDataBuffer, LPDWORD lpdwNumBytesReturned,
                            DWORD dwTapControllerState);

FTCJTAG_API
FTC_STATUS WINAPI JTAG_WriteRead(FTC_HANDLE ftHandle, BOOL bInstructionTestData, DWORD dwNumBitsToWriteRead,
                                 PWriteDataByteBuffer pWriteDataBuffer, DWORD dwNumBytesToWrite,
                                 PReadDataByteBuffer pReadDataBuffer, LPDWORD lpdwNumBytesReturned,
                                 DWORD dwTapControllerState);

FTCJTAG_API
FTC_STATUS WINAPI JTAG_GenerateClockPulses(FTC_HANDLE ftHandle, DWORD dwNumClockPulses);

FTCJTAG_API
FTC_STATUS WINAPI JTAG_ClearCmdSequence(void);

FTCJTAG_API
FTC_STATUS WINAPI JTAG_AddWriteCmd(BOOL bInstructionTestData, DWORD dwNumBitsToWrite,
                                   PWriteDataByteBuffer pWriteDataBuffer, DWORD dwNumBytesToWrite,
                                   DWORD dwTapControllerState);

FTCJTAG_API
FTC_STATUS WINAPI JTAG_AddReadCmd(BOOL bInstructionTestData, DWORD dwNumBitsToRead, DWORD dwTapControllerState);

FTCJTAG_API
FTC_STATUS WINAPI JTAG_AddWriteReadCmd(BOOL bInstructionTestData, DWORD dwNumBitsToWriteRead,
                                       PWriteDataByteBuffer pWriteDataBuffer, DWORD dwNumBytesToWrite,
                                       DWORD dwTapControllerState);

#define MAX_READ_CMDS_DATA_BYTES_BUFFER_SIZE 131071  // 128K bytes 

typedef BYTE ReadCmdSequenceDataByteBuffer[MAX_READ_CMDS_DATA_BYTES_BUFFER_SIZE];
typedef ReadCmdSequenceDataByteBuffer *PReadCmdSequenceDataByteBuffer;

FTCJTAG_API
FTC_STATUS WINAPI JTAG_ClearDeviceCmdSequence(FTC_HANDLE ftHandle);

FTCJTAG_API
FTC_STATUS WINAPI JTAG_AddDeviceWriteCmd(FTC_HANDLE ftHandle, BOOL bInstructionTestData, DWORD dwNumBitsToWrite,
                                         PWriteDataByteBuffer pWriteDataBuffer, DWORD dwNumBytesToWrite,
                                         DWORD dwTapControllerState);

FTCJTAG_API
FTC_STATUS WINAPI JTAG_AddDeviceReadCmd(FTC_HANDLE ftHandle, BOOL bInstructionTestData, DWORD dwNumBitsToRead, DWORD dwTapControllerState);

FTCJTAG_API
FTC_STATUS WINAPI JTAG_AddDeviceWriteReadCmd(FTC_HANDLE ftHandle, BOOL bInstructionTestData, DWORD dwNumBitsToWriteRead,
                                             PWriteDataByteBuffer pWriteDataBuffer, DWORD dwNumBytesToWrite,
                                             DWORD dwTapControllerState);

FTCJTAG_API
FTC_STATUS WINAPI JTAG_ExecuteCmdSequence(FTC_HANDLE ftHandle, PReadCmdSequenceDataByteBuffer pReadCmdSequenceDataBuffer,
                                          LPDWORD lpdwNumBytesReturned);

FTCJTAG_API
FTC_STATUS WINAPI JTAG_GetDllVersion(LPSTR lpDllVersionBuffer, DWORD dwBufferSize);

FTCJTAG_API
FTC_STATUS WINAPI JTAG_GetErrorCodeString(LPSTR lpLanguage, FTC_STATUS StatusCode,
                                          LPSTR lpErrorMessageBuffer, DWORD dwBufferSize);


#ifdef __cplusplus
}
#endif


#endif  /* FTCJTAG_H */