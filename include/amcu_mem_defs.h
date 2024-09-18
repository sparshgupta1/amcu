/**       ___
 *   ___ / _ \
 *  / _ ' (_) |       __  _
 * | (_) ,___/       / _|(_)
 *  \___/  _  _ __  | |_  _  _ __    ___  _ __  __ _
 *        | || '_ \ |  _|| || '_ \  / _ \| '__|/ _` |
 *        | || | | || |  | || | | ||  __/| |  | (_| |
 *        |_||_| |_||_|  |_||_| |_| \___||_|   \__,_|
 *
 * @file amumem_defs.h            @brief AMCU Memory definitions
 *
 * @author Sparsh Gupta
 *
 * @copyright 2023,2024 Infinera. All rights reserved.
 * Infinera Proprietary and Confidential.
 *
 * @details
 * Macros list the sizes for defined memory sapces in flash and RAM, it's used
 * linker script source to resolve the named constants
 *
 ***************************************************************************/
#ifndef AMCUMEMDEFS_H
#define AMCUMEMDEFS_H

#define FLASH_A_BASE_ADDR (0x10005000)
#define FLASH_SIZE (0x200000)
#define FW_IMAGE_PARTITION_SIZE (FLASH_SIZE / 2)
#define FLASH_B_BASE_ADDR (FLASH_A_BASE_ADDR + FW_IMAGE_PARTITION_SIZE)
#define FLASH_CM0P_APP_OFFSET 0x400
//#define FLASH_CM0P_SIZE 0x20000  //(When trying with PIC)
#define FLASH_CM0P_SIZE 0x2000 //(OG size)

#define FLASH_CALCMN_OFFSET (0xDB800)
#define FLASH_CALBRD_OFFSET (0xE0000)

#endif /* AMCUMEMDEFS_H */
