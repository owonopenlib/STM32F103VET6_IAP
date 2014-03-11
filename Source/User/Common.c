#include "Common.h"

/**
 * @brief  Test to see if a key has been pressed on the HyperTerminal
 * @param  key: The key pressed
 * @retval 1: Correct
 *         0: Error
 */
uint32_t SerialKeyPressed(uint8_t *key)
{

	if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET) {
		*key = (uint8_t) USART1->DR;
		return 1;
	} else {
		return 0;
	}
}

/**
 * @brief  Receive byte from sender
 * @param  c: Character
 * @param  timeout: Timeout
 * @retval 0: Byte received
 *        -1: Timeout
 */
int32_t SerialReceiveByte(uint8_t *c, uint32_t timeout)
{
	while (timeout-- > 0) {
		if (SerialKeyPressed(c) == 1) {
			return 0;
		}
	}
	return -1;
}

/**
 * @brief  Get a key from the HyperTerminal
 * @param  None
 * @retval The Key Pressed
 */
uint8_t GetKey(void)
{
	uint8_t key = 0;

	/* Waiting for user input */
	while (1) {
		if (SerialKeyPressed((uint8_t*) &key))
			break;
	}
	return key;

}

void SerialPutChar(char c)
{
	USART_SendData(USART1, c);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {
		;
	}
}

void SerialPutString(char *s)
{
	while (*s != '\0') {
		if (*s == '\n')
			SerialPutChar('\r');
		SerialPutChar(*s++);
	}
}

void Delay_mS(uint32_t n)
{
	uint32_t tmp;

	while (n--) {
		tmp = 4000;
		while (tmp--) {
			;
		}
	}
}

/**
 * @brief  Convert an Integer to a string
 * @param  str: The string
 * @param  intnum: The integer to be converted
 * @retval None
 */
void Int2Str(uint8_t* str, int32_t intnum)
{
	uint32_t i, Div = 1000000000, j = 0, Status = 0;

	for (i = 0; i < 10; i++) {
		str[j++] = (intnum / Div) + 48;

		intnum = intnum % Div;
		Div /= 10;
		if ((str[j - 1] == '0') & (Status == 0)) {
			j = 0;
		} else {
			Status++;
		}
	}
}

/**
 * @brief  Convert a string to an integer
 * @param  inputstr: The string to be converted
 * @param  intnum: The integer value
 * @retval 1: Correct
 *         0: Error
 */
uint32_t Str2Int(uint8_t *inputstr, int32_t *intnum)
{
	uint32_t i = 0, res = 0;
	uint32_t val = 0;

	if (inputstr[0] == '0' && (inputstr[1] == 'x' || inputstr[1] == 'X')) {
		if (inputstr[2] == '\0') {
			return 0;
		}
		for (i = 2; i < 11; i++) {
			if (inputstr[i] == '\0') {
				*intnum = val;
				/* return 1; */
				res = 1;
				break;
			}
			if (ISVALIDHEX(inputstr[i])) {
				val = (val << 4) + CONVERTHEX(inputstr[i]);
			} else {
				/* Return 0, Invalid input */
				res = 0;
				break;
			}
		}
		/* Over 8 digit hex --invalid */
		if (i >= 11) {
			res = 0;
		}
	} else /* max 10-digit decimal input */
	{
		for (i = 0; i < 11; i++) {
			if (inputstr[i] == '\0') {
				*intnum = val;
				/* return 1 */
				res = 1;
				break;
			} else if ((inputstr[i] == 'k' || inputstr[i] == 'K') && (i > 0)) {
				val = val << 10;
				*intnum = val;
				res = 1;
				break;
			} else if ((inputstr[i] == 'm' || inputstr[i] == 'M') && (i > 0)) {
				val = val << 20;
				*intnum = val;
				res = 1;
				break;
			} else if (ISVALIDDEC(inputstr[i])) {
				val = val * 10 + CONVERTDEC(inputstr[i]);
			} else {
				/* return 0, Invalid input */
				res = 0;
				break;
			}
		}
		/* Over 10 digit decimal --invalid */
		if (i >= 11) {
			res = 0;
		}
	}

	return res;
}
