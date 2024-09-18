/**************************************************************************//**
 * @file     startup_ARMCM4.S
 * @brief    CMSIS-Core(M) Device Startup File for Cortex-M4 Device
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

            /* Address of the NMI handler */
            #define CY_NMI_HANLDER_ADDR         0x0000000D
            
                .syntax  unified
                .arch    armv7e-m
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
                .long    MemManage_Handler                  /* -12 MPU Fault Handler */
                .long    BusFault_Handler                   /* -11 Bus Fault Handler */
                .long    UsageFault_Handler                 /* -10 Usage Fault Handler */
                .long    0                                  /*     Reserved */
                .long    0                                  /*     Reserved */
                .long    0                                  /*     Reserved */
                .long    0                                  /*     Reserved */
                .long    SVC_Handler                        /*  -5 SVC Handler */
                .long    DebugMon_Handler                   /*  -4 Debug Monitor Handler */
                .long    0                                  /*     Reserved */
                .long    PendSV_Handler                     /*  -2 PendSV Handler */
                .long    SysTick_Handler                    /*  -1 SysTick Handler */

                /* External interrupts                             Description */
                .long    ioss_interrupts_gpio_0_IRQHandler       /* GPIO Port Interrupt #0 */
                .long    ioss_interrupts_gpio_1_IRQHandler       /* GPIO Port Interrupt #1 */
                .long    ioss_interrupts_gpio_2_IRQHandler       /* GPIO Port Interrupt #2 */
                .long    ioss_interrupts_gpio_3_IRQHandler       /* GPIO Port Interrupt #3 */
                .long    ioss_interrupts_gpio_4_IRQHandler       /* GPIO Port Interrupt #4 */
                .long    ioss_interrupts_gpio_5_IRQHandler       /* GPIO Port Interrupt #5 */
                .long    ioss_interrupts_gpio_6_IRQHandler       /* GPIO Port Interrupt #6 */
                .long    ioss_interrupts_gpio_7_IRQHandler       /* GPIO Port Interrupt #7 */
                .long    ioss_interrupts_gpio_8_IRQHandler       /* GPIO Port Interrupt #8 */
                .long    ioss_interrupts_gpio_9_IRQHandler       /* GPIO Port Interrupt #9 */
                .long    ioss_interrupts_gpio_10_IRQHandler      /* GPIO Port Interrupt #10 */
                .long    ioss_interrupts_gpio_11_IRQHandler      /* GPIO Port Interrupt #11 */
                .long    ioss_interrupts_gpio_12_IRQHandler      /* GPIO Port Interrupt #12 */
                .long    ioss_interrupts_gpio_13_IRQHandler      /* GPIO Port Interrupt #13 */
                .long    ioss_interrupts_gpio_14_IRQHandler      /* GPIO Port Interrupt #14 */
                .long    ioss_interrupt_gpio_IRQHandler          /* GPIO All Ports */
                .long    ioss_interrupt_vdd_IRQHandler           /* GPIO Supply Detect Interrupt */
                .long    lpcomp_interrupt_IRQHandler             /* Low Power Comparator Interrupt */
                .long    scb_8_interrupt_IRQHandler              /* Serial Communication Block #8 (DeepSleep capable) */
                .long    srss_interrupt_mcwdt_0_IRQHandler       /* Multi Counter Watchdog Timer interrupt */
                .long    srss_interrupt_mcwdt_1_IRQHandler       /* Multi Counter Watchdog Timer interrupt */
                .long    srss_interrupt_backup_IRQHandler        /* Backup domain interrupt */
                .long    srss_interrupt_IRQHandler               /* Other combined Interrupts for SRSS (LVD, WDT, CLKCAL) */
                .long    cpuss_interrupts_ipc_0_IRQHandler       /* CPUSS Inter Process Communication Interrupt #0 */
                .long    cpuss_interrupts_ipc_1_IRQHandler       /* CPUSS Inter Process Communication Interrupt #1 */
                .long    cpuss_interrupts_ipc_2_IRQHandler       /* CPUSS Inter Process Communication Interrupt #2 */
                .long    cpuss_interrupts_ipc_3_IRQHandler       /* CPUSS Inter Process Communication Interrupt #3 */
                .long    cpuss_interrupts_ipc_4_IRQHandler       /* CPUSS Inter Process Communication Interrupt #4 */
                .long    cpuss_interrupts_ipc_5_IRQHandler       /* CPUSS Inter Process Communication Interrupt #5 */
                .long    cpuss_interrupts_ipc_6_IRQHandler       /* CPUSS Inter Process Communication Interrupt #6 */
                .long    cpuss_interrupts_ipc_7_IRQHandler       /* CPUSS Inter Process Communication Interrupt #7 */
                .long    cpuss_interrupts_ipc_8_IRQHandler       /* CPUSS Inter Process Communication Interrupt #8 */
                .long    cpuss_interrupts_ipc_9_IRQHandler       /* CPUSS Inter Process Communication Interrupt #9 */
                .long    cpuss_interrupts_ipc_10_IRQHandler      /* CPUSS Inter Process Communication Interrupt #10 */
                .long    cpuss_interrupts_ipc_11_IRQHandler      /* CPUSS Inter Process Communication Interrupt #11 */
                .long    cpuss_interrupts_ipc_12_IRQHandler      /* CPUSS Inter Process Communication Interrupt #12 */
                .long    cpuss_interrupts_ipc_13_IRQHandler      /* CPUSS Inter Process Communication Interrupt #13 */
                .long    cpuss_interrupts_ipc_14_IRQHandler      /* CPUSS Inter Process Communication Interrupt #14 */
                .long    cpuss_interrupts_ipc_15_IRQHandler      /* CPUSS Inter Process Communication Interrupt #15 */
                .long    scb_0_interrupt_IRQHandler              /* Serial Communication Block #0 */
                .long    scb_1_interrupt_IRQHandler              /* Serial Communication Block #1 */
                .long    scb_2_interrupt_IRQHandler              /* Serial Communication Block #2 */
                .long    scb_3_interrupt_IRQHandler              /* Serial Communication Block #3 */
                .long    scb_4_interrupt_IRQHandler              /* Serial Communication Block #4 */
                .long    scb_5_interrupt_IRQHandler              /* Serial Communication Block #5 */
                .long    scb_6_interrupt_IRQHandler              /* Serial Communication Block #6 */
                .long    scb_7_interrupt_IRQHandler              /* Serial Communication Block #7 */
                .long    scb_9_interrupt_IRQHandler              /* Serial Communication Block #9 */
                .long    scb_10_interrupt_IRQHandler             /* Serial Communication Block #10 */
                .long    scb_11_interrupt_IRQHandler             /* Serial Communication Block #11 */
                .long    scb_12_interrupt_IRQHandler             /* Serial Communication Block #12 */
                .long    csd_interrupt_IRQHandler                /* CSD (Capsense) interrupt */
                .long    cpuss_interrupts_dmac_0_IRQHandler      /* CPUSS DMAC, Channel #0 */
                .long    cpuss_interrupts_dmac_1_IRQHandler      /* CPUSS DMAC, Channel #1 */
                .long    cpuss_interrupts_dmac_2_IRQHandler      /* CPUSS DMAC, Channel #2 */
                .long    cpuss_interrupts_dmac_3_IRQHandler      /* CPUSS DMAC, Channel #3 */
                .long    cpuss_interrupts_dw0_0_IRQHandler       /* CPUSS DataWire #0, Channel #0 */
                .long    cpuss_interrupts_dw0_1_IRQHandler       /* CPUSS DataWire #0, Channel #1 */
                .long    cpuss_interrupts_dw0_2_IRQHandler       /* CPUSS DataWire #0, Channel #2 */
                .long    cpuss_interrupts_dw0_3_IRQHandler       /* CPUSS DataWire #0, Channel #3 */
                .long    cpuss_interrupts_dw0_4_IRQHandler       /* CPUSS DataWire #0, Channel #4 */
                .long    cpuss_interrupts_dw0_5_IRQHandler       /* CPUSS DataWire #0, Channel #5 */
                .long    cpuss_interrupts_dw0_6_IRQHandler       /* CPUSS DataWire #0, Channel #6 */
                .long    cpuss_interrupts_dw0_7_IRQHandler       /* CPUSS DataWire #0, Channel #7 */
                .long    cpuss_interrupts_dw0_8_IRQHandler       /* CPUSS DataWire #0, Channel #8 */
                .long    cpuss_interrupts_dw0_9_IRQHandler       /* CPUSS DataWire #0, Channel #9 */
                .long    cpuss_interrupts_dw0_10_IRQHandler      /* CPUSS DataWire #0, Channel #10 */
                .long    cpuss_interrupts_dw0_11_IRQHandler      /* CPUSS DataWire #0, Channel #11 */
                .long    cpuss_interrupts_dw0_12_IRQHandler      /* CPUSS DataWire #0, Channel #12 */
                .long    cpuss_interrupts_dw0_13_IRQHandler      /* CPUSS DataWire #0, Channel #13 */
                .long    cpuss_interrupts_dw0_14_IRQHandler      /* CPUSS DataWire #0, Channel #14 */
                .long    cpuss_interrupts_dw0_15_IRQHandler      /* CPUSS DataWire #0, Channel #15 */
                .long    cpuss_interrupts_dw0_16_IRQHandler      /* CPUSS DataWire #0, Channel #16 */
                .long    cpuss_interrupts_dw0_17_IRQHandler      /* CPUSS DataWire #0, Channel #17 */
                .long    cpuss_interrupts_dw0_18_IRQHandler      /* CPUSS DataWire #0, Channel #18 */
                .long    cpuss_interrupts_dw0_19_IRQHandler      /* CPUSS DataWire #0, Channel #19 */
                .long    cpuss_interrupts_dw0_20_IRQHandler      /* CPUSS DataWire #0, Channel #20 */
                .long    cpuss_interrupts_dw0_21_IRQHandler      /* CPUSS DataWire #0, Channel #21 */
                .long    cpuss_interrupts_dw0_22_IRQHandler      /* CPUSS DataWire #0, Channel #22 */
                .long    cpuss_interrupts_dw0_23_IRQHandler      /* CPUSS DataWire #0, Channel #23 */
                .long    cpuss_interrupts_dw0_24_IRQHandler      /* CPUSS DataWire #0, Channel #24 */
                .long    cpuss_interrupts_dw0_25_IRQHandler      /* CPUSS DataWire #0, Channel #25 */
                .long    cpuss_interrupts_dw0_26_IRQHandler      /* CPUSS DataWire #0, Channel #26 */
                .long    cpuss_interrupts_dw0_27_IRQHandler      /* CPUSS DataWire #0, Channel #27 */
                .long    cpuss_interrupts_dw0_28_IRQHandler      /* CPUSS DataWire #0, Channel #28 */
                .long    cpuss_interrupts_dw1_0_IRQHandler       /* CPUSS DataWire #1, Channel #0 */
                .long    cpuss_interrupts_dw1_1_IRQHandler       /* CPUSS DataWire #1, Channel #1 */
                .long    cpuss_interrupts_dw1_2_IRQHandler       /* CPUSS DataWire #1, Channel #2 */
                .long    cpuss_interrupts_dw1_3_IRQHandler       /* CPUSS DataWire #1, Channel #3 */
                .long    cpuss_interrupts_dw1_4_IRQHandler       /* CPUSS DataWire #1, Channel #4 */
                .long    cpuss_interrupts_dw1_5_IRQHandler       /* CPUSS DataWire #1, Channel #5 */
                .long    cpuss_interrupts_dw1_6_IRQHandler       /* CPUSS DataWire #1, Channel #6 */
                .long    cpuss_interrupts_dw1_7_IRQHandler       /* CPUSS DataWire #1, Channel #7 */
                .long    cpuss_interrupts_dw1_8_IRQHandler       /* CPUSS DataWire #1, Channel #8 */
                .long    cpuss_interrupts_dw1_9_IRQHandler       /* CPUSS DataWire #1, Channel #9 */
                .long    cpuss_interrupts_dw1_10_IRQHandler      /* CPUSS DataWire #1, Channel #10 */
                .long    cpuss_interrupts_dw1_11_IRQHandler      /* CPUSS DataWire #1, Channel #11 */
                .long    cpuss_interrupts_dw1_12_IRQHandler      /* CPUSS DataWire #1, Channel #12 */
                .long    cpuss_interrupts_dw1_13_IRQHandler      /* CPUSS DataWire #1, Channel #13 */
                .long    cpuss_interrupts_dw1_14_IRQHandler      /* CPUSS DataWire #1, Channel #14 */
                .long    cpuss_interrupts_dw1_15_IRQHandler      /* CPUSS DataWire #1, Channel #15 */
                .long    cpuss_interrupts_dw1_16_IRQHandler      /* CPUSS DataWire #1, Channel #16 */
                .long    cpuss_interrupts_dw1_17_IRQHandler      /* CPUSS DataWire #1, Channel #17 */
                .long    cpuss_interrupts_dw1_18_IRQHandler      /* CPUSS DataWire #1, Channel #18 */
                .long    cpuss_interrupts_dw1_19_IRQHandler      /* CPUSS DataWire #1, Channel #19 */
                .long    cpuss_interrupts_dw1_20_IRQHandler      /* CPUSS DataWire #1, Channel #20 */
                .long    cpuss_interrupts_dw1_21_IRQHandler      /* CPUSS DataWire #1, Channel #21 */
                .long    cpuss_interrupts_dw1_22_IRQHandler      /* CPUSS DataWire #1, Channel #22 */
                .long    cpuss_interrupts_dw1_23_IRQHandler      /* CPUSS DataWire #1, Channel #23 */
                .long    cpuss_interrupts_dw1_24_IRQHandler      /* CPUSS DataWire #1, Channel #24 */
                .long    cpuss_interrupts_dw1_25_IRQHandler      /* CPUSS DataWire #1, Channel #25 */
                .long    cpuss_interrupts_dw1_26_IRQHandler      /* CPUSS DataWire #1, Channel #26 */
                .long    cpuss_interrupts_dw1_27_IRQHandler      /* CPUSS DataWire #1, Channel #27 */
                .long    cpuss_interrupts_dw1_28_IRQHandler      /* CPUSS DataWire #1, Channel #28 */
                .long    cpuss_interrupts_fault_0_IRQHandler     /* CPUSS Fault Structure Interrupt #0 */
                .long    cpuss_interrupts_fault_1_IRQHandler     /* CPUSS Fault Structure Interrupt #1 */
                .long    cpuss_interrupt_crypto_IRQHandler       /* CRYPTO Accelerator Interrupt */
                .long    cpuss_interrupt_fm_IRQHandler           /* FLASH Macro Interrupt */
                .long    cpuss_interrupts_cm4_fp_IRQHandler      /* Floating Point operation fault */
                .long    cpuss_interrupts_cm0_cti_0_IRQHandler   /* CM0+ CTI #0 */
                .long    cpuss_interrupts_cm0_cti_1_IRQHandler   /* CM0+ CTI #1 */
                .long    cpuss_interrupts_cm4_cti_0_IRQHandler   /* CM4 CTI #0 */
                .long    cpuss_interrupts_cm4_cti_1_IRQHandler   /* CM4 CTI #1 */
                .long    tcpwm_0_interrupts_0_IRQHandler         /* TCPWM #0, Counter #0 */
                .long    tcpwm_0_interrupts_1_IRQHandler         /* TCPWM #0, Counter #1 */
                .long    tcpwm_0_interrupts_2_IRQHandler         /* TCPWM #0, Counter #2 */
                .long    tcpwm_0_interrupts_3_IRQHandler         /* TCPWM #0, Counter #3 */
                .long    tcpwm_0_interrupts_4_IRQHandler         /* TCPWM #0, Counter #4 */
                .long    tcpwm_0_interrupts_5_IRQHandler         /* TCPWM #0, Counter #5 */
                .long    tcpwm_0_interrupts_6_IRQHandler         /* TCPWM #0, Counter #6 */
                .long    tcpwm_0_interrupts_7_IRQHandler         /* TCPWM #0, Counter #7 */
                .long    tcpwm_1_interrupts_0_IRQHandler         /* TCPWM #1, Counter #0 */
                .long    tcpwm_1_interrupts_1_IRQHandler         /* TCPWM #1, Counter #1 */
                .long    tcpwm_1_interrupts_2_IRQHandler         /* TCPWM #1, Counter #2 */
                .long    tcpwm_1_interrupts_3_IRQHandler         /* TCPWM #1, Counter #3 */
                .long    tcpwm_1_interrupts_4_IRQHandler         /* TCPWM #1, Counter #4 */
                .long    tcpwm_1_interrupts_5_IRQHandler         /* TCPWM #1, Counter #5 */
                .long    tcpwm_1_interrupts_6_IRQHandler         /* TCPWM #1, Counter #6 */
                .long    tcpwm_1_interrupts_7_IRQHandler         /* TCPWM #1, Counter #7 */
                .long    tcpwm_1_interrupts_8_IRQHandler         /* TCPWM #1, Counter #8 */
                .long    tcpwm_1_interrupts_9_IRQHandler         /* TCPWM #1, Counter #9 */
                .long    tcpwm_1_interrupts_10_IRQHandler        /* TCPWM #1, Counter #10 */
                .long    tcpwm_1_interrupts_11_IRQHandler        /* TCPWM #1, Counter #11 */
                .long    tcpwm_1_interrupts_12_IRQHandler        /* TCPWM #1, Counter #12 */
                .long    tcpwm_1_interrupts_13_IRQHandler        /* TCPWM #1, Counter #13 */
                .long    tcpwm_1_interrupts_14_IRQHandler        /* TCPWM #1, Counter #14 */
                .long    tcpwm_1_interrupts_15_IRQHandler        /* TCPWM #1, Counter #15 */
                .long    tcpwm_1_interrupts_16_IRQHandler        /* TCPWM #1, Counter #16 */
                .long    tcpwm_1_interrupts_17_IRQHandler        /* TCPWM #1, Counter #17 */
                .long    tcpwm_1_interrupts_18_IRQHandler        /* TCPWM #1, Counter #18 */
                .long    tcpwm_1_interrupts_19_IRQHandler        /* TCPWM #1, Counter #19 */
                .long    tcpwm_1_interrupts_20_IRQHandler        /* TCPWM #1, Counter #20 */
                .long    tcpwm_1_interrupts_21_IRQHandler        /* TCPWM #1, Counter #21 */
                .long    tcpwm_1_interrupts_22_IRQHandler        /* TCPWM #1, Counter #22 */
                .long    tcpwm_1_interrupts_23_IRQHandler        /* TCPWM #1, Counter #23 */
                .long    pass_interrupt_sar_IRQHandler           /* SAR ADC interrupt */
                .long    audioss_0_interrupt_i2s_IRQHandler      /* I2S0 Audio interrupt */
                .long    audioss_0_interrupt_pdm_IRQHandler      /* PDM0/PCM0 Audio interrupt */
                .long    audioss_1_interrupt_i2s_IRQHandler      /* I2S1 Audio interrupt */
                .long    profile_interrupt_IRQHandler            /* Energy Profiler interrupt */
                .long    smif_interrupt_IRQHandler               /* Serial Memory Interface interrupt */
                .long    usb_interrupt_hi_IRQHandler             /* USB Interrupt */
                .long    usb_interrupt_med_IRQHandler            /* USB Interrupt */
                .long    usb_interrupt_lo_IRQHandler             /* USB Interrupt */
                .long    sdhc_0_interrupt_wakeup_IRQHandler      /* SDIO wakeup interrupt for mxsdhc */
                .long    sdhc_0_interrupt_general_IRQHandler     /* Consolidated interrupt for mxsdhc for everything else */
                .long    sdhc_1_interrupt_wakeup_IRQHandler      /* EEMC wakeup interrupt for mxsdhc, not used */
                .long    sdhc_1_interrupt_general_IRQHandler     /* Consolidated interrupt for mxsdhc for everything else */
                .size    __Vectors, . - __Vectors
                .equ    __VectorsSize, . - __Vectors


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
                ittt     ge
                ldrge    r0, [r1, r3]
                strge    r0, [r2, r3]
                bge      .L_loop0_0

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
                itt      ge
                strge    r0, [r1, r2]
                bge      .L_loop2_0

                adds     r3, #8
                b        .L_loop2
.L_loop2_done:
                /* Enable the FPU if used */
                bl Cy_SystemInitFpuEnable

            #ifndef __NO_SYSTEM_INIT
                bl    SystemInit
            #endif

                /* OS-specific low-level initialization */
                bl    cy_toolchain_init

                /* Call C/C++ static constructors */
                bl    __libc_init_array

                /* Execute main application */
                bl    main

                /* Call C/C++ static destructors */
                bl    __libc_fini_array

                /* Should never get here */
                b   .


                .fnend
                .size    Reset_Handler, . - Reset_Handler

/* The default macro is not used for HardFault_Handler
 * because this results in a poor debug illusion.
 */
                .thumb_func
                .type    HardFault_Handler, %function
                .weak    HardFault_Handler
                .fnstart
HardFault_Handler:
                b        .
                .fnend
                .size    HardFault_Handler, . - HardFault_Handler

                .thumb_func
                .type    Default_Handler, %function
                .weak    Default_Handler
                .fnstart
Default_Handler:
                b        .
                .fnend
                .size    Default_Handler, . - Default_Handler

/* Macro to define default exception/interrupt handlers.
 * Default handler are weak symbols with an endless loop.
 * They can be overwritten by real handlers.
 */
                .macro   Set_Default_Handler  Handler_Name
                .weak    \Handler_Name
                .set     \Handler_Name, Default_Handler
                .endm


/* Default exception/interrupt handler */

                def_irq_handler    NMI_Handler

    def_fault_Handler HardFault_Handler
    def_fault_Handler MemManage_Handler
    def_fault_Handler BusFault_Handler
    def_fault_Handler UsageFault_Handler

    def_irq_handler    SVC_Handler
    def_irq_handler    DebugMon_Handler
    def_irq_handler    PendSV_Handler
    def_irq_handler    SysTick_Handler

    def_irq_handler  ioss_interrupts_gpio_0_IRQHandler       /* GPIO Port Interrupt #0 */
    def_irq_handler  ioss_interrupts_gpio_1_IRQHandler       /* GPIO Port Interrupt #1 */
    def_irq_handler  ioss_interrupts_gpio_2_IRQHandler       /* GPIO Port Interrupt #2 */
    def_irq_handler  ioss_interrupts_gpio_3_IRQHandler       /* GPIO Port Interrupt #3 */
    def_irq_handler  ioss_interrupts_gpio_4_IRQHandler       /* GPIO Port Interrupt #4 */
    def_irq_handler  ioss_interrupts_gpio_5_IRQHandler       /* GPIO Port Interrupt #5 */
    def_irq_handler  ioss_interrupts_gpio_6_IRQHandler       /* GPIO Port Interrupt #6 */
    def_irq_handler  ioss_interrupts_gpio_7_IRQHandler       /* GPIO Port Interrupt #7 */
    def_irq_handler  ioss_interrupts_gpio_8_IRQHandler       /* GPIO Port Interrupt #8 */
    def_irq_handler  ioss_interrupts_gpio_9_IRQHandler       /* GPIO Port Interrupt #9 */
    def_irq_handler  ioss_interrupts_gpio_10_IRQHandler      /* GPIO Port Interrupt #10 */
    def_irq_handler  ioss_interrupts_gpio_11_IRQHandler      /* GPIO Port Interrupt #11 */
    def_irq_handler  ioss_interrupts_gpio_12_IRQHandler      /* GPIO Port Interrupt #12 */
    def_irq_handler  ioss_interrupts_gpio_13_IRQHandler      /* GPIO Port Interrupt #13 */
    def_irq_handler  ioss_interrupts_gpio_14_IRQHandler      /* GPIO Port Interrupt #14 */
    def_irq_handler  ioss_interrupt_gpio_IRQHandler          /* GPIO All Ports */
    def_irq_handler  ioss_interrupt_vdd_IRQHandler           /* GPIO Supply Detect Interrupt */
    def_irq_handler  lpcomp_interrupt_IRQHandler             /* Low Power Comparator Interrupt */
    def_irq_handler  scb_8_interrupt_IRQHandler              /* Serial Communication Block #8 (DeepSleep capable) */
    def_irq_handler  srss_interrupt_mcwdt_0_IRQHandler       /* Multi Counter Watchdog Timer interrupt */
    def_irq_handler  srss_interrupt_mcwdt_1_IRQHandler       /* Multi Counter Watchdog Timer interrupt */
    def_irq_handler  srss_interrupt_backup_IRQHandler        /* Backup domain interrupt */
    def_irq_handler  srss_interrupt_IRQHandler               /* Other combined Interrupts for SRSS (LVD, WDT, CLKCAL) */
    def_irq_handler  cpuss_interrupts_ipc_0_IRQHandler       /* CPUSS Inter Process Communication Interrupt #0 */
    def_irq_handler  cpuss_interrupts_ipc_1_IRQHandler       /* CPUSS Inter Process Communication Interrupt #1 */
    def_irq_handler  cpuss_interrupts_ipc_2_IRQHandler       /* CPUSS Inter Process Communication Interrupt #2 */
    def_irq_handler  cpuss_interrupts_ipc_3_IRQHandler       /* CPUSS Inter Process Communication Interrupt #3 */
    def_irq_handler  cpuss_interrupts_ipc_4_IRQHandler       /* CPUSS Inter Process Communication Interrupt #4 */
    def_irq_handler  cpuss_interrupts_ipc_5_IRQHandler       /* CPUSS Inter Process Communication Interrupt #5 */
    def_irq_handler  cpuss_interrupts_ipc_6_IRQHandler       /* CPUSS Inter Process Communication Interrupt #6 */
    def_irq_handler  cpuss_interrupts_ipc_7_IRQHandler       /* CPUSS Inter Process Communication Interrupt #7 */
    def_irq_handler  cpuss_interrupts_ipc_8_IRQHandler       /* CPUSS Inter Process Communication Interrupt #8 */
    def_irq_handler  cpuss_interrupts_ipc_9_IRQHandler       /* CPUSS Inter Process Communication Interrupt #9 */
    def_irq_handler  cpuss_interrupts_ipc_10_IRQHandler      /* CPUSS Inter Process Communication Interrupt #10 */
    def_irq_handler  cpuss_interrupts_ipc_11_IRQHandler      /* CPUSS Inter Process Communication Interrupt #11 */
    def_irq_handler  cpuss_interrupts_ipc_12_IRQHandler      /* CPUSS Inter Process Communication Interrupt #12 */
    def_irq_handler  cpuss_interrupts_ipc_13_IRQHandler      /* CPUSS Inter Process Communication Interrupt #13 */
    def_irq_handler  cpuss_interrupts_ipc_14_IRQHandler      /* CPUSS Inter Process Communication Interrupt #14 */
    def_irq_handler  cpuss_interrupts_ipc_15_IRQHandler      /* CPUSS Inter Process Communication Interrupt #15 */
    def_irq_handler  scb_0_interrupt_IRQHandler              /* Serial Communication Block #0 */
    def_irq_handler  scb_1_interrupt_IRQHandler              /* Serial Communication Block #1 */
    def_irq_handler  scb_2_interrupt_IRQHandler              /* Serial Communication Block #2 */
    def_irq_handler  scb_3_interrupt_IRQHandler              /* Serial Communication Block #3 */
    def_irq_handler  scb_4_interrupt_IRQHandler              /* Serial Communication Block #4 */
    def_irq_handler  scb_5_interrupt_IRQHandler              /* Serial Communication Block #5 */
    def_irq_handler  scb_6_interrupt_IRQHandler              /* Serial Communication Block #6 */
    def_irq_handler  scb_7_interrupt_IRQHandler              /* Serial Communication Block #7 */
    def_irq_handler  scb_9_interrupt_IRQHandler              /* Serial Communication Block #9 */
    def_irq_handler  scb_10_interrupt_IRQHandler             /* Serial Communication Block #10 */
    def_irq_handler  scb_11_interrupt_IRQHandler             /* Serial Communication Block #11 */
    def_irq_handler  scb_12_interrupt_IRQHandler             /* Serial Communication Block #12 */
    def_irq_handler  csd_interrupt_IRQHandler                /* CSD (Capsense) interrupt */
    def_irq_handler  cpuss_interrupts_dmac_0_IRQHandler      /* CPUSS DMAC, Channel #0 */
    def_irq_handler  cpuss_interrupts_dmac_1_IRQHandler      /* CPUSS DMAC, Channel #1 */
    def_irq_handler  cpuss_interrupts_dmac_2_IRQHandler      /* CPUSS DMAC, Channel #2 */
    def_irq_handler  cpuss_interrupts_dmac_3_IRQHandler      /* CPUSS DMAC, Channel #3 */
    def_irq_handler  cpuss_interrupts_dw0_0_IRQHandler       /* CPUSS DataWire #0, Channel #0 */
    def_irq_handler  cpuss_interrupts_dw0_1_IRQHandler       /* CPUSS DataWire #0, Channel #1 */
    def_irq_handler  cpuss_interrupts_dw0_2_IRQHandler       /* CPUSS DataWire #0, Channel #2 */
    def_irq_handler  cpuss_interrupts_dw0_3_IRQHandler       /* CPUSS DataWire #0, Channel #3 */
    def_irq_handler  cpuss_interrupts_dw0_4_IRQHandler       /* CPUSS DataWire #0, Channel #4 */
    def_irq_handler  cpuss_interrupts_dw0_5_IRQHandler       /* CPUSS DataWire #0, Channel #5 */
    def_irq_handler  cpuss_interrupts_dw0_6_IRQHandler       /* CPUSS DataWire #0, Channel #6 */
    def_irq_handler  cpuss_interrupts_dw0_7_IRQHandler       /* CPUSS DataWire #0, Channel #7 */
    def_irq_handler  cpuss_interrupts_dw0_8_IRQHandler       /* CPUSS DataWire #0, Channel #8 */
    def_irq_handler  cpuss_interrupts_dw0_9_IRQHandler       /* CPUSS DataWire #0, Channel #9 */
    def_irq_handler  cpuss_interrupts_dw0_10_IRQHandler      /* CPUSS DataWire #0, Channel #10 */
    def_irq_handler  cpuss_interrupts_dw0_11_IRQHandler      /* CPUSS DataWire #0, Channel #11 */
    def_irq_handler  cpuss_interrupts_dw0_12_IRQHandler      /* CPUSS DataWire #0, Channel #12 */
    def_irq_handler  cpuss_interrupts_dw0_13_IRQHandler      /* CPUSS DataWire #0, Channel #13 */
    def_irq_handler  cpuss_interrupts_dw0_14_IRQHandler      /* CPUSS DataWire #0, Channel #14 */
    def_irq_handler  cpuss_interrupts_dw0_15_IRQHandler      /* CPUSS DataWire #0, Channel #15 */
    def_irq_handler  cpuss_interrupts_dw0_16_IRQHandler      /* CPUSS DataWire #0, Channel #16 */
    def_irq_handler  cpuss_interrupts_dw0_17_IRQHandler      /* CPUSS DataWire #0, Channel #17 */
    def_irq_handler  cpuss_interrupts_dw0_18_IRQHandler      /* CPUSS DataWire #0, Channel #18 */
    def_irq_handler  cpuss_interrupts_dw0_19_IRQHandler      /* CPUSS DataWire #0, Channel #19 */
    def_irq_handler  cpuss_interrupts_dw0_20_IRQHandler      /* CPUSS DataWire #0, Channel #20 */
    def_irq_handler  cpuss_interrupts_dw0_21_IRQHandler      /* CPUSS DataWire #0, Channel #21 */
    def_irq_handler  cpuss_interrupts_dw0_22_IRQHandler      /* CPUSS DataWire #0, Channel #22 */
    def_irq_handler  cpuss_interrupts_dw0_23_IRQHandler      /* CPUSS DataWire #0, Channel #23 */
    def_irq_handler  cpuss_interrupts_dw0_24_IRQHandler      /* CPUSS DataWire #0, Channel #24 */
    def_irq_handler  cpuss_interrupts_dw0_25_IRQHandler      /* CPUSS DataWire #0, Channel #25 */
    def_irq_handler  cpuss_interrupts_dw0_26_IRQHandler      /* CPUSS DataWire #0, Channel #26 */
    def_irq_handler  cpuss_interrupts_dw0_27_IRQHandler      /* CPUSS DataWire #0, Channel #27 */
    def_irq_handler  cpuss_interrupts_dw0_28_IRQHandler      /* CPUSS DataWire #0, Channel #28 */
    def_irq_handler  cpuss_interrupts_dw1_0_IRQHandler       /* CPUSS DataWire #1, Channel #0 */
    def_irq_handler  cpuss_interrupts_dw1_1_IRQHandler       /* CPUSS DataWire #1, Channel #1 */
    def_irq_handler  cpuss_interrupts_dw1_2_IRQHandler       /* CPUSS DataWire #1, Channel #2 */
    def_irq_handler  cpuss_interrupts_dw1_3_IRQHandler       /* CPUSS DataWire #1, Channel #3 */
    def_irq_handler  cpuss_interrupts_dw1_4_IRQHandler       /* CPUSS DataWire #1, Channel #4 */
    def_irq_handler  cpuss_interrupts_dw1_5_IRQHandler       /* CPUSS DataWire #1, Channel #5 */
    def_irq_handler  cpuss_interrupts_dw1_6_IRQHandler       /* CPUSS DataWire #1, Channel #6 */
    def_irq_handler  cpuss_interrupts_dw1_7_IRQHandler       /* CPUSS DataWire #1, Channel #7 */
    def_irq_handler  cpuss_interrupts_dw1_8_IRQHandler       /* CPUSS DataWire #1, Channel #8 */
    def_irq_handler  cpuss_interrupts_dw1_9_IRQHandler       /* CPUSS DataWire #1, Channel #9 */
    def_irq_handler  cpuss_interrupts_dw1_10_IRQHandler      /* CPUSS DataWire #1, Channel #10 */
    def_irq_handler  cpuss_interrupts_dw1_11_IRQHandler      /* CPUSS DataWire #1, Channel #11 */
    def_irq_handler  cpuss_interrupts_dw1_12_IRQHandler      /* CPUSS DataWire #1, Channel #12 */
    def_irq_handler  cpuss_interrupts_dw1_13_IRQHandler      /* CPUSS DataWire #1, Channel #13 */
    def_irq_handler  cpuss_interrupts_dw1_14_IRQHandler      /* CPUSS DataWire #1, Channel #14 */
    def_irq_handler  cpuss_interrupts_dw1_15_IRQHandler      /* CPUSS DataWire #1, Channel #15 */
    def_irq_handler  cpuss_interrupts_dw1_16_IRQHandler      /* CPUSS DataWire #1, Channel #16 */
    def_irq_handler  cpuss_interrupts_dw1_17_IRQHandler      /* CPUSS DataWire #1, Channel #17 */
    def_irq_handler  cpuss_interrupts_dw1_18_IRQHandler      /* CPUSS DataWire #1, Channel #18 */
    def_irq_handler  cpuss_interrupts_dw1_19_IRQHandler      /* CPUSS DataWire #1, Channel #19 */
    def_irq_handler  cpuss_interrupts_dw1_20_IRQHandler      /* CPUSS DataWire #1, Channel #20 */
    def_irq_handler  cpuss_interrupts_dw1_21_IRQHandler      /* CPUSS DataWire #1, Channel #21 */
    def_irq_handler  cpuss_interrupts_dw1_22_IRQHandler      /* CPUSS DataWire #1, Channel #22 */
    def_irq_handler  cpuss_interrupts_dw1_23_IRQHandler      /* CPUSS DataWire #1, Channel #23 */
    def_irq_handler  cpuss_interrupts_dw1_24_IRQHandler      /* CPUSS DataWire #1, Channel #24 */
    def_irq_handler  cpuss_interrupts_dw1_25_IRQHandler      /* CPUSS DataWire #1, Channel #25 */
    def_irq_handler  cpuss_interrupts_dw1_26_IRQHandler      /* CPUSS DataWire #1, Channel #26 */
    def_irq_handler  cpuss_interrupts_dw1_27_IRQHandler      /* CPUSS DataWire #1, Channel #27 */
    def_irq_handler  cpuss_interrupts_dw1_28_IRQHandler      /* CPUSS DataWire #1, Channel #28 */
    def_irq_handler  cpuss_interrupts_fault_0_IRQHandler     /* CPUSS Fault Structure Interrupt #0 */
    def_irq_handler  cpuss_interrupts_fault_1_IRQHandler     /* CPUSS Fault Structure Interrupt #1 */
    def_irq_handler  cpuss_interrupt_crypto_IRQHandler       /* CRYPTO Accelerator Interrupt */
    def_irq_handler  cpuss_interrupt_fm_IRQHandler           /* FLASH Macro Interrupt */
    def_irq_handler  cpuss_interrupts_cm4_fp_IRQHandler      /* Floating Point operation fault */
    def_irq_handler  cpuss_interrupts_cm0_cti_0_IRQHandler   /* CM0+ CTI #0 */
    def_irq_handler  cpuss_interrupts_cm0_cti_1_IRQHandler   /* CM0+ CTI #1 */
    def_irq_handler  cpuss_interrupts_cm4_cti_0_IRQHandler   /* CM4 CTI #0 */
    def_irq_handler  cpuss_interrupts_cm4_cti_1_IRQHandler   /* CM4 CTI #1 */
    def_irq_handler  tcpwm_0_interrupts_0_IRQHandler         /* TCPWM #0, Counter #0 */
    def_irq_handler  tcpwm_0_interrupts_1_IRQHandler         /* TCPWM #0, Counter #1 */
    def_irq_handler  tcpwm_0_interrupts_2_IRQHandler         /* TCPWM #0, Counter #2 */
    def_irq_handler  tcpwm_0_interrupts_3_IRQHandler         /* TCPWM #0, Counter #3 */
    def_irq_handler  tcpwm_0_interrupts_4_IRQHandler         /* TCPWM #0, Counter #4 */
    def_irq_handler  tcpwm_0_interrupts_5_IRQHandler         /* TCPWM #0, Counter #5 */
    def_irq_handler  tcpwm_0_interrupts_6_IRQHandler         /* TCPWM #0, Counter #6 */
    def_irq_handler  tcpwm_0_interrupts_7_IRQHandler         /* TCPWM #0, Counter #7 */
    def_irq_handler  tcpwm_1_interrupts_0_IRQHandler         /* TCPWM #1, Counter #0 */
    def_irq_handler  tcpwm_1_interrupts_1_IRQHandler         /* TCPWM #1, Counter #1 */
    def_irq_handler  tcpwm_1_interrupts_2_IRQHandler         /* TCPWM #1, Counter #2 */
    def_irq_handler  tcpwm_1_interrupts_3_IRQHandler         /* TCPWM #1, Counter #3 */
    def_irq_handler  tcpwm_1_interrupts_4_IRQHandler         /* TCPWM #1, Counter #4 */
    def_irq_handler  tcpwm_1_interrupts_5_IRQHandler         /* TCPWM #1, Counter #5 */
    def_irq_handler  tcpwm_1_interrupts_6_IRQHandler         /* TCPWM #1, Counter #6 */
    def_irq_handler  tcpwm_1_interrupts_7_IRQHandler         /* TCPWM #1, Counter #7 */
    def_irq_handler  tcpwm_1_interrupts_8_IRQHandler         /* TCPWM #1, Counter #8 */
    def_irq_handler  tcpwm_1_interrupts_9_IRQHandler         /* TCPWM #1, Counter #9 */
    def_irq_handler  tcpwm_1_interrupts_10_IRQHandler        /* TCPWM #1, Counter #10 */
    def_irq_handler  tcpwm_1_interrupts_11_IRQHandler        /* TCPWM #1, Counter #11 */
    def_irq_handler  tcpwm_1_interrupts_12_IRQHandler        /* TCPWM #1, Counter #12 */
    def_irq_handler  tcpwm_1_interrupts_13_IRQHandler        /* TCPWM #1, Counter #13 */
    def_irq_handler  tcpwm_1_interrupts_14_IRQHandler        /* TCPWM #1, Counter #14 */
    def_irq_handler  tcpwm_1_interrupts_15_IRQHandler        /* TCPWM #1, Counter #15 */
    def_irq_handler  tcpwm_1_interrupts_16_IRQHandler        /* TCPWM #1, Counter #16 */
    def_irq_handler  tcpwm_1_interrupts_17_IRQHandler        /* TCPWM #1, Counter #17 */
    def_irq_handler  tcpwm_1_interrupts_18_IRQHandler        /* TCPWM #1, Counter #18 */
    def_irq_handler  tcpwm_1_interrupts_19_IRQHandler        /* TCPWM #1, Counter #19 */
    def_irq_handler  tcpwm_1_interrupts_20_IRQHandler        /* TCPWM #1, Counter #20 */
    def_irq_handler  tcpwm_1_interrupts_21_IRQHandler        /* TCPWM #1, Counter #21 */
    def_irq_handler  tcpwm_1_interrupts_22_IRQHandler        /* TCPWM #1, Counter #22 */
    def_irq_handler  tcpwm_1_interrupts_23_IRQHandler        /* TCPWM #1, Counter #23 */
    def_irq_handler  pass_interrupt_sar_IRQHandler           /* SAR ADC interrupt */
    def_irq_handler  audioss_0_interrupt_i2s_IRQHandler      /* I2S0 Audio interrupt */
    def_irq_handler  audioss_0_interrupt_pdm_IRQHandler      /* PDM0/PCM0 Audio interrupt */
    def_irq_handler  audioss_1_interrupt_i2s_IRQHandler      /* I2S1 Audio interrupt */
    def_irq_handler  profile_interrupt_IRQHandler            /* Energy Profiler interrupt */
    def_irq_handler  smif_interrupt_IRQHandler               /* Serial Memory Interface interrupt */
    def_irq_handler  usb_interrupt_hi_IRQHandler             /* USB Interrupt */
    def_irq_handler  usb_interrupt_med_IRQHandler            /* USB Interrupt */
    def_irq_handler  usb_interrupt_lo_IRQHandler             /* USB Interrupt */
    def_irq_handler  sdhc_0_interrupt_wakeup_IRQHandler      /* SDIO wakeup interrupt for mxsdhc */
    def_irq_handler  sdhc_0_interrupt_general_IRQHandler     /* Consolidated interrupt for mxsdhc for everything else */
    def_irq_handler  sdhc_1_interrupt_wakeup_IRQHandler      /* EEMC wakeup interrupt for mxsdhc, not used */
    def_irq_handler  sdhc_1_interrupt_general_IRQHandler     /* Consolidated interrupt for mxsdhc for everything else */


                .end