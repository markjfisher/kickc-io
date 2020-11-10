#include <stdint.h>
#include "atari-nio.h"
#include "atari-sio.h"
#include "atari-mem.h"

uint8_t nunit(uint8_t* devicespec) {
	uint8_t unit = 1;

	// Set unit to 1 unless explicitly specified.
	if (devicespec[1] == ':')
		unit = 1;
	else if (devicespec[2] == ':')
		unit = devicespec[1] - 0x30; // convert from alpha to integer.
	else
		unit = 1;

	return unit;
}

uint8_t nopen(uint8_t* devicespec, uint8_t mode, uint8_t trans) {
	uint8_t unit = nunit(devicespec);

	OS_dcb.ddevic = 0x71;
	OS_dcb.dunit  = unit;
	OS_dcb.dcomnd = 'O';
	OS_dcb.dstats = 0x80;
	OS_dcb.dbuf   = devicespec;
	OS_dcb.dtimlo = 0x1f;
	OS_dcb.dbyt   = 0x100;
	OS_dcb.daux1  = mode;
	OS_dcb.daux2  = trans;
	siov();

	if (OS_dcb.dstats != 1) {
		if (OS_dcb.dstats == 144) {
			nstatus(devicespec);
			return OS_dvstat[3]; // return extended error.
		} else 
			return OS_dcb.dstats; // Return SIO error.
	}
	
	return 1;
}

uint8_t nclose(uint8_t* devicespec) {
	uint8_t unit = nunit(devicespec);

	OS_dcb.ddevic = 0x71;
	OS_dcb.dunit  = unit;
	OS_dcb.dcomnd = 'C';
	OS_dcb.dstats = 0x00;
	OS_dcb.dbuf   = 0;
	OS_dcb.dtimlo = 0x1f;
	OS_dcb.dbyt   = 0x100;
	OS_dcb.daux1  = 0;
	OS_dcb.daux2  = 0;
	siov();

	if (OS_dcb.dstats != 1) {
		if (OS_dcb.dstats == 144) {
			nstatus(devicespec);
			return OS_dvstat[3]; // return extended error.
		} else
			return OS_dcb.dstats; // Return SIO error.
	}

	return 1;
}

uint8_t nstatus(uint8_t* devicespec) {
	// Clear the buffer as it appears to be retaining the old values.
	// for (uint8_t i: 0..3) *(OS_dvstat + i) = 0;

	uint8_t unit = nunit(devicespec);

	OS_dcb.ddevic = 0x71;
	OS_dcb.dunit  = unit;
	OS_dcb.dcomnd = 'S';
	OS_dcb.dstats = 0x40;
	OS_dcb.dbuf   = OS_dvstat;
	OS_dcb.dtimlo = 0x1f;
	OS_dcb.dbyt   = 4; // the dvstat block is 4 bytes.
	OS_dcb.daux1  = 0;
	OS_dcb.daux2  = 0;
	siov();

	return OS_dvstat[3];
}

uint8_t nread(uint8_t* devicespec, uint8_t* buf, uint16_t len) {
	uint8_t unit = nunit(devicespec);

	OS_dcb.ddevic = 0x71;
	OS_dcb.dunit  = unit;
	OS_dcb.dcomnd = 'R';
	OS_dcb.dstats = 0x40;
	OS_dcb.dbuf   = buf;
	OS_dcb.dtimlo = 0x1f;
	OS_dcb.dbyt   = len;
	OS_dcb.daux1  = <len;
	OS_dcb.daux2  = >len;
	siov();

	if (OS_dcb.dstats != 1) {
		if (OS_dcb.dstats == 144) {
			nstatus(devicespec);
			return OS_dvstat[3]; // return extended error.
		} else
			return OS_dcb.dstats; // Return SIO error.
	}
	
	return 1;
}

uint8_t nwrite(uint8_t* devicespec, uint8_t* buf, uint16_t len) {
	uint8_t unit = nunit(devicespec);

	OS_dcb.ddevic = 0x71;
	OS_dcb.dunit  = unit;
	OS_dcb.dcomnd = 'W';
	OS_dcb.dstats = 0x80;
	OS_dcb.dbuf   = buf;
	OS_dcb.dtimlo = 0x1f;
	OS_dcb.dbyt   = len;
	OS_dcb.daux1  = <len;
	OS_dcb.daux2  = >len;
	siov();

	if (OS_dcb.dstats != 1) {
		if (OS_dcb.dstats == 144) {
			nstatus(devicespec);
			return OS_dvstat[3]; // return extended error.
		} else
			return OS_dcb.dstats; // Return SIO error.
	}

	return 1;
}
