#include<stm32f411xe.h>
#include "uart_tx.h"

#define SYS_FREQ         16000000
#define APB1_CLK		 SYS_FREQ
#define UART_BAURATE	 115200
#define UART2EN			(1U<<17)
#define GPIOAEN			(1U<<0)
#define CR1_TE			(1U<<3)
#define CR1_EN			(1U<<13)
#define SR_TXE			(1U<<7)

void write_string(int wordlength,char* word);
static uint16_t compute_uart_bd(uint32_t PeriphClk,uint32_t BauRate);
static void uart_set_baurate(USART_TypeDef *USARTx,uint32_t PeriphClk, uint32_t BauRate);
void uart2_tx_init(void)
{
	/***************************Configure uart gpio pin*****************************/
	/*Enable clock access to gpioa*/
	RCC->AHB1ENR|=GPIOAEN;
	/*Set PA2 as alternate function mode*/
	GPIOA->MODER|=(1U<<5);
	GPIOA->MODER&=~(1U<<4);
	/*Set PA2 as alternate function type UART_TX (AF07)*/
	GPIOA->AFR[0]|=(1U<<8);
	GPIOA->AFR[0]|=(1U<<9);
	GPIOA->AFR[0]|=(1U<<10);
	GPIOA->AFR[0]&=~(1U<<11);
	/***************************Configure uart module*******************************/
	/*Enable clock access to uart2*/
	RCC->APB1ENR|=UART2EN;
	/*Configure baudrate*/
	uart_set_baurate(USART2,APB1_CLK, UART_BAURATE);
	/*Configure transfer direction*/
	USART2->CR1|=CR1_TE;
	/*Enable uart module*/
	USART2->CR1|=CR1_EN;
}

void uart2_write(int ch)
{
	/*Make sure the transmit data register is empty*/
	while(!(USART2->SR & SR_TXE));
	/*Write to transmit data register*/
	USART2->DR=(ch & 0xFF);
}

void write_string(int wordlength,char* word)
{
	for(int i=0;i<wordlength;i++)
	{
		uart2_write(word[i]);
	}

}

static void uart_set_baurate(USART_TypeDef *USARTx,uint32_t PeriphClk, uint32_t BauRate)
{
	USARTx->BRR=compute_uart_bd(PeriphClk,BauRate);

}
static uint16_t compute_uart_bd(uint32_t PeriphClk,uint32_t BauRate)
{
	return (PeriphClk+(BauRate/2U))/BauRate;
}
