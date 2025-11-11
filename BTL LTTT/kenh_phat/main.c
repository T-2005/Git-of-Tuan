#include <stm32f10x.h>
#include <stdint.h>
#include <stdio.h>

void USART1_Config(void);
void delay(uint16_t ms);
void USART1_Send_char(int ch);
void UART1_printf(char *s);
char USART1_read(void);
void __print_uint16(uint16_t num);
void GPIO_Config(void);
void ADC_Config(void);
uint16_t ADC_Read(void);
volatile uint16_t a;
void _float_to_binary(uint16_t a);
int r[16] = {0}, k = 0;
int cnt = 0;
int main(void)
{
    GPIO_Config();
    ADC_Config();
    USART1_Config();

    while (cnt < 33)
    {
        a = ADC_Read();          // doc giá tri tu PA0
        _float_to_binary(a);     // chuyen sang nhi phân
        
        // Gui nhi phân ra UART
        for (int i = 0; i < k; i++)
        {
            USART1_Send_char(r[i] + '0'); // chuyen 0/1 -> ki tu '0'/'1'
					cnt++;
        }

        UART1_printf("\r\n");    
        delay(500);             
				
    }
}

void _float_to_binary(uint16_t value)
{
    k = 0; 

    for (int i = 11; i >= 0; i--) // ADC 12-bit
    {
        r[k++] = (value >> i) & 1; // do may tinh luu so nguyen o dang bit nen minh su dung dich bit de luu vao mang
    }
}

		
void GPIO_Config(void)
{
//	RCC -> APB2ENR |= (1 << 2); // set clock cho port A
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN;

    GPIOA->CRL &= ~(0xF << (0 * 4));

    GPIOC->CRH &= ~(0xF << (5 * 4));
    GPIOC->CRH |= (0x2 << (5 * 4));
    GPIOC->BSRR = (1 << 13);

}
void ADC_Config(void) {
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    ADC1->SQR3 = 0; // bat kenh 0 -> PA0 

    ADC1->CR2 |= (1 << 20);
    ADC1->CR2 |= (7 << 17);

    ADC1->CR2 |= (1 << 0);
    for (volatile uint32_t i = 0; i < 1000; i++);

    ADC1->CR2 |= (1 << 3);
    while (ADC1->CR2 & (1 << 3));
    ADC1->CR2 |= (1 << 2);
    while (ADC1->CR2 & (1 << 2));
}

uint16_t ADC_Read(void) {
    ADC1->CR2 |= (1 << 22);
    while (!(ADC1->SR & (1 << 1)));
    return (uint16_t)ADC1->DR;
}
void USART1_Config(void)
{
	/*F cua bus APB1 max la 36MHz
		F cua bus APB2 max la 72MHz
		Mode = 00 - f(default); 01 - f(10MHz); 10 - f(2MHz); 11 - f(50MHz)*/
	RCC -> APB2ENR |= (1 << 14); // bat clock cho USART1
	RCC -> APB2ENR |= (1 << 2); // bat clock cho port A
	
	/*PA9 - Tx(output); PA10 - Rx(input)*/
	GPIOA -> CRH &= ~(0xF << (1 * 4)); // clear data on PA9
	GPIOA -> CRH |= (0xB << (1*4)); // set f cua Tx = 50MHz - che do output
	
	GPIOA -> CRH &= ~(0xF << (2 * 4)); // clear data on PA10
	GPIOA -> CRH |= (0x4 << (2 * 4)); // set Rx - che do 	 input
	
	/* setup toc do truyen - nhan tin cho PA*/
	USART1 -> BRR = 0x271; //(72MHz / 115200) = 625 -> 0x271; BRR -> baud rate register
	USART1 -> CR1 |= (1 << 13) | (1 << 3); // bit13 bat module USART1 va bit3 bat chuc nang chuyen Tx <=> UE = 1 & Tx = 1
	USART1 -> CR1 |= USART_CR1_RE; // bit2 bat chuc nang nhan Rx
}
void delay(uint16_t ms)
{
	for(int i = 0; i < ms; i++)
		for(int j = 0; j < 0xAFF; j++);
}
void USART1_Send_char(int ch)
{
	while(!(USART1 -> SR & (1 << 7))){} // TXE = 1 -> (data trong) -> dung truyen
	
		USART1 -> DR = (ch & 0xFF); 
	
}
void UART1_printf(char *s) // tro den chuoi ki tu kieu char
{
	while(*s)
	{
		USART1_Send_char(*s++);
	}
}
char USART1_read(void)
{
	while(!(USART1 -> SR & (1 << 5))){}
		return (USART1 -> DR);
}
void __print_uint16(uint16_t num)
{
	char buffer[10];
	sprintf(buffer, "%u", num); // chuy?n s? thành chu?i
	UART1_printf(buffer);
}	