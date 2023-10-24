#include "zh_ds18b20.h"

esp_err_t zh_ds18b20_init(const uint8_t pin)
{
    return zh_onewire_init(pin);
}

esp_err_t zh_ds18b20_read_temp(const uint8_t *device, float *temperature)
{
    esp_err_t err = 0;
    uint8_t temp[2] = {0};
    uint8_t rom[8] = {0};
    if (device == NULL)
    {
        err = zh_onewire_read_rom(rom);
        if (err != ESP_OK)
        {
            return err;
        }
        if (zh_onewire_skip_rom() != ESP_OK)
        {
            return ESP_FAIL;
        }
    }
    else
    {
        if (zh_onewire_match_rom(device) != ESP_OK)
        {
            return ESP_FAIL;
        }
    }
    zh_onewire_send_byte(0x44);
    vTaskDelay(750 / portTICK_PERIOD_MS);
    if (device == NULL)
    {
        err = zh_onewire_read_rom(rom);
        if (err != ESP_OK)
        {
            return err;
        }
        if (zh_onewire_skip_rom() != ESP_OK)
        {
            return ESP_FAIL;
        }
    }
    else
    {
        if (zh_onewire_match_rom(device) != ESP_OK)
        {
            return ESP_FAIL;
        }
    }
    zh_onewire_send_byte(0xBE);
    temp[0] = zh_onewire_read_byte();
    temp[1] = zh_onewire_read_byte();
    *temperature = (float)(temp[0] + (temp[1] * 256)) / 16;
    if (*temperature > 125 || *temperature < -55)
    {
        return ESP_ERR_INVALID_RESPONSE;
    }
    return ESP_OK;
}