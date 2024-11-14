#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>

//Battery Section
#include "Wire.h"
#include "MAX17043.h"
//Battery Section

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


// 'arrive', 40x40px
const unsigned char epd_bitmap_arrive [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x7e, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x0f, 0xff, 
	0xe0, 0x00, 0x00, 0x0f, 0xff, 0xf0, 0x00, 0x00, 0x1f, 0xff, 0xf0, 0x00, 0x00, 0x1f, 0xc7, 0xf8, 
	0x00, 0x00, 0x3f, 0x83, 0xf8, 0x00, 0x00, 0x3f, 0x01, 0xf8, 0x00, 0x00, 0x3f, 0x01, 0xf8, 0x00, 
	0x00, 0x3f, 0x01, 0xf8, 0x00, 0x00, 0x3f, 0x83, 0xf8, 0x00, 0x00, 0x3f, 0xc7, 0xf8, 0x00, 0x00, 
	0x1f, 0xff, 0xf0, 0x00, 0x00, 0x1f, 0xff, 0xf0, 0x00, 0x00, 0x0f, 0xff, 0xf0, 0x00, 0x00, 0x0f, 
	0xff, 0xe0, 0x00, 0x00, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x03, 0xff, 
	0x80, 0x00, 0x00, 0x03, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 
	0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 
	0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'left', 40x40px
const unsigned char epd_bitmap_left [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x01, 0xc0, 
	0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 
	0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xf8, 0x00, 
	0x00, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x3c, 0x00, 0x0f, 0x00, 0x00, 
	0x1e, 0x00, 0x07, 0x00, 0x00, 0x0f, 0x00, 0x07, 0x00, 0x00, 0x07, 0x80, 0x07, 0x00, 0x00, 0x03, 
	0xc0, 0x07, 0x00, 0x00, 0x01, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 
	0x07, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x07, 
	0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'right', 40x40px
const unsigned char epd_bitmap_right [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x01, 
	0xc0, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x78, 
	0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x0f, 0xff, 0xff, 0x00, 
	0x00, 0x1f, 0xff, 0xff, 0x80, 0x00, 0x3f, 0xff, 0xff, 0x00, 0x00, 0x78, 0x00, 0x1e, 0x00, 0x00, 
	0x70, 0x00, 0x3c, 0x00, 0x00, 0x70, 0x00, 0x78, 0x00, 0x00, 0x70, 0x00, 0xf0, 0x00, 0x00, 0x70, 
	0x01, 0xe0, 0x00, 0x00, 0x70, 0x01, 0xc0, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 
	0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 
	0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'sharpleft', 40x40px
const unsigned char epd_bitmap_sharpleft [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 
	0x78, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x00, 0x03, 0xfc, 
	0x00, 0x00, 0x00, 0x07, 0x9c, 0x00, 0x00, 0x00, 0x0f, 0x1c, 0x00, 0x00, 0x00, 0x1f, 0x1c, 0x00, 
	0x00, 0x10, 0x3c, 0x1c, 0x00, 0x00, 0x70, 0x78, 0x1c, 0x00, 0x00, 0x78, 0xf0, 0x1c, 0x00, 0x00, 
	0x79, 0xe0, 0x1c, 0x00, 0x00, 0x7b, 0xc0, 0x1c, 0x00, 0x00, 0x7f, 0x80, 0x1c, 0x00, 0x00, 0x7f, 
	0x00, 0x1c, 0x00, 0x00, 0x7e, 0x00, 0x1c, 0x00, 0x00, 0x7f, 0xf0, 0x1c, 0x00, 0x00, 0x7f, 0xf0, 
	0x1c, 0x00, 0x00, 0x3f, 0xf0, 0x1c, 0x00, 0x00, 0x10, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x1c, 
	0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'sharpright', 40x40px
const unsigned char epd_bitmap_sharpright [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 
	0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 
	0x00, 0x00, 0x7b, 0xc0, 0x00, 0x00, 0x00, 0x79, 0xe0, 0x00, 0x00, 0x00, 0x78, 0xf0, 0x00, 0x00, 
	0x00, 0x78, 0x78, 0x08, 0x00, 0x00, 0x78, 0x3c, 0x1c, 0x00, 0x00, 0x78, 0x1e, 0x1c, 0x00, 0x00, 
	0x78, 0x0f, 0x1c, 0x00, 0x00, 0x78, 0x07, 0x9c, 0x00, 0x00, 0x78, 0x03, 0xdc, 0x00, 0x00, 0x78, 
	0x01, 0xfc, 0x00, 0x00, 0x78, 0x00, 0xfc, 0x00, 0x00, 0x78, 0x0f, 0xfc, 0x00, 0x00, 0x78, 0x1f, 
	0xfc, 0x00, 0x00, 0x78, 0x1f, 0xfc, 0x00, 0x00, 0x78, 0x00, 0x40, 0x00, 0x00, 0x78, 0x00, 0x00, 
	0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'slightleft', 40x40px
const unsigned char epd_bitmap_slightleft [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x00, 0x1f, 
	0xfc, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x1f, 0xe0, 
	0x00, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x00, 0x00, 0x1e, 0xf8, 0x00, 0x00, 0x00, 0x1e, 0x7c, 0x00, 
	0x00, 0x00, 0x1e, 0x3e, 0x00, 0x00, 0x00, 0x0c, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 
	0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 
	0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 
	0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 
	0x38, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x38, 
	0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 
	0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 
	0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'slightright', 40x40px
const unsigned char epd_bitmap_slightright [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x00, 
	0x3f, 0xf8, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x07, 
	0xf8, 0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x00, 0x00, 0x1f, 0x78, 0x00, 0x00, 0x00, 0x3e, 0x78, 
	0x00, 0x00, 0x00, 0x7c, 0x78, 0x00, 0x00, 0x00, 0xf8, 0x30, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 
	0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 
	0x0f, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x1c, 
	0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 
	0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 
	0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 
	0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 
	0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'straight', 40x40px
const unsigned char epd_bitmap_straight [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x3e, 
	0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xc0, 
	0x00, 0x00, 0x03, 0xdd, 0xe0, 0x00, 0x00, 0x07, 0x9c, 0xf0, 0x00, 0x00, 0x0f, 0x1c, 0x70, 0x00, 
	0x00, 0x06, 0x1c, 0x30, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 
	0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 
	0x1c, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x1c, 
	0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 
	0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 
	0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'uturn', 40x40px
const unsigned char epd_bitmap_uturn [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x07, 
	0xf8, 0x00, 0x00, 0x00, 0x0f, 0xfe, 0x00, 0x00, 0x00, 0x1f, 0xff, 0x00, 0x00, 0x00, 0x3c, 0x0f, 
	0x00, 0x00, 0x00, 0x78, 0x07, 0x80, 0x00, 0x00, 0x70, 0x03, 0x80, 0x00, 0x00, 0x70, 0x03, 0xc0, 
	0x00, 0x00, 0xf0, 0x01, 0xc0, 0x00, 0x00, 0xf0, 0x01, 0xc0, 0x00, 0x00, 0xf0, 0x01, 0xc0, 0x00, 
	0x00, 0xf0, 0x01, 0xc0, 0x00, 0x00, 0xf0, 0x01, 0xc0, 0x00, 0x00, 0xf0, 0x01, 0xc0, 0x00, 0x00, 
	0xf0, 0x01, 0xc0, 0x00, 0x00, 0xf0, 0x01, 0xc0, 0x00, 0x00, 0xf0, 0x11, 0xcc, 0x00, 0x00, 0xf0, 
	0x39, 0xde, 0x00, 0x00, 0xf0, 0x3f, 0xfc, 0x00, 0x00, 0xf0, 0x1f, 0xf8, 0x00, 0x00, 0xf0, 0x0f, 
	0xf0, 0x00, 0x00, 0xf0, 0x07, 0xe0, 0x00, 0x00, 0xf0, 0x03, 0xc0, 0x00, 0x00, 0xf0, 0x01, 0x80, 
	0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 
	0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Define the struct
struct BitmapDict {
    const char* key;
    const unsigned char* bitmap;
};


// Create the dictionary
BitmapDict bitmapDictionary[] = {
    { "left", epd_bitmap_left },
    { "right", epd_bitmap_right },
    { "straight", epd_bitmap_straight },
    { "slight left", epd_bitmap_slightleft },
    { "slight right", epd_bitmap_slightright },
    { "sharp left", epd_bitmap_sharpleft },
    { "sharp right", epd_bitmap_sharpright },
    { "uturn", epd_bitmap_uturn },
    { "arrive", epd_bitmap_arrive },
};

// Number of items in the dictionary
const int dictSize = sizeof(bitmapDictionary) / sizeof(bitmapDictionary[0]);

// Function to retrieve a bitmap by key
const unsigned char* getBitmap(const char* direction) {
    for (int i = 0; i < dictSize; i++) {
        if (strcmp(bitmapDictionary[i].key, direction) == 0) {
            return bitmapDictionary[i].bitmap;
        }
    }
    return nullptr; // Return null if not found
}

// OLED display setup
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
String receivedMessage = ""; // Store the received Bluetooth message
uint32_t value = 0;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
    }

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string value = pCharacteristic->getValue().c_str();

        if (value.length() > 0) {
            receivedMessage = ""; // Clear previous message
            for (int i = 0; i < value.length(); i++) {
                receivedMessage += value[i]; // Append each character to receivedMessage
            }
            Serial.print("Received Value: ");
            Serial.println(receivedMessage);

            JsonDocument doc;
            DeserializationError error = deserializeJson(doc, receivedMessage);
            
            display.clearDisplay();
            if (error) {
              Serial.print(F("deserializeJson() failed: "));
              Serial.println(error.f_str());
              display.println("deserializeJson() failed: ");
            }else{
              const char* turn = doc["turn"];
              const char* distance = doc["distance"];

              const unsigned char* bitmap = getBitmap(turn);
              // Display the new message on the OLED
              display.drawBitmap(44, 0,  bitmap, 40, 40, WHITE);
              display.setTextColor(WHITE);
              display.setCursor(44, 45);
              display.setTextSize(1.6);
              // display.println(turn); 
              display.print(distance);
              display.println(" m");
            }
            //Battery Section
            float volts = FuelGauge.voltage();
            float pcnt = FuelGauge.percent();
            Serial.printf("(%.1f%%)\n", pcnt);
            display.setCursor(90, 10);
            // char buffer[32];
            // sprintf(buffer, "%.0f mV (%.1f%%)", volts, pcnt);
            // display.println(buffer);
            display.setTextSize(1);
            display.printf("%.0f%%", pcnt);
            //Battery Section

            display.display();
        }
    }
};

void setup() {
    Serial.begin(115200);
    
    //Battery Section
    Wire.begin(SDA, SCL);
    if (!FuelGauge.begin(&Wire)) {
      Serial.println("MAX17043 NOT found.\n");
      while (true) {}
    }
    FuelGauge.reset();
    delay(250);
    FuelGauge.quickstart();
    delay(125);
    //Battery Section

    // Initialize OLED display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("Disconnected");
    
    //Battery Section
    float volts = FuelGauge.voltage();
    float pcnt = FuelGauge.percent();
    Serial.printf("%.0fmV (%.1f%%)\n", volts, pcnt);
    display.setCursor(90, 10);
    // char buffer[32];
    // sprintf(buffer, "%.0f mV (%.1f%%)", volts, pcnt);
    // display.println(buffer);
    display.setTextSize(1);
    display.printf("%.0f%%", pcnt);
    //Battery Section

    display.display();

    // Initialize BLE
    BLEDevice::init("Smart Helmet");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    BLEService *pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID,
                        BLECharacteristic::PROPERTY_READ   |
                        BLECharacteristic::PROPERTY_WRITE  |
                        BLECharacteristic::PROPERTY_NOTIFY |
                        BLECharacteristic::PROPERTY_INDICATE
                      );
    pCharacteristic->setCallbacks(new MyCallbacks());
    pCharacteristic->addDescriptor(new BLE2902());
    pService->start();
    
    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);
    BLEDevice::startAdvertising();
    Serial.println("Waiting for a client connection to notify...");
}

void loop() {
    if (deviceConnected) {
        pCharacteristic->setValue((uint8_t*)&value, 4);
        pCharacteristic->notify();
        value++;
        delay(500);
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 10);
        display.println("Start Advertising");
        //Battery Section
        float volts = FuelGauge.voltage();
        float pcnt = FuelGauge.percent();
        Serial.printf("%.0fmV (%.1f%%)\n", volts, pcnt);
        display.setCursor(90, 10);
        display.setTextSize(1);
        display.printf("%.0f%%", pcnt);

        //Battery Section
        display.display();
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
        display.println("Set Your Destination...");
    }
}
