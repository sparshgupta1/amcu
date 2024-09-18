/**************************************************************************//**
 * @file     startup_ARMCM0plus.S
 * @brief    CMSIS-Core(M) Device Startup File for Cortex-M0plus Device
 * @version  V2.2.0
 * @date     26. May 2021
 ******************************************************************************/
/*
 * Copyright (c) 2009-2021 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
                #define CY_NMI_HANLDER_ADDR         0x0000000D

                #define CY_CPU_VTOR_ADDR            0xE000ED08

                .syntax  unified
                .arch    armv6-m

                .section .vectors
                .align   2
                .globl   __Vectors
                .globl   __Vectors_End
                .globl   __Vectors_Size
__Vectors:
                .long    __StackTop                         /*     Top of Stack */
                .long    Reset_Handler                      /*     Reset Handler */
                .long    CY_NMI_HANLDER_ADDR                /* -14 NMI Handler */
                .long    HardFault_Handler                  /* -13 Hard Fault Handler */
                .long    0                                  /*     Reserved */
                .long    0                                  /*     Reserved */
                .long    0                                  /*     Reserved */
                .long    0                                  /*     Reserved */
                .long    0                                  /*     Reserved */
                .long    0                                  /*     Reserved */
                .long    0                                  /*     Reserved */
                .long    SVC_Handler                        /*  -5 SVCall Handler */
                .long    0                                  /*     Reserved */
                .long    0                                  /*     Reserved */
                .long    PendSV_Handler                     /*  -2 PendSV Handler */
                .long    SysTick_Handler                    /*  -1 SysTick Handler */

                /* Interrupts */
                .long    NvicMux0_IRQHandler                     /* CPU User Interrupt #0 */
                .long    NvicMux1_IRQHandler                     /* CPU User Interrupt #1 */
                .long    NvicMux2_IRQHandler                     /* CPU User Interrupt #2 */
                .long    NvicMux3_IRQHandler                     /* CPU User Interrupt #3 */
                .long    NvicMux4_IRQHandler                     /* CPU User Interrupt #4 */
                .long    NvicMux5_IRQHandler                     /* CPU User Interrupt #5 */
                .long    NvicMux6_IRQHandler                     /* CPU User Interrupt #6 */
                .long    NvicMux7_IRQHandler                     /* CPU User Interrupt #7 */
                .long    Internal0_IRQHandler                    /* Internal SW Interrupt #0 */
                .long    Internal1_IRQHandler                    /* Internal SW Interrupt #1 */
                .long    Internal2_IRQHandler                    /* Internal SW Interrupt #2 */
                .long    Internal3_IRQHandler                    /* Internal SW Interrupt #3 */
                .long    Internal4_IRQHandler                    /* Internal SW Interrupt #4 */
                .long    Internal5_IRQHandler                    /* Internal SW Interrupt #5 */
                .long    Internal6_IRQHandler                    /* Internal SW Interrupt #6 */
                .long    Internal7_IRQHandler                    /* Internal SW Interrupt #7 */
                .size    __Vectors, . - __Vectors
                .equ    __VectorsSize, . - __Vectors

            .section .ram_vectors
            .align 2
            .globl __ramVectors
        __ramVectors:
            .space  __VectorsSize
            .size   __ramVectors, . - __ramVectors

                .thumb
                .section .text
                .align   2

                .thumb_func
                .type    Reset_Handler, %function
                .globl   Reset_Handler
                .fnstart
Reset_Handler:
                ldr      r4, =__copy_table_start__
                ldr      r5, =__copy_table_end__

.L_loop0:
                cmp      r4, r5
                bge      .L_loop0_done
                ldr      r1, [r4]                /* source address */
                ldr      r2, [r4, #4]            /* destination address */
                ldr      r3, [r4, #8]            /* word count */
                lsls     r3, r3, #2              /* byte count */

.L_loop0_0:
                subs     r3, #4                  /* decrement byte count */
                blt      .L_loop0_0_done
                ldr      r0, [r1, r3]
                str      r0, [r2, r3]
                b        .L_loop0_0

.L_loop0_0_done:
                adds     r4, #12
                b        .L_loop0

.L_loop0_done:

                ldr      r3, =__zero_table_start__
                ldr      r4, =__zero_table_end__

.L_loop2:
                cmp      r3, r4
                bge      .L_loop2_done
                ldr      r1, [r3]                /* destination address */
                ldr      r2, [r3, #4]            /* word count */
                lsls     r2, r2, #2              /* byte count */
                movs     r0, 0

.L_loop2_0:
                subs     r2, #4                  /* decrement byte count */
                blt      .L_loop2_0_done
                str      r0, [r1, r2]
                b        .L_loop2_0
.L_loop2_0_done:

                adds     r3, #8
                b        .L_loop2
.L_loop2_done:
                /* Update Vector Table Offset Register. */
                ldr r0, =__ramVectors
                ldr r1, =CY_CPU_VTOR_ADDR
                str r0, [r1]
                dsb 0xF

                bl       SystemInit

                bl       main

                .fnend
                .size    Reset_Handler, . - Reset_Handler

        .thumb_func
        .weak    Default_Handler
        .type    Default_Handler, %function
Default_Handler:
        b        .
        .size    Default_Handler, . - Default_Handler
        .weak    Cy_SysLib_FaultHandler
        .type    Cy_SysLib_FaultHandler, %function

Cy_SysLib_FaultHandler:
    b    .
    .size    Cy_SysLib_FaultHandler, . - Cy_SysLib_FaultHandler
    .type Fault_Handler, %function

Fault_Handler:
    /* Storing LR content for Creator call stack trace */
    push {LR}
    movs r0, #4
    mov r1, LR
    tst r0, r1
    beq .L_MSP
    mrs r0, PSP
    b .L_API_call
.L_MSP:
    mrs r0, MSP
    /* Compensation of stack pointer address due to pushing 4 bytes of LR */
    adds r0, r0, #4
.L_API_call:
    bl Cy_SysLib_FaultHandler
    b   .
    .size    Fault_Handler, . - Fault_Handler

.macro    def_fault_Handler    fault_handler_name
    .weak    \fault_handler_name
    .set    \fault_handler_name, Fault_Handler
    .endm

/* Macro to define default exception/interrupt handlers.
 * Default handler are weak symbols with an endless loop.
 * They can be overwritten by real handlers.
 */
                .macro   Set_Default_Handler  Handler_Name
                .weak    \Handler_Name
                .set     \Handler_Name, Default_Handler
                .endm


/* Default exception/interrupt handler */

                Set_Default_Handler  NMI_Handler
                def_fault_Handler    HardFault_Handler
                Set_Default_Handler  SVC_Handler
                Set_Default_Handler  PendSV_Handler
                Set_Default_Handler  SysTick_Handler

                Set_Default_Handler  NvicMux0_IRQHandler                     /* CPU User Interrupt #0 */
                Set_Default_Handler  NvicMux1_IRQHandler                     /* CPU User Interrupt #1 */
                Set_Default_Handler  NvicMux2_IRQHandler                     /* CPU User Interrupt #2 */
                Set_Default_Handler  NvicMux3_IRQHandler                     /* CPU User Interrupt #3 */
                Set_Default_Handler  NvicMux4_IRQHandler                     /* CPU User Interrupt #4 */
                Set_Default_Handler  NvicMux5_IRQHandler                     /* CPU User Interrupt #5 */
                Set_Default_Handler  NvicMux6_IRQHandler                     /* CPU User Interrupt #6 */
                Set_Default_Handler  NvicMux7_IRQHandler                     /* CPU User Interrupt #7 */
                Set_Default_Handler  Internal0_IRQHandler                    /* Internal SW Interrupt #0 */
                Set_Default_Handler  Internal1_IRQHandler                    /* Internal SW Interrupt #1 */
                Set_Default_Handler  Internal2_IRQHandler                    /* Internal SW Interrupt #2 */
                Set_Default_Handler  Internal3_IRQHandler                    /* Internal SW Interrupt #3 */
                Set_Default_Handler  Internal4_IRQHandler                    /* Internal SW Interrupt #4 */
                Set_Default_Handler  Internal5_IRQHandler                    /* Internal SW Interrupt #5 */
                Set_Default_Handler  Internal6_IRQHandler                    /* Internal SW Interrupt #6 */
                Set_Default_Handler  Internal7_IRQHandler                    /* Internal SW Interrupt #7 */


                .end