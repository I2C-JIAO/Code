#ifndef __USART_H
#define __USART_H
#include <stdio.h>
void Usart_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_Printf(char *format, ...);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_SendString(char *String);
void Serial_SendArray(uint8_t *Array, uint16_t Length);

extern uint8_t Serial_RxPacket[];
extern uint8_t Serial_TxPacket[];

void USART1_IRQHandler(void);
void Serial_SendPacket(void);

uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);
#endif
