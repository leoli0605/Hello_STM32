/**
 * @file ctl_usart.c
 * @author leoli (jafee201153@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-03-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "ctl_usart.h"

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
USART_TypeDef *USARTx = UART7;
#pragma endregion variables

/*  */
#pragma region External Functions

#pragma endregion External Functions

/*  */
#pragma region Exported Functions
void USART_WriteData(char data) {
    LL_USART_TransmitData8(USARTx, data);
    while (!LL_USART_IsActiveFlag_TXE(USARTx))
        ;
}
#pragma endregion Exported Functions

/*  */
#pragma region Private Functions

#pragma endregion Private Functions

#ifdef __cplusplus
}
#endif
