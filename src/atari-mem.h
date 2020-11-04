#include "atari-os.h"

char * const RTCLOK  = 0x12;
void * const CIOV    = 0xE456;
void * const SIOV    = 0xE459;
void * const COLDSV  = 0xE477;

#define iocb0 (*(struct __iocb*) 0x340)
#define iocb1 (*(struct __iocb*) 0x350)
#define iocb2 (*(struct __iocb*) 0x360)
#define iocb3 (*(struct __iocb*) 0x370)
#define iocb4 (*(struct __iocb*) 0x380)
#define iocb5 (*(struct __iocb*) 0x390)
#define iocb6 (*(struct __iocb*) 0x3A0)
#define iocb7 (*(struct __iocb*) 0x3B0)
