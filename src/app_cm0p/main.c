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

#define CM0_VTABLE_REG_ADDR (0x40201120)

#define BOOT_CM0P_SIZE (0x4000)
#define BOOT_CM0P_START (0x10000400)

#define CPUSS_CM0_CTL_REG_ADDR (0x40201000)
#define CPUSS_CM0_CTL_REG_VAL (0x05fa0000)

#define M4_START_ADDRESS (0x10000000 + 0x2000)

#define BREG0 (0x40271000)

// static uint32_t randomData1[0x10] CY_SECTION(".cy_ram_tst1");

typedef void (*func_ptr_t)(void);
void vDeInitAndJumpToMainFirmware(func_ptr_t vPlatformSpecificDeinit,
                                  uint32_t u32FirmwareOffset,
                                  uint32_t u32FirmwareAddress)
{
    uint32_t u32FirmwareStackPointerAddress = 0;
    uint32_t u32FirmwareResetHandlerAddress = 0;
    volatile uint32_t *d                    = (uint32_t *)(0x080FF54C);
    // volatile uint32_t *tst3                 = (uint32_t *)(0x080FF534);
    // volatile uint32_t *tst1                 = (uint32_t *)(0x080FF500);
    uint32_t *pu32FwFlashPointer  = (uint32_t *)u32FirmwareAddress;
    uint32_t u32RegistersChecksum = 0;

    // Read 4 first bytes from FW, the stack pointer
    u32FirmwareStackPointerAddress = *pu32FwFlashPointer;
    //*tst1                          = u32FirmwareStackPointerAddress;
    // Read 4 next bytes from FW, reset handler address
    pu32FwFlashPointer++;
    u32FirmwareResetHandlerAddress = *pu32FwFlashPointer;
    // Patch it with offset
    u32FirmwareResetHandlerAddress += u32FirmwareOffset;
    *d = u32FirmwareResetHandlerAddress;

    // Calculate simple checksum of the registers to be passed
    u32RegistersChecksum = u32FirmwareAddress ^ u32FirmwareOffset;
    //*tst3                = 0xB1;
    if (vPlatformSpecificDeinit != NULL)
    {
        vPlatformSpecificDeinit();
    }
    //*tst3 = 0xB2;
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
    //*tst3 = 0xB4;
    // Store firmware offset to r11 (hoop in case we have limited Cortex-M0)
    asm("ldr r6, %0; mov r11, r6;" : "=m"(u32FirmwareOffset) : :);
    //*tst3 = 0xB5;
    // Store registers checksum to r12 (hoop in case we have limited Cortex-M0)
    asm("ldr r6, %0; mov r12, r6;" : "=m"(u32RegistersChecksum) : :);
    //*tst3                = 0x1111B6;
    volatile uint32_t *a = (uint32_t *)(0x080FF540);
    volatile uint32_t *b = (uint32_t *)(0x080FF544);
    volatile uint32_t *c = (uint32_t *)(0x080FF548);

    *a = u32FirmwareAddress; // u32FirmwareAddress;
    *b = u32FirmwareOffset;  // u32FirmwareOffset;
    *c = u32RegistersChecksum;

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

/* MAIN FUNCTION */
int main(void)
{
    // TEST TO SEE IF CM0p STARTED
    volatile uint32_t *tst = (uint32_t *)(0x080FF514);
    *tst                   = 0xF0032013;

    Cy_SysEnableCM4(M4_START_ADDRESS);

    // to show that enable cm4 function was passed
    // randomData1[1] = 0xBEEFC0DE;

    uint32_t u32JumpAddress = 0x10109400;
    vDeInitAndJumpToMainFirmware(&vL432kc_DeInit, 0x109000, u32JumpAddress);
    for (;;)
    {
    }
    /* never get here */
}

/* [] END OF FILE */