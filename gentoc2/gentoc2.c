/***************************************************************************
 *        ___
 *   ___ / _ \
 *  / _ ' (_) |       __  _
 * | (_) ,___/       / _|(_)
 *  \___/  _  _ __  | |_  _  _ __    ___  _ __  __ _
 *        | || '_ \ |  _|| || '_ \  / _ \| '__|/ _` |
 *        | || | | || |  | || | | ||  __/| |  | (_| |
 *        |_||_| |_||_|  |_||_| |_| \___||_|   \__,_|
 *
 * @file gentoc2.c
 *
 * @copyright 2023 Infinera. All rights reserved.
 * Infinera Proprietary and Confidential.
 *
 * @details Host implementation  to create amcu_toc2.bin and
 * amcu_public_key.bin to be used during manufacturing.
 ***************************************************************************/

/******************************************************************************
 * Copyright 2020-2022, Cypress Semiconductor Corporation (an Infineon company) or
 * an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
 *
 * This software, including source code, documentation and related
 * materials ("Software") is owned by Cypress Semiconductor Corporation
 * or one of its affiliates ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products.  Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 *******************************************************************************/

/*
 * This is a standalone file with no dependencies on any external libraries
 * or files other than standard Linux includes and libraries
 *
 * Please READ Cypress TRM and APP Notes to understand the boot process, data
 * structures involved and field upgrade consequences. The TOC2 crc's generated
 * by this program is used in amcuapp.bin See fw_upg.c during a commit
 * operation. It is recommenend not to change any if the data in this file, for
 * example setting a new TOC2_FLAG will cause the crc16 field to change and
 * because of which a POR reset will fail. Changing values in this file has
 * consequences for manufacturing, upgrade process and boot integrity. As the
 * TOC2 is a singleton, upgrades/downgrades will fail if CRC values are
 * different. The commit command does not calculate the crc's on the fly but
 * instead uses pre-calculated values. PLEASE DO NOT EDIT THIS FILE, without
 * consulting with the program/manager and getting required approvals.
 */

#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

/*
 **********************************************************************
 *   Structures and constants for Cypress TOC2 structures in SFlash
 **********************************************************************
 */

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
    uint16_t rsvd1;       /*  zero */
    uint16_t crc;         /* CRC16-CCITT */
} toc2_t;

#define CY_PS_TOC2_MAGICNUMBER (0x01211220UL)
#define CY_START_OF_FLASH 0x10000000
#define CY_PS_APP_FORMAT_CYPRESS 1
#define SFLASH_PUBLIC_KEY_ADDR 0x16005A00
#define CY_PS_FLASHBOOT_CLK_50MHZ 0x2
#define CY_PS_FLASHBOOT_WAIT_20MS 0x0
#define CY_PS_FLASHBOOT_WAIT_10MS 0x1
#define CY_PS_FLASHBOOT_VALIDATE_YES 2
#define CY_PS_FLASHBOOT_VALIDATE_NO 1

/* Determines if SWJ pins are configured by flash boot */
#define CY_PS_FLASHBOOT_SWJ_PINS_DISABLE (0x01) /* Disable Debug pins */
#define CY_PS_FLASHBOOT_SWJ_PINS_ENABLE (0x02)  /* Enable Debug pins */
#define CY_PS_TOC_FLAGS_CLOCKS_POS 0
#define CY_PS_TOC_FLAGS_DELAY_POS 2
#define CY_PS_TOC_FLAGS_APP_VERIFY_POS 7
#define CY_PS_TOC_FLAGS_SWJ_ENABLE_POS 5

/*
 * Need SWJ pin for jlink ? or can it be disabled
 */

#define CY_PS_FLASHBOOT_FLAGS                                                  \
    ((CY_PS_FLASHBOOT_CLK_50MHZ << CY_PS_TOC_FLAGS_CLOCKS_POS) |               \
     (CY_PS_FLASHBOOT_WAIT_20MS << CY_PS_TOC_FLAGS_DELAY_POS) |                \
     (CY_PS_FLASHBOOT_VALIDATE_NO << CY_PS_TOC_FLAGS_APP_VERIFY_POS) |        \
     (CY_PS_FLASHBOOT_SWJ_PINS_ENABLE << CY_PS_TOC_FLAGS_SWJ_ENABLE_POS))

/*
 * Fill in constant parts of toc2
 * crc is calculated and filled in.
 */

toc2_t toc2 = {
    .objSize     = sizeof(toc2_t) - sizeof(uint32_t),
    .magicNum    = CY_PS_TOC2_MAGICNUMBER,
    .userKeyAddr = 0UL,
    .appAddr1    = CY_START_OF_FLASH,
    .appFormat1  = CY_PS_APP_FORMAT_CYPRESS,
    .appAddr2    = 0,
    .shashObj    = 1UL,
    .sigKeyAddr  = SFLASH_PUBLIC_KEY_ADDR,
    .tocFlags    = CY_PS_FLASHBOOT_FLAGS,
    .crc         = 0UL,
};

/*
 **************************************
 * Crypto Public key Structs constants
 **************************************
 */

/* Public key definition structure as expected by the Crypto driver */
typedef struct
{
    uint32_t moduloAddr; /* Address of the public key modulus */
    uint32_t moduloSize; /* Size (bits) of the modulus part of the public key */
    uint32_t expAddr;    /* Address of the public key exponent */
    uint32_t expSize; /* Size (bits) of the exponent part of the public key */
    uint32_t barrettAddr;       /* Address of the Barret coefficient */
    uint32_t inverseModuloAddr; /* Address of the binary inverse modulo */
    uint32_t rBarAddr;          /* Address of the (2^moduloLength mod modulo) */
} cy_ps_stc_crypto_public_key_t;

/* Macros used to define the Public key. */

/* RSASSA-PKCS1-v1_5-2048 signature scheme */
#define CY_PS_PUBLIC_KEY_RSA_2048 (0UL)
/* RSASSA-PKCS1-v1_5-1024 signature scheme */
#define CY_PS_PUBLIC_KEY_RSA_1024 (1UL)
/* Offset to public key struct in number of bytes */
#define CY_PS_PUBLIC_KEY_STRUCT_OFFSET (8UL)
/* Modulus length of the RSA key */
#define CY_PS_PUBLIC_KEY_MODULOLENGTH (256UL)
/* Exponent length of the RSA key */
#define CY_PS_PUBLIC_KEY_EXPLENGTH (32UL)
/* Size of Byte in number of bits */
#define CY_PS_PUBLIC_KEY_SIZEOF_BYTE (8UL)

typedef struct
{
    uint32_t objSize;         /* Public key Object size */
    uint32_t signatureScheme; /* Signature scheme */
    cy_ps_stc_crypto_public_key_t
        publicKeyStruct; /* Public key definition struct */
    uint8_t moduloData[CY_PS_PUBLIC_KEY_MODULOLENGTH]; /* Modulo data */
    uint8_t expData[CY_PS_PUBLIC_KEY_EXPLENGTH];       /* Exponent data */
    uint8_t barrettData[CY_PS_PUBLIC_KEY_MODULOLENGTH +
                        4UL]; /* Barret coefficient data */
    uint8_t inverseModuloData[CY_PS_PUBLIC_KEY_MODULOLENGTH]; /* Binary inverse
                                                                 modulo data */
    uint8_t rBarData[CY_PS_PUBLIC_KEY_MODULOLENGTH]; /* 2^moduloLength mod
                                                        modulo data */
} cy_ps_stc_public_key_t;

/*
 * Public key data is from the python generated "C" snippet file.
 */

/* Public key in SFlash */
cy_ps_stc_public_key_t cy_publicKey = {
    .objSize         = sizeof(cy_ps_stc_public_key_t),
    .signatureScheme = CY_PS_PUBLIC_KEY_RSA_2048,
    .publicKeyStruct =
        {
            .moduloAddr = (uint32_t)(SFLASH_PUBLIC_KEY_ADDR) +
                          offsetof(cy_ps_stc_public_key_t, moduloData),
            .moduloSize =
                CY_PS_PUBLIC_KEY_SIZEOF_BYTE * CY_PS_PUBLIC_KEY_MODULOLENGTH,
            .expAddr = (uint32_t)(SFLASH_PUBLIC_KEY_ADDR) +
                       offsetof(cy_ps_stc_public_key_t, expData),
            .expSize =
                CY_PS_PUBLIC_KEY_SIZEOF_BYTE * CY_PS_PUBLIC_KEY_EXPLENGTH,
            .barrettAddr = (uint32_t)(SFLASH_PUBLIC_KEY_ADDR) +
                           offsetof(cy_ps_stc_public_key_t, barrettData),
            .inverseModuloAddr =
                (uint32_t)(SFLASH_PUBLIC_KEY_ADDR) +
                offsetof(cy_ps_stc_public_key_t, inverseModuloData),
            .rBarAddr = (uint32_t)(SFLASH_PUBLIC_KEY_ADDR) +
                        offsetof(cy_ps_stc_public_key_t, rBarData),
        },

   .moduloData =
{
    0xC7u, 0x78u, 0x90u, 0x9Au, 0x85u, 0xB6u, 0x5Eu, 0x0Bu,
    0x3Cu, 0x13u, 0x7Bu, 0x09u, 0x0Eu, 0x2Du, 0xA9u, 0x0Au,
    0xBEu, 0xE1u, 0x0Eu, 0x73u, 0x42u, 0xE0u, 0x4Bu, 0x79u,
    0x13u, 0xAFu, 0x74u, 0xC3u, 0x70u, 0x7Cu, 0x64u, 0x82u,
    0x0Fu, 0xC8u, 0xC8u, 0xE8u, 0x4Bu, 0xDBu, 0x73u, 0xE6u,
    0xBDu, 0xB9u, 0xBFu, 0x5Au, 0x22u, 0x40u, 0xD9u, 0xF2u,
    0x07u, 0xCEu, 0x15u, 0x75u, 0xA4u, 0x7Fu, 0xA6u, 0xEFu,
    0x51u, 0x99u, 0x80u, 0x86u, 0x43u, 0xFBu, 0x5Au, 0x60u,
    0xFBu, 0x49u, 0x2Du, 0xBAu, 0x45u, 0xDBu, 0xABu, 0xC0u,
    0x9Cu, 0xC3u, 0x71u, 0xFDu, 0xA5u, 0x36u, 0xC9u, 0x6Du,
    0xC9u, 0x33u, 0xD1u, 0x0Du, 0xA5u, 0x2Fu, 0xDBu, 0x91u,
    0xC6u, 0x29u, 0xF1u, 0x1Fu, 0xCEu, 0xCAu, 0x7Au, 0xC1u,
    0xE6u, 0x97u, 0xC3u, 0x72u, 0x9Cu, 0x7Au, 0xEEu, 0xF4u,
    0xE4u, 0x9Cu, 0x89u, 0xB5u, 0x7Du, 0xF3u, 0xEEu, 0x75u,
    0x44u, 0x41u, 0x7Du, 0xDCu, 0xB1u, 0xCBu, 0x9Eu, 0x66u,
    0x29u, 0x59u, 0x92u, 0xF3u, 0x44u, 0x39u, 0xBEu, 0x44u,
    0xEAu, 0x01u, 0x70u, 0x0Au, 0xECu, 0xB5u, 0x88u, 0x23u,
    0x71u, 0x59u, 0x3Cu, 0xAEu, 0x35u, 0x4Fu, 0x7Cu, 0xBDu,
    0xA6u, 0x62u, 0x01u, 0x6Cu, 0x24u, 0x7Eu, 0xC2u, 0x77u,
    0xBAu, 0x42u, 0x0Cu, 0xBEu, 0x41u, 0x0Cu, 0x21u, 0xE1u,
    0x25u, 0x20u, 0x8Bu, 0xE2u, 0xDAu, 0x7Eu, 0x6Du, 0xD9u,
    0x3Fu, 0x29u, 0x12u, 0x4Bu, 0x2Cu, 0xDDu, 0x85u, 0x3Eu,
    0x56u, 0x5Eu, 0x58u, 0xFEu, 0xE8u, 0xEDu, 0xF2u, 0x35u,
    0xA9u, 0xCDu, 0x5Cu, 0x7Fu, 0x0Bu, 0x8Cu, 0x83u, 0xF6u,
    0x76u, 0xBAu, 0x4Au, 0xBEu, 0xD1u, 0x5Du, 0x9Du, 0xAAu,
    0x49u, 0xD4u, 0x31u, 0x93u, 0x95u, 0xB7u, 0x41u, 0xE2u,
    0xD0u, 0x7Au, 0x6Bu, 0x35u, 0xE9u, 0x3Au, 0x47u, 0xD7u,
    0xECu, 0x64u, 0x76u, 0xD4u, 0x6Cu, 0x4Du, 0x35u, 0x33u,
    0xE3u, 0xB3u, 0xEFu, 0xDDu, 0x9Au, 0xF1u, 0x47u, 0x4Cu,
    0xF0u, 0x1Au, 0xE7u, 0xEFu, 0x7Fu, 0xEFu, 0x48u, 0xC3u,
    0x89u, 0x6Eu, 0x13u, 0x26u, 0x91u, 0x16u, 0xD5u, 0x72u,
    0xC0u, 0xADu, 0x36u, 0x26u, 0x6Fu, 0x42u, 0x88u, 0x84u,
},
.expData =
{
    0x01u, 0x00u, 0x01u, 0x00u,
},
.barrettData =
{
    0xEEu, 0xFBu, 0x92u, 0x39u, 0xBAu, 0xB8u, 0x87u, 0x4Cu,
    0x8Du, 0x37u, 0xE0u, 0x5Du, 0x3Du, 0x95u, 0x7Cu, 0x7Eu,
    0x8Du, 0xADu, 0x78u, 0x62u, 0x1Eu, 0x76u, 0x69u, 0x0Cu,
    0x0Eu, 0x42u, 0x12u, 0x73u, 0x8Bu, 0x94u, 0x6Fu, 0x92u,
    0x8Du, 0x7Fu, 0x41u, 0x3Fu, 0x77u, 0x9Cu, 0x06u, 0xD6u,
    0x5Au, 0x63u, 0x1Au, 0xDDu, 0xA8u, 0xA1u, 0x44u, 0x07u,
    0xD2u, 0x3Au, 0x8Eu, 0x40u, 0x74u, 0x5Eu, 0xDFu, 0xF0u,
    0xA9u, 0x1Eu, 0xC1u, 0x0Du, 0x1Du, 0x59u, 0x56u, 0xEBu,
    0x3Eu, 0x15u, 0x44u, 0xDAu, 0x64u, 0x4Eu, 0x4Du, 0xF8u,
    0x1Du, 0xEBu, 0xB1u, 0xA4u, 0x50u, 0x22u, 0x20u, 0x80u,
    0x2Au, 0x32u, 0x4Cu, 0x12u, 0x9Fu, 0x81u, 0x6Bu, 0xB1u,
    0x54u, 0x56u, 0x26u, 0x17u, 0x15u, 0xD7u, 0x63u, 0xD2u,
    0x12u, 0x9Bu, 0x46u, 0x1Au, 0xF2u, 0xCBu, 0x81u, 0xC1u,
    0x65u, 0x56u, 0x46u, 0x14u, 0x85u, 0x9Au, 0x73u, 0xD3u,
    0x79u, 0x0Eu, 0x40u, 0x47u, 0x16u, 0x83u, 0x3Du, 0x03u,
    0x63u, 0xB3u, 0xF0u, 0x2Au, 0xDDu, 0x56u, 0x9Du, 0x52u,
    0x82u, 0xA1u, 0xD2u, 0x2Eu, 0xAAu, 0x4Fu, 0x82u, 0x0Du,
    0xD5u, 0x78u, 0xADu, 0x41u, 0x82u, 0xA0u, 0xB5u, 0x18u,
    0x37u, 0xECu, 0x1Eu, 0xFCu, 0xE6u, 0x1Cu, 0xD5u, 0x62u,
    0xBDu, 0xEAu, 0xEEu, 0x8Au, 0x05u, 0xD7u, 0xDBu, 0xC2u,
    0x0Fu, 0x72u, 0x8Fu, 0x4Bu, 0xA1u, 0x5Fu, 0x0Au, 0xB5u,
    0x62u, 0xDEu, 0xA0u, 0x92u, 0x97u, 0xD0u, 0xDAu, 0x78u,
    0xD8u, 0xE8u, 0xBCu, 0x62u, 0x9Cu, 0x35u, 0x5Bu, 0xCAu,
    0xFBu, 0xB2u, 0x6Bu, 0x31u, 0x38u, 0x00u, 0x29u, 0x5Bu,
    0x5Eu, 0x11u, 0x26u, 0xADu, 0xEEu, 0xC8u, 0xAFu, 0xA2u,
    0xD8u, 0x5Du, 0x49u, 0x39u, 0xD4u, 0xD2u, 0x36u, 0xA0u,
    0xC8u, 0xECu, 0xD4u, 0xF4u, 0xCDu, 0x08u, 0x73u, 0x6Cu,
    0x1Eu, 0x02u, 0xEBu, 0xD3u, 0x34u, 0x98u, 0x8Au, 0x03u,
    0x20u, 0x8Du, 0x93u, 0xACu, 0x23u, 0x58u, 0x85u, 0x9Cu,
    0x6Bu, 0x90u, 0xC3u, 0x37u, 0xD8u, 0x45u, 0x67u, 0x79u,
    0x0Fu, 0xF9u, 0x9Du, 0x8Bu, 0x66u, 0x24u, 0xCAu, 0x08u,
    0xEAu, 0xCCu, 0x6Du, 0x1Fu, 0x6Fu, 0xACu, 0x7Du, 0xEEu,
    0x01u, 0x00u, 0x00u, 0x00u,
},
.inverseModuloData =
{
    0x09u, 0x37u, 0x3Fu, 0xC2u, 0xDCu, 0xE7u, 0x39u, 0xF7u,
    0x7Au, 0x55u, 0xF6u, 0x53u, 0xD5u, 0x3Fu, 0x0Du, 0x93u,
    0x2Cu, 0xF2u, 0xD7u, 0xC5u, 0x5Eu, 0x8Au, 0x26u, 0x4Cu,
    0xB7u, 0x5Eu, 0x8Bu, 0x3Bu, 0xD7u, 0xBDu, 0x25u, 0x70u,
    0xA3u, 0x73u, 0x2Au, 0x0Bu, 0x88u, 0x70u, 0xEFu, 0x7Bu,
    0x87u, 0x1Eu, 0x24u, 0x0Fu, 0x1Fu, 0x3Au, 0x15u, 0xC1u,
    0xACu, 0xFDu, 0x0Du, 0x02u, 0x63u, 0x70u, 0x28u, 0x70u,
    0x3Du, 0x23u, 0x8Cu, 0x53u, 0x6Bu, 0xB5u, 0x27u, 0xFDu,
    0x80u, 0x8Fu, 0x69u, 0xB6u, 0xF9u, 0xEFu, 0x70u, 0x36u,
    0xB4u, 0x26u, 0xEFu, 0xD4u, 0x4Eu, 0xFEu, 0xF1u, 0x72u,
    0x33u, 0xB5u, 0x57u, 0x01u, 0x8Cu, 0x41u, 0x28u, 0xFBu,
    0x7Au, 0x98u, 0x0Du, 0x97u, 0xBFu, 0x99u, 0x62u, 0xE2u,
    0x5Fu, 0x25u, 0x21u, 0x4Au, 0x5Fu, 0xECu, 0x92u, 0xD4u,
    0xD2u, 0x55u, 0x75u, 0x46u, 0xD0u, 0xDCu, 0x39u, 0x07u,
    0xA0u, 0xA8u, 0x1Eu, 0xFAu, 0xE6u, 0xF9u, 0x53u, 0xC4u,
    0xEFu, 0x59u, 0x49u, 0x7Du, 0x53u, 0x3Eu, 0x05u, 0xBFu,
    0x57u, 0xF5u, 0xEFu, 0x2Au, 0x25u, 0x1Cu, 0xC3u, 0xF0u,
    0x0Au, 0x1Au, 0xDFu, 0xC3u, 0x8Du, 0x7Au, 0x58u, 0x4Fu,
    0xB7u, 0x39u, 0xF0u, 0x40u, 0x96u, 0xFAu, 0xA3u, 0x83u,
    0x37u, 0x2Au, 0x78u, 0x37u, 0x08u, 0x5Eu, 0xA7u, 0xEFu,
    0xA6u, 0x31u, 0x0Du, 0x12u, 0xB9u, 0x07u, 0xC8u, 0x67u,
    0x8Fu, 0x78u, 0x92u, 0x22u, 0xBDu, 0x3Du, 0x25u, 0xF5u,
    0x76u, 0xE4u, 0x04u, 0x90u, 0x43u, 0x70u, 0x2Eu, 0xC7u,
    0xB2u, 0x62u, 0x69u, 0xFFu, 0x61u, 0x84u, 0xEFu, 0x3Au,
    0xB2u, 0x72u, 0x41u, 0x64u, 0x08u, 0xE6u, 0xB8u, 0xFCu,
    0x7Bu, 0xCBu, 0x94u, 0xBEu, 0xAAu, 0xFDu, 0xD3u, 0x26u,
    0x2Bu, 0xB8u, 0x73u, 0x08u, 0xB9u, 0xC5u, 0xACu, 0xCBu,
    0xE8u, 0x26u, 0x03u, 0x82u, 0xE6u, 0x76u, 0xBEu, 0xE7u,
    0xCBu, 0xA8u, 0xB5u, 0xB9u, 0x66u, 0xE9u, 0xEBu, 0xDFu,
    0x88u, 0x9Fu, 0xD1u, 0xFEu, 0x61u, 0xE4u, 0x18u, 0x26u,
    0xC0u, 0x9Fu, 0x3Cu, 0x30u, 0x34u, 0xE5u, 0xA3u, 0x13u,
    0xE5u, 0x78u, 0xF4u, 0x11u, 0xFDu, 0x52u, 0x05u, 0xC7u,
},
.rBarData =
{
    0x39u, 0x87u, 0x6Fu, 0x65u, 0x7Au, 0x49u, 0xA1u, 0xF4u,
    0xC3u, 0xECu, 0x84u, 0xF6u, 0xF1u, 0xD2u, 0x56u, 0xF5u,
    0x41u, 0x1Eu, 0xF1u, 0x8Cu, 0xBDu, 0x1Fu, 0xB4u, 0x86u,
    0xECu, 0x50u, 0x8Bu, 0x3Cu, 0x8Fu, 0x83u, 0x9Bu, 0x7Du,
    0xF0u, 0x37u, 0x37u, 0x17u, 0xB4u, 0x24u, 0x8Cu, 0x19u,
    0x42u, 0x46u, 0x40u, 0xA5u, 0xDDu, 0xBFu, 0x26u, 0x0Du,
    0xF8u, 0x31u, 0xEAu, 0x8Au, 0x5Bu, 0x80u, 0x59u, 0x10u,
    0xAEu, 0x66u, 0x7Fu, 0x79u, 0xBCu, 0x04u, 0xA5u, 0x9Fu,
    0x04u, 0xB6u, 0xD2u, 0x45u, 0xBAu, 0x24u, 0x54u, 0x3Fu,
    0x63u, 0x3Cu, 0x8Eu, 0x02u, 0x5Au, 0xC9u, 0x36u, 0x92u,
    0x36u, 0xCCu, 0x2Eu, 0xF2u, 0x5Au, 0xD0u, 0x24u, 0x6Eu,
    0x39u, 0xD6u, 0x0Eu, 0xE0u, 0x31u, 0x35u, 0x85u, 0x3Eu,
    0x19u, 0x68u, 0x3Cu, 0x8Du, 0x63u, 0x85u, 0x11u, 0x0Bu,
    0x1Bu, 0x63u, 0x76u, 0x4Au, 0x82u, 0x0Cu, 0x11u, 0x8Au,
    0xBBu, 0xBEu, 0x82u, 0x23u, 0x4Eu, 0x34u, 0x61u, 0x99u,
    0xD6u, 0xA6u, 0x6Du, 0x0Cu, 0xBBu, 0xC6u, 0x41u, 0xBBu,
    0x15u, 0xFEu, 0x8Fu, 0xF5u, 0x13u, 0x4Au, 0x77u, 0xDCu,
    0x8Eu, 0xA6u, 0xC3u, 0x51u, 0xCAu, 0xB0u, 0x83u, 0x42u,
    0x59u, 0x9Du, 0xFEu, 0x93u, 0xDBu, 0x81u, 0x3Du, 0x88u,
    0x45u, 0xBDu, 0xF3u, 0x41u, 0xBEu, 0xF3u, 0xDEu, 0x1Eu,
    0xDAu, 0xDFu, 0x74u, 0x1Du, 0x25u, 0x81u, 0x92u, 0x26u,
    0xC0u, 0xD6u, 0xEDu, 0xB4u, 0xD3u, 0x22u, 0x7Au, 0xC1u,
    0xA9u, 0xA1u, 0xA7u, 0x01u, 0x17u, 0x12u, 0x0Du, 0xCAu,
    0x56u, 0x32u, 0xA3u, 0x80u, 0xF4u, 0x73u, 0x7Cu, 0x09u,
    0x89u, 0x45u, 0xB5u, 0x41u, 0x2Eu, 0xA2u, 0x62u, 0x55u,
    0xB6u, 0x2Bu, 0xCEu, 0x6Cu, 0x6Au, 0x48u, 0xBEu, 0x1Du,
    0x2Fu, 0x85u, 0x94u, 0xCAu, 0x16u, 0xC5u, 0xB8u, 0x28u,
    0x13u, 0x9Bu, 0x89u, 0x2Bu, 0x93u, 0xB2u, 0xCAu, 0xCCu,
    0x1Cu, 0x4Cu, 0x10u, 0x22u, 0x65u, 0x0Eu, 0xB8u, 0xB3u,
    0x0Fu, 0xE5u, 0x18u, 0x10u, 0x80u, 0x10u, 0xB7u, 0x3Cu,
    0x76u, 0x91u, 0xECu, 0xD9u, 0x6Eu, 0xE9u, 0x2Au, 0x8Du,
    0x3Fu, 0x52u, 0xC9u, 0xD9u, 0x90u, 0xBDu, 0x77u, 0x7Bu,
},


};

/*
 * crc16_ccitt for checksumming the toc2 table
 */

/*
 * Table to calculate CRC16 variant known as CRC-CCITT-FALSE
 * Reflected bits order, does not augment final value.
 */
uint16_t const crc_ccitt_false_table[256] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7, 0x8108,
    0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF, 0x1231, 0x0210,
    0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6, 0x9339, 0x8318, 0xB37B,
    0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE, 0x2462, 0x3443, 0x0420, 0x1401,
    0x64E6, 0x74C7, 0x44A4, 0x5485, 0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE,
    0xF5CF, 0xC5AC, 0xD58D, 0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6,
    0x5695, 0x46B4, 0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D,
    0xC7BC, 0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B, 0x5AF5,
    0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12, 0xDBFD, 0xCBDC,
    0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A, 0x6CA6, 0x7C87, 0x4CE4,
    0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41, 0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD,
    0xAD2A, 0xBD0B, 0x8D68, 0x9D49, 0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13,
    0x2E32, 0x1E51, 0x0E70, 0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A,
    0x9F59, 0x8F78, 0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E,
    0xE16F, 0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E, 0x02B1,
    0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256, 0xB5EA, 0xA5CB,
    0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D, 0x34E2, 0x24C3, 0x14A0,
    0x0481, 0x7466, 0x6447, 0x5424, 0x4405, 0xA7DB, 0xB7FA, 0x8799, 0x97B8,
    0xE75F, 0xF77E, 0xC71D, 0xD73C, 0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657,
    0x7676, 0x4615, 0x5634, 0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9,
    0xB98A, 0xA9AB, 0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882,
    0x28A3, 0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
    0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92, 0xFD2E,
    0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9, 0x7C26, 0x6C07,
    0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1, 0xEF1F, 0xFF3E, 0xCF5D,
    0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8, 0x6E17, 0x7E36, 0x4E55, 0x5E74,
    0x2E93, 0x3EB2, 0x0ED1, 0x1EF0};

static inline uint16_t crc_ccitt_false_byte(uint16_t crc, const uint8_t c)
{
    return (crc << 8) ^ crc_ccitt_false_table[(crc >> 8) ^ c];
}

/**
 * crc_ccitt_false - recompute the CRC (CRC-CCITT-FALSE variant)
 * for the data buffer
 * @crc: previous CRC value
 * @buffer: data pointer
 * @len: number of bytes in the buffer
 */
uint16_t crc_ccitt_false(uint16_t crc, uint8_t *buffer, size_t len)
{
    while (len--)
        crc = crc_ccitt_false_byte(crc, *buffer++);
    return crc;
}

/*
 * Returns the crc16 of toc2
 */

uint16_t gentoc2_crc16()
{
    uint16_t crc16;

    crc16 = crc_ccitt_false(0xffff, (uint8_t *)&toc2,
                            sizeof(toc2) - sizeof(uint32_t));

    return crc16;
}

/*
 **************************************************************************
 * gentoc2 application
 * This application genrates binary files to be programmed into the Sflash
 * output file: amcu_toc2.bin is programmed to the toc2 region at
 * address 0x16007c00
 * output file: amcu_public_key.bin  is programmed to the SFlash public key
 * region at 0x16005A00.
 * Prints out the CRC for PartitionA to be used in fw_upg.c
 * To generate the crc for partitionB manually change the application start
 * address to 0x10100000 and rebuild/run the program.
 **************************************************************************
 */

const char *toc2_fn = "amcu_toc2.bin";
const char *key_fn  = "amcu_public_key.bin";

int create_toc2_bin(void)
{

    int outfd;
    int rc = -1;

    printf("Writing AMCU toc2  to %s\n", toc2_fn);

    outfd = open(toc2_fn, O_CREAT | O_WRONLY | O_TRUNC, 0666);
    if (outfd < 0)
    {
        printf("Error creating toc2 file: %s\n", strerror(errno));
        return (1);
    }

    toc2.crc = gentoc2_crc16();

    printf("toc2.crc = 0x%4.4x\n", toc2.crc);

    rc = write(outfd, &toc2, sizeof(toc2));
    if (rc < 0)
    {
        printf("Error writing toc2 file: %s\n", strerror(errno));
        close(outfd);
        return (2);
    }

    close(outfd);
    return (0);
}

int create_public_key_bin(void)
{

    int outfd;
    int rc = -1;

    printf("Writing AMCU public_key to %s\n", key_fn);

    outfd = open(key_fn, O_CREAT | O_WRONLY | O_TRUNC, 0666);
    if (outfd < 0)
    {
        printf("Error creating public key file: %s\n", strerror(errno));
        return (1);
    }

    rc = write(outfd, &cy_publicKey, sizeof(cy_publicKey));
    if (rc < 0)
    {
        printf("Error writing public key file: %s\n", strerror(errno));
        close(outfd);
        return (2);
    }

    close(outfd);
    return (0);
}

int main(void)
{
    printf("Utility to generate Sflash Toc2 and public key binary files\n");
    printf("For programming Cypress PSoc6 Soc used in AMCU\n");

    create_toc2_bin();

    create_public_key_bin();

    exit(0);
}
