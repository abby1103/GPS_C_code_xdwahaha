/**************************************************************************//**
 * @file     GLCD.h
 * @brief    Display driver template
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

#ifndef _GLCD_H
#define _GLCD_H

typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;
typedef unsigned       __int64 uint64_t;

#define   __RO     volatile const
#define   __WO     volatile
#define   __RW     volatile

/* LCD - Register Layout Typedef */
typedef struct {
  __RW uint32_t LCDTiming0;         /*0x000 Horizontal Axis Panel Control Register*/
  __RW uint32_t LCDTiming1;         /*0x004 Vertical Axis Panel Control Register */
  __RW uint32_t LCDTiming2;         /*0x008 Clock and Signal Polarity Control Register */
  __RW uint32_t LCDTiming3;         /*0x00c Line End Control Register */
  __RW uint32_t LCDUPBASE;          /*0x010 Upper and Lower Panel Frame Base Address Registers */
  __RW uint32_t LCDLPBASE;          /*0x014 Upper and Lower Panel Frame Base Address Registers */
  __RW uint32_t LCDControl;         /*0x018 LCD Control Register */
  __RW uint32_t LCDIMSC;            /*0x01c Interrupt Mask Set/Clear Register */
  __RO uint32_t LCDRIS;             /*0x020 Raw Interrupt Status Register */
  __RO uint32_t LCDIMIS;            /*0x024 Interrupt Mask Set/Clear Register */
  __WO uint32_t LCDICR;             /*0x028 LCD Interrupt Clear Register*/
  __RO uint32_t LCDUPCURR;          /*0x02c LCD Upper and Lower Panel Current Address Value Registers */
  __RO uint32_t LCDLPCURR;          /*0x030 LCD Upper and Lower Panel Current Address Value Registers */
  uint32_t RESERVED_0[115];         /*0x034 - 0x1fc*/
  __RW uint32_t LCDPalette[128];    /*0x200-0x3FC 256x16-bit Color Palette Registers*/
  uint32_t RESERVED_1[256];         /*0x400 - 0x7fc*/
  __RW uint32_t CursorImage[256];   /*0x800-0xBFC Cursor Image RAM Register*/
  __RW uint32_t ClcdCrsrCtrl;       /*0xc00 Cursor Control Register */
  __RW uint32_t ClcdCrsrConfig;     /*0xc04 Cursor Configuration Register */
  __RW uint32_t ClcdCrsrPalette0;   /*0xc08 Cursor Palette Registers */
  __RW uint32_t ClcdCrsrPalette1;   /*0xc0c Cursor Palette Registers */
  __RW uint32_t ClcdCrsrXY;         /*0xc10 Cursor XY Position Register */
  __RW uint32_t ClcdCrsrClip;       /*0xc14 Cursor Clip Position Register */
  uint32_t RESERVED_2[2];           /*0xc18 - 0xc1c*/
  __RW uint32_t ClcdCrsrIMSC;       /*0xc20 Cursor Interrupt Mask Set/Clear Register */
  __WO uint32_t ClcdCrsrICR;        /*0xc24 Cursor Interrupt Clear Register */
  __RO uint32_t ClcdCrsrRIS;        /*0xc28 Cursor Raw Interrupt Status Register */
  __RO uint32_t ClcdCrsrMIS;        /*0xc2c Cursor Masked Interrupt Status Register */
  uint32_t RESERVED_3[116];         /*0xc30 - 0xdfc*/
  uint32_t RESERVED_T[3];           /*0xf00 - 0xf08*/
  uint32_t RESERVED_4[53];          /*0xf0c - 0xfdc*/
  __RO uint32_t CLCDPeriphID0;      /*0xfe0 Peripheral Identification Register 0 Register*/
  __RO uint32_t CLCDPeriphID1;      /*0xfe4 Peripheral Identification Register 1 Register*/
  __RO uint32_t CLCDPeriphID2;      /*0xfe8 Peripheral Identification Register 2 Register*/
  __RO uint32_t CLCDPeriphID3;      /*0xfec Peripheral Identification Register 3 Register*/
  __RO uint32_t CLCDPCellID0;       /*0xff0 PrimeCell Identification Register 0*/
  __RO uint32_t CLCDPCellID1;       /*0xff4 PrimeCell Identification Register 1*/
  __RO uint32_t CLCDPCellID2;       /*0xff8 PrimeCell Identification Register 2*/
  __RO uint32_t CLCDPCellID3;       /*0xffc PrimeCell Identification Register 3*/
} LCD_Type;

#define GLCD_BASE       (0x1C1F0000u)  /* CS3 0x1C000000 + 0x1F0000 (VE ARM Cortex-A Series memory map) */
#define GLCD            ((LCD_Type *)GLCD_BASE)


/* VE Registers */
#define VE_SYS_BASE       0x1C010000	 // VE System
#define VE_SYS_CFG_DATA  (*(volatile unsigned int *)(VE_SYS_BASE + 0x0a0))
#define VE_SYS_CFG_CTRL  (*(volatile unsigned int *)(VE_SYS_BASE + 0x0a4))

/*------------------------------------------------------------------------------
  Color coding
  GLCD is coded:   15..11 blue, 10..5 green, 4..0 red
 *----------------------------------------------------------------------------*/
                            
/* GLCD BGR color definitions                                                 */
#define Black           0x0000      /*   0,   0,   0 */
#define Maroon          0x000F      /*   0,   0, 128 */
#define Navy            0x7800      /* 128,   0,   0 */
#define DarkGreen       0x03E0      /*   0, 128,   0 */
#define Olive           0x03EF      /*   0, 128, 128 */
#define DarkCyan        0x7BE0      /* 128, 128,   0 */
#define Purple          0x780F      /* 128,   0, 128 */
#define LightGrey       0xC618      /* 192, 192, 192 */
#define DarkGrey        0x7BEF      /* 128, 128, 128 */
#define Red             0x001F      /*   0,   0, 255 */
#define Blue            0xF800      /* 255,   0,   0 */
#define Green           0x07E0      /*   0, 255,   0 */
#define Yellow          0x07FF      /*   0, 255, 255 */
#define Cyan            0xFFE0      /* 255, 255, 0   */
#define Magenta         0xF81F      /* 255,   0, 255 */
#define White           0xFFFF      /* 255, 255, 255 */

extern void GLCD_Init           (void);
extern void GLCD_WindowMax      (void);
extern void GLCD_PutPixel       (unsigned int x, unsigned int y);
extern void GLCD_SetTextColor   (unsigned short color);
extern void GLCD_SetBackColor   (unsigned short color);
extern void GLCD_Clear          (unsigned short color);
extern void GLCD_DrawChar       (unsigned int x,  unsigned int y, unsigned int cw, unsigned int ch, unsigned char *c);
extern void GLCD_DisplayChar    (unsigned int ln, unsigned int col, unsigned char fi, unsigned char  c);
extern void GLCD_DisplayString  (unsigned int ln, unsigned int col, unsigned char fi, unsigned char *s);
extern void GLCD_ClearLn        (unsigned int ln, unsigned char fi);
extern void GLCD_Bargraph       (unsigned int x,  unsigned int y, unsigned int w, unsigned int h, unsigned int val);
extern void GLCD_Bitmap         (unsigned int x,  unsigned int y, unsigned int w, unsigned int h, unsigned char *bitmap);
extern void GLCD_ScrollVertical (unsigned int dy);

extern void GLCD_WrCmd          (unsigned char cmd);
extern void GLCD_WrReg          (unsigned char reg, unsigned short val);

#define FRAMEBUFFER 0x20650000

#endif /* _GLCD_H */
