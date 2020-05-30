/**************************************************************************//**
* @file          Serial.h
* $Rev: 507 $
* $Date:: 2013-04-10 12:30:21 +0900#$
* @brief         SCIF macros
******************************************************************************/

/* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved */

#ifndef SERIAL_H_
#define SERIAL_H_

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>
#include "iodefine.h"
#include "Renesas_RZ_A1.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/******************************************************************************
Macro definitions
******************************************************************************/

#define SCIF_FSR_BIT_DR    (1u <<  0)
#define SCIF_FSR_BIT_RDF   (1u <<  1)
#define SCIF_FSR_BIT_PER   (1u <<  2)
#define SCIF_FSR_BIT_FER   (1u <<  3)
#define SCIF_FSR_BIT_BRK   (1u <<  4)
#define SCIF_FSR_BIT_TDFE  (1u <<  5)
#define SCIF_FSR_BIT_TEND  (1u <<  6)
#define SCIF_FSR_BIT_ER    (1u <<  7)

#define SCIF_FSR_BIT_ALLERR (SCIF_FSR_BIT_ER | SCIF_FSR_BIT_BRK | SCIF_FSR_BIT_FER | SCIF_FSR_BIT_PER)

#define SCIF_FCR_BIT_LOOP  (1u <<  0)
#define SCIF_FCR_BIT_RFRST (1u <<  1)
#define SCIF_FCR_BIT_TFRST (1u <<  2)
#define SCIF_FCR_BIT_MCE   (1u <<  3)


#define SCIF_SMR_BIT_STOP  (1u <<  3)
#define SCIF_SMR_BIT_OE    (1u <<  4)
#define SCIF_SMR_BIT_PE    (1u <<  5)
#define SCIF_SMR_BIT_CHR   (1u <<  6)
#define SCIF_SMR_BIT_CA    (1u <<  7)

#define SCIF_SMR_CKS00 (0u)
#define SCIF_SMR_CKS01 (1u)
#define SCIF_SMR_CKS10 (2u)
#define SCIF_SMR_CKS11 (3u)

#define SCIF_SCR_BIT_RE    (1u <<  4)
#define SCIF_SCR_BIT_TE    (1u <<  5)

#define SCIF_SCR_CKE00 (0u)
#define SCIF_SCR_CKE01 (1u)
#define SCIF_SCR_CKE10 (2u)
#define SCIF_SCR_CKE11 (3u)


#define SCIF_LSR_BIT_ORER  (1u <<  0)

extern void SER_Init(void);
extern void SER_Enable(void);
extern void SER_Disable(void);
extern void SER_PutChar(uint8_t buffer);
extern uint32_t SER_GetChar (void);
extern void SER_Set_baud_rate(uint32_t baud_rate);
extern void interrupt_SER(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SERIAL_H_ */
