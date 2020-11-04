/**
 * N: I/O
 */

#ifndef NIO_H
#define NIO_H

/**
 * Open N: device with devicespec
 * @param devicespec - an N: device spec, e.g. N:TCP://FOO.COM:1234/
 * @param translation mode, 0=none, 1=cr, 2=lf, 3=cr/lf
 * @return error code, or 1 if successful.
 */
unsigned char nopen(char* devicespec, unsigned char trans);

/**
 * Close N: device with devicespec
 * @param devicespec - an N: device spec to close (the unit number is extracted)
 * @return error code, or 1 if successful.
 */
unsigned char nclose(char* devicespec);

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
unsigned char nstatus(char* devicespec);

/**
 * Read # of bytes from specific N: device.
 * @param devicespec - an N: device spec to read bytes from.
 * @param buf - The buffer to read into, must be at least as big as len.
 * @param len - The # of bytes to read (up to 65535)
 * @return error code, or 1 if successful, buf is filled with data.
 */
unsigned char nread(char* devicespec, unsigned char* buf, unsigned short len);

/**
 * Write # of bytes to specific N: device.
 * @param devicespec - an N: device spec to write to.
 * @param buf - The buffer to write to device, should be at least as big as len.
 * @param len - The # of bytes to write (up to 65535)
 * @return error code, or 1 if successful, buf is filled with data.
 */
unsigned char nwrite(char* devicespec, unsigned char* buf, unsigned short len);

#endif /* NIO_H */
