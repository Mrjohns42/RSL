/*
 * GENERATED FILE - DO NOT EDIT
 * (C) Code Red Technologies Ltd, 2008-9    
 * Generated C linker script file for LPC1343 
 * (created from nxp_lpc13_c.ld (v3.1.4 (200912230917)) on Mon Jan 11 14:36:17 PST 2010)
*/

INCLUDE "../linker/lpc1114_lib_ram_crp.ld"
INCLUDE "../linker/lpc1114_mem_ram_crp.ld"

ENTRY(ResetISR)

SECTIONS
{
	.text :
	{
		FILL(0xFF);
		KEEP(*(.isr_vector))
		*(.text*)
		*(.rodata*)

	} > RamLoc8


	/* for exception handling/unwind - some Newlib functions (in common with C++ and STDC++) use this. */
	
	.ARM.extab : 
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} > RamLoc8

	__exidx_start = .;
	.ARM.exidx :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} > RamLoc8
	__exidx_end = .;

	_etext = .;
		
	.data :
	{
		_data = .;
		*(vtable)
		*(.data*)
		_edata = .;
	} > RamLoc8 AT>MFlash32
	
	/* zero initialized data */
	.bss :
	{
		_bss = .;
		*(.bss*)
		*(COMMON)
		_ebss = .;
	} > RamLoc8
	
	/* Where we put the heap with cr_clib */
	.cr_heap :
	{
		end = .;
		_pvHeapStart = .;
	} > RamLoc8

	_vRamTop = __top_RamLoc8 ;
	_vStackTop = _vRamTop;
}
