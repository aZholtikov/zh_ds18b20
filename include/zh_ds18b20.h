#pragma once

#include "zh_onewire.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief      Initialize 1-Wire interface.
     *
     * @param[in]  pin  1-Wire bus gpio connection.
     *
     * @return
     *              - ESP_OK if initialization was successful
     *              - ESP_ERR_INVALID_ARG if select incorrect gpio number
     */
    esp_err_t zh_ds18b20_init(const uint8_t pin);

    /**
     * @brief      Read temperature from a specific 1-Wire device on bus.
     *
     * @param[in]  device        Pointer to a buffer containing an eight-byte rom value.
     *                           Can be NULL if only one 1-Wire device is present on the bus.
     * @param[out]  temperature  Pointer to a buffer containing the received temperature.
     *
     * @return
     *              - ESP_OK if read was successful
     *              - ESP_FAIL if there are no 1-Wire device available on the bus or the device is not responding
     *              - ESP_ERR_INVALID_CRC if more than one 1-Wire device is present on the bus (case device is NULL)
     *              - ESP_ERR_INVALID_RESPONSE if device is available but not responding or incorrect rom value (case device is not NULL)
     */
    esp_err_t zh_ds18b20_read_temp(const uint8_t *device, float *temperature);

#ifdef __cplusplus
}
#endif