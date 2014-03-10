#ifndef __COMMON_H__
#define __COMMON_H__

#include "stm32f10x.h"

/* Common routines */
#define IS_AF(c)  ((c >= 'A') && (c <= 'F'))
#define IS_af(c)  ((c >= 'a') && (c <= 'f'))
#define IS_09(c)  ((c >= '0') && (c <= '9'))
#define ISVALIDHEX(c)  IS_AF(c) || IS_af(c) || IS_09(c)
#define ISVALIDDEC(c)  IS_09(c)
#define CONVERTDEC(c)  (c - '0')

#define CONVERTHEX_alpha(c)  (IS_AF(c) ? (c - 'A'+10) : (c - 'a'+10))
#define CONVERTHEX(c)   (IS_09(c) ? (c - '0') : CONVERTHEX_alpha(c))

void SerialPutChar(char c);
void SerialPutString(char *s);
void Delay_mS(uint32_t n);
uint8_t GetKey(void);
uint32_t SerialKeyPressed(uint8_t *key);
void Int2Str(uint8_t* str,int32_t intnum);
uint32_t Str2Int(uint8_t *inputstr,int32_t *intnum);

#endif /* __COMMON_H__ */
