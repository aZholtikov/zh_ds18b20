# ESP32 ESP-IDF and ESP8266 RTOS SDK component for 1-Wire DS18B20 sensor

There are two branches - for ESP8266 family and for ESP32 family. Please use the appropriate one.

## Using

In an existing project, run the following command to install the components:

```text
For ESP8266 family:

cd ../your_project/components
git clone -b esp8266 --recursive http://git.zh.com.ru/alexey.zholtikov/zh_onewire.git
git clone http://git.zh.com.ru/alexey.zholtikov/zh_ds18b20.git

For ESP32 family:

cd ../your_project/components
git clone -b esp32 --recursive http://git.zh.com.ru/alexey.zholtikov/zh_onewire.git
git clone http://git.zh.com.ru/alexey.zholtikov/zh_ds18b20.git
```

In the application, add the component:

```c
#include "zh_ds18b20.h"
```

## Examples

Only one 1-Wire DS18B20 sensor on bus:

```c
#include "stdio.h"
#include "zh_ds18b20.h"

void app_main(void)
{
    float temperature = 0;
    ESP_ERROR_CHECK(zh_ds18b20_init(GPIO));
    switch (zh_ds18b20_read_temp(NULL, &temperature))
    {
    case ESP_OK:
        printf("Temperature: %f\n", temperature);
        break;
    case ESP_FAIL:
        printf("There are no 1-Wire device available on the bus or the device is not responding.\n");
        break;
    case ESP_ERR_INVALID_CRC:
        printf("More than one 1-Wire device is present on the bus.\n");
        break;
    case ESP_ERR_INVALID_RESPONSE:
        printf("Device is available but not responding or incorrect rom value.\n");
        break;
    default:
        break;
    }
}
```

One or more 1-Wire DS18B20 sensors on bus:

```c
#include "stdio.h"
#include "zh_ds18b20.h"

void app_main(void)
{
    uint8_t *rom = NULL;
    float temperature = 0;
    ESP_ERROR_CHECK(zh_ds18b20_init(GPIO));
    if (zh_onewire_reset() == ESP_FAIL)
    {
        printf("There are no 1-Wire devices available on the bus.\n");
    }
    else
    {
        zh_onewire_search_rom_init();
        for (;;)
        {
            rom = zh_onewire_search_rom_next();
            if (rom == NULL)
            {
                break;
            }
            printf("Found device ROM: ");
            for (uint8_t i = 0; i < 8; ++i)
            {
                printf("%X ", *(rom++));
            }
            rom -= 8;
            zh_ds18b20_read_temp(rom, &temperature);
            printf("Temperature: %f\n", temperature);
        }
    }
}
```