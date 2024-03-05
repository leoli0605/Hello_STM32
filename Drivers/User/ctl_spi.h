/**
 * @file ctl_spi.h
 * @author leoli (jafee201153@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-03-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __CTL_SPI_H
#define __CTL_SPI_H

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
typedef struct _SPI_Pin_t {
    GPIO_TypeDef *Port; /*!< GPIO port used for the pin */
    uint32_t Pin;       /*!< pin selection */
} SPI_Pin_t;

typedef struct _SPI_DMA_Handle_t {
    DMA_TypeDef *DMAx; /*!< DMA registers base address */
    uint32_t Stream;   /*!< DMA stream selection */
} SPI_DMA_Handle_t;

typedef struct _SPI_Handle_t {
    SPI_TypeDef *Instance;          /*!< SPI registers base address */
    SPI_Pin_t SSB_Pin;              /*!< SSB pin selection */
    SPI_Pin_t SCLK_Pin;             /*!< SCLK pin selection */
    SPI_Pin_t MOSI_Pin;             /*!< MOSI pin selection */
    SPI_Pin_t MISO_Pin;             /*!< MISO pin selection */
    SPI_DMA_Handle_t Tx_DMA_Handle; /*!< Tx DMA handle parameters */
    SPI_DMA_Handle_t Rx_DMA_Handle; /*!< Rx DMA handle parameters */
} SPI_Handle_t;
#pragma endregion typedef

/*  */
#pragma region defines

#pragma endregion defines

/*  */
#pragma region Exported Functions
void SPI_RegisterHandle(SPI_Handle_t *pHandle, SPI_TypeDef *Instance, SPI_Pin_t SSB_Pin, SPI_Pin_t SCLK_Pin, SPI_Pin_t MOSI_Pin, SPI_Pin_t MISO_Pin,
                        SPI_DMA_Handle_t Tx_DMA_Handle, SPI_DMA_Handle_t Rx_DMA_Handle);
void SPI_Enable(SPI_Handle_t *pHandle);
void SPI_Disable(SPI_Handle_t *pHandle);
void SPI_SetMode(SPI_Handle_t *pHandle, uint8_t mode);
uint8_t SPI_GetMode(SPI_Handle_t *pHandle);
void SPI_SetClockDiv(SPI_Handle_t *pHandle, uint8_t div);
uint8_t SPI_GetClockDiv(SPI_Handle_t *pHandle);
void SPI_SetSSB(SPI_Handle_t *pHandle, bool state);
void SPI_TransmitData(SPI_Handle_t *pHandle, uint8_t data);
uint8_t SPI_ReceiveData(SPI_Handle_t *pHandle);
void SPI_TransmitData_DMA(SPI_Handle_t *pHandle, uint8_t *pData, uint32_t Size);
void SPI_TransmitData_DMA_Blocking(SPI_Handle_t *pHandle, uint8_t *pData, uint32_t Size);
void SPI_ReceiveData_DMA(SPI_Handle_t *pHandle, uint8_t *pData, uint32_t Size);
void SPI_ReceiveData_DMA_Blocking(SPI_Handle_t *pHandle, uint8_t *pData, uint32_t Size);
#pragma endregion Exported Functions

#ifdef __cplusplus
}
#endif

#endif  // __CTL_SPI_H
