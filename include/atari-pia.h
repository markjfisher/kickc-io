#ifndef __PIA_H
#define __PIA_H

/* Define a structure with the PIA register offsets */
struct __pia {
	/* port A data r/w */
    unsigned char   porta;
    
    // disable the ROM between $C000-$CFFF and $D800-$FFFF by setting bit 0 to 0.
	// Bit 1 controls BASIC; if 0, BASIC is enabled, if 1, it is disabled
	// Bits 2 and 3 control the 1200XL LEDs; 0 means on, 1 means off
	// Bits 4-6 are reserved (unused) in the XL and 65XE.
	// Bits 4 and 5 in the 130XE are used to enable bank switching
	// Bit 7 controls the RAM region $5000-$57FF, normally enabled (1).
    unsigned char   portb;

    unsigned char   pactl;  /* port A control */
    unsigned char   pbctl;  /* port B control */
};

const char PORTB_SELFTEST_OFF = %10000000;
const char PORTB_BASIC_OFF    = %00000010;
const char PORTB_SYSTEM_ON    = %00000001;


#endif
