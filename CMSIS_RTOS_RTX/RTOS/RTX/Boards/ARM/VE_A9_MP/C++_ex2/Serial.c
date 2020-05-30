 /**************************************************************************//**
 * @file     Serial.c
 * @brief    Simple polled UART driver. Needs to be completed 
 * @version
 * @date     07 February 2013
 *
 * @note
 *
 ******************************************************************************/
/* Copyright (c) 2011 - 2013 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/
   
#include "Serial.h"

void SER_Init(void)
{
	UART0->UARTCR = UART0->UARTCR | 0x301;	// Enable UART and enable TX/RX
}

void SER_Enable(void)
{
    UART0->UARTCR = UART_CR_UARTEN | UART_CR_TXE | UART_CR_RXE;
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
void SER_Disable(void)
{
	UART0->UARTCR = 0x0;
}

/*----------------------------------------------------------------------------
  Set baud rate
 *----------------------------------------------------------------------------*/
void SER_Set_baud_rate(uint32_t baud_rate)
{
	uint32_t divider;
	uint32_t mod;
	uint32_t fraction;

    /*
     * Set baud rate
     *
     * IBRD = UART_CLK / (16 * BAUD_RATE)
     * FBRD = ROUND((64 * MOD(UART_CLK,(16 * BAUD_RATE))) / (16 * BAUD_RATE))
     */
    divider   = UART0_CLK / (16 * baud_rate);
    mod       = UART0_CLK % (16 * baud_rate);
    fraction  = (((8 * mod) / baud_rate) >> 1) + (((8 * mod) / baud_rate) & 1);

    UART0->UARTIBRD = divider;
    UART0->UARTFBRD = fraction;
}

/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
void SER_PutChar(char c)
{
    while ( (UART0->UARTFR & 0x08));	// Wait for UART TX to become free. Note that FIFOs are not being used here
	    UART0->UARTDR = c;
}

/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
char SER_GetChar (void)
{
	while ( !(UART0->UARTFR & 0x40));	// Wait for UART TX to become free. Note that FIFOs are not being used here
	    return UART0->UARTDR;
}

/*----------------------------------------------------------------------------
  Serial Uart interrupt handler
 *----------------------------------------------------------------------------*/
void interrupt_SER(void)
{
	//your code here
}

