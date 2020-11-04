#pragma target(atariio)
#pragma pc(0x0600)

#include <stdint.h>
#include "atari-nio.h"
#include "conio.h"

void main() {
	char url[256]; 
	print("NETCAT--N: DEVICESPEC?\n");
	get_line(url,128);
	print(url);

	for(;;) ;
}
