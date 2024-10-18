/*        ___
 *   ___ / _ \
 *  / _ ' (_) |       __  _
 * | (_) ,___/       / _|(_)
 *  \___/  _  _ __  | |_  _  _ __    ___  _ __  __ _
 *        | || '_ \ |  _|| || '_ \  / _ \| '__|/ _` |
 *        | || | | || |  | || | | ||  __/| |  | (_| |
 *        |_||_| |_||_|  |_||_| |_| \___||_|   \__,_|
 *
 * main.c     Main for XR_TMCU application linked with FreeRTOS and
 *            a bsp. The Required tasks are created and the scheduler
 *            is started.
 *
 * Copyright (C) 2021,2022,2023,2024 Infinera. All rights reserved.
 *
 * Infinera Proprietary and Confidential
 *
 */

#include "cy_device.h"
#include "cy_pdl.h"
#include "cybsp.h"

/* Macros */
#define CM0_VTABLE_REG_ADDR (0x40201120)
#define CM4_VTABLE_REG_ADDR (0x40200200)

#define BOOT_CM0P_SIZE (0x4000)

#define CPUSS_CM0_CTL_REG_ADDR (0x40201000)
#define CPUSS_CM0_CTL_REG_VAL (0x05fa0000)

#define APP_CM0P_START_ADDRESS (0x10000400)
#define APP_CM4_START_ADDRESS (0x10020000)

#define M0_IMAGE_B_SHIFTED_START (0x10003C00) //(0x100E0400)
#define M4_IMAGE_B_SHIFTED_START (0x10023800) //(0x10100000)

#define M0_IMAGE_B_STANDARD_START (0x10100400)
#define M4_IMAGE_B_STANDARD_START (0x10120000)

#define AMCU_APP_END 0x3700
#define CY_PS_DIGSIG_SIZE 256
#define CY_PS_CPUID (0xC6000000UL)
#define CM0P_VECTOR_TABLE_OFFSET (0x400 - 0x10)

/* Table of Content structure */
typedef struct
{
    uint32_t objSize;     /* Object size (Bytes) */
    uint32_t magicNum;    /* TOC ID (magic number) */
    uint32_t userKeyAddr; /* Secure key address in user Flash */
    uint32_t smifCfgAddr; /* SMIF configuration structure */
    uint32_t appAddr1;    /* First user application object address */
    uint32_t appFormat1;  /* First user application format */
    uint32_t appAddr2;    /* Second user application object address */
    uint32_t appFormat2;  /* Second user application format */
    uint32_t shashObj;    /* Number of additional objects to be verified */
    uint32_t sigKeyAddr;  /* Signature verification key address */
    uint32_t addObj[116]; /* Additional objects to include in S-HASH */
    uint32_t tocFlags;    /* Flags in TOC to control Flash boot options */
    uint32_t crc;         /* CRC16-CCITT */
} cy_stc_ps_toc_t;

/* Application header in Cypress format */
typedef struct
{
    uint32_t objSize;       /* Object size (Bytes) */
    uint32_t appId;         /* Application ID/version */
    uint32_t appAttributes; /* Attributes (reserved for future use) */
    uint32_t numCores;      /* Number of cores */
    uint32_t core0Vt;       /* (CM0+)VT offset - offset to the vector table
                                        from that entry */
    uint32_t core0Id;       /* CM0+ core ID */
} cy_stc_ps_appheader_t;

/*
 * Application header for CM0P
 * It is kept here because the CM0P application is part of the app_cm4 elf
 * image as a binary blob. The related TOC2 changes are part of this
 * Module/File.
 */

CY_SECTION(".cy_app_header")
__USED static const cy_stc_ps_appheader_t cy_ps_appheader = {
    .objSize       = AMCU_APP_END - CY_PS_DIGSIG_SIZE,
    .appId         = 0UL, /* Unused */
    .appAttributes = 0UL,
    .numCores      = 1UL, /* Only CM0+, CM4 is managed by FW */
    .core0Vt       = CM0P_VECTOR_TABLE_OFFSET, /* CM0_PS_VT_OFFSET */
    .core0Id       = CY_PS_CPUID               /* CM0+ core ID */
};

CY_SECTION(".cy_app_signature")
__USED
CY_ALIGN(4) static const uint8_t cy_ps_appSignature[CY_PS_DIGSIG_SIZE] = {0u};

typedef void (*func_ptr_t)(void);
void vDeInitAndJumpToMainFirmware(func_ptr_t vPlatformSpecificDeinit,
                                  uint32_t u32FirmwareOffset,
                                  uint32_t u32FirmwareAddress)
{
    uint32_t u32FirmwareStackPointerAddress = 0;
    uint32_t u32FirmwareResetHandlerAddress = 0;
    // volatile uint32_t *d                    = (uint32_t *)(0x080FF54C);
    volatile uint32_t *tst3       = (uint32_t *)(0x080FF534);
    volatile uint32_t *tst1       = (uint32_t *)(0x080FF500);
    uint32_t *pu32FwFlashPointer  = (uint32_t *)u32FirmwareAddress;
    uint32_t u32RegistersChecksum = 0;

    // Read 4 first bytes from FW, the stack pointer
    u32FirmwareStackPointerAddress = *pu32FwFlashPointer;
    *tst1                          = u32FirmwareStackPointerAddress;
    // Read 4 next bytes from FW, reset handler address
    pu32FwFlashPointer++;
    u32FirmwareResetHandlerAddress = *pu32FwFlashPointer;
    // Patch it with offset
    u32FirmwareResetHandlerAddress += u32FirmwareOffset;
    //*d = u32FirmwareResetHandlerAddress;

    // Calculate simple checksum of the registers to be passed
    u32RegistersChecksum = u32FirmwareAddress ^ u32FirmwareOffset;
    *tst3                = 0xB1;
    if (vPlatformSpecificDeinit != NULL)
    {
        vPlatformSpecificDeinit();
    }
    *tst3 = 0xB2;
    // Everything set; time to store addresses bootloader is going to pass to
    // firmware via registers. After this the firmware knows what should be done
    // and it does things (system memory remapping, vector table things, global
    // offset table operations) autonomously.
    //*tst3 = 0xB3;
    // Store firmware absolute address to r10 (hoop in case we have limited
    // Cortex-M0)
    asm("ldr r6, %0; mov r10, r6" : "=m"(u32FirmwareAddress) : :);
    //  asm ("ldr r6, %0; mov r10, r6"
    //        :"=m"(0x10009000)
    //        :
    //        :);
    *tst3 = 0xB4;
    // Store firmware offset to r11 (hoop in case we have limited Cortex-M0)
    asm("ldr r6, %0; mov r11, r6;" : "=m"(u32FirmwareOffset) : :);
    *tst3 = 0xB5;
    // Store registers checksum to r12 (hoop in case we have limited Cortex-M0)
    asm("ldr r6, %0; mov r12, r6;" : "=m"(u32RegistersChecksum) : :);
    //*tst3 = 0x1111B6;
    // volatile uint32_t *a = (uint32_t *)(0x080FF540);
    // volatile uint32_t *b = (uint32_t *)(0x080FF544);
    // volatile uint32_t *c = (uint32_t *)(0x080FF548);

    // *a = u32FirmwareAddress; // u32FirmwareAddress;
    // *b = u32FirmwareOffset;  // u32FirmwareOffset;
    // *c = u32RegistersChecksum;

    // Actual jump
    // asm("mov sp, %0; bx %1;" : : "r"(0x10009000), "r"(0x10009004));
    // uint32_t* sp = (uint32_t *)(0x10009000);
    // uint32_t resetHandler = *(uint32_t *)(0x10009000+4);
    __asm volatile("MSR msp, %0" : : "r"(&u32FirmwareStackPointerAddress));
    ((func_ptr_t)u32FirmwareResetHandlerAddress)();
    //*tst3 = 0xB7;
}

static void vL432kc_DeInit(void)
{
    __disable_irq();
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL  = 0;
}

int main(void)
{
    // TEST TO SEE IF CM4 STARTED
    volatile uint32_t *breg0 = (uint32_t *)(0x080FF518);
    *breg0                   = 0xA0DEDEAD;

    // As and 0s to show that cm0 core started
    // randomData2[0] = 0xAAAA4444;

    /* 1
     * Determine if you need cold or warm boot.
     * Use some backup register to determine if this is the first time
     * ever running the amcu image.
     */
    // if( something )
    // set cold/warm boot register

    /* 2
     * Check the value of the vector table to know
     * if it is safe to jump.
     * If not, need to stay in current image.
     * Use a backup register to store info about
     * things that go bad.
     */
    // vector table addr != 0xFFFFFFFF;
    // if it is go to other image...

    /* 3
     * Change vector table bases for M0p and M4.
     */
    // volatile uint32_t *vtbl_addr_cm0 = (uint32_t *)(CM0_VTABLE_REG_ADDR);
    //*vtbl_addr_cm0                   = M0_IMAGE_B_SHIFTED_START;

    // volatile uint32_t *vtbl_addr_cm4 = (uint32_t *)(CM4_VTABLE_REG_ADDR);
    //*vtbl_addr_cm4                   = M4_IMAGE_B_SHIFTED_START;

    // /* 4
    //  * Trick the system in believing we are doing HIBERNATION by setting a
    //  * predefined TOKEN in the PWR_HIBERNATE register along with all possible
    //  * hibernate sources.
    //  */
    // SRSS_PWR_HIBERNATE =
    //     (SRSS_PWR_HIBERNATE & HIBERNATE_WAKEUP_MASK) | HIBERNATE_TOKEN;

    /* 5
     * Reset CM0p.
     */
    // volatile uint32_t *cpuss_cm0_ctl = (uint32_t *)(CPUSS_CM0_CTL_REG_ADDR);
    //*cpuss_cm0_ctl                   = CPUSS_CM0_CTL_REG_VAL;

    uint32_t u32JumpAddress = 0x10129000;
    vDeInitAndJumpToMainFirmware(&vL432kc_DeInit, 0x109000, u32JumpAddress);
    for (;;)
    {
    }
}

/* [] END OF FILE */
