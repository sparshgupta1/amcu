#ifndef _IMAGE_INFO_H_
#define _IMAGE_INFO_H_

#include <stdint.h>

// Sometimes howering might show false results in debugger, but these still have
// right values.
extern uint32_t gu32FirmwareAbsPosition;
extern uint32_t gu32FirmwareOffset;
extern uint32_t gu32FirmwareAbsOffsetChecksum;
extern uint32_t gu32RamVectorTableBegin;
extern int var1;
void foo(volatile uint32_t *d, uint32_t val);
#endif // #define _IMAGE_INFO_H_
