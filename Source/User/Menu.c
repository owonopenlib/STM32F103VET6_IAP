#include "Menu.h"
#include "main.h"

void SerialGetKey(uint8_t *c)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET) {
		;
	}
	*c = USART_ReceiveData(USART1);
}

void MenuMain(void)
{
	uint8_t c;

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

	SerialGetKey(&c);

	switch (c) {
	case '1':
		SerialPutString("serial input 1.\n");
		break;
	case '2':
		SerialPutString("serial input 2.\n");
		break;
	case '3':
		SerialPutString("serial input 3.\n");
		break;
	default:
		SerialPutString("go to default.\n");
		break;
	}
}
