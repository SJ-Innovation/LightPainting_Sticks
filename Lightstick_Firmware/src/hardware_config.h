#ifndef FIRMWARE_USER_CONFIG_H
#define FIRMWARE_USER_CONFIG_H

#define PIN_BUTTON1 2
#define PIN_BUTTON2 3
#define PIN_BUTTON3 4

#define PIN_LED1 5
#define PIN_LED2 6
#define PIN_LED3 7

#define STICK_NUM_LEDS 144
#define STICK_DATA_PIN 6

#define SD_CS_PIN 10
#define SD_MOSI_PIN 11
#define SD_MISO_PIN 12
#define SD_SCK_PIN 13

#define READS_PER_CYCLE 2 // Higher means more small reads from SD, lower = faster but more mem used.

#define PSU_MAX_CURRENT_MA 1000 // For WIE sticks, dont set more than 3A.

#endif //FIRMWARE_USER_CONFIG_H
