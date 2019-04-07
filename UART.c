/*
 * UART.c
 *
 *  Created on: 06/04/2019
 *      Author: Diego  Charles and Ruben Charles
 *    brief:  implementation driver for UART module
 */

#include "UART.h"
//global g_uart0_mail_box
uart_mail_box_t global_g_uart0_mail_box  ={0};
void UART_init(uart_channel_t uart_channel, uint32_t system_clk , uart_baud_rate_t baud_rate)
{
	/* UART baud rate = UART module clock / (16 × (SBR[12:0] + BRFD))*/
	float float_SBR= (float)system_clk/ (16*baud_rate);
	float BFRD=  float_SBR -(float)((uint32_t)(float_SBR));
	uint32_t SBR=(uint32_t)(float_SBR);
	uint32_t BFRA= (BFRD*32) ;
	SBR = system_clk / (16* (uint32_t)baud_rate);
	/* initializing selected uart channel with her baud rate*/
	switch(uart_channel)
	{
	case  UART_0 :
		SIM->SCGC4 |= SIM_SCGC4_UART0_MASK ;/* enabling clock*/
		UART0->C2 &= ~ ( UART_C2_TE_MASK );/*disabling transmitter*/
		UART0->C2 &= ~ ( UART_C2_RE_MASK );/*disabling receptor*/
		UART0->BDH = (SBR>>8) & (UART_BDH_SBR_MASK) ;  /* Copiar los bits UART baud rate [12:8] a los bits SRB del registro UART0_BDH */
		UART0->BDL = (SBR) & ( UART_BDL_SBR_MASK );
		UART0->C4  |= (BFRA) & (UART_C4_BRFA_MASK);
		/*Habilitar el transmisor y el receptor de la UART en el registro UART0_C2*/
		UART0->C2 |= ( UART_C2_TE_MASK );/* enabling transmitter*/
		UART0->C2 |=  ( UART_C2_RE_MASK );/* enabling receptor*/
		break;
	case  UART_1 :
		SIM->SCGC4 |= SIM_SCGC4_UART1_MASK ;
		break;
	case  UART_2 :
		SIM->SCGC4 |= SIM_SCGC4_UART2_MASK ;
		break;
	case  UART_3 :
		SIM->SCGC4 |= SIM_SCGC4_UART3_MASK ;
		break;
	case  UART_4 :
		SIM->SCGC1 |= SIM_SCGC1_UART4_MASK  ;
		break;
	case  UART_5 :
		SIM->SCGC1 |= SIM_SCGC1_UART5_MASK ;
		break;
	default:
		break;
	}
}
void UART_interrupt_enable(uart_channel_t uart_channel)
{
	/* enabling correspondig reception interrupt in register UART_C2 ,RIE flag*/
	switch (uart_channel)
	{
	case UART_0:
		UART0->C2 |= UART_C2_RIE_MASK ;
		break;
	case UART_1:
		UART1->C2 |= UART_C2_RIE_MASK ;
		break;
	case UART_2:
		UART2->C2 |= UART_C2_RIE_MASK ;
		break;
	case UART_3:
		UART3->C2 |= UART_C2_RIE_MASK ;
		break;
	case UART_4:
		UART4->C2 |= UART_C2_RIE_MASK ;
		break;
	case UART_5:
		UART0->C2 |= UART_C2_RIE_MASK ;
		break;
	default:
		break;
	}
}
void UART_put_char (uart_channel_t uart_channel, uint8_t character)
{
	/* Before transmitting its important to check if the  UART channel
	 * is not transmitting already checking  flag*/
	char sended_char= character;
	switch (uart_channel)
	{
	case UART_0:
		while(0 == (UART0->S1 & UART_S1_TC_MASK ));/* wait until flag is on zero*/

			UART0->D = character ;
		break;
	case UART_1:
		while(0 != (UART1->S1 & UART_S1_TDRE_MASK ));/* wait until flag is on zero*/
		UART1->D = character ;
		break;
	case UART_2:
		while(1 != (UART2->S1 & UART_S1_TDRE_MASK ));/* wait until flag is on zero*/
		UART2->D = character ;
		break;
	case UART_3:
		while(1 != (UART3->S1 & UART_S1_TDRE_MASK ));/* wait until flag is on zero*/
		UART3->D = character ;
		break;
	case UART_4:
		if(0 != (UART4->S1 & UART_S1_TDRE_MASK ))/* wait until flag is on zero*/
		UART4->D = character ;
		break;
	case UART_5:
		while(1 != (UART5->S1 & UART_S1_TDRE_MASK ));/* wait until flag is on zero*/
		UART5->D = character ;
		break;
	default:
		break;
	}
}
void UART_put_string(uart_channel_t uart_channel, int8_t* string)
{
	/* send string till null character*/
	while (0 != *string )
	{
		UART_put_char ( uart_channel, *(string) );

		string++;

	}

}
