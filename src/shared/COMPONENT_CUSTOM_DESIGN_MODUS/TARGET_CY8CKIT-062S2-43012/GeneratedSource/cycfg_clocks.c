/*******************************************************************************
 * File Name: cycfg_clocks.c
 *
 * Description:
 * Clock configuration
 * This file was automatically generated and should not be modified.
 * Tools Package 2.4.0.5972
 * mtb-pdl-cat1 2.4.0.14850
 * personalities 6.0.0.0
 * udd 3.0.0.2024
 *
 ********************************************************************************
 * Copyright 2022 Cypress Semiconductor Corporation (an Infineon company) or
 * an affiliate of Cypress Semiconductor Corporation.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ********************************************************************************/

#include "cycfg_clocks.h"

// FIXME J.S 02/03/23, requires removal of certain AMCU file auto-generation.
// fix for this would be covered by FW-9493
#if 0
#if defined(CY_USING_HAL)
    const cyhal_resource_inst_t peri_0_div_16_0_obj = 
    {
        .type = CYHAL_RSC_CLOCK,
        .block_num = peri_0_div_16_0_HW,
        .channel_num = peri_0_div_16_0_NUM,
    };
#endif // defined (CY_USING_HAL)
#endif

#if defined(CY_USING_HAL)
const cyhal_resource_inst_t CYBSP_CSD_CLK_DIV_obj = {
    .type        = CYHAL_RSC_CLOCK,
    .block_num   = CYBSP_CSD_CLK_DIV_HW,
    .channel_num = CYBSP_CSD_CLK_DIV_NUM,
};
#endif // defined (CY_USING_HAL)

void init_cycfg_clocks(void)
{
// FIXME J.S 02/03/23, requires removal of certain AMCU file auto-generation.
// fix for this would be covered by FW-9493
#if 0
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_16_BIT, 0U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_16_BIT, 0U, 108U);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_16_BIT, 0U);
#if defined(CY_USING_HAL)
    cyhal_hwmgr_reserve(&peri_0_div_16_0_obj);
#endif // defined (CY_USING_HAL)
#endif

    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_8_BIT, 0U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT, 0U, 0U);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_8_BIT, 0U);
#if defined(CY_USING_HAL)
    cyhal_hwmgr_reserve(&CYBSP_CSD_CLK_DIV_obj);
#endif // defined (CY_USING_HAL)
}
