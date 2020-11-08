// Cold start the system
void cold_start();

// Turns off OS with custom PORTB($D301) value.
void systemOffB(char port_b);

// Turns off OS and BASIC. Gives maximum available RAM
void systemOff();

// Turns on OS back and resets machine
void systemResetB(char port_b);

// Calls with default arg
void systemReset();

// Enable and set custom handler for Vertical Blank Interrupt.
// To set vertical blank interrupt vector from your inline assembly code
void enableVBLI(void *vblptr);

// Disables custom routine of Vertical Blank Interrupt
void disableVBLI();

// Enables IRQ Interrupts
void enableIRQ();

// Disables IRQ Interrupts
void disableIRQ();

// Set IRQ Interrupt Vector
void setIRQ(void *irqptr);

// Enable and set custom handler for Display List Interrupt
void enableDLI(void *dliptr);
void enableDLI2(void *dliptr);

// Disables Display List Interrupts
void disableDLI();

// Sets font charset located at specified page of memory
void setCharset(char msb);
