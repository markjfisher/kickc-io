#pragma target(atariio)
#pragma pc(0x2000)

#include <stdint.h>
#include <stdlib.h>
#include "atari-mem.h"
#include "atari-nio.h"

char const * DVSTAT = 0x02EA;
char const * EXTERR = 0x02ED;

char const * DDEVIC = 0x0300;
char const * DUNIT  = 0x0301;
char const * DCOMND = 0x0302;
char const * DSTATS = 0x0303;
char const * DBUF   = 0x0304;
char const * DTIMLO = 0x0306;
char const * DUNUSE = 0x0307;
word const * DBYT   = 0x0308;
word const * DAUX   = 0x030A;
char const * DAUX1  = 0x030A;
char const * DAUX2  = 0x030B;

char const * PACTL  = 0xD302;

// Debug output memory address
char const * OUT    = 0x3000;

// our flag for data
volatile char trip = 0;

void main() {
	word prevV;
	// char *url = "N:TCP://BBS.FOZZTEXX.NET/";
	char *url = "N:TNFS://192.168.1.97/test1.txt";
	// char *url = "N:HTTPS://API.IPIFY.ORG/";
	char trans = 0; // No translation
	char result;
	char buf[16];

	// Done the direct way
	// Open, assuming unit = 1
	*DDEVIC = 0x71;
	*DUNIT  = 1;
	*DCOMND = 'O';
	*DSTATS = 0x80;
	*DBUF   = <url;
	*(DBUF+1) = >url;
	*DTIMLO = 0x1f;
	*DBYT   = 256;
	*DAUX1  = 4; // R
	*DAUX2  = trans;
	sio();

	*OUT = *DSTATS;
	print("open direct\n"); print_stat(); print("\n");
	
	// Close
	*DDEVIC = 0x71;
	*DUNIT  = 1;
	*DCOMND = 'C';
	*DSTATS = 0x00;
	*DBUF   = 0;
	*DTIMLO = 0x1F;
	*DBYT   = 0;
	*DAUX1  = 0;
	*DAUX2  = 0;
	sio();

	*(OUT+1) = *DSTATS;
	print("close direct\n"); print_stat(); print("\n");
	
	// Open it using dcb block
	OS_dcb.ddevic = 0x71;
	OS_dcb.dunit  = 1;
	OS_dcb.dcomnd = 'O';
	OS_dcb.dstats = 0x80;
	OS_dcb.dbuf   = url;
	OS_dcb.dtimlo = 0x1f;
	OS_dcb.dbyt   = 0x100;
	OS_dcb.daux1  = 12;
	OS_dcb.daux2  = trans;
	sio();

	*(OUT+2) = OS_dcb.dstats;
	print("open dcb\n"); print_stat(); print("\n");

	// Close it using dcb block
	OS_dcb.ddevic = 0x71;
	OS_dcb.dunit  = 1;
	OS_dcb.dcomnd = 'C';
	OS_dcb.dstats = 0x00;
	OS_dcb.dbuf   = 0;
	OS_dcb.dtimlo = 0x1f;
	OS_dcb.dbyt   = 0;
	OS_dcb.daux1  = 0;
	OS_dcb.daux2  = 0;
	sio();

	*(OUT+3) = OS_dcb.dstats;
	print("close dcb\n"); print_stat(); print("\n");

	// Now do it with nio routines
	//result = nopen(url, 12, trans);
	//print_stat(); print("\n");
	//if (result != 1) {
		//print("Error opening: "); print_error(result);
	//} else {
		//result = nstatus(url);
		//print_stat(); print("\n");

		//// ignore the buffer size, just read 14 bytes.
		//result = nread(url, buf, 14);
		//if (result != 1) {
			//print("Error reading: "); print_error(result);
			//print_stat(); print("\n");
		//} else {
			//print("buf: "); printl(buf, 14); print("\n");
		//}

	//}

	//result = nclose(url);
}

void print_stat() {
	uint8_t tmp[8];
	print("dv ");
	uctoa(OS_dvstat[0], tmp, 16); print("0:$"); print(tmp); print(" ");
	uctoa(OS_dvstat[1], tmp, 16); print("1:$"); print(tmp); print(" ");
	uctoa(OS_dvstat[2], tmp, 16); print("2:$"); print(tmp); print(" ");
	uctoa(OS_dvstat[3], tmp, 16); print("3:$"); print(tmp); print(" ");
}

inline void sio() {
	asm(clobbers "AXY") {
		jsr SIOV
	}
}

void print_error(uint8_t err) {
	uint8_t tmp[8];

	uctoa(err, tmp, 10);
	print(tmp);
	print("\n");
}

//interrupt(hardware_none) void ih() {
	//trip = 1;
	//asm {
		//pla
	//}
//}
