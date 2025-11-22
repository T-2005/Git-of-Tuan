#ifndef UART_H
#define UART_H
#include <stdint.h>
#include <stm32f10x.h>
#include <stdio.h>
void USART2_Config(void);
void delay(uint16_t ms);
void USART2_Send_char(int ch);
void UART2_printf(char *s);
char USART2_read(void);
void __print_uint16_usart2(uint16_t num);

#endif