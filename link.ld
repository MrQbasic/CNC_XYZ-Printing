MEMORY
{
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 1024K
  RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 128K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
  /* Vector table at start of flash */
  .vectors ORIGIN(FLASH) :
  {
      KEEP(*(.vectors))
  } > FLASH

  /* Code + read-only data in flash */
  .text : {
    *(.text*)
    *(.rodata*)
  } > FLASH

  /* Initialized data in RAM, with load address in flash */
  .data : 
  {
    _sdata = .;               /* start of RAM section */
    _sidata = LOADADDR(.data); /* corresponding flash address */
    *(.data*)
    _edata = .;               /* end of RAM section */
  } > RAM AT > FLASH

  /* Uninitialized data in RAM */
  .bss : {
    _sbss = .;
    *(.bss*)
    *(COMMON)
    _ebss = .;
  } > RAM
}