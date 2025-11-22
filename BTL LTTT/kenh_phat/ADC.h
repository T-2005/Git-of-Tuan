#ifndef ADC_H
#define ADC_H
#include <stdint.h>
#include <stm32f10x.h>
#include <stdio.h>
void ADC_Config(void);
uint16_t ADC_Read(void);
void GPIO_Config(void);
#endif