OUTPUT_ARCH( "riscv" )
ENTRY(_start)

SECTIONS
{
  . = 0x80200000; 
  _start = . ;

  .text : {
    *(.text.entry)    
    *(.text*)
  }

  . = ALIGN(0x1000) ;

  _text_end = . ;

  .rodata : {
    *(.rodata*)
  }

  . = ALIGN(0x1000) ;

  _rodata_end = . ;


  .data : {
    *(.data*)
  }

  . = ALIGN(0x1000) ;

  _data_end = . ;

  .bss : {
    *(.bss*)
    *(COMMON)
  }
  
  . = ALIGN(0x1000) ;

  _end = . ;
}




