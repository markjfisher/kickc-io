#include "atari-mem.h"

void ciov() {
	asm(clobbers "AXY") {
		ldx #0
		jmp CIOV
	}
}
