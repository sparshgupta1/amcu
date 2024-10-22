#include "cy_device.h"
#include "cy_pdl.h"
#include "cybsp.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

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

#define AMCU_APP_END 0xDB800
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


int main(void)
{
    // TESTS TO SEE IF CM4 STARTED
    volatile uint32_t *breg3 = (uint32_t *)(0x0803C200);
    double d                 = 36.23;
    *breg3                   = sqrt(d);
    char buf[256];
    volatile uint32_t *breg4 = (uint32_t *)(0x0803C208);
    uint8_t *p_e             = (uint8_t *)malloc(sizeof(uint8_t) * 1);
    *p_e                     = 0x4;
    *breg4                   = (uint32_t)sbrk(0);
    volatile uint32_t *breg5 = (uint32_t *)(0x0803C20C);
    *breg5                   = 0x1A2B3CD4;
    int retVal = snprintf(buf, sizeof(buf), "ABC-%c", 3); /*Not working but not going to the Fault_handler*/
    *breg5                   = 0xA1B2C3D4 + retVal;      /*This piece of code is never reached*/
    for (;;)
    {
    }
}

/* [] END OF FILE */
