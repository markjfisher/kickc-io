/**
 * Simple conio for E:
 */

#include <string.h>
#include "atari-mem.h"
#include "atari-cio.h"

void printl(const char* c, unsigned int l) {
	char *p = c;

	// fix \n
	while (*p != 0) {
		if (*p == '\n') *p = 0x9b;
		p++;
	}

	OS_iocb0.buffer=c;
	OS_iocb0.buflen=l;
	OS_iocb0.command=IOCB_PUTCHR;
	ciov();
}

void printc(char* c) {
	OS_iocb0.buffer=c;
	OS_iocb0.buflen=1;
	OS_iocb0.command=IOCB_PUTCHR;
	ciov();
}

void print(const char* c) {
	unsigned int l=strlen(c);
	printl(c,l);
}

void get_line(char* buf, unsigned int len) {
	OS_iocb0.buffer=buf;
	OS_iocb0.buflen=len;
	OS_iocb0.command=IOCB_GETREC;
	ciov();
}

char get_char() {
	char buf;
	OS_iocb0.buffer=&buf;
	OS_iocb0.buflen=1;
	OS_iocb0.command=IOCB_GETCHR;
	ciov();
	return buf;
}

/*

        .include "atari.inc"
        .export _cgetc
        .import setcursor
        .import KEYBDV_handler

_cgetc:
        jsr     setcursor
        lda     #12
        sta     ICAX1Z          ; fix problems with direct call to KEYBDV
        jsr     KEYBDV_handler
        ldx     #0
        rts


KEYBDV_handler:

        lda     #>(kret-1)
        pha
        lda     #<(kret-1)
        pha
        lda     PORTB
        sta     cur_KEYBDV_PORTB
        enable_rom
        lda     KEYBDV+5
        pha
        lda     KEYBDV+4
        pha
        rts             ; call keyboard handler
kret:   pha
        disable_rom_val cur_KEYBDV_PORTB
        pla
        rts



.macro  enable_rom
        lda     PORTB
        ora     #1
        wsync
        sta     PORTB
        set_chbase $E0
.endmacro

.macro  enable_rom_quick
        lda     PORTB
        ora     #1
        sta     PORTB
        set_chbase $E0
.endmacro


.macro  disable_rom
        lda     PORTB
        and     #$fe
        wsync
        sta     PORTB
        set_chbase >__CHARGEN_START__
.endmacro

.macro  disable_rom_quick
        lda     PORTB
        and     #$fe
        sta     PORTB
        set_chbase >__CHARGEN_START__
.endmacro

.macro  disable_rom_val val
        lda     val
        wsync
        sta     PORTB
        set_chbase >__CHARGEN_START__
.endmacro


 */
