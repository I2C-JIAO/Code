#ifndef __USART_H
#define __USART_H
#include <stdio.h>
#include <stdarg.h>
void Usart_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_Printf(char *format, ...);
int fputc(int ch, FILE *f);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
uint32_t Serial_Pow(uint32_t X, uint32_t Y);
void Serial_SendString(char *String);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendByte(uint8_t Byte);
#endif
