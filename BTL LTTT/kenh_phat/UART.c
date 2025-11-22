#include "UART.h"
#include <stm32f10x.h>
void USART2_Config(void)
{
	RCC -> APB1ENR |= (1 << 17); // bat clock cho usart2
	RCC -> APB2ENR |= (1 << 2);

	
	GPIOA -> CRL &= ~(0xF << 8); // PA2, PA3
	GPIOA -> CRL |= (0xB <<  8); // set f cua Tx = 50MHz - che do ouput
	
	GPIOA -> CRL &= ~(0xF << 12); 
	GPIOA -> CRL |= (0x4 << 12);
	

	USART2 -> BRR = 0x139;
	USART2 -> CR1 |= (1 << 13) | (1 << 3); // bat module usart2 va cho phep truyen
	USART2 -> CR1 |= (1 << 2); // cho phep nhan
}
	

void delay(uint16_t ms)
{
	for(int i = 0; i < ms; i++)
		for(int j = 0; j < 0xAFF; j++);
}
void USART2_Send_char(int ch)
{
	while(!(USART2 -> SR & (1 << 7))){} // TXE = 1 -> (data trong) -> dung truyen
	
		USART2 -> DR = (ch & 0xFF); 
	
}
void UART2_printf(char *s) // tro den chuoi ki tu kieu char
{
	while(*s)
	{
		USART2_Send_char(*s++);
	}
}
char USART2_read(void)
{
	while(!(USART2 -> SR & (1 << 5))){}
		return (USART2 -> DR);
}
void __print_uint16_usart2(uint16_t num)
{
	char buffer[10];
	sprintf(buffer, "%u", num); // chuy?n s? thành chu?i
	UART2_printf(buffer);
}	
		