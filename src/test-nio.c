#pragma target(atarixl)
#pragma encoding(atascii)
#pragma zp_reserve(0x00..0x7f)

#include <stdint.h>
#include "atari-nio.h"

void main() {
	uint8_t *OUT = 0x8000;
	uint8_t r = nopen("spec", 'W');
	*(OUT) = r;
}
