#include "Menu.h"
#include "Common.h"
#include "Ymodem.h"
#include "flash_if.h"

typedef void (*pFunction)(void);
pFunction Jump_To_Application;
uint32_t JumpAddress;
uint8_t FileName[FILE_NAME_LENGTH];
uint8_t Buffer1024[1024] = { 0 };

void SerialDownload(void);
void ExecuteApp(void);

void MenuMain(void)
{
	uint8_t key;

	SerialPutString("\n");
	SerialPutString(
		"================== Main Menu =============================\n");
	SerialPutString(
		"  Download Image To the STM32F103VET6 Internal Flash --- 1\n");
	SerialPutString(
		"  Upload Image From the STM32F103VET6 Internal Flash --- 2\n");
	SerialPutString(
		"  Execute The New Program ------------------------------ 3\n");
	SerialPutString(
		"==========================================================\n");
	SerialPutString("\n");

	key = GetKey();

	switch (key) {
	case '1':
		SerialPutString("serial input 1.\n");
		SerialDownload();
		break;
	case '2':
		SerialPutString("serial input 2.\n");
		break;
	case '3':
		SerialPutString("serial input 3.\n");
		ExecuteApp();
		break;
	default:
		SerialPutString("go to default.\n");
		break;
	}
}

/**
 * @brief  Download a file via serial port
 * @param  None
 * @retval None
 */
void SerialDownload(void)
{
	uint8_t number_str[10] = "          ";
	int32_t size = 0;

	FileName[0] = "update.bin";

	SerialPutString(
		"Waiting for the file to be sent ... (press 'a' to abort)\n");
	FLASH_Unlock();
	size = Ymodem_Receive(&Buffer1024[0]);
	FLASH_Lock();
	if (size > 0) {
		SerialPutString("Programming Completed Successfully!\n"
			"--------------------------------\n"
			"Name: ");
		SerialPutString(FileName);
		Int2Str(number_str, size);
		SerialPutString("\nSize: ");
		SerialPutString(number_str);
		SerialPutString(" Bytes\n");
		SerialPutString("--------------------------------\n");
	} else if (size == -1) {
		SerialPutString(
			"The image size is higher than the allowed space memory!\n");
	} else if (size == -2) {
		SerialPutString("Verification failed!\n");
	} else if (size == -3) {
		SerialPutString("Aborted by user.\n");
	} else {
		SerialPutString("Failed to receive the file!\n");
	}
}

void ExecuteApp(void)
{
	if (((*(__IO uint32_t*) APPLICATION_ADDRESS ) & 0x2FFE0000) == 0x20000000) {
		SerialPutString("Execute user Program.\n");

		JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
		/* Jump to user application */
		Jump_To_Application = (pFunction) JumpAddress;
		/* Initialize user application's Stack Pointer */
		__set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
		Jump_To_Application();
	} else {
		SerialPutString("No user Program.\n");
	}
}

