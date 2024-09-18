/********************************************************************************
 * File Name:   main.c
 *
 * Description: This is the source code for CM0+ in the the Dual CPU IPC
 *Semaphore Application for ModusToolbox.
 *
 * Related Document: See README.md
 *
 *
 *********************************************************************************
 * Copyright 2020-2021 Cypress Semiconductor Corporation (an Infineon
 *company) or an affiliate of Cypress Semiconductor Corporation.  All rights
 *reserved.
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
 *********************************************************************************/

/*        ___
 *   ___ / _ \
 *  / _ ' (_) |       __  _
 * | (_) ,___/       / _|(_)
 *  \___/  _  _ __  | |_  _  _ __    ___  _ __  __ _
 *        | || '_ \ |  _|| || '_ \  / _ \| '__|/ _` |
 *        | || | | || |  | || | | ||  __/| |  | (_| |
 *        |_||_| |_||_|  |_||_| |_| \___||_|   \__,_|
 *
 * main.c      AMCU Application
 *
 * Copyright (C) 2024 Infinera. All rights reserved.
 * Written by Infinera, Corp.
 *
 * Infinera Proprietary and Confidential
 *
 */
#include "cy_pdl.h"
#include "cy_result.h"
#include "cybsp.h"
// #include "cyw20829a0kml.h"

#include <stdlib.h>

#define CM0_VTABLE_REG_ADDR (0x40201120)

#define BOOT_CM0P_SIZE (0x4000)
#define BOOT_CM0P_START (0x10000400)

#define CPUSS_CM0_CTL_REG_ADDR (0x40201000)
#define CPUSS_CM0_CTL_REG_VAL (0x05fa0000)

#define M4_START_ADDRESS (0x10002000)

#define BREG0 (0x40271000)

// static uint32_t randomData1[0x10] CY_SECTION(".cy_ram_tst1");

/* MAIN FUNCTION */

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

int main(void)
{
    // TEST TO SEE IF CM0p STARTED
    volatile uint32_t *tst   = (uint32_t *)(0x080FF514);
    volatile uint32_t *pc = (uint32_t *)(0x080FF510);
    *tst = 0xDEADA0DE;
    
    *pc = (uint32_t)get_pc();
    // uint8_t imageAddress = fw_upg_get_running_image();

    // if (imageAddress == 0xA)
    // {
    //     Cy_SysEnableCM4(0x10007000);
    // }else if (imageAddress == 0xB){
    //     Cy_SysEnableCM4(0x10107000);
    // }else{
    //     *tst = 0x1A2B3C4D;
    // }
    for (;;)
    {
    }
    /* never get here */
}

/* [] END OF FILE */