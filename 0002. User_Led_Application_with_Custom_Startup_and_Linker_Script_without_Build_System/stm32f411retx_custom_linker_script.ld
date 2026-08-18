/* Entry point executed after reset */
ENTRY(Reset_Handler)

/* Physical memory layout for STM32F411RE */
MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
  SRAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

/* Initial main stack pointer value loaded by the vector table */
_estack = ORIGIN(SRAM) + LENGTH(SRAM);

/* Minimum dynamic memory area reserved for malloc/newlib */
_Min_Heap_Size = 0x200;

/* Minimum downward-growing stack space reserved for MSP */
_Min_Stack_Size = 0x400;

/* Output section placement and linker-visible symbols */
SECTIONS
{
  /* Interrupt vector table placed at the beginning of FLASH */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector))
    . = ALIGN(4);
  } >FLASH

  /* Executable code and legacy constructor/destructor tables */
  .text :
  {
    . = ALIGN(4);

    /* Start address of the code section */
    _stext = .;

    /* Regular program code */
    *(.text)
    *(.text*)

    /* ARM/Thumb interworking glue emitted by the toolchain when needed */
    *(.glue_7)
    *(.glue_7t)

    /* C runtime entry/exit hooks */
    KEEP(*(.init))
    KEEP(*(.fini))

    /* Legacy C++ constructor lists */
    *crtbegin.o(.ctors)
    *crtbegin?.o(.ctors)
    *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
    *(SORT(.ctors.*))
    *(.ctors)

    /* Legacy C++ destructor lists */
    *crtbegin.o(.dtors)
    *crtbegin?.o(.dtors)
    *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
    *(SORT(.dtors.*))
    *(.dtors)

    /* Exception unwinding frame information */
    KEEP(*(.eh_frame*))

    . = ALIGN(4);

    /* End address of the code section */
    _etext = .;
  } >FLASH

  /* Read-only constants such as const objects and string literals */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)
    *(.rodata*)
    . = ALIGN(4);
  } >FLASH

  /* ARM exception unwinding helper table entries */
  .ARM.extab (READONLY) :
  {
    . = ALIGN(4);
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    . = ALIGN(4);
  } >FLASH

  /* ARM exception index table used to locate unwind metadata */
  .ARM.exidx (READONLY) :
  {
    . = ALIGN(4);
    __exidx_start = .;
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    __exidx_end = .;
    . = ALIGN(4);
  } >FLASH

  /* Functions run before normal static initialization */
  .preinit_array (READONLY) :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN(__preinit_array_start = .);
    KEEP(*(.preinit_array*))
    PROVIDE_HIDDEN(__preinit_array_end = .);
    . = ALIGN(4);
  } >FLASH

  /* Static constructor function pointer table */
  .init_array (READONLY) :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN(__init_array_start = .);
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array*))
    PROVIDE_HIDDEN(__init_array_end = .);
    . = ALIGN(4);
  } >FLASH

  /* Static destructor function pointer table */
  .fini_array (READONLY) :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN(__fini_array_start = .);
    KEEP(*(SORT(.fini_array.*)))
    KEEP(*(.fini_array*))
    PROVIDE_HIDDEN(__fini_array_end = .);
    . = ALIGN(4);
  } >FLASH

  /* Flash load address for initialized data copied to SRAM at reset */
  _sidata = LOADADDR(.data);

  /* Initialized read-write data stored in FLASH, executed from SRAM */
  .data :
  {
    . = ALIGN(4);

    /* Start address of initialized data in SRAM */
    _sdata = .;

    /* Standard initialized variables */
    *(.data)
    *(.data*)

    /* Functions or data explicitly requested to live in SRAM */
    *(.RamFunc)
    *(.RamFunc*)

    . = ALIGN(4);

    /* End address of initialized data in SRAM */
    _edata = .;
  } >SRAM AT> FLASH

  /* Zero-initialized data and common symbols placed in SRAM */
  .bss :
  {
    . = ALIGN(4);

    /* Start address of the zero-initialized region */
    _sbss = .;
    __bss_start__ = _sbss;

    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);

    /* End address of the zero-initialized region */
    _ebss = .;
    __bss_end__ = _ebss;
  } >SRAM

  /* Explicit heap/stack boundary model used by sysmem.c */

  /* Highest valid stack address in SRAM. */
  __StackTop = ORIGIN(SRAM) + LENGTH(SRAM);

  /* Lowest address the reserved stack is allowed to reach */
  __StackLimit = __StackTop - _Min_Stack_Size;

  /* First heap byte after the statically allocated SRAM sections */
  __HeapBase = ALIGN(_ebss, 8);

  /* Heap grows upward until it reaches the reserved stack area */
  __HeapLimit = __StackLimit;

  /* Symbols consumed by newlib _sbrk() implementation */
  PROVIDE(end = __HeapBase);
  PROVIDE(_end = __HeapBase);

  /* Conventional stack symbol for debuggers/startup code */
  PROVIDE(__stack = __StackTop);

  /* Ensure heap and reserved stack do not overlap */
  ASSERT(__HeapLimit >= __HeapBase, "heap overlaps stack")

  /* Ensure the remaining free RAM still satisfies the minimum heap request */
  ASSERT((__HeapLimit - __HeapBase) >= _Min_Heap_Size, "insufficient RAM for minimum heap and stack")

  /* Remove unused standard library archive members from the final image */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  /* ARM EABI build attributes metadata */
  .ARM.attributes 0 :
  {
    *(.ARM.attributes)
  }
}
