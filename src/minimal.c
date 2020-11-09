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

char const * DCB_ADD = 0x0300;

// Debug output memory address
char const * OUT    = 0x3000;

// our flag for data
volatile char trip = 0;

char *url1 = "N:TNFS://192.168.1.97/test2.txt";
char *url2 = "N:HTTPS://API.IPIFY.ORG/?format=json";
char *url3 = "N:HTTP://API.IPIFY.ORG/?format=json";
char *url4 = "N:HTTPS://BBC.CO.UK/";
char *url5 = "N:TCP://BBS.FOZZTEXX.NET/";

char trans = 3;
char result;
word dataLen;
word prevV;

void main() {
	clearOut();

	*OS_shflok = 0;
	*OS_lmargn = 0;

	// some personal preferences for keyboard repeats :)
	*OS_krpdel = 10;
	*OS_keyrep = 4;

	nio(url1);
	nio(url2);
	nio(url3);
	nio(url4);
	nio(url5);

}

void hex(char *name, char *add) {
	uint8_t tmp[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	print(name); print(" ");
	uctoa(add[0], tmp, 16); print("0:$"); print(tmp); print(" ");
	uctoa(add[1], tmp, 16); print("1:$"); print(tmp); print(" ");
	uctoa(add[2], tmp, 16); print("2:$"); print(tmp); print(" ");
	uctoa(add[3], tmp, 16); print("3:$"); print(tmp); print("\n");
}

void clearOut() {
	for(uint16_t i = 0; i < 0x2000; i++) *(OUT + i) = 0;
}

inline void sio() {
	asm(clobbers "AXY") {
		jsr SIOV
	}
}

void print_error(uint8_t err) {
	uint8_t tmp[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	uctoa(err, tmp, 10);
	print(tmp);
	print("\n");
}

void nio(char const *url) {
	print("u:"); print(url); print("\n");
	result = nopen(url, 4, trans);
	if (result != 1) {
		print("could not open url! "); print_error(result);
	} else {
		nstatus(url);
		hex("status dvstat:", OS_dvstat);
	}

	//dataLen = *((word *) OS_dvstat);
	//result = nread(url, OUT, dataLen);
	//hex("dcb", DCB_ADD);
	//hex("vst", OS_dvstat);
	// print("out: "); printl(OUT, dataLen); print("\n");

	result = nclose(url);

}

void direct() {
	char *url = "N:HTTPS://API.IPIFY.ORG/";
	//////////////////////////////////////////////////////
	// DIRECT
	//////////////////////////////////////////////////////

	print("direct\n");
	//////////////////////////////////////////////////////
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
	hex("dcb", DCB_ADD);
	hex("vst", OS_dvstat);

	//////////////////////////////////////////////////////
	// Status
	*DDEVIC = 0x71;
	*DUNIT  = 1;
	*DCOMND = 'S';
	*DSTATS = 0x40;
	*DBUF   = <OS_dvstat;
	*(DBUF+1) = >OS_dvstat;
	*DTIMLO = 0x1F;
	*DBYT   = 4;
	*DAUX1  = 0;
	*DAUX2  = 0;
	sio();

	hex("dcb", DCB_ADD);
	hex("vst", OS_dvstat);

	//////////////////////////////////////////////////////
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

	hex("dcb", DCB_ADD);
	hex("vst", OS_dvstat);

}

void dcb() {
	char *url = "N:HTTPS://API.IPIFY.ORG/";
	//////////////////////////////////////////////////////
	// OS DCB
	//////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////
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

	hex("dcb", DCB_ADD);
	hex("vst", OS_dvstat);

	//////////////////////////////////////////////////////
	// Status
	OS_dcb.ddevic = 0x71;
	OS_dcb.dunit  = 1;
	OS_dcb.dcomnd = 'S';
	OS_dcb.dstats = 0x40;
	OS_dcb.dbuf   = OS_dvstat;
	OS_dcb.dtimlo = 0x1f;
	OS_dcb.dbyt   = 4;
	OS_dcb.daux1  = 0;
	OS_dcb.daux2  = 0;
	sio();

	hex("dcb", DCB_ADD);
	hex("vst", OS_dvstat);

	//////////////////////////////////////////////////////
	// Read
	dataLen = *((word *) OS_dvstat);

	OS_dcb.ddevic = 0x71;
	OS_dcb.dunit  = 1;
	OS_dcb.dcomnd = 'R';
	OS_dcb.dstats = 0x40;
	OS_dcb.dbuf   = OUT;
	OS_dcb.dtimlo = 0x1f;
	OS_dcb.dbyt   = dataLen;
	OS_dcb.daux1  = <dataLen;
	OS_dcb.daux2  = >dataLen;
	sio();

	hex("dcb", DCB_ADD);
	hex("vst", OS_dvstat);
	print("out: "); printl(OUT, dataLen); print("\n");

	//////////////////////////////////////////////////////
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

	hex("dcb", DCB_ADD);
	hex("vst", OS_dvstat);

}

interrupt(hardware_none) void ih() {
	trip = 1;
	asm {
		pla
	}
}
