#include "zh_ds18b20.h"

#define SCRATCHPAD_LENGTH 8 // Scratchpad length.

#define CONVERT_TEMPERATURE 0x44 // Initiates temperature conversion.
#define READ_SCRATCHPAD 0xBE     // Reads bytes from scratchpad.

#define pgm_read_byte(addr) (*(const uint8_t *)(addr))

static uint8_t _crc_calculate(const uint8_t *scratchpad);

static const char *TAG = "zh_ds18b20";

static const uint8_t _crc_table[] = {
    0x00, 0x5E, 0xBC, 0xE2, 0x61, 0x3F, 0xDD, 0x83,
    0xC2, 0x9C, 0x7E, 0x20, 0xA3, 0xFD, 0x1F, 0x41,
    0x00, 0x9D, 0x23, 0xBE, 0x46, 0xDB, 0x65, 0xF8,
    0x8C, 0x11, 0xAF, 0x32, 0xCA, 0x57, 0xE9, 0x74};

esp_err_t zh_ds18b20_read(const uint8_t *device, float *temperature)
{
    ESP_LOGI(TAG, "DS18B20 read begin.");
    uint8_t scratchpad[9];
    if (device == NULL)
    {
        if (zh_onewire_skip_rom() != ESP_OK)
        {
            ESP_LOGE(TAG, "DS18B20 read fail.");
            return ESP_FAIL;
        }
    }
    else
    {
        if (zh_onewire_match_rom(device) != ESP_OK)
        {
            ESP_LOGE(TAG, "DS18B20 read fail.");
            return ESP_FAIL;
        }
    }
    zh_onewire_send_byte(CONVERT_TEMPERATURE);
    vTaskDelay(750 / portTICK_PERIOD_MS);
    if (device == NULL)
    {
        if (zh_onewire_skip_rom() != ESP_OK)
        {
            ESP_LOGE(TAG, "DS18B20 read fail.");
            return ESP_FAIL;
        }
    }
    else
    {
        if (zh_onewire_match_rom(device) != ESP_OK)
        {
            ESP_LOGE(TAG, "DS18B20 read fail.");
            return ESP_FAIL;
        }
    }
    zh_onewire_send_byte(READ_SCRATCHPAD);
    for (uint8_t i = 0; i < 9; ++i)
    {
        scratchpad[i] = zh_onewire_read_byte();
    }
    if (scratchpad[8] != _crc_calculate(scratchpad))
    {
        ESP_LOGE(TAG, "DS18B20 read fail. Invalid CRC.");
        return ESP_ERR_INVALID_CRC;
    }
    *temperature = (float)(scratchpad[0] + (scratchpad[1] * 256)) / 16;
    ESP_LOGI(TAG, "DS18B20 read success.");
    return ESP_OK;
}

static uint8_t _crc_calculate(const uint8_t *scratchpad)
{
    uint8_t crc = 0;
    uint8_t length = SCRATCHPAD_LENGTH;
    while (length--)
    {
        crc = *scratchpad++ ^ crc;
        crc = pgm_read_byte(_crc_table + (crc & 0x0f)) ^ pgm_read_byte(_crc_table + 16 + ((crc >> 4) & 0x0f));
    }
    return crc;
}