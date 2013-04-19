/*
 * GENERATED FILE - DO NOT EDIT
 * (C) Code Red Technologies Ltd, 2008-9
 * Generated linker script include file for 
 * (created from LinkMemoryTemplate (v3.1.4 (200912230917)) on Mon Jan 11 14:36:17 PST 2010)
*/

MEMORY
{
  /* Define each memory region */
  MFlash8 (rx) : ORIGIN = 0x0, LENGTH = 0x2000 /* 8k */
  RamLoc2 (rwx) : ORIGIN = 0x10000000, LENGTH = 0x800 /* 2k bytes */

}
  /* Define a symbol for the top of each memory region */
  __top_MFlash8 = 0x0 + 0x2000;
  __top_RamLoc2 = 0x10000100 + 0x0700;

