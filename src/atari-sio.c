#include "atari-mem.h"

void siov() {
	asm(clobbers "AXY") {
		jmp SIOV
	}
}

void rtclr() {
	*(RTCLOK + 0) = 0;
	*(RTCLOK + 1) = 0;
	*(RTCLOK + 2) = 0;
}

void cold_start() {
	asm(clobbers "AXY") {
		jmp COLDSV
	}
}
