// Equivalent functions from https://gitlab.com/bocianu/blibs/-/blob/master/b_system.pas

#include <6502.h>
#include "atari-mem.h"
#include "atari-util.h"

volatile char __nmien = 0;

inline void cold_start() {
	asm(clobbers "AXY") {
		jmp COLDSV
	}
}

// Turns OS back on and resets machine
void systemResetB(char port_b) {
	SEI();
	ANTIC.nmien = 0;
	PIA.portb = port_b;
	asm(clobbers "AXY") {
		jmp (RESVEC)
	}
}

void systemReset() {
	systemResetB(PORTB_SYSTEM_ON);
}

// Turns off OS with custom PORTB($D301) value.
void systemOffB(char port_b) {
	SEI();
	ANTIC.nmien = 0;
	PIA.portb = port_b;

	// work around for getting access to struct values in asm blocks.
	unsigned char * const anticNMIEN = &(ANTIC.nmien);
	unsigned char * const anticNMIST = &(ANTIC.nmist);

	// TODO: extract the logic from this into C functions rather than the intertwined asm.
	asm {
		lda #<nmi
		sta NMIVEC
		lda #>nmi
		sta NMIVEC+1

		lda #<systemOffB.iret
		sta IRQVEC
		sta systemOffB.vblvec
		sta systemOffB.dlivec

		lda #>systemOffB.iret
		sta IRQVEC+1
		sta systemOffB.vblvec+1
		sta systemOffB.dlivec+1

		lda #$40
		sta anticNMIEN
		sta __nmien
		bne systemOffB.stop
	nmi:
		bit anticNMIST
		bpl systemOffB.vbl
	}
	kickasm {{
		.label dlivec = * + 1
		jmp dlivec
	}}

	asm {
		rti
	vbl:
		inc RTCLOK+2
		bne systemOffB.vblvec-1
		inc RTCLOK+1
		bne systemOffB.vblvec-1
		inc RTCLOK
	}
	kickasm {{
		.label vblvec = * + 1
	}}
	asm(clobbers "AXY") {
		jmp systemOffB.vblvec
	iret:
		rti
	stop:
	}
}

void systemOff() {
	systemOffB(PORTB_BASIC_OFF + PORTB_SELFTEST_OFF + %01111100);
}

// Enable and set custom handler for Vertical Blank Interrupt.
// To set vertical blank interrupt vector from your inline assembly code
void enableVBLI(void *vblptr) {
	waitFrame();

	ANTIC.nmien = 0;
	asm {
		lda #<vblptr
		sta systemOffB.vblvec
		lda #>vblptr
		sta systemOffB.vblvec+1
	}
	
	ANTIC.nmien = __nmien | 0x40;
	__nmien = ANTIC.nmien;
	
}

// Disables custom routine of Vertical Blank Interrupt
void disableVBLI() {
	waitFrame();

	// work around for getting access to struct values in asm blocks.
	unsigned char * const anticNMIEN = &(ANTIC.nmien);

	ANTIC.nmien = 0;
	asm {
		lda #<systemOffB.iret
		sta systemOffB.vblvec
		lda #>systemOffB.iret
		sta systemOffB.vblvec+1
	}
	ANTIC.nmien = __nmien | 0x40;
	__nmien = ANTIC.nmien;

}

// Enables IRQ Interrupts
inline void enableIRQ() {
	CLI();
}

// Disables IRQ Interrupts
inline void disableIRQ() {
	SEI();
}

inline void setIRQ(void * irqptr) {
	asm {
		lda #<irqptr
		sta IRQVEC
		lda #>irqptr
		sta IRQVEC+1
	}
}

// Enable and set custom handler for Display List Interrupt
void enableDLI(void *dliptr) {
	waitFrame();
	
	ANTIC.nmien = 0;
	asm {
		lda #<dliptr
		sta systemOffB.dlivec
		lda #>dliptr
		sta systemOffB.dlivec+1
	}
	ANTIC.nmien = __nmien | 0xc0;
	__nmien = ANTIC.nmien;
}

// TODO: Remove this when I can work out how to pass functions as parameter without compiler getting error
void enableDLI2(void *dliptr) {
	waitFrame();

	ANTIC.nmien = 0;
	asm {
		lda #<dliptr
		sta systemOffB.dlivec
		lda #>dliptr
		sta systemOffB.dlivec+1
	}
	ANTIC.nmien = __nmien | 0xc0;
	__nmien = ANTIC.nmien;
}

// Disables Display List Interrupts
void disableDLI() {
	waitFrame();
	ANTIC.nmien = __nmien = 0x40;
}

void setCharset(char msb) {
	*CHBASE = msb;
}
