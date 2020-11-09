#pragma target(atariio)
#pragma pc(0x2000)
// #pragma var_model(ma_mem)

#include <stdint.h>
#include <stdlib.h>
#include <6502.h>
#include "atari-nio.h"
#include "atari-system.h"
#include "conio.h"

volatile __ma char trip = 0;

void main() {
	uint8_t running = 1;
	uint8_t tmp[8];
	uint8_t r;
	uint16_t oldVprced;
	bool echo = false;
	uint8_t trans=0;

	uint16_t bw = 0;

	// uint8_t  rx_buf[8192];
	uint8_t  const * rx_buf = 0x3000;

	// uint8_t  *url = "N:HTTP://ICANHAZIP.COM/";
	// uint8_t  *url = "N:TNFS://homesoft.irata.online/";
	// uint8_t  *url = "N:TNFS://192.168.1.97/";
	// uint8_t  *url = "N2:TNFS://192.168.1.97/test.txt";
	// uint8_t  *url = "N:TCP://BBS.FOZZTEXX.NET/";

	uint8_t url[256];
	print("NETCAT--N: DEVICESPEC?\n");
	get_line(url, 128);
	
	print("\nTRANS--0=NONE, 1=CR, 2=LF, 3=CR/LF?\n");
	get_line(tmp, 7);
	trans = (uint8_t) atoi(tmp);

	print("\nLOCAL ECHO?--Y=YES, N=NO?\n");
	get_line(tmp, 7);
	echo = (tmp[0] == 'Y' ? true : false);

	print("url: "); print(url); print("\n");

	r = nopen(url, FUJI_APPEND, trans);
	if (r != 1) {
		print("Error! opening url: "); print_error(r); print("\n");
		waitFrames(50);
		return;
	}

	// Tell PIA about proceed vector
	trip = 0;
	oldVprced = *((word *) VPRCED);
	*VPRCED = &ih;
	PIA.pactl |= 1;

	while(running == 1) {
		if (trip == 0) {
			continue;
		}
		
		r = nstatus(url);
		if (r == 136) {
			print("Disconnected\n");
			running = 0;
			continue;
		} else if (r != 1) {
			print("status error: ");
			print_error(r);
			running = 0;
			continue;
		}


		bw = *((word *) OS_dvstat);

		// debug
		print_stat(); utoa(bw, tmp, 16); print("bw: $"); print(tmp); print("\n");

		if (bw > 8192) {
			bw = 8192;
		}
		
		if (bw > 0) {
			r = nread(url, rx_buf, bw);
			
			if (r != 1) {
				print("read error: ");
				print_error(r);
				running = 0;
				continue;
			}
			
			printl(rx_buf, bw);
			
			// reset for next loop
			trip = 0;
			PIA.pactl |= 1;
		}
		
	}
	print("Closing...\n");
	r = nclose(url);

	*VPRCED = oldVprced;
	print("Exiting...\n");
	waitFrames(10);

}

void print_stat() {
	uint8_t tmp[8];
	print("dv ");
	uctoa(OS_dvstat[0], tmp, 16); print("0:$"); print(tmp); print(" ");
	uctoa(OS_dvstat[1], tmp, 16); print("1:$"); print(tmp); print(" ");
	uctoa(OS_dvstat[2], tmp, 16); print("2:$"); print(tmp); print(" ");
	uctoa(OS_dvstat[3], tmp, 16); print("3:$"); print(tmp); print(" ");
}

void print_error(unsigned char err) {
	uint8_t tmp[8];

	uctoa(err, tmp, 10);
	print(tmp);
	print("\n");
}

interrupt(hardware_none) void ih() {
	trip = 1;
	asm {
		pla
	}
}
