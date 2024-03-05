/**
 * @file ctl_i2c.h
 * @author leoli (jafee201153@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-03-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __CTL_I2C_H
#define __CTL_I2C_H

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
#pragma region defines

#pragma endregion defines

/*  */
#pragma region Exported Functions
bool I2C_WriteBytes(I2C_TypeDef *I2Cx, uint8_t slave_address, uint8_t *pData, uint16_t Size, uint32_t Timeout);
bool I2C_ReadBytes(I2C_TypeDef *I2Cx, uint8_t slave_address, uint8_t *pData, uint16_t Size, uint32_t Timeout);
void I2C_Write0A1D(I2C_TypeDef *I2Cx, uint8_t slave_address, uint8_t data);
uint8_t I2C_Read0A1D(I2C_TypeDef *I2Cx, uint8_t slave_address);
void I2C_Write0A2D(I2C_TypeDef *I2Cx, uint8_t slave_address, uint16_t data);
uint8_t I2C_Read0A2D(I2C_TypeDef *I2Cx, uint8_t slave_address);
void I2C_Write1A1D(I2C_TypeDef *I2Cx, uint8_t slave_address, uint8_t address, uint8_t data);
uint8_t I2C_Read1A1D(I2C_TypeDef *I2Cx, uint8_t slave_address, uint8_t address);
void I2C_Write1A2D(I2C_TypeDef *I2Cx, uint8_t slave_address, uint8_t address, uint16_t data);
uint16_t I2C_Read1A2D(I2C_TypeDef *I2Cx, uint8_t slave_address, uint8_t address);
void I2C_Write2A1D(I2C_TypeDef *I2Cx, uint8_t slave_address, uint16_t address, uint8_t data);
uint8_t I2C_Read2A1D(I2C_TypeDef *I2Cx, uint8_t slave_address, uint16_t address);
void I2C_Write2A2D(I2C_TypeDef *I2Cx, uint8_t slave_address, uint16_t address, uint16_t data);
uint16_t I2C_Read2A2D(I2C_TypeDef *I2Cx, uint8_t slave_address, uint16_t address);
#pragma endregion Exported Functions

#ifdef __cplusplus
}
#endif

#endif  // __CTL_I2C_H
