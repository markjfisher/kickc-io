/**
 * N: I/O
 */

#include "atari-nio.h"
#include "atari-sio.h"
#include "atari-os.h"

#include "stddef.h"

unsigned char nunit(char* devicespec) {
	unsigned char unit=1;

	// Set unit to 1 unless explicitly specified.
	if (devicespec[1]==':')
		unit=1;
	else if (devicespec[2]==':')
		unit=devicespec[1]-0x30; // convert from alpha to integer.
	else
		unit=1;

	return unit;
}

unsigned char nopen(char* devicespec, unsigned char trans) {
	unsigned char unit=nunit(devicespec);

	OS_dcb.ddevic=0x71;
	OS_dcb.dunit=unit;
	OS_dcb.dcomnd='O';
	OS_dcb.dstats=0x80;
	OS_dcb.dbuf=devicespec;
	OS_dcb.dtimlo=0x1f;
	OS_dcb.dbyt=256;
	OS_dcb.daux1=12;
	OS_dcb.daux2=trans;
	siov();

	if (OS_dcb.dstats != 1) {
		if (OS_dcb.dstats == 144) {
			nstatus(devicespec);
			return OS_dvstat[3]; // return extended error.
		} else 
			return OS_dcb.dstats; // Return SIO error.
	} else
		return 0;
}

unsigned char nclose(char* devicespec) {
	unsigned char unit=nunit(devicespec);

	OS_dcb.ddevic=0x71;
	OS_dcb.dunit=unit;
	OS_dcb.dcomnd='C';
	OS_dcb.dstats=0x00;
	OS_dcb.dbuf=NULL;
	OS_dcb.dtimlo=0x1f;
	OS_dcb.dbyt=256;
	OS_dcb.daux1=0;
	OS_dcb.daux2=0;
	siov();

	if (OS_dcb.dstats!=1) {
		if (OS_dcb.dstats==144) {
			nstatus(devicespec);
			return OS_dvstat[3]; // return extended error.
		} else
			return OS_dcb.dstats; // Return SIO error.
	} else
		return 0;
}

unsigned char nstatus(char* devicespec) {
	unsigned char unit=nunit(devicespec);

	OS_dcb.ddevic=0x71;
	OS_dcb.dunit=unit;
	OS_dcb.dcomnd='S';
	OS_dcb.dstats=0x40;
	OS_dcb.dbuf=OS_dvstat;
	OS_dcb.dtimlo=0x1f;
	OS_dcb.dbyt=sizeof(OS_dvstat);
	OS_dcb.daux1=0;
	OS_dcb.daux2=0;
	siov();

	return OS_dvstat[3];
}

unsigned char nread(char* devicespec, unsigned char* buf, unsigned short len) {
	unsigned char unit=nunit(devicespec);

	OS_dcb.ddevic=0x71;
	OS_dcb.dunit=unit;
	OS_dcb.dcomnd='R';
	OS_dcb.dstats=0x40;
	OS_dcb.dbuf=buf;
	OS_dcb.dtimlo=0x1f;
	OS_dcb.dbyt=len;
	OS_dcb.daux1 = <len;
	OS_dcb.daux1 = >len;
	siov();

	if (OS_dcb.dstats!=1) {
		if (OS_dcb.dstats==144) {
			nstatus(devicespec);
			return OS_dvstat[3]; // return extended error.
		} else
			return OS_dcb.dstats; // Return SIO error.
	} else
		return 0;
}

unsigned char nwrite(char* devicespec, unsigned char* buf, unsigned short len) {
	unsigned char unit=nunit(devicespec);

	OS_dcb.ddevic=0x71;
	OS_dcb.dunit=unit;
	OS_dcb.dcomnd='W';
	OS_dcb.dstats=0x80;
	OS_dcb.dbuf=buf;
	OS_dcb.dtimlo=0x1f;
	OS_dcb.dbyt=len;
	OS_dcb.daux1 = <len;
	OS_dcb.daux2 = >len;
	siov();

	if (OS_dcb.dstats!=1) {
		if (OS_dcb.dstats==144) {
			nstatus(devicespec);
			return OS_dvstat[3]; // return extended error.
		} else
			return OS_dcb.dstats; // Return SIO error.
	} else
		return 0;
}
