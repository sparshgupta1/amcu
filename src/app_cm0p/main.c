#include "cy_pdl.h"
#include "cy_result.h"
#include "cybsp.h"
#include "clock.h"

#include <stdlib.h>

#define CM0_VTABLE_REG_ADDR (0x40201120)

#define BOOT_CM0P_SIZE (0x4000)
#define BOOT_CM0P_START (0x10000400)

#define CPUSS_CM0_CTL_REG_ADDR (0x40201000)
#define CPUSS_CM0_CTL_REG_VAL (0x05fa0000)

#define FLASH_IMAGE_A_ADDR 0x10000000
#define FLASH_IMAGE_B_ADDR 0x10100000
#define M4_START_OFFSET 0x20000

#define M4_START_ADDRESS (0x10002000)

#define BREG0 (0x40271000)

/* MAIN FUNCTION */

int main(void)
{
    uint32_t m4_addr;

    boot_info_t bootInfo;
    volatile uint32_t *breg0 = (uint32_t *)(BACKUP_BREG0);
    bootInfo.value           = *breg0;
    uint8_t bootType         = bootInfo.BITS.boot;

    /*
     * Initialize the device and board peripherals
     *
     * Note:
     * cybsp_init() MUST BE CALLED FROM BOTH M0 and M4 sides. Required to be
     * called from M4 side to initialize HAL, and must be called from M0 side
     * to initialize HW registers (Clocks, peripherals, etc.).
     */
    cy_rslt_t bspInitRes = cybsp_init();
    bspInitRes = bspInitRes;

    clock_configure_all(bootType);
	
    m4_addr = 0x10000000 + 0x2000; /*Start the Cm4 bootloader code*/
    volatile uint32_t *breg2 = (uint32_t *)(0x080FF510);
    *breg2                   = 0xDEADC0DE;
    Cy_SysEnableCM4(m4_addr);
    while (1)
    {
    }
}

/* [] END OF FILE */