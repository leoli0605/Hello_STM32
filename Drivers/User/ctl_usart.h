/**
 * @file ctl_usart.h
 * @author leoli (jafee201153@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-03-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __CTL_USART_H
#define __CTL_USART_H

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

#pragma endregion includes

/*  */
#pragma region typedef

#pragma endregion typedef

/*  */
#pragma region defines

#pragma endregion defines

/*  */
#pragma region Exported Functions
void USART_WriteData(char data);
#pragma endregion Exported Functions

#ifdef __cplusplus
}
#endif

#endif  // __CTL_USART_H
