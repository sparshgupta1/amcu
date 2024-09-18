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

#define M0_IMAGE_B_SHIFTED_START (0x10004400) //(0x100E0400)
#define M4_IMAGE_B_SHIFTED_START (0x10024000) //(0x10100000)

#define M0_IMAGE_B_STANDARD_START (0x10100400)
#define M4_IMAGE_B_STANDARD_START (0x10120000)

//#define AMCU_APP_END 0x5000 //(#0x3700 OG boot size, 0x4000 for python build)
#define AMCU_APP_END 0x4000 //(#0x3700 OG boot size, 0x4000 for python build)
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

__attribute__((__noinline__)) void *get_pc()
{
    return __builtin_return_address(0);
}

int fw_upg_get_running_image(void)
{
    if (((uintptr_t)&fw_upg_get_running_image < 0x10100000) &&
        ((uintptr_t)&fw_upg_get_running_image > 0x10000000))
    {
        return 0xA;
    }
    else
    {
        return 0xB;
    }
}
int var;
void foo(void){
    var = 1;
}
int main(void)
{
    foo();
    // TEST TO SEE IF CM4 STARTED
    volatile uint32_t *pc    = (uint32_t *)(0x080FF518);
    volatile uint32_t *tst  =  (uint32_t *)(0x080FF51C);
    volatile uint32_t *tst1  = (uint32_t *)(0x080FF520);
    *tst = 0xA0DEDEAD;
    *pc = (uint32_t)get_pc();
    *tst1 = (uint32_t)fw_upg_get_running_image();
    for (;;)
    {
    }
}

/* [] END OF FILE */
