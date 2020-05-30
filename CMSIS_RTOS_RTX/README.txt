* -------------------------------------------------------------------
* Copyright (C) 2014 ARM Limited. All rights reserved.
* Project:     RTX for Cortex-A9
* Date:        7th Oct 2014
* -------------------------------------------------------------------


Introduction
============

This port of RTX is designed for running on a single Cortex-A9 core (either
a single Cortex-A9 processor, or a single core in a Cortex-A9 MP cluster).
It is not designed to work across multiple processors, because RTX kernel 
resources and APIs must only be used for interactions between threads running 
on a single CPU (see "Notes on this Port" below).

Two sets of Examples are provided: one that runs on the Cortex-A9x4 MP FVP model;
the other designed for the Renesas RZ/A1H "GENMAI" board.

The Cortex-A9x4 MP FVP model version of the Examples run on a single core in the
Cortex-A9 MP cluster, where RTX runs on core 0 only (the others cores are put
to sleep with a WFI instruction on startup), and peripherals such as the timer
and GIC are accessed via the Private Peripheral interface.
The Renesas RZ/A1 examples version runs on a single Cortex-A9 processor (without 
the MP features), so peripherals are accessed via hard-coded base addresses.


Directory structure
===================

Within this CMSIS_RTOS_RTX.zip are the following sub-directories:
 
Include                                  - Contains CMSIS headers required for operation of RTX
RTOS/RTX/Boards/Renesas/RZ_A1H_GENMAI    - Contains examples targeted at Renesas RZ_A1H_GENMAI board
RTOS/RTX/Boards/ARM/VE_A9_MP             - Contains examples targeted at ARM VE_A9_MP Fixed Virtual Platform (FVP) model
RTOS/RTX/Doc                             - Contains documentation for CMSIS-RTOS-RTX v4.74 package. Access via RTOS/RTX/Doc/index.html.
RTOS/RTX/INC                             - Contains headers to map from CMSIS-RTOS standard to RTX implementation
RTOS/RTX/LIB                             - Contains a build of the RTX kernel library
RTOS/RTX/SRC                             - Source of the RTX kernel
RTOS/RTX/Templates/Renesas_RZ_A1H_GENMAI - Renesas RZ_A1H_GENMAI board template project to use as a starting point for RTX applications
RTOS/RTX/Templates/VE_A9_MP              - ARM VE_A9_MP FVP model template project to use as a starting point for RTX applications

CMSIS-RTOS RTX Cortex-A9 Guide
==============================

This guide begins with basic information to show how to build and run the examples,
followed by information about the general structure of the codebase and some notes
on getting started with RTX development.  Then some technical internal details on
the cache, MMU, interrupts and exception handling.  Finally a list of known issues
and a discussion on CMSIS Compliance.

It is best to read this guide in conjunction with the documentation provided in
RTOS/RTX/Doc/index.html.

Provided Examples for Renesas RZ_A1H_GENMAI Board
=================================================

The following examples are provided:

RTOS/RTX/Boards/Renesas/RZ_A1H_GENMAI/C++_ex1 (see note in "Building" below)
RTOS/RTX/Boards/Renesas/RZ_A1H_GENMAI/C++_ex2
RTOS/RTX/Boards/Renesas/RZ_A1H_GENMAI/Mail
RTOS/RTX/Boards/Renesas/RZ_A1H_GENMAI/Message
RTOS/RTX/Boards/Renesas/RZ_A1H_GENMAI/RTX_ex1
RTOS/RTX/Boards/Renesas/RZ_A1H_GENMAI/RTX_ex2
RTOS/RTX/Boards/Renesas/RZ_A1H_GENMAI/RTX_Traffic (Video Graphics not supported, but see "Using the Screen View" below)
RTOS/RTX/Boards/Renesas/RZ_A1H_GENMAI/Semaphore

Provided Examples for ARM VE_A9_MP FVP model
============================================

The following examples are provided:

RTOS/RTX/Boards/ARM/VE_A9_MP/C++_ex1 (see note in "Building" below)
RTOS/RTX/Boards/ARM/VE_A9_MP/C++_ex2
RTOS/RTX/Boards/ARM/VE_A9_MP/Mail
RTOS/RTX/Boards/ARM/VE_A9_MP/Message
RTOS/RTX/Boards/ARM/VE_A9_MP/RTX_ex1
RTOS/RTX/Boards/ARM/VE_A9_MP/RTX_ex2
RTOS/RTX/Boards/ARM/VE_A9_MP/RTX_Traffic (see "Using the Screen View" below)
RTOS/RTX/Boards/ARM/VE_A9_MP/Semaphore

Provided RTX Kernel
===================

The RTX kernel is provided at:
./RTOS/RTX/SRC (source files)
./RTOS/RTX/LIB/ARM (library file)
./Include (Include files)


Running the Examples
====================

All examples are provided pre-built and ready to be run.
First, import CMSIS_RTOS_RTX.zip into the DS-5 Eclipse Workspace as follows:

Running an example via Eclipse
------------------------------

1)  Launch Eclipse for DS-5.
2)  Close the Welcome screen, if it appears.
3)  Go to 'File' menu and select 'Import...'.  The Import wizard opens.
4)  Open the 'General' folder.
5)  Select 'Existing Projects into Workspace' and press 'Next'.
6)  Select 'Select archive file' and press 'Browse...' next to it.  A file dialog opens.
7)  Navigate to the DS-5 Examples directory, such as C:\Program Files\DS-5\examples, select CMSIS_RTOS_RTX.zip, then press 'OK'.
    The 'Projects:' field populates with all of the projects found in that folder.
8)  Press 'Finish'. The 'Project' view populates with all of the projects.
9)  Go to 'Window' menu and select 'Open Perspective'->'Other'->'DS-5 Debug'.  The DS-5 Debug perspective appears.
10) Go to 'Run' menu and select 'Debug Configurations...'.  The 'Debug Configurations' dialog opens.
11) Open the 'DS-5 Debugger' node.
12) Select the debug configuration with a name matching the project that you wish to run/debug, then press 'Debug'.

Note that some DS-5 toolkit licences do not allow the ARM VE_A9_MP FVP model to be run.
To enable this, purchase a licence upgrade to DS-5 Professional or Ultimate Edition.

Task-aware breakpoints
----------------------

"Task-aware breakpoints" relate to sub-routines that might be called from several different threads.  If an "ordinary" breakpoint is set on a sub-routine, code execution will stop each time it gets called.  
If a "task-aware breakpoint" is set on that sub-routine, it will stop only if that sub-routine was called from a selected task(s).  To see this in action:

1) Launch RTX_Traffic-Cortex-A9x4-FVP.  The code stops at $Sub$$main.
2) Using the CLI, set a breakpoint on the "real" main with "b $Super$$main", and run to it.  The code stops at main() in Traffic.c.
3) Scroll down through Traffic.c.  Notice that osDelay() is called from several different tasks, such as "lcd", "lights", "keyread".
4) Using the CLI, set a breakpoint on osDelay()with "b osDelay", and run to it.  Do not make it a task-aware breakpoint yet.
In the Debug Control view, you should see osDelay() was called from main().
5) Run again.  In the Debug Control view, you should now see osDelay() was called from lcd().
6) Run again.  In the Debug Control view, you should now see osDelay() was called from lights().
7) Run again.  In the Debug Control view, you should now see osDelay() was called from keyread().  Running again and again, it stays with keyread()
8) Suppose you want to only stop on this function when it is called from lcd().  Right-mouse click on the osDelay breakpoint, select Properties, tick "Break on Selected Threads or Cores", 
and scroll-down to the list to "lcd #6".  Tick lcd.
9) Run again.  After a few seconds, code stops in osDelay().  In the Debug Control view, you should now see osDelay() was called from lcd().  
This "task-aware breakpoint" has filtered-out function calls from only those threads you are interested in.

Using the Screen View
---------------------

The RTX_Traffic example simulates a set of traffic lights, and is able to display an animation on them on a screen.
The Cortex-A9x4 MP FVP model incorporates a model of an LCD display that the ARM/VE_A9_MP/RTX_Traffic example makes use of via an LCD driver.
The Renesas/RZ_A1H_GENMAI/RTX_Traffic example has no LCD driver, but you can use DS-5 Debugger's Screen view instead.

To use DS-5 Debugger Screen view, open the view in the DS-5 Debug perspective with Window->Show View->Screen, then configure it as:

Base Address = 0x20650000 for the Renesas/RZ_A1H_GENMAI/RTX_Traffic example
Base Address = 0x80900000 for the ARM/VE_A9_MP/RTX_Traffic example
Screen Width = 320
Screen Height = 240
Scan Line Alignment = 1
Pixel Type = BGR_16_565
Pixel Byte Order = Little Endian

The Screen view will refresh when code execution is stopped.  To see this in action, set a "task-aware breakpoint" on lcd() as described above, with the breakpoint configured to stop, 
then continue execution automatically.  You can then watch the traffic lights animation in the Screen view.


Building the Examples
=====================

All examples and the RTX kernel library are provided with both Eclipse projects and Makefiles.
This allows building in the Eclipse IDE or on the command line.

Some DS-5 toolkit licences have a limit on the size of the generated code.  For example, the "DS-5 Starter Kit for Renesas RZ" 
has a limit of 32KB.  If this limit is exceeded, the ARM linker will report:
Fatal error: L6050U: The code size of this image (64864 bytes) exceeds the maximum allowed for this version of the linker.
The C++_ex1 example provided exceeds this limit, because it is a C++ project and uses the standard library (rather than Microlib), 
so cannot be rebuilt with such a licence.  To run/debug this example, use the prebuilt executable instead of attempting to rebuild it.  
Alternatively, to eliminate the code size limit, purchase a licence upgrade to "DS-5 Renesas RZ Edition", DS-5 Professional or Ultimate Edition.


Building on the command line with Makefiles
-------------------------------------------

1)  Open the 'DS-5 Command Prompt' via the Start Menu
2)  Change directory to the project that you wish to build in the 'CMSIS_RTOS_RTX' project tree in the Workspace, for example:
C:\Documents and Settings\yourname\My Documents\DS-5 Workspace\CMSIS_RTOS_RTX\RTOS\RTX\Boards\ARM\VE_A9_MP\Semaphore
3)  Type 'make'

This will generate a .axf file in the project directory.

To delete all generated files, type 'make clean'.


Building using Eclipse
----------------------

1)  Ensure that you have imported the projects (steps 1 to 12 under 'Running an example via Eclipse', above).
2)  Go to 'Window' menu and select 'Open Perspective'->'C/C++'.
3)  Select the project that you wish to build in the 'Project' view.
4)  Go to 'Project' menu and select 'Build'. This will generate a .axf file in the project directory.

To delete generated object files, go to 'Project' menu and select 'Clean'.

To build with full optimisation, go to 'Project' menu and select 'Build
Configurations'->'Set Active'->'Release' and then build again to generate a new
.axf file. Note that the debug illusion provided by fully optimised builds is
likely to be impaired.


Notes on this Port
==================

RTX was originally designed to run on Cortex-M class devices.  Some concepts
relevant for this port to Cortex-A are listed here.  You should bear in mind 
the contents of this section when reading the documentation in RTOS/RTX/Doc.

The SysTick system timer in Cortex-M has no direct equivalent in Cortex-A.
Cortex-A ports require the user to define a OS system tick in RTX_Conf_CM.c 
by setting "#define OS_SYSTICK 0" and providing an implementation of os_tick_init()
and os_tick_irqack(). See RTX_Conf_CM.c for implementation examples.

Threads in Cortex-M may run with or without privilege. In Cortex-A, threads may 
run in User mode (unprivileged) or System mode (privileged).

There are only two stacks in Cortex-M and either may be accessed by a thread.
In Cortex-A, each core mode has its own stack.

A-profile cores such as Cortex-A9 exploit the weakly-ordered property of the
ARM memory model.  A consequence of this is that RTX kernel resources and APIs
must only be used for interactions between threads running on a single CPU.
RTX kernel resources/APIs are not synchronization-safe for interactions between:
* The CPU running RTX and other CPUs
* The CPU running RTX and other devices in the system
* The L1 caches (I-cache, D-cache and TLBs) of the CPU running RTX


Structure of the Files
======================

Kernel Files
------------

The following directories are used primarily by the RTX kernel library and 
should not normally need to be modified by end users:

Include        Files providing access to features of cores
RTOS/RTX/INC   Files to map from RTX to CMSIS-RTOS
RTOS/RTX/SRC   Kernel source and headers, covering kernel functionality and mappings on to specific cores.
RTOS/LIB/ARM   Location of the built kernel library


Device Files
------------

RTOS/RTX/Templates contains board support files. These files are expected 
to be copied by end users and used as the basis of RTX applications.


Documentation Files
-------------------

RTOS/RTX/Doc contains documentation for the latest CMSIS-RTOS RTX release. 
Note that this documentation refers only to RTX for M-class.


Example Files
-------------

RTOS/RTX/Boards contains example RTX applications.



Creating an RTX Application
===========================

To create a new RTX application:

Copy the template files into a new directory.
To access cmsis_os functionality, make sure to #include "cmsis_os.h".
To access BSP functionality, make sure to #include "<board>.h". Where possible, 
functionality should be accessed via <board>.h, even when available in other 
headers.

Some of the examples contain additional drivers that have not been integrated 
into the main BSP. You can copy code from the examples to access this 
functionality. These drivers usually must be accessed via their own headers.

If your example uses the heap, be sure to give the heap a size by assigning a 
value to the Heap_Size symbol in startup_<board>.s. You may also wish to 
adjust the stack sizes given in the *_Stack_Size symbols in this file.

Stack sizes for threads, and many other configuration parameters, are 
configured in RTX_Conf_CM.c. See RTOS/Doc/index.html for more details.

Note that comments in RTX_Conf_CM.c such as:
//   <o>Default Thread stack size [bytes] <64-4096:8><#/4>
are intended for use by Keil's Microcontroller Development Kit (MDK), not DS-5.
The Keil MDK Configuration Wizard asks for a stack size in bytes from the user, 
then divides the byte number by 4 to give a number of words, which is then written 
into the #define line.  So the stack size #define is in words, not bytes, for example:
 #define OS_STKSIZE     200
Here, the Thread stack size is 200 words.

The files pl310.h and pl310.c provide an example of how to integrate your own
drivers into the board support package.


Extending the RTX Kernel
========================

You can extend the RTX kernel by defining your own system calls. New calls are 
declared in RTOS/RTX/SRC/SVC_Table.s. An example implementation is the 
CacheEnable call in RTOS/RTX/SRC/Templates/<board>/system_<board>.c.

One reason to define new system calls is to give user mode threads access to 
restricted functionality.


Technical Internal Details
==========================

Memory & Cache Management
-------------------------

The MMU is programmed with a memory map in mmu_<board>.s.  This file is called 
from startup_<board>.s.  The memory map is programmed in various ways to 
demonstrate different ways of describing memory with the MMU.

An API to set memory attributes is available.  Functions are provided in
Include/core_ca_mmu.h (and accessible via <board>.h) to populate translation 
tables, and macros to create or modify descriptors are given in VE_A9_MP.h
Example descriptors can be seen in mmu_<board>.s and startup_<board>.s.

The L1 cache is initialised in system_<board>.c.  Code to initialise L2 cache
is provided, but commented-out for VE_A9_MP Fixed Virtual Platform (FVP) model
because L2 is not modelled.

The CacheEnable function in system_<board>.c is an SVC so that it can be 
called when RTX threads run in user mode.  CacheEnable is called from function 
$Sub$$main in the same file.  $Sub$$main is called after scatterloading 
completes, but just before the main function of the application.

Calls to clean and invalidate the L1 cache are in Include/core_caFunc.h.
Calls to clean and invalidate the L2 cache are in pl310.h and pl310.c.
Both sets of calls are accessible through <board>.h. The way in which they are used 
will depend on the application and on the cache configuration.

Calls to manipulate the instruction cache and TLBs have been provided along 
with the L1 data cache calls.

Handlers for synchronous parity errors on prefetch abort and data abort are provided.
Handlers for other cases must be provided by the user. 


Interrupt Management
--------------------

A pre-emptible IRQ handler is provided. This handler looks up the address of
IRQ handling functions in an array. If a handler is defined then we branch to
it, before returning to the outer IRQ handler to perform GIC maintenance. If
no handler is defined we clear the interrupt and return.  The relevant code 
is in system_<board>.c.

To avoid a nested interrupt corrupting LR when a BL/BLX is executed inside an
ISR, ISR processing is handled in SVC mode, rather than IRQ mode.  
For an example, see IRQ_Handler in startup_VE_A9_MP.s for an example,

To allow nested service calls (calls to 'os' functions from a service call
context) to be handled correctly, including those called from within an ISR
(IRQ->SVC->SVC), IRQNestLevel is used to track whether code is executing inside
an ISR, and the depth of nesting.

IRQ Handlers can be registered at build time by directly editing the array
IRQTable[], or at run time by calling function InterruptHandlerRegister. 
Handlers can be unregistered at runtime by calling InterruptHandlerUnregister. 
Both of these functions are available to code that #includes <board>.h.

IRQ Handlers can expect to receive the IRQ number as their first argument and
the base address of the private peripheral block as their second argument. They
are expected to return to the assembler IRQ handler, which will then perform
any necessary GIC maintenance before returning. IRQHandlers must comply with 
the ARM Procedure Call Standard.

Calls to interact with the GIC are defined in Include/core_ca9.h and
accessible through <board>.h. Calls for all GIC functionality are provided.
Calls to handle cascading interrupts are not provided.

The lowest two software generated interrupts and the lowest three interrupt
priorities are reserved and must not be used.

The timing functions in rt_CMSIS.c like osDelay, osWait, osTimerStart take a
millisec parameter.  These functions call rt_ms2tick() to convert millisecs to 
system ticks.  In rt_ms2tick(), the value of millisec will be rounded down to 
0xFFFE if millisec is greater than 0xFFFF.  Therefore, if millisec parameter for 
e.g. osDelay() is greater than 0xFFFF, osDelay() will wait for 0xFFFE millisec.


Exception Handling
------------------

The Vector table contains branch instructions to first-level exception handlers 
that are written in assembler: Reset_Handler, Undef_Handler, SVC_Handler,
PAbt_Handler, DAbt_Handler, IRQ_Handler, FIQ_Handler.

DAbt_Handler, PAbt_Handler and Undef_Handler call second-level handlers that
are written in C code : CDAbtHandler, CPAbtHandler and CUndefHandler respectively.  
These second-level handlers are only intended to be called from the 
first-level handlers - they are not intended to be called from application C code.

Skeleton second-level exception handlers are provided in RTOS/RTX/Templates/<board>/system_<board>.c.  
These handlers contain helpful boilerplate code, but as the correct action to take will be 
application-specific, do not implement much functionality.  The second-level handlers are:

1) void CDAbtHandler(uint32_t DFSR, uint32_t DFAR, uint32_t LR)
This takes three parameters, DFSR, DFAR and LR.  It does return any value.
The passing-in of LR as a parameter is intended as a debugging aid.
For the case of synchronous parity errors, this function simply returns back to
try to re-execute the failing instruction in the application code that caused the
abort (at the pre-adjusted address LR - 8).  For all other types of fault, the
supplied code stops in an endless loop.  It is up to the programmer to modify
this code to implement any other desired application-specific behaviour.

2) void CPAbtHandler(uint32_t IFSR, uint32_t IFAR, uint32_t LR)
This takes three parameters, IFSR, IFAR and LR.  It does return any value.
The passing-in of LR as a parameter is intended as a debugging aid.
For the case of synchronous parity errors, this function simply returns back to
try to re-execute the failing instruction in the application code that caused the
abort (at the pre-adjusted address LR - 4).  For all other types of fault, the
supplied code stops in an endless loop.  It is up to the programmer to modify
this code to implement any other desired application-specific behaviour.

3) uint32_t CUndefHandler(uint32_t opcode, uint32_t state, uint32_t LR)
This takes three parameters, opcode, state and LR.  It returns the amount that LR
should be decremented by Undef_Handler.
The passing-in of LR as a parameter is intended as a debugging aid.
If the opcode is a VFP/NEON instruction, then the FPU gets enabled, and the
code returns to retry the instruction.  It is up to the programmer to modify
this code to implement any other desired application-specific behaviour.
The return value depends on what action should be performed next:
- If the failing instruction has been emulated, and you simply want to execute
the next instruction in the application code, then return 0.
- If some maintenance has been performed and you want to retry the failing
instruction in the application code, then return the same value of 'state' as
was passed-in.  This will either be 2 (to return to an Thumb state instruction),
or return 4 (to return to an ARM state instruction).  Do not attempt to change
processor state by returning the other value than was passed-in.


Miscellaneous Requirements
--------------------------

Branch prediction is enabled on startup.

VFP & NEON are not enabled on startup, but rather on first use of a VFP/NEON
instruction (via the Undef handler). VFP and NEON registers are not saved and
restored lazily, but are only saved and restored by threads that use them.


Technical Data
==============

This section lists the technical data of CMSIS-RTOS RTX-A9:

Description                     Limits
-----------                     ------

Defined Tasks                   Unlimited
Active Threads                  250 max
Mailboxes                       Unlimited
Semaphores                      Unlimited
Mutexes                         Unlimited
Signals/Events                  16 per thread
Timer Callbacks                 Unlimited
Code Size                       ~7 KB typical (e.g. Traffic example)
                                12 KB worst case (all of RTX_CA9_L.lib)
RAM Space for Kernel            300 bytes + Stack space for modes
RAM Space for a Thread          52 bytes + Stack space for threads
RAM Space for a Mailbox         Number of messages * 4 + 16 bytes
RAM Space for a Semaphore       8 bytes
RAM Space for a Mutex           12 bytes
RAM Space for a User Timer      20 bytes
Hardware Requirements           Hardware timer
Thread context switch time &
Interrupt lockout time          Not deterministic, system dependent (see note 4)

Notes
1 "Unlimited" means that the RTX kernel does not impose any limitations on the number.
  However, the available system memory resources limit the number of items you can create.
2 RAM requirements depend on the number of concurrently running threads.
3 The code and RAM size is calculated for the ARM Compiler 5 (armcc),
  compiled with '--thumb -O3 -Ospace', and when using the MicroLib runtime library.

4 The original design of the ARM CMSIS RTX Real-time Operating System was for the RTOS
to run on Cortex-M processors.  Cortex-M processors offer deterministic interrupt latency
which means a worst case thread context switch time can be calculated for a given system.
Cortex-A class processors do not have a deterministic interrupt latency due to architectural factors, 
for example, the use of caches and TLBs will affect the interrupt latency because line fills,
cache evictions and table walks all take time to complete.  Additonally, LDM/STM instructions
cannot be interrupted and may take additional cycles to handle if a data abort occurs.
Ultimately, interrupt latency for RTX-A9 depends on your A9-based system and the code
(RTOS and applications) running on it.  If you need to measure interrupt latency then 
you will need to do this yourself for your code running on your system.


Known Issues
============

* Post-service handler requests may not be serviced immediately
* Non-essential, non-CMSIS-RTOS calls available in M-class RTX may not be 
  implemented (e.g. access to CoreSight registers)
* Reduced context switch unimplemented - all core registers are saved on every
  context switch.
* Interrupts of non-SVC, non-IRQ exception handlers may corrupt OS state. 
  Non-SVC exception handlers do not enable interrupts for this reason.
* Compiler abstractions not implemented for compilers other than armcc.


CMSIS Compliance
================

CMSIS is a specification for Cortex-M profile devices. Although it is
possible to port RTX to Cortex-A devices, the CMSIS specification cannot 
be assumed to apply to such devices. Nor do implementations of such
devices imply any specification.

Most of the changes required for the Cortex-A9 port are confined to CMSIS-RTOS
files. However, because of the way that CMSIS-RTOS is structured, some Cortex-A9
changes are included with the CMSIS-CORE files, in the Include directory at the
root of this deliverable.

The major differences between the A-profile files and the true CMSIS-CORE files
are due to the less defined and more complex nature of A-profile. M-profile 
devices have a fixed memory map and so it is possible to define much more data
in generic files such as these. M-profile devices also have less functionality
and so in some cases need less definition than is required for A-profile devices.

The detailed relationships between the A-profile and M-profile files in the Include
directory are:

core_ca9.h
----------

A limited subset of the equivalent core_cm*.h files, covering:
  * Declaring preprocessor macros to abstract over different compilers
  * Declaring preprocessor macros to define aspects of the core configuration,
    such as availability of an FPU.
  * Declaring a struct to describe the state of the APSR

The core_cm*.h files additionally define:
  * Many more structs describing registers that are specific to M-profile
  * Bitmasks for accessing registers that are specific to M-profile
  * Interrupt controller functions
  * Systick functions
  * Trace functions


core_caFunc.h
-------------

Defines functions for accessing registers that are part of the core, including
functions that get and set values of registers and functions that perform more complex 
operations, such as cache invalidation. For Cortex-A9 this includes:
  * Access to CPSR, SP and LR
  * A setPSP function that sets the USR mode SP (PSP is an M-profile concept)
  * Access to some FP registers
  * Access to the CBAR, which defines the base address of certain memory-mapped peripherals
  * Access to some MMU registers
  * L1 cache and branch predictor maintenance functions

This header can only be compiled with armcc.  The functionality is limited to what is required
for RTX to work.

core_cmFunc.h offers similar functionality for those features which are supported on M-profile,
and supports compilers other than armcc.


core_caInstr.h
--------------

This header simply includes core_cmInstr.h, with __CORTEX_M set to 3 to enable the full range
of functionality in this header.  This provides access to various assembly-level operations
from C code.


core_ca_mmu.h
-------------

This header is unique to A-profile.  It defines operations relating to MMU programming.


Changes since DS-5 v5.19
========================

* Corrected os_tick_val() in RTX_Conf_CM.c in Renesas examples to read the right timer register (OSTM0.OSTMnCNT)
* Added os_tick_val() and os_tick_ovf() into RTX_Conf_CM.c for other examples, in the same way as for the Semaphore example
* Updated some header files in Renesas examples (Renesas_RZ_A1.h register definitions and region security attributes, 
  updated GIC_DistributorInfo() in gic.c to workaround a problem in RZ/A1H GIC 390 ITLinesNumber, updated iodefines).
* In core_ca_mmu.h, now use 0b11 encoding for Read-only AP[1:0] as recommended by the ARM ARM


Changes since DS-5 v5.18
========================

* The version of RTX on which this port to Cortex-A9 is based has been updated from RTX 4.6 to RTX 4.74.
* CMSIS_RTOS_RTX.zip can now be imported directly into Eclipse (as an archive file) rather than having to unzip it first outside of Eclipse.  A new top-level "container" project 
(that is itself not buildable) has been added alongside the existing (buildable) lower level example projects, that makes use of DS-5 Eclipse "Search for nested projects" import feature.
* The "Semaphore" examples have been updated to demonstrate the use of osKernelSysTick().  To acheive this, added an implementation of os_tick_val() and os_tick_ovf() into RTX_Conf_CM.c,
and set osFeature_SysTick to 1 in cmsis_os.h, and added a call to print out osKernelSysTick() in Semaphore.c.
* The duplicate 'iodefines' header files in each Renesas project have been factored-out into a single '\CMSIS_RTOS_RTX\RTOS\RTX\Boards\Renesas\RZ_A1H_GENMAI\iodefines' directory.
* Some common header files in the Examples projects have been factored-out into a new /INC directory, for both the ARM VE_A9_MP FVP model and Renesas RZ_A1H_GENMAI Board examples.
* Hivecs is now explicitly disabled before setting VBAR in startup_Renesas_RZ_A1.s for Renesas targets.


Changes since DS-5 v5.17
========================

* Corrected the OS_X_INIT macro in rt_HAL_CA.h, so the GIC Distributor will now forward interrupts to the CPU, when compiled at -O1 or higher.
* Corrected rt_svc_init() in rt_HAL_CA.h, so PendSV can now be triggered through an SGI, when compiled at -O1 or higher.
* Added "Technical Data" section to this readme, that includes the code and data size for RTX and internal limits.


Changes since DS-5 v5.16
========================

* Added a description of "task-aware breakpoints" and the use of DS-5 Debugger's Screen view to this document.
* Renamed the scatter-files file-extension from .sct to .scat, for compatibility with the DS-5 scatter-file viewer.
* The scatter-files have been modified so that RO-DATA is now placed in a root region, so that it no longer needs to be copied on startup.


Changes since DS-5 v5.15
========================

* The SVC_Handler in HAL_CA9.c is now re-entrant, to allow nested SVC calls safely
* The instruction sequence that swaps SP and User SP on a task switch in HAL_CA9.c has been corrected to avoid stack corruption (leading to a data abort) if an interrupt occurs during that sequence
* A workaround for GIC-390 errata 733075 has been added to the interrupt handler in startup_Renesas_RZ_A1.s
* The variable types and code in GetIRQStatus in gic.c, and related comments in gic.h, have been corrected
* A description of the millisec parameter as used in timing functions like osDelay(), osWait(), osTimerStart() has been added into README.txt
* Page table entries for NORFLASH and SPI_IO have been corrected in the Renesas examples in mmu_Renesas_RZ_A1.c
* LR is now passed into exception handlers in startup_Renesas_RZ_A1.s/system_Renesas_RZ_A1.c, as a debugging aid
* Interrupt latency has been improved slightly in HAL_CA9.c by disabling interrupts as late as possible and re-enabling them sooner
* A new comment in IRQ_Handler now explains that spurious interrupt ID 1023 does not need any special handling
* The makefiles have been improved: the library is now rebuilt if necessary, and compilation now depends on header files too
* A space-safe version of 'notdir' has been added into the makefiles, so they now build on Windows
* Rogue "Warning: A1786W" when building with Eclipse projects and makefiles is now suppressed
* The Eclipse projects for the RTX Examples have been reorganised to avoid name clashes so that both ARM & Renesas examples can be imported together.
* The name of the Eclipse project for the RTX Library has been changed to the clearer "RTX_CA9_Library", and the Eclipse projects for the RTX Examples now depend on it
* The C++ Examples are moved up one directory level and renamed for consistency
* The .launch files have been updated for compatibility with DS-5 v5.16 Debugger's updated Debug Configurations
* Corrected some typos in comments
