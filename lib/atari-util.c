#include "atari-mem.h"

void waitFrames(signed char frames) {
	while(frames > 0) {
		waitFrame();
		frames--;
	}
}

inline void waitFrame() {
	asm {
			lda RTCLOK+2
		!:	cmp RTCLOK+2
			beq !-
	}
}

void rtclr() {
	*(RTCLOK + 0) = 0;
	*(RTCLOK + 1) = 0;
	*(RTCLOK + 2) = 0;
}
