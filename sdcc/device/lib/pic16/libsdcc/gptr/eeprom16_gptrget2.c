/*-------------------------------------------------------------------------
   eeprom16_gptrget2.c - get 2 byte value from EEPROM via a generic pointer

   Copyright (C) 2012 Raphael Neider <rneider AT web.de>

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License 
   along with this library; see the file COPYING. If not, write to the
   Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.

   As a special exception, if you link this library with other files,
   some of which are compiled with SDCC, to produce an executable,
   this library does not by itself cause the resulting executable to
   be covered by the GNU General Public License. This exception does
   not however invalidate any other reasons why the executable file
   might be covered by the GNU General Public License.
-------------------------------------------------------------------------*/

/* the return value is expected to be in (FSR0H, PRODH, PRODL, WREG),
 * therefore we choose return type void here. Generic pointer is expected
 * to be in (WREG, PRODL, FSR0L), so function arguments are void, too */

extern EEADR;
extern EEADRH;
extern EECON1;
extern EEDATA;
extern FSR0L;
extern INTCON;
extern PRODL;
extern TBLPTRL;

void
__eeprom16_gptrget2(void) __naked
{
    __asm
        MOVFF   _INTCON, _TBLPTRL   ; save previous interupt state
        BCF     _INTCON, 7, 0       ; GIE = 0: disable interrupts

        BCF     _EECON1, 7, 0       ; EEPGD = 0: access EEPROM, not program memory
        BCF     _EECON1, 6, 0       ; CFGS = 0: access EEPROM, not config words

        MOVFF   _FSR0L, _EEADR      ; address first byte
        MOVFF   _PRODL, _EEADRH     ; address first byte, high address bits
        BSF     _EECON1, 0, 0       ; RD = 1: read EEPROM
        MOVF    _EEDATA, 0, 0       ; W = EEPROM[adr]

        INFSNZ  _EEADR, 1, 0        ; address second byte
        INCF    _EEADRH, 1, 0       ; high address bits
        BSF     _EECON1, 0, 0       ; RD = 1: read EEPROM
        MOVFF   _EEDATA, _PRODL     ; PRODL = EEPROM[adr+1]

        BTFSC   _TBLPTRL, 7, 0      ; check previous interrupt state
        BSF     _INTCON, 7, 0       ; conditionally re-enable interrupts

        RETURN
    __endasm;
}