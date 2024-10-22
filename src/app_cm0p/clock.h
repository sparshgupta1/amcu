#ifndef _CLOCK_H__
#define _CLOCK_H__


#include "xrobj_hdr_defs.h"

/**
 * PERIPHERAL SPI CLOCK CONFIGURATION
 */

#define SPI_CLOCK               PCLK_SCB1_CLOCK

/**
 * Assign divider type and number for SPI
 */
#define SPI_CLK_DIV_TYPE    (CY_SYSCLK_DIV_8_BIT)
#define SPI_CLK_DIV_NUM     (1U)

/**
 * @brief Core system clock frequency defined by PSoC 6 PDL and required
 * by CMSIS.
 */
extern uint32_t SystemCoreClock;

/**
 * @brief Defines counter value loaded into the SysTick
 * counter register to achieve 1ms period.
 *
 * Maximum ticks for the SysTick timer before it rolls over
 */
#define SYSTICK_TIMER_MAX (SystemCoreClock / 1000U)

/**
 * @brief Defines counter value loaded into the SysTick
 * counter register to achieve 1ms period
 */
#define SYSTICK_TIMER_REG_LOAD_MAX (SYSTICK_TIMER_MAX - 1U)

/**
 * Mask for maximum ticks for the SysTick timer
 */
#define SYSTICK_TIMER_MASK (0x00FFFFFFU)

extern void clock_configure_all(const uint8_t bootType);

extern uint64_t clock_uptime(void);

#endif // _CLOCK_H__
