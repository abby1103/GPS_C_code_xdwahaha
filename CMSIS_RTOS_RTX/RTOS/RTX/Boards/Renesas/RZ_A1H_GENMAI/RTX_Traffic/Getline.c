/*----------------------------------------------------------------------------
 * Name:    Getline.c
 * Purpose: Line Edited Character Input
 * Notes:
 *----------------------------------------------------------------------------
 * Copyright (c) 1999-2009 KEIL, 2009-2012 ARM Germany GmbH
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  - Neither the name of ARM  nor the names of its contributors may be used 
 *    to endorse or promote products derived from this software without 
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/

#include <stdio.h>
#include "Serial.h"

#define CNTLQ      0x11
#define CNTLS      0x13
#define DEL        0x7F
#define BACKSPACE  0x08
#define CR         0x0D
#define LF         0x0A


/*----------------------------------------------------------------------------
  Line Editor
 *----------------------------------------------------------------------------*/
void getline (char *line, int n) {
  int  cnt = 0;
  char c;

  do {
    if ((c = SER_GetChar()) == CR) c = LF; /* read character                 */
    if (c == BACKSPACE  ||  c == DEL) {    /* process backspace              */
      if (cnt != 0)  {
        cnt--;                             /* decrement count                */
        line--;                            /* and line pointer               */
        putchar (BACKSPACE);               /* echo backspace                 */
        putchar (' ');
        putchar (BACKSPACE);
      }
    }
    else if (c != CNTLQ && c != CNTLS) {   /* ignore Control S/Q             */
      putchar (*line = c);                 /* echo and store character       */
      line++;                              /* increment line pointer         */
      cnt++;                               /* and count                      */
    }
  } while (cnt < n - 1  &&  c != LF);      /* check limit and line feed      */
  *(line - 1) = 0;                         /* mark end of string             */
}
