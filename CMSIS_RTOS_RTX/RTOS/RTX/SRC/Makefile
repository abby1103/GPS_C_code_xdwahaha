# Copyright (C) ARM Ltd 2012-2013
# Select build rules based on Windows or Unix
ifdef WINDIR
DONE=@if exist $(1) echo Build completed.
RM=if exist $(1) del /q $(1)
SHELL=$(WINDIR)\system32\cmd.exe
SEP=\\
else
ifdef windir
DONE=@if exist $(1) echo Build completed.
RM=if exist $(1) del /q $(1)
SHELL=$(windir)\system32\cmd.exe
SEP=\\
else
DONE=@if [ -f $(1) ]; then echo Build completed.; fi
RM=rm -f $(1)
SEP=/
endif
endif

TOOLCHAIN=ARM
CC=armcc
AS=armasm
AR=armar
CFLAGS=-D__CMSIS_RTOS -D__CORTEX_A9 --cpu Cortex-A9 -D__EVAL -D__FPU_PRESENT \
       --apcs=interwork -g -O0 --thumb --li --split_sections -I. -I../INC -I../../../Include --md --diag_suppress=1786
ASFLAGS=--apcs=interwork --cpu Cortex-A9 --pd "__CMSIS_RTOS SETA 1" --pd "__EVAL SETA 1" --pd "__FPU_PRESENT SETA 1" -g --depend $*.d
ARFLAGS=

OBJECTS=$(addsuffix .o, $(basename $(wildcard rt_*.c)) HAL_CA $(TOOLCHAIN)/SVC_Table $(TOOLCHAIN)/HAL_CA9)

all: ../LIB/ARM/RTX_CA9_L.lib

../LIB/ARM/RTX_CA9_L.lib: $(OBJECTS)
	$(AR) -rv $(ARFLAGS) --create $@ $(OBJECTS)

includes = $(wildcard *.h)
$(OBJECTS) : $(includes)

nearlyclean:
	$(call RM,*.o)
	$(call RM,ARM$(SEP)*.o)
	$(call RM,*.d)
	$(call RM,ARM$(SEP)*.d)

clean: nearlyclean
	$(call RM,..$(SEP)LIB$(SEP)ARM$(SEP)RTX_CA9_L.lib)
