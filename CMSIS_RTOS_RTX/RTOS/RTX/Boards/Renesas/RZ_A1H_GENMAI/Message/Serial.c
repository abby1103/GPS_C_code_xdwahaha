 /**************************************************************************//**
 * @file     Serial.c
 * @brief    Simple polled UART driver.
 * @version
 * @date     19 Sept 2013
 *
 ******************************************************************************/

/* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved */

#include "Serial.h"

void SER_Init(void)
{
    /* ==== SCIF initial setting ==== */
    CPG.STBCR4 &= (uint8_t)~(CPG_STBCR4_BIT_MSTP45);     /* enable SCIF2 clock */

    /* ---- Serial control register (SCSCR2) setting ---- */
    /* SCIF transmitting and receiving operations stop */
    SCIF2.SCSCR = (uint16_t)0x0000u;

    /* ---- FIFO control register (SCFCR2) setting ---- */
    /* Transmit FIFO reset */
    /* Receive FIFO data register reset */
    SCIF2.SCFCR |= (uint16_t)(SCIF_FCR_BIT_TFRST | SCIF_FCR_BIT_RFRST);

    /* ---- Serial status register(SCFSR2) setting ---- */
    /* ER,BRK,RDF,DR bit clear */
    SCIF2.SCFSR &= (uint16_t)~(SCIF_FSR_BIT_ER | SCIF_FSR_BIT_BRK | SCIF_FSR_BIT_RDF | SCIF_FSR_BIT_DR);

    /* ---- Line status register (SCLSR2) setting ---- */
    /* ORER bit clear */
    SCIF2.SCLSR &= (uint16_t)~(SCIF_LSR_BIT_ORER);

    /* ---- Serial control register (SCSCR2) setting ---- */
    /* B'00 : Internal CLK */
    SCIF2.SCSCR &= (uint16_t)~(SCIF_SCR_CKE11);

    /* ---- Serial mode register (SCSMR2) setting ---- */
    /*  Communication mode 0: Asynchronous mode
        Character length   0: 8-bit data
        Parity enable      0: Add and check are disabled
        Stop bit length    0: 1 stop bit */
    SCIF2.SCSMR &= (uint16_t)~(SCIF_SMR_BIT_CA | SCIF_SMR_BIT_CHR | SCIF_SMR_BIT_PE | SCIF_SMR_BIT_OE | SCIF_SMR_BIT_STOP | SCIF_SMR_CKS11);

    /* ---- Sets the Serial extension mode register (SCEMR2) ---- */
    /* Baud rate generator double-speed mode, 0: Normal mode */
    /* Base clock select in asynchronous mode, */
    /*   0: Base clock is 16 times the bit rate */
    SCIF2.SCEMR = (uint16_t)0x0000u;

    /* ---- Bit rate register (SCBRR2) setting ---- */
    /* 53 = 38400bps */
    /* 35 = 57600bps */
    /* 17 = 115200bps */
    /* 8  = 230400bps */
    SCIF2.SCBRR = (uint8_t)17u; /* 115200bps */


    /* ---- FIFO control register (SCFCR2) setting ---- */
    /*  RTS output active trigger        :Initial value
        Receive FIFO data trigger        :1-data
        Transmit FIFO data trigger       :0-data
        Modem control enable             :Disabled
        Receive FIFO data register reset :Disabled
        Loop-back test                   :Disabled */
    SCIF2.SCFCR = (uint16_t)0x0030u;

    /* ---- Serial port register (SCSPTR2) setting ---- */
    /* Serial port  break output(SPB2IO)  1: Enabled */
    /* Serial port break data(SPB2DT)  1: High-level */
    SCIF2.SCSPTR |= (uint16_t)0x0003u;

    /* GPIO Settings:P3_0 / TxD2 */
    GPIO.PIBC3  &= (uint16_t)~(GPIO_BIT_N0);
    GPIO.PBDC3  &= (uint16_t)~(GPIO_BIT_N0);
    GPIO.PM3    |= (uint16_t) (GPIO_BIT_N0);
    GPIO.PMC3   &= (uint16_t)~(GPIO_BIT_N0);
    GPIO.PIPC3  &= (uint16_t)~(GPIO_BIT_N0);

    GPIO.PBDC3  &= (uint16_t)~(GPIO_BIT_N0);
    GPIO.PFC3   |= (uint16_t) (GPIO_BIT_N0);
    GPIO.PFCE3  &= (uint16_t)~(GPIO_BIT_N0);
    GPIO.PFCAE3 |= (uint16_t) (GPIO_BIT_N0);

    GPIO.PIPC3  |= (uint16_t) (GPIO_BIT_N0);
    GPIO.PMC3   |= (uint16_t) (GPIO_BIT_N0);

    /* GPIO Settings:P3_2 / RxD2 */
    GPIO.PIBC3  &= (uint16_t)~(GPIO_BIT_N2);
    GPIO.PBDC3  &= (uint16_t)~(GPIO_BIT_N2);
    GPIO.PM3    |= (uint16_t) (GPIO_BIT_N2);
    GPIO.PMC3   &= (uint16_t)~(GPIO_BIT_N2);
    GPIO.PIPC3  &= (uint16_t)~(GPIO_BIT_N2);

    GPIO.PBDC3  &= (uint16_t)~(GPIO_BIT_N2);
    GPIO.PFC3   |= (uint16_t) (GPIO_BIT_N2);
    GPIO.PFCE3  |= (uint16_t) (GPIO_BIT_N2);
    GPIO.PFCAE3 &= (uint16_t)~(GPIO_BIT_N2);

    GPIO.PIPC3  |= (uint16_t) (GPIO_BIT_N2);
    GPIO.PMC3   |= (uint16_t) (GPIO_BIT_N2);

    SER_Enable();
}

void SER_Enable(void)
{
    SCIF2.SCSCR |= (uint16_t) (SCIF_SCR_BIT_TE | SCIF_SCR_BIT_RE);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
void SER_Disable(void)
{
    SCIF2.SCSCR &= (uint16_t)~(SCIF_SCR_BIT_TE | SCIF_SCR_BIT_RE);
}

/*----------------------------------------------------------------------------
  Set baud rate
 *----------------------------------------------------------------------------*/
void SER_Set_baud_rate(uint32_t baud_rate)
{
    //your code here
}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
void SER_PutChar(uint8_t buffer)
{
    /* Check if it is possible to transmit (TDFE flag) */
    while (0u == (SCIF2.SCFSR & SCIF_FSR_BIT_TDFE))
    {
        /* Wait */
    }

    /* Write the receiving data in TDR */
    SCIF2.SCFTDR = buffer;

    /* Clear TDFE and TEND flag */
    SCIF2.SCFSR &= (uint16_t)~(SCIF_FSR_BIT_TEND | SCIF_FSR_BIT_TDFE);
}

/*----------------------------------------------------------------------------
  Read character from Serial Port
 *----------------------------------------------------------------------------*/
uint32_t SER_GetChar (void)
{
    uint8_t data;

    /* Confirming receive error(ER,DR,BRK) */
    if (SCIF2.SCFSR & SCIF_FSR_BIT_ALLERR)
    {
        /* Detect receive error */
        SCIF2.SCSCR &= (uint16_t)~(SCIF_SCR_BIT_RE);    /* Disable reception  */
        SCIF2.SCFCR |= (uint16_t)  SCIF_FCR_BIT_RFRST;  /* Reset receiving FIFO          */
        SCIF2.SCFCR &= (uint16_t)~(SCIF_FCR_BIT_RFRST); /* Clearing FIFO reception reset */
        SCIF2.SCFSR &= (uint16_t)~(SCIF_FSR_BIT_ALLERR);/* Error bit clear    */
        SCIF2.SCSCR |= (uint16_t)  SCIF_SCR_BIT_RE;     /* Enable reception   */
        return 0;
    }

    /* Is there receive FIFO data? */
    while (0u == (SCIF2.SCFSR & SCIF_FSR_BIT_RDF))
    {
        /* WAIT */
    }

    /* Read receive data */
    data = SCIF2.SCFRDR;
    /* Clear RDF */
    SCIF2.SCFSR &= (uint16_t)~(SCIF_FSR_BIT_RDF);

    /* Is it overflowed? */
    if (0u != (SCIF2.SCLSR & SCIF_LSR_BIT_ORER))
    {
        SCIF2.SCLSR &= (uint16_t)~(SCIF_LSR_BIT_ORER);       /* ORER clear */
    }

    return data;
}

/*----------------------------------------------------------------------------
  Serial Uart interrupt handler
 *----------------------------------------------------------------------------*/
void interrupt_SER(void)
{
    //your code here
}
