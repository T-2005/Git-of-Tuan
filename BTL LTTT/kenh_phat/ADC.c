#include "ADC.h"
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