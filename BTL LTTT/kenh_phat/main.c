#include <stm32f10x.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "ADC.h"
#include "UART.h"


volatile uint16_t a;
void _float_to_binary(uint16_t a);
int r[12] = {0}, k = 0;
int cnt = 0;
char kitu[40];
int main(void)
{
    GPIO_Config();
    ADC_Config();
    USART2_Config();
		
    while (1){
			
		//UART1_printf("HELLO_FROM_STM32\n");
			 uint8_t cmd = USART2_read();  // d?c 1 byte l?nh t? UART

        if (cmd == '1')  // n?u nh?n l?nh '1' thì th?c hi?n do ADC
        {
            cnt = 0;  // reset v? trí ghi trong buffer

            for (int j = 0; j < 3; j++)  // d?c 3 m?u ADC
            {
                a = ADC_Read();          // d?c giá tr? ADC (0–4095)
                _float_to_binary(a);     // chuy?n sang nh? phân

                for (int i = 0; i < k; i++)
                    kitu[cnt++] = r[i] + '0';
            }

            kitu[cnt] = '\0';  // k?t thúc chu?i
						
           for(int i = 0; i < 32 ; i++)
						{
							USART2_Send_char(kitu[i]);
							delay(1);
						}
						//UART1_printf("HELLO_FROM_STM32HELLO_FROM_STM32");

        }
				else NVIC_SystemReset();
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


 
		




