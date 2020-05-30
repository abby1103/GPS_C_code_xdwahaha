/**************************************************************************//**
 * @file     Retarget.c
 * @brief    This file contains re-implementations of functions whose 
 *           C library implementations rely on semihosting.
 * @version
 * @date     25 March 2013
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

#include <stdio.h>
#include "Serial.h"



#define TRUE 1
#define FALSE 0



/*
** Importing __use_no_semihosting ensures that our image doesn't link
** with any C Library code that makes direct use of semihosting.
*/

#pragma import(__use_no_semihosting)

/*
** Retargeted I/O
** ==============
** The following C library functions make use of semihosting
** to read or write characters to the debugger console: fputc(),
** fgetc(), and _ttywrch().  They must be retargeted to write to
** the model's UART.  __backspace() must also be retargeted
** with this layer to enable scanf().  See the Compiler and
** Libraries Guide.
*/

/*
** These must be defined to avoid linking in stdio.o from the
** C Library
*/

struct __FILE { int handle;   /* Add whatever you need here */};
FILE __stdout;
FILE __stdin;
FILE __stderr;

/*
** __backspace must return the last char read to the stream
** fgetc() needs to keep a record of whether __backspace was
** called directly before it
*/
int last_char_read;
int backspace_called;


int fgetc(FILE *f)
{
    unsigned char tempch;
    tempch = SER_GetChar();
    last_char_read = (int)tempch;       /* backspace must return this value */
    return tempch;
}


int fputc(int ch, FILE *f)
{
    unsigned char tempch = ch;
    if (tempch == 10) SER_PutChar(13);
    SER_PutChar(tempch);
    return ch;
}

int fclose(FILE *f) {
	return (0);
}

int fseek (FILE *f, long nPos, int nMode) {
	return 0;
}

int fflush (FILE *f) {
	return 0;
}

void _ttywrch(int ch)
{
    unsigned char tempch = ch;
    if (tempch == 10) SER_PutChar(13);
    SER_PutChar(tempch);
}

/*
** The effect of __backspace() should be to return the last character
** read from the stream, such that a subsequent fgetc() will
** return the same character again.
*/

int __backspace(FILE *f)
{
    backspace_called = TRUE;
    return 1;
}

/*
** Exception Signaling and Handling
** ================================
** The C library implementations of ferror() uses semihosting directly
** and must therefore be retargeted.  This is a minimal reimplementation.
** _sys_exit() is called after the user's main() function has exited.  The C library
** implementation uses semihosting to report to the debugger that the application has
** finished executing.
*/

int ferror(FILE *f)
{
    return EOF;
}

void _sys_exit(int return_code)
{
    while(1);
}



