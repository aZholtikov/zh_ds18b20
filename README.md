# ESP32 ESP-IDF and ESP8266 RTOS SDK component for 1-Wire DS18B20 sensor

## Tested on

1. ESP8266 RTOS_SDK v3.4
2. ESP32 ESP-IDF v5.2

## Dependencies

1. [zh_onewire](https://github.com/aZholtikov/zh_onewire.git)

## Using

In an existing project, run the following command to install the components:

```text
cd ../your_project/components
git clone https://github.com/aZholtikov/zh_onewire.git
git clone https://github.com/aZholtikov/zh_ds18b20.git
```

In the application, add the component:

```c
#include "zh_ds18b20.h"
```

## Examples

One or more 1-Wire DS18B20 sensors on bus:

```c
#include "zh_ds18b20.h"

void app_main(void)
{
    esp_log_level_set("zh_onewire", ESP_LOG_NONE);
    esp_log_level_set("zh_ds18b20", ESP_LOG_NONE);
    uint8_t *rom = NULL;
    float temperature = 0.0;
    zh_onewire_init(GPIO_NUM_5);
    if (zh_onewire_reset() != ESP_OK)
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
            printf("\n");
            rom -= 8;
            zh_ds18b20_read(rom, &temperature);
            printf("Temperature: %0.2f\n", temperature);
        }
    }
}
```

Any [feedback](mailto:github@azholtikov.ru) will be gladly accepted.
