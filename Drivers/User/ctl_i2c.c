/**
 * @file ctl_i2c.c
 * @author leoli (jafee201153@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-03-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "ctl_i2c.h"

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
__STATIC_INLINE bool I2C_TransmitMultipleBytes(I2C_TypeDef *I2Cx, uint8_t *pData, uint16_t Size, uint32_t Timeout);
__STATIC_INLINE bool I2C_ReceiveMultipleBytes(I2C_TypeDef *I2Cx, uint8_t *pData, uint16_t Size, uint32_t Timeout);
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
/**
 * @brief I2C Write Bytes, 7-bit addressing mode (master)
 *
 * @param I2Cx I2C Instance
 * @param slave_address 7-bit slave address, will be shifted left 1 bit and ANDed with 0xFE automatically
 * @param pData Pointer to data buffer
 * @param Size Amount of data to be sent
 * @param Timeout Timeout duration
 * @return true
 * @return false
 */
bool I2C_WriteBytes(I2C_TypeDef *I2Cx, uint8_t slave_address, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
    uint32_t tickstart = 0U;

    /* Init tickstart for timeout management*/
    tickstart = HAL_GetTick();

    /* Generate Start */
    LL_I2C_GenerateStartCondition(I2Cx);

    /* Wait until SB flag is set */
    while (LL_I2C_IsActiveFlag_SB(I2Cx) == 0U) {
        if ((Timeout == 0U) || ((HAL_GetTick() - tickstart) > Timeout)) {
            return false;
        }
    }

    /* Send slave address */
    slave_address = slave_address << 1;
    slave_address &= 0xFE;
    LL_I2C_TransmitData8(I2Cx, slave_address);

    /* Wait until ADDR flag is set */
    while (LL_I2C_IsActiveFlag_ADDR(I2Cx) == 0U) {
        if ((Timeout == 0U) || ((HAL_GetTick() - tickstart) > Timeout)) {
            return false;
        }
    }

    /* Clear ADDR flag */
    LL_I2C_ClearFlag_ADDR(I2Cx);

    /* Transmit data */
    if (I2C_TransmitMultipleBytes(I2Cx, pData, Size, Timeout) == false) {
        return false;
    }

    return true;
}

/**
 * @brief I2C Read Bytes, 7-bit addressing mode (master)
 *
 * @param I2Cx I2C Instance
 * @param slave_address 7-bit slave address, will be shifted left 1 bit and ORed with 0x01 automatically
 * @param pData Pointer to data buffer
 * @param Size Amount of data to be received
 * @param Timeout Timeout duration
 * @return
 */
bool I2C_ReadBytes(I2C_TypeDef *I2Cx, uint8_t slave_address, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
    uint32_t tickstart = 0U;

    /* Init tickstart for timeout management*/
    tickstart = HAL_GetTick();

    /* Generate Start */
    LL_I2C_GenerateStartCondition(I2Cx);

    /* Wait until SB flag is set */
    while (LL_I2C_IsActiveFlag_SB(I2Cx) == 0U) {
        if ((Timeout == 0U) || ((HAL_GetTick() - tickstart) > Timeout)) {
            return false;
        }
    }

    /* Send slave address */
    slave_address = slave_address << 1;
    slave_address |= 0x01;
    LL_I2C_TransmitData8(I2Cx, slave_address);

    /* Wait until ADDR flag is set */
    while (LL_I2C_IsActiveFlag_ADDR(I2Cx) == 0U) {
        if ((Timeout == 0U) || ((HAL_GetTick() - tickstart) > Timeout)) {
            return false;
        }
    }

    /* Clear ADDR flag */
    LL_I2C_ClearFlag_ADDR(I2Cx);

    /* Receive data */
    if (I2C_ReceiveMultipleBytes(I2Cx, pData, Size, Timeout) == false) {
        return false;
    }

    return true;
}

/**
 * @brief I2C Write with 0-byte address and 1-byte data
 *
 * @param I2Cx I2C Instance
 * @param slave_address 7-bit slave address, will be shifted left 1 bit and ANDed with 0xFE automatically
 * @param data Data to be sent
 */
void I2C_Write0A1D(I2C_TypeDef *I2Cx, uint8_t slave_address, uint8_t data) {
    I2C_WriteBytes(I2Cx, slave_address, &data, 1, 100);
}

/**
 * @brief I2C Read with 0-byte address and 1-byte data
 *
 * @param I2Cx I2C Instance
 * @param slave_address 7-bit slave address, will be shifted left 1 bit and ORed with 0x01 automatically
 * @return uint8_t Data received
 */
uint8_t I2C_Read0A1D(I2C_TypeDef *I2Cx, uint8_t slave_address) {
    uint8_t data;
    I2C_ReadBytes(I2Cx, slave_address, &data, 1, 100);
    return data;
}

/**
 * @brief I2C Write with 0-byte address and 2-byte data
 *
 * @param I2Cx I2C Instance
 * @param slave_address 7-bit slave address, will be shifted left 1 bit and ANDed with 0xFE automatically
 * @param data Data to be sent
 */
void I2C_Write0A2D(I2C_TypeDef *I2Cx, uint8_t slave_address, uint16_t data) {
    I2C_WriteBytes(I2Cx, slave_address, (uint8_t *)&data, 2, 100);
}

/**
 * @brief I2C Read with 0-byte address and 2-byte data
 *
 * @param I2Cx I2C Instance
 * @param slave_address 7-bit slave address, will be shifted left 1 bit and ORed with 0x01 automatically
 * @return uint16_t Data received
 */
uint8_t I2C_Read0A2D(I2C_TypeDef *I2Cx, uint8_t slave_address) {
    uint16_t data;
    I2C_ReadBytes(I2Cx, slave_address, (uint8_t *)&data, 2, 100);
    return data;
}

/**
 * @brief I2C Write with 1-byte address and 1-byte data
 *
 * @param I2Cx I2C Instance
 * @param slave_address 7-bit slave address, will be shifted left 1 bit and ANDed with 0xFE automatically
 * @param address 1-byte address
 * @param data Data to be sent
 */
void I2C_Write1A1D(I2C_TypeDef *I2Cx, uint8_t slave_address, uint8_t address, uint8_t data) {
    uint8_t tmp[2];
    tmp[0] = address;
    tmp[1] = data;
    I2C_WriteBytes(I2Cx, slave_address, tmp, 2, 100);
}

/**
 * @brief I2C Read with 1-byte address and 1-byte data
 *
 * @param I2Cx I2C Instance
 * @param slave_address 7-bit slave address, will be shifted left 1 bit and ORed with 0x01 automatically
 * @param address 1-byte address
 * @return uint8_t Data received
 */
uint8_t I2C_Read1A1D(I2C_TypeDef *I2Cx, uint8_t slave_address, uint8_t address) {
    uint8_t tmp[2];
    tmp[0] = address;
    I2C_ReadBytes(I2Cx, slave_address, tmp, 2, 100);
    return tmp[1];
}

/**
 * @brief I2C Write with 1-byte address and 2-byte data
 *
 * @param I2Cx I2C Instance
 * @param slave_address 7-bit slave address, will be shifted left 1 bit and ANDed with 0xFE automatically
 * @param address 1-byte address
 * @param data Data to be sent
 */
void I2C_Write1A2D(I2C_TypeDef *I2Cx, uint8_t slave_address, uint8_t address, uint16_t data) {
    uint8_t tmp[3];
    tmp[0] = address;
    tmp[1] = data & 0xff;
    tmp[2] = (data >> 8) & 0xff;
    I2C_WriteBytes(I2Cx, slave_address, tmp, 3, 100);
}

/**
 * @brief I2C Read with 1-byte address and 2-byte data
 *
 * @param I2Cx I2C Instance
 * @param slave_address 7-bit slave address, will be shifted left 1 bit and ORed with 0x01 automatically
 * @param address 1-byte address
 * @return uint16_t Data received
 */
uint16_t I2C_Read1A2D(I2C_TypeDef *I2Cx, uint8_t slave_address, uint8_t address) {
    uint8_t tmp[3];
    tmp[0] = address;
    I2C_ReadBytes(I2Cx, slave_address, tmp, 3, 100);
    return (tmp[2] << 8) | tmp[1];
}

/**
 * @brief I2C Write with 2-byte address and 1-byte data
 *
 * @param I2Cx I2C Instance
 * @param slave_address 7-bit slave address, will be shifted left 1 bit and ANDed with 0xFE automatically
 * @param address 2-byte address
 * @param data Data to be sent
 */
void I2C_Write2A1D(I2C_TypeDef *I2Cx, uint8_t slave_address, uint16_t address, uint8_t data) {
    uint8_t tmp[3];
    tmp[0] = address & 0xff;
    tmp[1] = (address >> 8) & 0xff;
    tmp[2] = data;
    I2C_WriteBytes(I2Cx, slave_address, tmp, 3, 100);
}

/**
 * @brief I2C Read with 2-byte address and 1-byte data
 *
 * @param I2Cx I2C Instance
 * @param slave_address 7-bit slave address, will be shifted left 1 bit and ORed with 0x01 automatically
 * @param address 2-byte address
 * @return uint8_t Data received
 */
uint8_t I2C_Read2A1D(I2C_TypeDef *I2Cx, uint8_t slave_address, uint16_t address) {
    uint8_t tmp[3];
    tmp[0] = address & 0xff;
    tmp[1] = (address >> 8) & 0xff;
    I2C_ReadBytes(I2Cx, slave_address, tmp, 3, 100);
    return tmp[2];
}

/**
 * @brief I2C Write with 2-byte address and 2-byte data
 *
 * @param I2Cx I2C Instance
 * @param slave_address 7-bit slave address, will be shifted left 1 bit and ANDed with 0xFE automatically
 * @param address 2-byte address
 * @param data Data to be sent
 */
void I2C_Write2A2D(I2C_TypeDef *I2Cx, uint8_t slave_address, uint16_t address, uint16_t data) {
    uint8_t tmp[4];
    tmp[0] = address & 0xff;
    tmp[1] = (address >> 8) & 0xff;
    tmp[2] = data & 0xff;
    tmp[3] = (data >> 8) & 0xff;
    I2C_WriteBytes(I2Cx, slave_address, tmp, 4, 100);
}

/**
 * @brief I2C Read with 2-byte address and 2-byte data
 *
 * @param I2Cx I2C Instance
 * @param slave_address 7-bit slave address, will be shifted left 1 bit and ORed with 0x01 automatically
 * @param address 2-byte address
 * @return uint16_t Data received
 */
uint16_t I2C_Read2A2D(I2C_TypeDef *I2Cx, uint8_t slave_address, uint16_t address) {
    uint8_t tmp[4];
    tmp[0] = address & 0xff;
    tmp[1] = (address >> 8) & 0xff;
    I2C_ReadBytes(I2Cx, slave_address, tmp, 4, 100);
    return (tmp[3] << 8) | tmp[2];
}
#pragma endregion Exported Functions

/*  */
#pragma region Private Functions
/**
 * @brief I2C Transmit Multiple Bytes
 *
 * @param I2Cx I2C Instance
 * @param pData Pointer to data buffer
 * @param Size Amount of data to be sent
 * @param Timeout Timeout duration
 * @return
 */
__STATIC_INLINE bool I2C_TransmitMultipleBytes(I2C_TypeDef *I2Cx, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
    uint32_t tickstart = 0U;

    /* Init tickstart for timeout management*/
    tickstart = HAL_GetTick();

    /* Wait until TXE flag is set */
    while (LL_I2C_IsActiveFlag_TXE(I2Cx) == 0U) {
        if ((Timeout == 0U) || ((HAL_GetTick() - tickstart) > Timeout)) {
            return false;
        }
    }

    /* While there is data to be written */
    while (Size > 0U) {
        /* Write data to DR */
        LL_I2C_TransmitData8(I2Cx, *pData);

        /* Increment Buffer pointer */
        pData++;

        /* Decrement the Size counter */
        Size--;

        /* Wait until TXE flag is set */
        while (LL_I2C_IsActiveFlag_TXE(I2Cx) == 0U) {
            if ((Timeout == 0U) || ((HAL_GetTick() - tickstart) > Timeout)) {
                return false;
            }
        }
    }

    /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
    /* Wait until STOPF flag is set */
    while (LL_I2C_IsActiveFlag_STOP(I2Cx) == 0U) {
        if ((Timeout == 0U) || ((HAL_GetTick() - tickstart) > Timeout)) {
            return false;
        }
    }

    /* Clear STOP Flag */
    LL_I2C_ClearFlag_STOP(I2Cx);

    return true;
}

/**
 * @brief I2C Receive Multiple Bytes
 *
 * @param I2Cx I2C Instance
 * @param pData Pointer to data buffer
 * @param Size Amount of data to be received
 * @param Timeout Timeout duration
 * @return
 */
__STATIC_INLINE bool I2C_ReceiveMultipleBytes(I2C_TypeDef *I2Cx, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
    uint32_t tickstart = 0U;

    /* Init tickstart for timeout management*/
    tickstart = HAL_GetTick();

    /* Wait until RXNE flag is set */
    while (LL_I2C_IsActiveFlag_RXNE(I2Cx) == 0U) {
        if ((Timeout == 0U) || ((HAL_GetTick() - tickstart) > Timeout)) {
            return false;
        }
    }

    /* While there is data to be read */
    while (Size > 0U) {
        if (Size == 1U) {
            /* Disable Acknowledge */
            LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_NACK);

            /* Generate Stop */
            LL_I2C_GenerateStopCondition(I2Cx);
        }

        /* Read data from DR */
        *pData = LL_I2C_ReceiveData8(I2Cx);

        /* Increment Buffer pointer */
        pData++;

        /* Decrement the Size counter */
        Size--;

        /* Wait until RXNE flag is set */
        while (LL_I2C_IsActiveFlag_RXNE(I2Cx) == 0U) {
            if ((Timeout == 0U) || ((HAL_GetTick() - tickstart) > Timeout)) {
                return false;
            }
        }
    }

    /* Clear NACK flag */
    LL_I2C_ClearFlag_NACK(I2Cx);

    return true;
}
#pragma endregion Private Functions

#ifdef __cplusplus
}
#endif
