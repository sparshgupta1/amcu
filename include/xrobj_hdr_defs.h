/*        ___
 *   ___ / _ \
 *  / _ ' (_) |       __  _
 * | (_) ,___/       / _|(_)
 *  \___/  _  _ __  | |_  _  _ __    ___  _ __  __ _
 *        | || '_ \ |  _|| || '_ \  / _ \| '__|/ _` |
 *        | || | | || |  | || | | ||  __/| |  | (_| |
 *        |_||_| |_||_|  |_||_| |_| \___||_|   \__,_|
 *
 * xrobj_hdr_defs.h  -  Calibration & Persistent Archive Record Definition
 *
 * Copyright (C) 2021, 2022, 2023,2024 Infinera. All rights reserved.
 *
 * Infinera Proprietary and Confidential
 */
#ifndef XROBJ_HDR_DEFS_H_
#define XROBJ_HDR_DEFS_H_

#include "amcu_mem_defs.h"

#include <stdint.h>

#define OBJNAME_MAX_SIZE 20

/*
 * Index limits
 */
#define INDEX_ALL_DIFFDATA 0xAB7C
#define ALIGN_STRUCT (2)
typedef enum
{
    WARM,
    LKG,
    ARCH_CNT
} ArchiveIndex;

#define WARM_ARCHIVE_NAME "WARM"
#define LKG_ARCHIVE_NAME "LKG"

#define CALRECORDHEADERLEN 8
/*CALBRD_NAME needs to be exactly CALRECORDHEADERLEN(8) characters long*/
#define CALBRD_NAME "INFNCALB"

/*The valid object name is based off ascii characters limited to
 * [0(0x30)-9(0x39)], [A(0x41)-Z(0x5A)] & _(0x5F). We use this magic number and
 * substract this against the object name. */
#define CAL_ENCRYPT_MAGIC_NUM (0x2F)

/*Calibration version where the data is unencrypted and peeking into the raw
 * binary doesn't yield any meaningful ASCII value*/
#define CAL_UNECRYTPTED_VERSION (1)

#define CAL_VERSION (2)
#define WARM_VERSION (1)
#define LKG_VERSION (1)

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*
 * This structure defines the header for one entry in a Calibration or
 * Persistent Archive Record.
 */
typedef struct xrobjbitstreamhdr
{
    uint8_t xtraSize; /* header size plus any rounding padding */
    char objectName[OBJNAME_MAX_SIZE];
    uint8_t parmNum;
    uint8_t padding[ALIGN_STRUCT];
    uint16_t recType;
    uint16_t startIndex;
    uint32_t recSize;
#define LKG_STD_PARM 0x01    /* lkg from Parameters */
#define WARM_STD_PARM 0x11   /* warm from Parameters */
#define WARM_PIC_PARM 0x12   /* warm from PicData setpoint */
#define WARM_DAC_PARM 0x13   /* warm from DacData setpoint */
#define WARM_ARY_PARM 0x14   /* warm from array */
#define WARM_STD_PARM16 0x15 /* warm from 16-bit parameter */
} xrobjbitstreamhdr;

typedef struct archivehdr
{
    uint32_t size;       /* count of bytes following header */
    uint32_t crc32;      /* CRC protects data following */
    uint32_t version;    /* standard form (modified from older 16 bits) */
    uint32_t generation; /* generation for archives */
} archivehdr_t;
#define PERSISTENT_ARCHIVE_HEADER_SIZE (sizeof(archivehdr_t) / sizeof(uint8_t))

#define BACKUP_BREG0 (0x40271000)
#define BACKUP_BREG1 (0x40271004)
typedef enum
{
    RESET_OR_PWRCYCLE = 0, // Powercycle/RESET
    BOOT_COLD         = 1, // Match TR_INIT
    BOOT_WARM         = 2  // Match TR_INIT
} BootTypes_t;

#define RESET_OR_PWRCYCLE_STR "reset/powercycle"
#define BOOT_COLD_STR "coldboot"
#define BOOT_WARM_STR "warmboot"
typedef union boot_info {
    uint32_t value;
    struct
    {
        uint32_t boot : 2U;
        uint32_t pboot : 2U;
        uint32_t wb_count : 14U;
        uint32_t cb_count : 14U;
    } BITS;
} boot_info_t;

#define DEFAULTNEWRECORD "NRC"
#define DEFAULTENDRECORD "END"
#define CONTINUEEXISTINGRECORD "ERC"
#define SIZE_OF(s, m) (sizeof(((s *)0)->m))
#define XROBJ_BITSTREAM_HDR_NOOBJNAME_SIZE                                     \
    (((sizeof(xrobjbitstreamhdr)) / (sizeof(uint8_t))) -                       \
     (SIZE_OF(xrobjbitstreamhdr, objectName)))
#endif /* XROBJ_HDR_DEFS_H_ */
