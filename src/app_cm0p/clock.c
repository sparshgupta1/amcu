#include "clock.h"

#include "cy_pdl.h"


/**
 * Callback function index for rollover detection
 */
#define SYSTICK_CALLBACK_ID_ROLLOVER_CNTR 0U

/**
 * @brief Counts the number of SysTick rollovers
 *
 * @note Used by sysTickRolloverAccumulator()
 *
 */
static uint64_t g_sysTickRolloverCntr = 0U;

/**
 * @brief Contains the total rollover time since the system was booted
 *
 */
static uint64_t g_sysTickRolloverTotal = 0U;

/**
 * @brief SysTick rollover counter callback routine
 *
 * @note Called by the SysTick NVIC interrupt
 */
static void sysTickRolloverAccumulator(void)
{
    g_sysTickRolloverCntr++;
    g_sysTickRolloverTotal = (SYSTICK_TIMER_MAX * g_sysTickRolloverCntr);
}

/**
 * @brief Starts SysTick hardware timer
 *
 */
static void sysTickInit(void)
{
    Cy_SysTick_Init(CY_SYSTICK_CLOCK_SOURCE_CLK_CPU,
                    SYSTICK_TIMER_REG_LOAD_MAX);

    Cy_SysTick_Callback p_rollFn = &sysTickRolloverAccumulator;

    Cy_SysTick_SetCallback(SYSTICK_CALLBACK_ID_ROLLOVER_CNTR, p_rollFn);
}

/**
 * Returns the accumulated time in system units since the system was booted.
 * The accumulated time is stored in a 64-bit value that will eventually
 * overflow.  The current implementation utilizes the SysTick peripheral of the
 * PSoC 6 device. The SysTick peripheral is configured by FreeRTOS running on
 * the CM4 core.
 *
 * @note
 * The PSoC 6 hardware seems to allow only one core to configure the SysTick
 * peripheral, even though the datasheet and TRM states that there are two
 * SysTick peripherals -- one for each core. Once configured, the current
 * value can be read by either core, but only one SysTick peripheral provides
 * ticks.
 *
 * SysTick as configured, provides a 1 ms period. At this rate, overflow will
 * occur in approximately 5,849.5 years.
 *
 * The caller is responsible for checking this overflow condition.
 *
 * @return uint64_t accumulated time in ticks, since the system was started
 */
uint64_t clock_uptime(void)
{
    uint32_t intrSet = Cy_SysLib_EnterCriticalSection();
    uint64_t uptime =
        g_sysTickRolloverTotal + (SYSTICK_TIMER_MAX - SYSTICK_VAL);

    Cy_SysLib_ExitCriticalSection(intrSet);

    return uptime;
}

/**
 * @brief Configures all peripheral clocks prior to firmware
 * running
 *
 * @param bootType specifies types of restart
 */
void clock_configure_all(const uint8_t bootType)
{
#define PERI_CLK_DIVIDER 0U

    // start hardware system timer
    sysTickInit();

    // intialize only the SysTick
    if (BOOT_WARM == bootType)
        return;

    // update the peripheral clock
    if (PERI_CLK_DIVIDER != Cy_SysClk_ClkPeriGetDivider())
    {
        Cy_SysClk_ClkPeriSetDivider(PERI_CLK_DIVIDER);
    }

    /**
     * Updates clock settings. Must be issued after any changes
     * made via call(s) to Cy_SysClk_ClkPeriSetDivider().
     */
    SystemCoreClockUpdate();

    /**
     * Configure SPI Clock
     *
     * Connect assigned divider to be a clock source for SPI
     */
    Cy_SysClk_PeriphAssignDivider(SPI_CLOCK, SPI_CLK_DIV_TYPE, SPI_CLK_DIV_NUM);

    /**
     * Enable SPI peripheral to operate up to the maximum supported
     * data rate.
     */
    Cy_SysClk_PeriphSetDivider(SPI_CLK_DIV_TYPE, SPI_CLK_DIV_NUM, 0U);
    Cy_SysClk_PeriphEnableDivider(SPI_CLK_DIV_TYPE, SPI_CLK_DIV_NUM);
}
