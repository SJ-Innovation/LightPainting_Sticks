#include "Arduino.h"
#include "hardware_config.h"
#include <FastLED.h>
#include <SD.h>

// 0 is at the top, (STICK_NUM_LEDS-1) is at the bottom
CRGB Stick_LEDS[STICK_NUM_LEDS];
File SDReadFile;
int PixelHoldTime = 0;

void Init_IO() {
    pinMode(PIN_BUTTON1, INPUT_PULLUP);
    pinMode(PIN_BUTTON2, INPUT_PULLUP);
    pinMode(PIN_BUTTON3, INPUT_PULLUP);
    pinMode(PIN_LED1, OUTPUT);
    pinMode(PIN_LED2, OUTPUT);
    pinMode(PIN_LED3, OUTPUT);
}

void Init_SD() {
    while (!SD.begin(SD_CS_PIN)) {
        Serial.println(F("Retrying SD Init"));
    }
    while (!(SDReadFile = SD.open("/printme.lsk", FILE_READ))) {
        Serial.println(F("Cannot Find printme.lsk"));
    }
}

void Init_LEDS() {
    FastLED.addLeds<WS2812B, STICK_DATA_PIN, GRB>(Stick_LEDS, STICK_NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, PSU_MAX_CURRENT_MA);
}

#define CACHE_SIZE ((STICK_NUM_LEDS/READS_PER_CYCLE)*3)
byte Cache[CACHE_SIZE];
int CurrentCacheLoc = CACHE_SIZE;

byte ReadPixelPartByte() { // Called in sequence. Reads seq from cache / SD
    if (CurrentCacheLoc >= CACHE_SIZE) { // Have we read all cached?
        CurrentCacheLoc = 0;
        memset8(Cache, 0, CACHE_SIZE); // Blank the cache
        SDReadFile.read(Cache, CACHE_SIZE); // Read a block from SD
    }
    return Cache[CurrentCacheLoc++]; // Next unused element from cache
}

void ReadNextLineLEDs() {
    for (int PixNum = 0; PixNum < STICK_NUM_LEDS; PixNum++) {
        Stick_LEDS[PixNum].r = ReadPixelPartByte();
        Stick_LEDS[PixNum].g = ReadPixelPartByte();
        Stick_LEDS[PixNum].b = ReadPixelPartByte();
    }
}


void setup() { // Runs once on connection to power
    Serial.begin(115200);
    Init_IO();
    Init_LEDS();
    Init_SD();
    SDReadFile.seek(0); // Move to start of file
    PixelHoldTime = SDReadFile.read() << 8; // First byte in file
    PixelHoldTime = PixelHoldTime | (SDReadFile.read() & 0xFF); // Second byte in file
}





void loop() { // Runs repeatedly until the end of time
    unsigned long StartTime = millis();
    ReadNextLineLEDs();
    FastLED.show();
    unsigned long ExecTook = millis() - StartTime;
    int DelayTime = PixelHoldTime - ExecTook;
    if (DelayTime > PixelHoldTime || DelayTime < 0) {
        DelayTime = 0;
    }
    delay(DelayTime);
}