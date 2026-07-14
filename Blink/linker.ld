
ENTRY(Reset_Handler)


/*Alokacja pamieci, jej rozmiaru oraz okreslenie stosu i sterty*/

MEMORY
{
FLASH(rx):ORIGIN =0x08000000,LENGTH =512k
SRAM(rwx):ORIGIN =0x20000000,LENGTH =64k
}

_estack = ORIGIN(SRAM)+LENGTH(SRAM);

_max_heap_size = 0x200;
_max_stack_size = 0x400;

/*Definicja sekcji*/
SECTIONS
{
.text :
{
	. = ALIGN(4);
	*(.isr_vector_tbl)
	*(.text)
	*(.rodata)
	. = ALIGN(4);
_etext = .;
} > FLASH

_sidata = LOADADDR(.data);

.data :
{
. = ALIGN(4);
_sdata = .;
*(.data)
. = ALIGN(4);
_edata = .;
} > SRAM AT> FLASH

.bss :
{
	. = ALIGN(4);
	_sbss = .;
	*(.bss)
	. = ALIGN(4);
	_ebss = .;
}> SRAM
}
