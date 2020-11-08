#include "atari-mem.h"
#include "atari-sio.h"

inline void siov() {
	asm(clobbers "AXY") {
		jsr SIOV
	}
}
