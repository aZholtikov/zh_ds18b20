#pragma once

#include "stdio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "zh_onewire.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Read temperature from a specific 1-Wire device on bus.
     *
     * @param[in] device Pointer to a buffer containing an eight-byte rom value. Can be NULL if only one 1-Wire device is present on the bus.
     * 
     * @param[out] temperature Pointer to a buffer containing the received temperature.
     * 
     * @attention 1-Wire bus must be initialized first. @code zh_onewire_init(GPIO_NUM_X) @endcode
     *
     * @return
     *              - ESP_OK if read was successful
     *              - ESP_FAIL if any error
     *              - ESP_ERR_INVALID_CRC if check CRC is fail
     */
    esp_err_t zh_ds18b20_read(const uint8_t *device, float *temperature);

#ifdef __cplusplus
}
#endif