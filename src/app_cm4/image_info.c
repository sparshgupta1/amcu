#include "image_info.h"

// Sometimes howering might show false results in debugger, but these still have
// right values.
uint32_t gu32FirmwareAbsPosition;       // = 0x10109000;
uint32_t gu32FirmwareOffset;            //  = 0x102000;
uint32_t gu32FirmwareAbsOffsetChecksum; //  = 0x1000B000;
uint32_t gu32RamVectorTableBegin;       // = 0x0803d200;
int var1 = 0xab;
void foo(volatile uint32_t *d, uint32_t val)
{
    *d = 0xABC;
}