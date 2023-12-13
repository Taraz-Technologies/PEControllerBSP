/*
******************************************************************************
**
**  File        : LinkerScript.ld
**
**  Author      : STM32CubeIDE
**
**  Abstract    : Linker script for STM32H7 series
**                      2048Kbytes FLASH
**                      512Kbytes RAM
**
**                Set heap size, stack size and stack location according
**                to application requirements.
**
**                Set memory bank area and size if external memory is used.
**
**  Target      : STMicroelectronics STM32
**
**  Distribution: The file is distributed as is without any warranty
**                of any kind.
**
*****************************************************************************
** @attention
**
** <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
** All rights reserved.</center></h2>
**
** This software component is licensed by ST under BSD 3-Clause license,
** the "License"; You may not use this file except in compliance with the
** License. You may obtain a copy of the License at:
**                        opensource.org/licenses/BSD-3-Clause
**
*****************************************************************************
*/

/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = ORIGIN(DTCM) + LENGTH(DTCM); /* end of "DTCM" Ram type memory */

_Min_Heap_Size = 0x200; /* required amount of heap  */
_Min_Stack_Size = 0x400; /* required amount of stack */

/* Memories definition */
MEMORY
{
  FLASH  (rx)    : ORIGIN = 0x08000000, LENGTH = 64K 		/* Memory is divided. Actual start is 0x08000000 and actual length is 2048K */
  DTCM (xrw)  	 : ORIGIN = 0x20000000, LENGTH = 128K		/* This is the DTCM RAM */
  ITCM (xrw)  	 : ORIGIN = 0x00000000, LENGTH = 64K
}

/* Sections */
SECTIONS
{
  /* used by the startup to initialize data */
  _siisr_vector = LOADADDR(.isr_vector);
  /* The startup code into "FLASH" Rom type memory */
  .isr_vector :
  {
    . = ALIGN(4);
    _sisr_vector = .;    /* create a global symbol at isr start */
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
    _eisr_vector = .;    /* create a global symbol at isr end */
  } >ITCM AT>FLASH
  
  /* The program code and other data goes into FLASH */
  .text.Reset_Handler :
  {
    . = ALIGN(4);
    *(.text.Reset_Handler)
    . = ALIGN(4);
  } >FLASH
  
  /* used by the startup to initialize data */
  _sitext = LOADADDR(.text);
  /* The program code and other data into "FLASH" Rom type memory */
  .text :
  {
    . = ALIGN(4);
    _stext = .;        /* create a global symbol at code start */
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* create a global symbol at code end */
  } >ITCM AT>FLASH
  
  /* used by the startup to initialize data */
  _sirodata = LOADADDR(.rodata);
  /* Constant data into "FLASH" Rom type memory */
  .rodata :
  {
    . = ALIGN(4);
    _srodata = .;
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
    _erodata = .;
  } >ITCM AT>FLASH

  /* used by the startup to initialize data */
  _siextab = LOADADDR(.ARM.extab);
  .ARM.extab   : { 
    . = ALIGN(4);
    _sextab = .;
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    . = ALIGN(4);
    _eextab = .; 
  } >ITCM AT>FLASH
  
  /* used by the startup to initialize data */
  _siexidx = LOADADDR(.ARM); 
  .ARM : {
    . = ALIGN(4);
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
    . = ALIGN(4);
  } >ITCM AT>FLASH

  /* used by the startup to initialize data */
  _sipreinit_array = LOADADDR(.preinit_array);
  .preinit_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
    . = ALIGN(4);
  } >ITCM AT>FLASH
  
  /* used by the startup to initialize data */
  _siinit_array = LOADADDR(.init_array);
  .init_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
    . = ALIGN(4);
  } >ITCM AT>FLASH
  
  /* used by the startup to initialize data */
  _sifini_array = LOADADDR(.fini_array);
  .fini_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
    . = ALIGN(4);
  } >ITCM AT>FLASH
  
  /* Used by the startup to initialize data */
  _sidata = LOADADDR(.data);

  /* Initialized data sections into "DTCM" Ram type memory */
  .data : 
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */
    *(.tCritical)
    *(.RamFunc)        /* .RamFunc sections */
    *(.RamFunc*)       /* .RamFunc* sections */

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
  } >DTCM AT> FLASH

  /* Uninitialized data section into "DTCM" Ram type memory */
  . = ALIGN(4);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >DTCM

  /* User_heap_stack section, used to check that there is enough "DTCM" Ram  type memory left */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >DTCM

  /* Remove information from the compiler libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
