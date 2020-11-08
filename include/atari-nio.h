/**
 * N: I/O
 */

#ifndef NIO_H
#define NIO_H

#include <stdint.h>

#define FUJI_READ 0x04
#define FUJI_WRITE 0x08
#define FUJI_APPEND ((FUJI_READ) | (FUJI_WRITE))

/**
 * Open N: device with devicespec
 * @param devicespec - an N: device spec, e.g. N:TCP://FOO.COM:1234/
 * @param mode - 12 = R/W, 8 = write, 4 = read
 * @param translation mode, 0=none, 1=cr, 2=lf, 3=cr/lf
 * @return error code, or 1 if successful.
 */
uint8_t nopen(uint8_t* devicespec, uint8_t mode, uint8_t trans);

/**
 * Close N: device with devicespec
 * @param devicespec - an N: device spec to close (the unit number is extracted)
 * @return error code, or 1 if successful.
 */
uint8_t nclose(uint8_t* devicespec);

/**
 * Get status of specific N: device
 * @param devicespec - an N: device spec to status (the unit number is extracted)
 * @return error code, or 1 if successful, DVSTAT is also filled with status info.
 *
 * TODO: Check this. There is no OS.dcb.dvstat[], but there is OS.dvstat[]
 * Format of DVSTAT:
 * OS.dcb.dvstat[0] = # of bytes waiting LO
 * OS.dcb.dvstat[1] = # of bytes waiting HI
 * OS.dcb.dvstat[2] = reserved
 * OS.dcb.dvstat[3] = Error code of last I/O operation. !1 = error.
 */
uint8_t nstatus(uint8_t* devicespec);

/**
 * Read # of bytes from specific N: device.
 * @param devicespec - an N: device spec to read bytes from.
 * @param buf - The buffer to read into, must be at least as big as len.
 * @param len - The # of bytes to read (up to 65535)
 * @return error code, or 1 if successful, buf is filled with data.
 */
uint8_t nread(uint8_t* devicespec, uint8_t* buf, uint16_t len);

/**
 * Write # of bytes to specific N: device.
 * @param devicespec - an N: device spec to write to.
 * @param buf - The buffer to write to device, should be at least as big as len.
 * @param len - The # of bytes to write (up to 65535)
 * @return error code, or 1 if successful, buf is filled with data.
 */
uint8_t nwrite(uint8_t* devicespec, uint8_t* buf, uint16_t len);

#endif /* NIO_H */
