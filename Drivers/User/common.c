/**
 * @file common.c
 * @author leoli (jafee201153@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-03-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*  */
#pragma region includes

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

#pragma endregion includes

/*  */
#pragma region typedef

#pragma endregion typedef

/*  */
#pragma region Private Function Prototypes

#pragma endregion Private Function Prototypes

/*  */
#pragma region defines

#pragma endregion defines

/*  */
#pragma region variables

#pragma endregion variables

/*  */
#pragma region External Functions

#pragma endregion External Functions

/*  */
#pragma region Exported Functions
void udelay(uint32_t us) {
    if (us > 0) {
        uint32_t ticks;
        uint32_t told, tnow, tcnt = 0;
        uint32_t reload = SysTick->LOAD;
        ticks           = us * (SystemCoreClock / 1000000);
        told            = SysTick->VAL;
        while (true) {
            tnow = SysTick->VAL;
            if (tnow != told) {
                if (tnow < told)
                    tcnt += told - tnow;
                else
                    tcnt += reload - tnow + told;
                told = tnow;
                if (tcnt >= ticks) break;
            }
        }
    }
}
#pragma endregion Exported Functions

/*  */
#pragma region Private Functions

#pragma endregion Private Functions

#ifdef __cplusplus
}
#endif
