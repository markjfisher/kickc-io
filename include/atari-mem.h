#include "atari-os.h"

char * const RTCLOK  = 0x12;
void * const CIOV    = 0xE456;
void * const SIOV    = 0xE459;
void * const COLDSV  = 0xE477;

#define OS_iocb0 (*(struct __iocb*) 0x340)
#define OS_iocb1 (*(struct __iocb*) 0x350)
#define OS_iocb2 (*(struct __iocb*) 0x360)
#define OS_iocb3 (*(struct __iocb*) 0x370)
#define OS_iocb4 (*(struct __iocb*) 0x380)
#define OS_iocb5 (*(struct __iocb*) 0x390)
#define OS_iocb6 (*(struct __iocb*) 0x3A0)
#define OS_iocb7 (*(struct __iocb*) 0x3B0)

// Minimal replacements for cc65 full struct types.

// OS_dcb - device control block
#define OS_dcb (*(struct __dcb*) 0x300)
unsigned char * OS_dvstat = 0x2EA;
