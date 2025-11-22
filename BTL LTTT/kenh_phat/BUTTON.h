#ifndef BUTTON_H
#define BUTTON_H
#include <stm32f10x.h>
#include <stdint.h>
#define button_bat GPIO_Pin_8
#define button_tat GPIO_Pin_13
extern int check;
void Button_Config(void);
void Led_Config(void);
void TIMER_Config(void);
void bat_led(void);
void tat_led(void);
void TIM2_Handler(void);
void delay_ms(uint32_t ms);
void PC13_Button(void);
#endif