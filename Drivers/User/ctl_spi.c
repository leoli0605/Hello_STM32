/**
 * @file ctl_spi.c
 * @author leoli (jafee201153@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-03-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "ctl_spi.h"

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
#define IS_SPI_MODE(MODE) (((MODE) == 0) || ((MODE) == 1) || ((MODE) == 2) || ((MODE) == 3))

#define IS_SPI_CLK_DIV(DIV) \
    (((DIV) == 2) || ((DIV) == 4) || ((DIV) == 8) || ((DIV) == 16) || ((DIV) == 32) || ((DIV) == 64) || ((DIV) == 128) || ((DIV) == 255))

#define SPI_IS_READY(pHandle)                                                                                                            \
    (LL_DMA_IsEnabledStream(pHandle->Tx_DMA_Handle.DMAx, pHandle->Tx_DMA_Handle.Stream) ||                                               \
     LL_DMA_IsEnabledStream(pHandle->Rx_DMA_Handle.DMAx, pHandle->Rx_DMA_Handle.Stream) || LL_SPI_IsActiveFlag_TXE(pHandle->Instance) || \
     LL_SPI_IsActiveFlag_BSY(pHandle->Instance))

#define SPI_ClearStatusAndFlag(pHandle)                                               \
    LL_DMA_DisableStream(pHandle->Tx_DMA_Handle.DMAx, pHandle->Tx_DMA_Handle.Stream); \
    LL_DMA_DisableStream(pHandle->Rx_DMA_Handle.DMAx, pHandle->Rx_DMA_Handle.Stream); \
    LL_SPI_DisableDMAReq_RX(pHandle->Instance);                                       \
    LL_SPI_DisableDMAReq_TX(pHandle->Instance);                                       \
    LL_DMA_ClearFlag_DME0(pHandle->Tx_DMA_Handle.DMAx);                               \
    LL_DMA_ClearFlag_DME1(pHandle->Rx_DMA_Handle.DMAx);                               \
    LL_DMA_ClearFlag_FE0(pHandle->Tx_DMA_Handle.DMAx);                                \
    LL_DMA_ClearFlag_FE1(pHandle->Rx_DMA_Handle.DMAx);                                \
    LL_DMA_ClearFlag_HT0(pHandle->Tx_DMA_Handle.DMAx);                                \
    LL_DMA_ClearFlag_HT1(pHandle->Rx_DMA_Handle.DMAx);                                \
    LL_DMA_ClearFlag_TC0(pHandle->Tx_DMA_Handle.DMAx);                                \
    LL_DMA_ClearFlag_TC1(pHandle->Rx_DMA_Handle.DMAx);                                \
    LL_DMA_ClearFlag_TE0(pHandle->Tx_DMA_Handle.DMAx);                                \
    LL_DMA_ClearFlag_TE1(pHandle->Rx_DMA_Handle.DMAx)

#define SPI_CHECK_READY_AND_CLEAR(pHandle) \
    while (SPI_IS_READY(pHandle)) {        \
        SPI_ClearStatusAndFlag(pHandle);   \
    }
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
 * @brief Register SPI handle
 *
 * @param pHandle Pointer to a SPI handle which contains the configuration information for the specified SPI module.
 * @param Instance SPI registers base address
 * @param SSB_Pin SSB pin selection
 * @param SCLK_Pin SCLK pin selection
 * @param MOSI_Pin MOSI pin selection
 * @param MISO_Pin MISO pin selection
 * @param Tx_DMA_Handle Tx DMA handle parameters
 * @param Rx_DMA_Handle Rx DMA handle parameters
 */
void SPI_RegisterHandle(SPI_Handle_t *pHandle, SPI_TypeDef *Instance, SPI_Pin_t SSB_Pin, SPI_Pin_t SCLK_Pin, SPI_Pin_t MOSI_Pin, SPI_Pin_t MISO_Pin,
                        SPI_DMA_Handle_t Tx_DMA_Handle, SPI_DMA_Handle_t Rx_DMA_Handle) {
    pHandle->Instance      = Instance;
    pHandle->SSB_Pin       = SSB_Pin;
    pHandle->SCLK_Pin      = SCLK_Pin;
    pHandle->MOSI_Pin      = MOSI_Pin;
    pHandle->MISO_Pin      = MISO_Pin;
    pHandle->Tx_DMA_Handle = Tx_DMA_Handle;
    pHandle->Rx_DMA_Handle = Rx_DMA_Handle;
}

/**
 * @brief Enable SPI
 *
 * @param pHandle Pointer to a SPI handle which contains the configuration information for the specified SPI module.
 */
void SPI_Enable(SPI_Handle_t *pHandle) {
    LL_SPI_InitTypeDef SPI_InitStruct = {0};
    SPI_InitStruct.TransferDirection  = LL_SPI_FULL_DUPLEX;
    SPI_InitStruct.Mode               = LL_SPI_MODE_MASTER;
    SPI_InitStruct.DataWidth          = LL_SPI_DATAWIDTH_8BIT;
    SPI_InitStruct.ClockPolarity      = LL_SPI_POLARITY_HIGH;
    SPI_InitStruct.ClockPhase         = LL_SPI_PHASE_2EDGE;
    SPI_InitStruct.NSS                = LL_SPI_NSS_SOFT;
    SPI_InitStruct.BaudRate           = LL_SPI_BAUDRATEPRESCALER_DIV4;
    SPI_InitStruct.BitOrder           = LL_SPI_MSB_FIRST;
    SPI_InitStruct.CRCCalculation     = LL_SPI_CRCCALCULATION_DISABLE;
    SPI_InitStruct.CRCPoly            = 7;
    LL_SPI_Init(pHandle->Instance, &SPI_InitStruct);
    LL_SPI_SetStandard(pHandle->Instance, LL_SPI_PROTOCOL_MOTOROLA);
    LL_SPI_DisableNSSPulseMgt(pHandle->Instance);

    LL_SPI_Enable(pHandle->Instance);
}

/**
 * @brief Disable SPI
 *
 * @param pHandle Pointer to a SPI handle which contains the configuration information for the specified SPI module.
 */
void SPI_Disable(SPI_Handle_t *pHandle) {
    LL_SPI_DeInit(pHandle->Instance);
    LL_GPIO_ResetOutputPin(pHandle->SSB_Pin.Port, pHandle->SSB_Pin.Pin);
    LL_GPIO_ResetOutputPin(pHandle->SCLK_Pin.Port, pHandle->SCLK_Pin.Pin);
    LL_GPIO_ResetOutputPin(pHandle->MOSI_Pin.Port, pHandle->MOSI_Pin.Pin);
}

/**
 * @brief Set SPI mode
 *
 * @param pHandle Pointer to a SPI handle which contains the configuration information for the specified SPI module.
 * @param mode SPI mode, 0: CPOL=0, CPHA=0; 1: CPOL=0, CPHA=1; 2: CPOL=1, CPHA=0; 3: CPOL=1, CPHA=1
 */
void SPI_SetMode(SPI_Handle_t *pHandle, uint8_t mode) {
    assert_param(IS_SPI_MODE(mode));

    if (!LL_SPI_IsEnabled(pHandle->Instance)) {
        SPI_Enable(pHandle);
    }

    switch (mode) {
        case 0:
            LL_SPI_SetClockPhase(pHandle->Instance, LL_SPI_PHASE_1EDGE);
            LL_SPI_SetClockPolarity(pHandle->Instance, LL_SPI_POLARITY_LOW);
            break;

        case 1:
            LL_SPI_SetClockPhase(pHandle->Instance, LL_SPI_PHASE_2EDGE);
            LL_SPI_SetClockPolarity(pHandle->Instance, LL_SPI_POLARITY_LOW);
            break;

        case 2:
            LL_SPI_SetClockPhase(pHandle->Instance, LL_SPI_PHASE_1EDGE);
            LL_SPI_SetClockPolarity(pHandle->Instance, LL_SPI_POLARITY_HIGH);
            break;

        case 3:
            LL_SPI_SetClockPhase(pHandle->Instance, LL_SPI_PHASE_2EDGE);
            LL_SPI_SetClockPolarity(pHandle->Instance, LL_SPI_POLARITY_HIGH);
            break;

        default:
            break;
    }

    SPI_TransmitData(pHandle, 0x00);
}

/**
 * @brief Get SPI mode
 *
 * @param pHandle Pointer to a SPI handle which contains the configuration information for the specified SPI module.
 * @return SPI mode, 0: CPOL=0, CPHA=0; 1: CPOL=0, CPHA=1; 2: CPOL=1, CPHA=0; 3: CPOL=1, CPHA=1
 */
uint8_t SPI_GetMode(SPI_Handle_t *pHandle) {
    uint32_t phase    = LL_SPI_GetClockPhase(pHandle->Instance);
    uint32_t polarity = LL_SPI_GetClockPolarity(pHandle->Instance);
    if (phase == LL_SPI_PHASE_1EDGE && polarity == LL_SPI_POLARITY_LOW)
        return 0;
    else if (phase == LL_SPI_PHASE_2EDGE && polarity == LL_SPI_POLARITY_LOW)
        return 1;
    else if (phase == LL_SPI_PHASE_1EDGE && polarity == LL_SPI_POLARITY_HIGH)
        return 2;
    else if (phase == LL_SPI_PHASE_2EDGE && polarity == LL_SPI_POLARITY_HIGH)
        return 3;
    else
        return 0;
}

/**
 * @brief Set SPI clock divider
 *
 * @param pHandle Pointer to a SPI handle which contains the configuration information for the specified SPI module.
 * @param div SPI clock divider, 2, 4, 8, 16, 32, 64, 128, 255
 */
void SPI_SetClockDiv(SPI_Handle_t *pHandle, uint8_t div) {
    assert_param(IS_SPI_CLK_DIV(div));

    if (!LL_SPI_IsEnabled(pHandle->Instance)) {
        SPI_Enable(pHandle);
    }

    if (div == 2)
        LL_SPI_SetBaudRatePrescaler(pHandle->Instance, LL_SPI_BAUDRATEPRESCALER_DIV2);
    else if (div == 4)
        LL_SPI_SetBaudRatePrescaler(pHandle->Instance, LL_SPI_BAUDRATEPRESCALER_DIV4);
    else if (div == 8)
        LL_SPI_SetBaudRatePrescaler(pHandle->Instance, LL_SPI_BAUDRATEPRESCALER_DIV8);
    else if (div == 16)
        LL_SPI_SetBaudRatePrescaler(pHandle->Instance, LL_SPI_BAUDRATEPRESCALER_DIV16);
    else if (div == 32)
        LL_SPI_SetBaudRatePrescaler(pHandle->Instance, LL_SPI_BAUDRATEPRESCALER_DIV32);
    else if (div == 64)
        LL_SPI_SetBaudRatePrescaler(pHandle->Instance, LL_SPI_BAUDRATEPRESCALER_DIV64);
    else if (div == 128)
        LL_SPI_SetBaudRatePrescaler(pHandle->Instance, LL_SPI_BAUDRATEPRESCALER_DIV128);
    else if (div == 255)
        LL_SPI_SetBaudRatePrescaler(pHandle->Instance, LL_SPI_BAUDRATEPRESCALER_DIV256);

    SPI_TransmitData(pHandle, 0x00);
}

/**
 * @brief Get SPI clock divider
 *
 * @param pHandle Pointer to a SPI handle which contains the configuration information for the specified SPI module.
 * @return SPI clock divider, 2, 4, 8, 16, 32, 64, 128, 255
 */
uint8_t SPI_GetClockDiv(SPI_Handle_t *pHandle) {
    uint32_t prescaler = LL_SPI_GetBaudRatePrescaler(pHandle->Instance);
    if (prescaler == LL_SPI_BAUDRATEPRESCALER_DIV2)
        return 2;
    else if (prescaler == LL_SPI_BAUDRATEPRESCALER_DIV4)
        return 4;
    else if (prescaler == LL_SPI_BAUDRATEPRESCALER_DIV8)
        return 8;
    else if (prescaler == LL_SPI_BAUDRATEPRESCALER_DIV16)
        return 16;
    else if (prescaler == LL_SPI_BAUDRATEPRESCALER_DIV32)
        return 32;
    else if (prescaler == LL_SPI_BAUDRATEPRESCALER_DIV64)
        return 64;
    else if (prescaler == LL_SPI_BAUDRATEPRESCALER_DIV128)
        return 128;
    else if (prescaler == LL_SPI_BAUDRATEPRESCALER_DIV256)
        return 255;
    else
        return 0;
}

/**
 * @brief Set SPI SSB pin state
 *
 * @param pHandle Pointer to a SPI handle which contains the configuration information for the specified SPI module.
 * @param state SSB pin state, true: high; false: low
 */
void SPI_SetSSB(SPI_Handle_t *pHandle, bool state) {
    if (state)
        LL_GPIO_SetOutputPin(pHandle->SSB_Pin.Port, pHandle->SSB_Pin.Pin);
    else
        LL_GPIO_ResetOutputPin(pHandle->SSB_Pin.Port, pHandle->SSB_Pin.Pin);
}

/**
 * @brief Transmit data via SPI
 *
 * @param pHandle Pointer to a SPI handle which contains the configuration information for the specified SPI module.
 * @param data Data to be transmitted
 */
void SPI_TransmitData(SPI_Handle_t *pHandle, uint8_t data) {
    const uint32_t wait = 25000;
    uint32_t counter    = 0;
    /* Transmit value, cause SPI is full-duplex, it needs to transmit and receive data whether you need it or not */
    LL_SPI_TransmitData8(pHandle->Instance, data);
    while (!LL_SPI_IsActiveFlag_TXE(pHandle->Instance)) {
        counter++;
        if (counter > wait) break;
    }
    counter = 0;
    while (!LL_SPI_IsActiveFlag_RXNE(pHandle->Instance)) {
        counter++;
        if (counter > wait) break;
    }
    counter = 0;
    LL_SPI_ReceiveData8(pHandle->Instance);
}

/**
 * @brief Receive data via SPI
 *
 * @param pHandle Pointer to a SPI handle which contains the configuration information for the specified SPI module.
 * @return Received data
 */
uint8_t SPI_ReceiveData(SPI_Handle_t *pHandle) {
    const uint32_t wait = 25000;
    uint32_t counter    = 0;
    /* Receive value, cause SPI is full-duplex, it needs to transmit and receive data whether you need it or not */
    LL_SPI_TransmitData8(pHandle->Instance, 0x00);
    while (!LL_SPI_IsActiveFlag_TXE(pHandle->Instance)) {
        counter++;
        if (counter > wait) break;
    }
    counter = 0;
    while (!LL_SPI_IsActiveFlag_RXNE(pHandle->Instance)) {
        counter++;
        if (counter > wait) break;
    }
    counter       = 0;
    uint8_t value = LL_SPI_ReceiveData8(pHandle->Instance);
    return value;
}

/**
 * @brief Transmit data via SPI with DMA (non-blocking)
 *
 * @param pHandle Pointer to a SPI handle which contains the configuration information for the specified SPI module.
 * @param pData Pointer to data to be transmitted
 * @param Size Data size to be transmitted
 */
void SPI_TransmitData_DMA(SPI_Handle_t *pHandle, uint8_t *pData, uint32_t Size) {
    /* Transmit value via DMA, cause SPI is full-duplex, it needs to transmit and receive data whether you need it or not */
    uint8_t value = 0;
    LL_DMA_ConfigAddresses(pHandle->Tx_DMA_Handle.DMAx, pHandle->Tx_DMA_Handle.Stream, (uint32_t)LL_SPI_DMA_GetRegAddr(pHandle->Instance),
                           (uint32_t)(&value), LL_DMA_GetDataTransferDirection(pHandle->Tx_DMA_Handle.DMAx, pHandle->Tx_DMA_Handle.Stream));
    LL_DMA_SetDataLength(pHandle->Tx_DMA_Handle.DMAx, pHandle->Tx_DMA_Handle.Stream, Size);

    LL_DMA_ConfigAddresses(pHandle->Rx_DMA_Handle.DMAx, pHandle->Rx_DMA_Handle.Stream, (uint32_t)(&pData[0]),
                           (uint32_t)LL_SPI_DMA_GetRegAddr(pHandle->Instance),
                           LL_DMA_GetDataTransferDirection(pHandle->Rx_DMA_Handle.DMAx, pHandle->Rx_DMA_Handle.Stream));
    LL_DMA_SetDataLength(pHandle->Rx_DMA_Handle.DMAx, pHandle->Rx_DMA_Handle.Stream, Size);

    LL_SPI_EnableDMAReq_RX(pHandle->Instance);
    LL_SPI_EnableDMAReq_TX(pHandle->Instance);
    LL_DMA_EnableIT_TC(pHandle->Tx_DMA_Handle.DMAx, pHandle->Tx_DMA_Handle.Stream);
    LL_DMA_EnableIT_TC(pHandle->Rx_DMA_Handle.DMAx, pHandle->Rx_DMA_Handle.Stream);
    LL_DMA_EnableStream(pHandle->Tx_DMA_Handle.DMAx, pHandle->Tx_DMA_Handle.Stream);
    LL_DMA_EnableStream(pHandle->Rx_DMA_Handle.DMAx, pHandle->Rx_DMA_Handle.Stream);
}

/**
 * @brief Transmit data via SPI with DMA (blocking)
 *
 * @param pHandle Pointer to a SPI handle which contains the configuration information for the specified SPI module.
 * @param pData Pointer to data to be transmitted
 * @param Size Data size to be transmitted
 */
void SPI_TransmitData_DMA_Blocking(SPI_Handle_t *pHandle, uint8_t *pData, uint32_t Size) {
    /* Transmit value via DMA, cause SPI is full-duplex, it needs to transmit and receive data whether you need it or not */
    uint8_t value = 0;
    LL_DMA_ConfigAddresses(pHandle->Tx_DMA_Handle.DMAx, pHandle->Tx_DMA_Handle.Stream, (uint32_t)LL_SPI_DMA_GetRegAddr(pHandle->Instance),
                           (uint32_t)(&value), LL_DMA_GetDataTransferDirection(pHandle->Tx_DMA_Handle.DMAx, pHandle->Tx_DMA_Handle.Stream));
    LL_DMA_SetDataLength(pHandle->Tx_DMA_Handle.DMAx, pHandle->Tx_DMA_Handle.Stream, Size);

    LL_DMA_ConfigAddresses(pHandle->Rx_DMA_Handle.DMAx, pHandle->Rx_DMA_Handle.Stream, (uint32_t)(&pData[0]),
                           (uint32_t)LL_SPI_DMA_GetRegAddr(pHandle->Instance),
                           LL_DMA_GetDataTransferDirection(pHandle->Rx_DMA_Handle.DMAx, pHandle->Rx_DMA_Handle.Stream));
    LL_DMA_SetDataLength(pHandle->Rx_DMA_Handle.DMAx, pHandle->Rx_DMA_Handle.Stream, Size);

    LL_SPI_EnableDMAReq_RX(pHandle->Instance);
    LL_SPI_EnableDMAReq_TX(pHandle->Instance);
    LL_DMA_EnableIT_TC(pHandle->Tx_DMA_Handle.DMAx, pHandle->Tx_DMA_Handle.Stream);
    LL_DMA_EnableIT_TC(pHandle->Rx_DMA_Handle.DMAx, pHandle->Rx_DMA_Handle.Stream);
    LL_DMA_EnableStream(pHandle->Tx_DMA_Handle.DMAx, pHandle->Tx_DMA_Handle.Stream);
    LL_DMA_EnableStream(pHandle->Rx_DMA_Handle.DMAx, pHandle->Rx_DMA_Handle.Stream);

    SPI_CHECK_READY_AND_CLEAR(pHandle);
}

/**
 * @brief Receive data via SPI with DMA (non-blocking)
 *
 * @param pHandle Pointer to a SPI handle which contains the configuration information for the specified SPI module.
 * @param pData Pointer to data to be received
 * @param Size Data size to be received
 */
void SPI_ReceiveData_DMA(SPI_Handle_t *pHandle, uint8_t *pData, uint32_t Size) {
    /* Transmit and receive value via DMA, cause SPI is full-duplex, it needs to transmit and receive data whether you need it or not  */
    LL_DMA_ConfigAddresses(pHandle->Rx_DMA_Handle.DMAx, pHandle->Rx_DMA_Handle.Stream, (uint32_t)(&pData[0]),
                           (uint32_t)LL_SPI_DMA_GetRegAddr(pHandle->Instance),
                           LL_DMA_GetDataTransferDirection(pHandle->Rx_DMA_Handle.DMAx, pHandle->Rx_DMA_Handle.Stream));
    LL_DMA_SetDataLength(pHandle->Rx_DMA_Handle.DMAx, pHandle->Rx_DMA_Handle.Stream, Size);

    LL_DMA_ConfigAddresses(pHandle->Tx_DMA_Handle.DMAx, pHandle->Tx_DMA_Handle.Stream, (uint32_t)LL_SPI_DMA_GetRegAddr(pHandle->Instance),
                           (uint32_t)(&pData[0]), LL_DMA_GetDataTransferDirection(pHandle->Tx_DMA_Handle.DMAx, pHandle->Tx_DMA_Handle.Stream));
    LL_DMA_SetDataLength(pHandle->Tx_DMA_Handle.DMAx, pHandle->Tx_DMA_Handle.Stream, Size);

    LL_SPI_EnableDMAReq_TX(pHandle->Instance);
    LL_SPI_EnableDMAReq_RX(pHandle->Instance);
    LL_DMA_EnableIT_TC(pHandle->Tx_DMA_Handle.DMAx, pHandle->Tx_DMA_Handle.Stream);
    LL_DMA_EnableIT_TC(pHandle->Rx_DMA_Handle.DMAx, pHandle->Rx_DMA_Handle.Stream);
    LL_DMA_EnableStream(pHandle->Tx_DMA_Handle.DMAx, pHandle->Tx_DMA_Handle.Stream);
    LL_DMA_EnableStream(pHandle->Rx_DMA_Handle.DMAx, pHandle->Rx_DMA_Handle.Stream);
}

/**
 * @brief Receive data via SPI with DMA (blocking)
 *
 * @param pHandle Pointer to a SPI handle which contains the configuration information for the specified SPI module.
 * @param pData Pointer to data to be received
 * @param Size Data size to be received
 */
void SPI_ReceiveData_DMA_Blocking(SPI_Handle_t *pHandle, uint8_t *pData, uint32_t Size) {
    /* Transmit and receive value via DMA, cause SPI is full-duplex, it needs to transmit and receive data whether you need it or not  */
    LL_DMA_ConfigAddresses(pHandle->Rx_DMA_Handle.DMAx, pHandle->Rx_DMA_Handle.Stream, (uint32_t)(&pData[0]),
                           (uint32_t)LL_SPI_DMA_GetRegAddr(pHandle->Instance),
                           LL_DMA_GetDataTransferDirection(pHandle->Rx_DMA_Handle.DMAx, pHandle->Rx_DMA_Handle.Stream));
    LL_DMA_SetDataLength(pHandle->Rx_DMA_Handle.DMAx, pHandle->Rx_DMA_Handle.Stream, Size);

    LL_DMA_ConfigAddresses(pHandle->Tx_DMA_Handle.DMAx, pHandle->Tx_DMA_Handle.Stream, (uint32_t)LL_SPI_DMA_GetRegAddr(pHandle->Instance),
                           (uint32_t)(&pData[0]), LL_DMA_GetDataTransferDirection(pHandle->Tx_DMA_Handle.DMAx, pHandle->Tx_DMA_Handle.Stream));
    LL_DMA_SetDataLength(pHandle->Tx_DMA_Handle.DMAx, pHandle->Tx_DMA_Handle.Stream, Size);

    LL_SPI_EnableDMAReq_TX(pHandle->Instance);
    LL_SPI_EnableDMAReq_RX(pHandle->Instance);

    LL_DMA_EnableIT_TC(pHandle->Tx_DMA_Handle.DMAx, pHandle->Tx_DMA_Handle.Stream);
    LL_DMA_EnableIT_TC(pHandle->Rx_DMA_Handle.DMAx, pHandle->Rx_DMA_Handle.Stream);
    LL_DMA_EnableStream(pHandle->Tx_DMA_Handle.DMAx, pHandle->Tx_DMA_Handle.Stream);
    LL_DMA_EnableStream(pHandle->Rx_DMA_Handle.DMAx, pHandle->Rx_DMA_Handle.Stream);

    SPI_CHECK_READY_AND_CLEAR(pHandle);
}
#pragma endregion Exported Functions

/*  */
#pragma region Private Functions

#pragma endregion Private Functions

#ifdef __cplusplus
}
#endif
