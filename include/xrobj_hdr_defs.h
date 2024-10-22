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
#endif /* XROBJ_HDR_DEFS_H_ */
