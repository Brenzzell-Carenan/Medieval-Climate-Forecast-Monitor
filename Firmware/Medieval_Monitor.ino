#include <WiFi.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Trioe.h>
#include <TRIOE_ROOT_CA.h>
#include <math.h>

const char* WIFI_SSID     = "BatStateU-DevOps";
const char* WIFI_PASSWORD = "Dev3l$06";
const char* TELEMETRY_URL = "https://hub.trioe.dev/api/v1/devices/TRIOE-01B850E0ECC1/telemetry/";
const char* STATE_URL     = "https://hub.trioe.dev/api/v1/devices/TRIOE-01B850E0ECC1/state/";
const char* API_KEY       = "thub_moOXiZISNO_p4GjRnsnPBEua_PUWYx1ikHzHsXsBoTg";

#define OLED_SCL    4  
#define OLED_SDA    5  
#define OLED_RES    6  
#define OLED_DC     7  
#define OLED_CS     8  

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128

const unsigned char PROGMEM temp_icon[] = {
  0b00110000, 0b01001000, 0b01011000, 0b01001000,
  0b01011000, 0b10011100, 0b10111100, 0b01111000
};

const unsigned char PROGMEM humi_icon[] = {
  0b00010000, 0b00010000, 0b00101000, 0b00101000,
  0b01000100, 0b01000100, 0b00111000, 0b00000000
};

const unsigned char PROGMEM pres_icon[] = {
  0b00111000, 0b01000100, 0b10010010, 0b10001010,
  0b10000110, 0b01000100, 0b00111000, 0b00000000
};

const unsigned char PROGMEM icon_boat_50x50[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
	0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x08, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x08, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x13, 
	0xe4, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x08, 0xe4, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x08, 0x74, 0x3f, 
	0x00, 0x00, 0x00, 0x00, 0x08, 0xf2, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x09, 0xf6, 0x3f, 0x80, 0x06, 
	0x00, 0x00, 0x0b, 0xf4, 0x3f, 0x80, 0x0e, 0x00, 0x00, 0x0b, 0xfc, 0x3f, 0x80, 0x3c, 0x00, 0x00, 
	0x0b, 0xf8, 0x7f, 0x00, 0x78, 0x00, 0x00, 0x0b, 0xfb, 0xff, 0x00, 0xf0, 0x00, 0x00, 0x1b, 0xff, 
	0xfe, 0x03, 0xe0, 0x00, 0x00, 0x17, 0xff, 0xfc, 0xff, 0xc0, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 
	0x80, 0x00, 0x00, 0x3f, 0xf3, 0xc3, 0xff, 0x00, 0x00, 0x00, 0x3f, 0xf1, 0xcf, 0xfe, 0x00, 0x00, 
	0x00, 0x03, 0x01, 0xcf, 0xfc, 0x00, 0x00, 0x07, 0xe7, 0x01, 0xdf, 0xfc, 0x00, 0x00, 0x07, 0xff, 
	0x01, 0xff, 0xfe, 0x00, 0x00, 0x07, 0xff, 0x01, 0xff, 0xfe, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 
	0xfe, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xfe, 0x00, 
	0x00, 0x03, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x03, 
	0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x03, 0xff, 0xff, 
	0xff, 0xf0, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char PROGMEM icon_ball_16x16[] = {
  0x07, 0xe0, 0x1f, 0xf8, 0x3f, 0xfc, 0x7f, 0xfe,
  0x7f, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x7f, 0xfe, 0x7f, 0xfe,
  0x3f, 0xfc, 0x1f, 0xf8, 0x07, 0xe0, 0x00, 0x00
};

const unsigned char PROGMEM icon_fish_32x32[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00,
  0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0xff, 0x00,
  0x00, 0x01, 0xff, 0x80, 0xc0, 0x03, 0xff, 0x80,
  0xe0, 0x07, 0xff, 0xc0, 0xf0, 0x0f, 0xff, 0xe0,
  0x78, 0x1f, 0xff, 0xf0, 0x3c, 0x3f, 0xfd, 0xf0,
  0x1f, 0x7f, 0xff, 0xf8, 0x0f, 0xff, 0xff, 0xf8,
  0x07, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xe0,
  0x1f, 0xdf, 0xff, 0xc0, 0x3e, 0x8f, 0xff, 0x80,
  0x7c, 0x07, 0xfe, 0x00, 0x38, 0x03, 0xf8, 0x00,
  0x10, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char PROGMEM icon_dino_run1_32x32[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xff, 0xc0, 0x00, 0x01, 0xff, 0xc0,
  0x00, 0x01, 0xd1, 0xc0, 0x00, 0x01, 0xff, 0xc0,
  0x00, 0x01, 0xff, 0xc0, 0x00, 0x01, 0xff, 0xc0,
  0x00, 0x01, 0xf0, 0x00, 0x00, 0x01, 0xff, 0x00,
  0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0xfc, 0x00,
  0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0xdf, 0x00,
  0x00, 0x00, 0xdf, 0x00, 0x80, 0x01, 0xbf, 0x00,
  0xc0, 0x03, 0xff, 0x00, 0x60, 0x07, 0xff, 0x00,
  0x30, 0x0f, 0xff, 0x00, 0x78, 0x1f, 0xfe, 0x00,
  0xfc, 0x3f, 0xfc, 0x00, 0x7f, 0xff, 0xf8, 0x00,
  0x1f, 0xff, 0xf0, 0x00, 0x07, 0xff, 0xe0, 0x00,
  0x03, 0xf7, 0xc0, 0x00, 0x01, 0xe2, 0x00, 0x00,
  0x00, 0xc3, 0x00, 0x00, 0x00, 0xc1, 0x80, 0x00,
  0x00, 0x61, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char PROGMEM icon_dino_run2_32x32[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xff, 0xc0, 0x00, 0x01, 0xff, 0xc0,
  0x00, 0x01, 0xd1, 0xc0, 0x00, 0x01, 0xff, 0xc0,
  0x00, 0x01, 0xff, 0xc0, 0x00, 0x01, 0xff, 0xc0,
  0x00, 0x01, 0xf0, 0x00, 0x00, 0x01, 0xff, 0x00,
  0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0xfc, 0x00,
  0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0xdf, 0x00,
  0x00, 0x00, 0xdf, 0x00, 0x80, 0x01, 0xbf, 0x00,
  0xc0, 0x03, 0xff, 0x00, 0x60, 0x07, 0xff, 0x00,
  0x30, 0x0f, 0xff, 0x00, 0x78, 0x1f, 0xfe, 0x00,
  0xfc, 0x3f, 0xfc, 0x00, 0x7f, 0xff, 0xf8, 0x00,
  0x1f, 0xff, 0xf0, 0x00, 0x07, 0xff, 0xe0, 0x00,
  0x03, 0xf7, 0xc0, 0x00, 0x01, 0x83, 0x80, 0x00,
  0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
  0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char PROGMEM icon_ptero_32x16[] = {
  0x00, 0x00, 0x30, 0x00,
  0x00, 0x00, 0x38, 0x00,
  0x00, 0x00, 0x3c, 0x00,
  0x00, 0x00, 0x3e, 0x00,
  0x00, 0x00, 0x3f, 0x00,
  0x01, 0xc0, 0x3f, 0x80,
  0x03, 0xe0, 0x3f, 0xc0,
  0x07, 0xf0, 0x3f, 0xe0,
  0x0f, 0xf8, 0x3f, 0xf0,
  0x1f, 0xfc, 0x3f, 0xf0,
  0x00, 0x7f, 0xff, 0xfe,
  0x00, 0x1f, 0xff, 0x00,
  0x00, 0x07, 0xfe, 0x00,
  0x00, 0x03, 0xfc, 0x00,
  0x00, 0x01, 0xf0, 0x00,
  0x00, 0x00, 0x00, 0x00
};

const unsigned char PROGMEM icon_dragon_frame1_64x32[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x20, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x1e, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x86, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x10, 0xe1, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x9f, 0x61, 0xc0, 0x00, 
	0x00, 0x00, 0x00, 0x23, 0x07, 0xb0, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x78, 0x07, 0x70, 0xf8, 0x00, 
	0x00, 0x00, 0x00, 0x10, 0x08, 0xf0, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x71, 0x9c, 0xe0, 
	0x00, 0x00, 0x00, 0x02, 0x40, 0x78, 0x7f, 0xc0, 0x00, 0x00, 0x00, 0x07, 0x00, 0x73, 0xe3, 0x80, 
	0x00, 0x00, 0x00, 0xf8, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x7f, 0xfc, 0x00, 0x00, 
	0x00, 0x00, 0x3c, 0x7f, 0xfb, 0xf0, 0x00, 0x00, 0x00, 0x01, 0xc7, 0xff, 0xff, 0xc0, 0x00, 0x00, 
	0x00, 0x06, 0x1f, 0xf0, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x60, 0x00, 0x00, 0x00, 
	0x00, 0xff, 0x80, 0x00, 0x60, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 
	0x0f, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char PROGMEM icon_dragon_frame2_64x32[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0xc0, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x41, 0xa0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x60, 0xfc, 0x00, 
	0x00, 0x00, 0x00, 0x02, 0x07, 0xc0, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0f, 0xc1, 0xff, 0xe0, 
	0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0x7f, 0xc0, 0x00, 0x00, 0x00, 0x70, 0x0f, 0xe3, 0xc0, 0x00, 
	0x00, 0x00, 0x60, 0x60, 0x0f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x0f, 0xfc, 0x00, 0x00, 
	0x00, 0x00, 0x3c, 0x70, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x01, 0xf3, 0xf0, 0x07, 0xe0, 0x00, 0x00, 
	0x00, 0x07, 0x3f, 0xf0, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x20, 0x07, 0xe0, 0x00, 0x00, 
	0x00, 0xff, 0x00, 0x20, 0x00, 0x40, 0x00, 0x00, 0x03, 0xe0, 0x01, 0xc0, 0x01, 0x80, 0x00, 0x00, 
	0x0f, 0x00, 0x00, 0x60, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x06, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x10, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x30, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0xc1, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char PROGMEM icon_sun[] = {
  0x01, 0x80, 0x41, 0x82, 0x20, 0x04, 0x07, 0xe0,
  0x0c, 0x30, 0x18, 0x18, 0xb0, 0x0d, 0xb0, 0x0d,
  0x18, 0x18, 0x0c, 0x30, 0x07, 0xe0, 0x20, 0x04,
  0x41, 0x82, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00
};

const unsigned char PROGMEM icon_cloud[] = {
  0x00, 0x00, 0x01, 0xc0, 0x06, 0x30, 0x0c, 0x18,
  0x10, 0x0c, 0x33, 0x86, 0x66, 0x03, 0x4c, 0x01,
  0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x7f, 0xfe,
  0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char PROGMEM icon_rain[] = {
  0x01, 0xc0, 0x06, 0x30, 0x0c, 0x18, 0x10, 0x0c,
  0x33, 0x86, 0x66, 0x03, 0x4c, 0x01, 0x80, 0x01,
  0x7f, 0xfe, 0x3f, 0xfc, 0x00, 0x00, 0x22, 0x22,
  0x44, 0x44, 0x88, 0x88, 0x11, 0x11, 0x00, 0x00
};

enum SurferType { BOAT, BALL, FISH, DINO, DRAGON, OFF };
SurferType userSelectedSurfer = OFF; 
SurferType currentSurfer = OFF;     

enum ThemeMode { DAY, NIGHT };
ThemeMode themeMode = DAY;

unsigned long lastFishJumpTrigger = 0;
const unsigned long FISH_JUMP_INTERVAL = 3000;
const unsigned long FISH_JUMP_DURATION = 800;
const int MAX_JUMP_HEIGHT = 16;

unsigned long lastPteroTrigger = 0;
const unsigned long PTERO_INTERVAL = 5000; 
const unsigned long PTERO_FLIGHT_DURATION = 1800; 

const unsigned long SPLASH_INTERVAL = 250; 

uint8_t currentDisplayPage = 0; 
bool autoRotatePages = true;    

const unsigned long SCREEN_PAGE_DELAY = 3000; 
unsigned long lastPageSwitch = 0;

const unsigned long GRAPH_UPDATE_INTERVAL = 200;
unsigned long lastGraphUpdate = 0;

#define GRAPH_POINTS 50
float tempHistory[GRAPH_POINTS] = {0};
float humiHistory[GRAPH_POINTS] = {0};
float presHistory[GRAPH_POINTS] = {0};
uint8_t historyIndex = 0;

#define FORECAST_SAMPLES 12
float forecastPressureBuffer[FORECAST_SAMPLES];
uint8_t forecastBufferIndex = 0;
bool forecastBufferFull = false;
unsigned long lastForecastSampleTime = 0;
const unsigned long FORECAST_SAMPLE_INTERVAL = 300000; 
const char* currentForecast = "Analyzing...";

unsigned long lastWiFiRetry = 0;
const unsigned long WIFI_RETRY_INTERVAL = 10000;

Adafruit_SH1107 display = Adafruit_SH1107(
  SCREEN_WIDTH, SCREEN_HEIGHT, OLED_SDA, OLED_SCL, OLED_DC, OLED_RES, OLED_CS
);

Adafruit_BME280 bme;
Adafruit_Sensor *bme_temp = bme.getTemperatureSensor();
Adafruit_Sensor *bme_pressure = bme.getPressureSensor();
Adafruit_Sensor *bme_humidity = bme.getHumiditySensor();

TrioeClient trioe(TELEMETRY_URL, API_KEY);

void drawRotatedBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, float angleDegrees, uint16_t color) {
  float rad = angleDegrees * 0.0174532925f;
  float cosA = cos(rad);
  float sinA = sin(rad);

  int16_t centerX = w / 2;
  int16_t centerY = h / 2;
  int bytesPerRow = (w + 7) / 8;

  for (int16_t destY = 0; destY < h; destY++) {
    for (int16_t destX = 0; destX < w; destX++) {
      float relX = destX - centerX;
      float relY = destY - centerY;

      int16_t srcX = round(relX * cosA + relY * sinA + centerX);
      int16_t srcY = round(-relX * sinA + relY * cosA + centerY);

      if (srcX >= 0 && srcX < w && srcY >= 0 && srcY < h) {
        uint8_t byteVal = pgm_read_byte(bitmap + (srcY * bytesPerRow) + (srcX / 8));
        if (byteVal & (0x80 >> (srcX % 8))) {
          display.drawPixel(x + destX, y + destY, color);
        }
      }
    }
  }
}

const char* predictWeather(float currentSLP, float currentHumidity) {
  if (lastForecastSampleTime == 0) {
    for (int i = 0; i < FORECAST_SAMPLES; i++) {
      forecastPressureBuffer[i] = currentSLP;
    }
    lastForecastSampleTime = millis();
    return "Analyzing...";
  }

  if (millis() - lastForecastSampleTime >= FORECAST_SAMPLE_INTERVAL) {
    lastForecastSampleTime = millis();
    forecastPressureBuffer[forecastBufferIndex] = currentSLP;
    forecastBufferIndex = (forecastBufferIndex + 1) % FORECAST_SAMPLES;
    if (forecastBufferIndex == 0) forecastBufferFull = true;
  }

  uint8_t oldestIndex = forecastBufferFull ? forecastBufferIndex : 0;
  float pressureDifference = currentSLP - forecastPressureBuffer[oldestIndex];

  if (pressureDifference > 1.5f) {
    return "Improving / Sunny";
  } else if (pressureDifference < -1.5f) {
    return (currentHumidity > 75.0f) ? "Rain / Thunderstorms" : "Deteriorating / Cloudy";
  } else if (pressureDifference < -0.5f && currentHumidity > 80.0f) {
    return "Chance of Rain";
  } else {
    if (currentSLP > 1015.0f) return "Stable / Clear";
    if (currentSLP < 1005.0f) return "Unstable / Rain Likely";
    return "Stable / Partly Cloudy";
  }
}

float calculateRainProbability(const char* forecast, float humidity, float seaLevelPressure) {
  if (strstr(forecast, "Thunderstorms")) return 90.0f;
  if (strstr(forecast, "Rain Likely")) return 80.0f;
  if (strstr(forecast, "Chance of Rain")) return 60.0f;
  if (strstr(forecast, "Deteriorating")) return 45.0f;
  if (strstr(forecast, "Partly Cloudy")) return 25.0f;
  if (strstr(forecast, "Sunny") || strstr(forecast, "Clear")) return 5.0f;

  float baseProb = (humidity - 40.0f) * 0.8f;
  if (seaLevelPressure < 1005.0f) baseProb += 20.0f;
  return constrain(baseProb, 0.0f, 100.0f);
}

const char* getRainStatus(float probability) {
  if (probability >= 80.0f) return "Rain Expected / Heavy";
  if (probability >= 60.0f) return "High Chance of Rain";
  if (probability >= 40.0f) return "Moderate / Likely Rain";
  if (probability >= 20.0f) return "Low Chance of Rain";
  return "No Rain Expected";
}

const char* getShortForecast(const char* fullForecast) {
  if (strstr(fullForecast, "Sunny")) return "Sunny";
  if (strstr(fullForecast, "Thunderstorms")) return "Stormy";
  if (strstr(fullForecast, "Deteriorating")) return "Cloudy";
  if (strstr(fullForecast, "Chance of Rain")) return "Light Rain";
  if (strstr(fullForecast, "Clear")) return "Clear";
  if (strstr(fullForecast, "Unstable")) return "Rain Likely";
  if (strstr(fullForecast, "Analyzing")) return "Analyzing";
  return "Partly Cloudy";
}

void drawUI(const char* title, const unsigned char* icon, float currentValue, const char* unit, float history[], float minVal, float maxVal, int decimals, const char* forecast) {
  uint16_t bgColor = (themeMode == DAY) ? SH110X_WHITE : SH110X_BLACK;
  uint16_t fgColor = (themeMode == DAY) ? SH110X_BLACK : SH110X_WHITE;

  display.clearDisplay();
  display.fillRect(0, 0, 128, 128, bgColor);

  display.drawRoundRect(2, 2, 124, 124, 4, fgColor);
  display.drawFastHLine(2, 20, 124, fgColor);

  display.drawBitmap(6, 6, icon, 8, 8, fgColor);
  display.setTextSize(1);
  display.setTextColor(fgColor);
  display.setCursor(18, 6);
  display.print(title);

  display.setTextSize(2);
  display.setCursor(6, 25);
  display.print(currentValue, decimals);
  
  display.setTextSize(1);
  display.setCursor(display.getCursorX() + 2, 32);
  display.print(unit);

  display.drawFastHLine(2, 43, 124, fgColor);
  display.setCursor(6, 46);
  display.print("FCST: ");
  display.print(getShortForecast(forecast));

  int graphX = 6, graphY = 57, graphWidth = 116, graphHeight = 52;
  display.drawRect(graphX, graphY, graphWidth, graphHeight, fgColor);

  // --- Thin Dotted Gridlines ---
  for (int y = graphY + 12; y < graphY + graphHeight - 2; y += 12) {
    for (int x = graphX + 1; x < graphX + graphWidth - 1; x += 4) {
      display.drawPixel(x, y, fgColor);
    }
  }
  for (int x = graphX + 23; x < graphX + graphWidth - 1; x += 23) {
    for (int y = graphY + 1; y < graphY + graphHeight - 1; y += 4) {
      display.drawPixel(x, y, fgColor);
    }
  }

  display.setCursor(graphX + 4, graphY + 3);
  display.setTextColor(fgColor); 
  display.setTextSize(1);
  display.print("GRAPH");

  float range = (maxVal - minVal);
  if (range <= 0) range = 1.0f;

  int surfX = 0, surfY = 0, slopeYDelta = 0;
  int centerIndex = (GRAPH_POINTS - 1) / 2;

  int plotHeight = graphHeight - 14;
  int topOffset = 6;

  for (int i = 0; i < GRAPH_POINTS - 1; i++) {
    int idx1 = (historyIndex + i) % GRAPH_POINTS;
    int idx2 = (historyIndex + i + 1) % GRAPH_POINTS;

    int x1 = graphX + 1 + (i * (graphWidth - 2)) / (GRAPH_POINTS - 1);
    int x2 = graphX + 1 + ((i + 1) * (graphWidth - 2)) / (GRAPH_POINTS - 1);

    int y1 = (graphY + graphHeight - 2) - (int)(((history[idx1] - minVal) / range) * plotHeight);
    int y2 = (graphY + graphHeight - 2) - (int)(((history[idx2] - minVal) / range) * plotHeight);

    y1 = y1 + topOffset;
    y2 = y2 + topOffset;

    y1 = constrain(y1, graphY + 10, graphY + graphHeight - 2);
    y2 = constrain(y2, graphY + 10, graphY + graphHeight - 2);

    display.drawLine(x1, y1, x2, y2, fgColor);
    display.drawLine(x1, y1 + 1, x2, y2 + 1, fgColor);

    if (i == centerIndex) {
      surfX = x1; surfY = y1; slopeYDelta = y2 - y1;
    }
  }

  if (currentSurfer != OFF) {
    const unsigned char* surferBitmap = icon_boat_50x50;
    int spriteWidth = 50;
    int spriteHeight = 50;

    if (currentSurfer == BOAT) {
      surferBitmap = icon_boat_50x50; spriteWidth = 50; spriteHeight = 50;
    } else if (currentSurfer == BALL) {
      surferBitmap = icon_ball_16x16; spriteWidth = 16; spriteHeight = 16;
    } else if (currentSurfer == FISH) {
      surferBitmap = icon_fish_32x32; spriteWidth = 32; spriteHeight = 32;
    } else if (currentSurfer == DINO) {
      bool animFrame = (millis() / 100) % 2; 
      surferBitmap = animFrame ? icon_dino_run1_32x32 : icon_dino_run2_32x32;
      spriteWidth = 32; 
      spriteHeight = 32;
    } else if (currentSurfer == DRAGON) {
      bool wingFrame = (millis() / 250) % 2;
      surferBitmap = wingFrame ? icon_dragon_frame2_64x32 : icon_dragon_frame1_64x32;
      spriteWidth = 64;
      spriteHeight = 32;
    }

    int surfOffsetX = surfX - (spriteWidth / 2);
    int surfOffsetY;

    if (currentSurfer == BOAT) {
      surfOffsetY = surfY - 44; 
    } else if (currentSurfer == FISH) {
      surfOffsetY = surfY - (int)(spriteHeight * 0.55f);
    } else if (currentSurfer == DINO) {
      surfOffsetY = surfY - 28;
    } else if (currentSurfer == DRAGON) {
      surfOffsetY = surfY - 28; 
    } else {
      surfOffsetY = surfY - (spriteHeight - 2);
    }

    if (currentSurfer == DRAGON) {
      int bobbing = (int)(sin(millis() / 250.0f) * 2.0f);
      display.drawBitmap(surfOffsetX, surfOffsetY + bobbing, surferBitmap, spriteWidth, spriteHeight, fgColor);
    } else if (currentSurfer == DINO) {
      unsigned long now = millis();
      
      if (now - lastPteroTrigger >= PTERO_INTERVAL) {
        lastPteroTrigger = now;
      }

      unsigned long elapsedPtero = now - lastPteroTrigger;

      if (elapsedPtero < PTERO_FLIGHT_DURATION) {
        float progress = (float)elapsedPtero / (float)PTERO_FLIGHT_DURATION;
        int pteroX = (graphX + graphWidth) - (int)(progress * (graphWidth + 32));
        int pteroY = graphY + 18; 

        display.drawBitmap(pteroX, pteroY, icon_ptero_32x16, 32, 16, fgColor);

        if (progress >= 0.20f && progress <= 0.80f) {
          float jumpProgress = (progress - 0.20f) / 0.60f;
          int dinoJumpHeight = (int)(4.0f * 18.0f * jumpProgress * (1.0f - jumpProgress));
          surfOffsetY -= dinoJumpHeight;
        }
      }

      int drawY = surfOffsetY;
      if (slopeYDelta > 1) drawY += 2;
      else if (slopeYDelta < -1) drawY -= 2;

      display.drawBitmap(surfOffsetX, drawY, surferBitmap, spriteWidth, spriteHeight, fgColor);

    } else if (currentSurfer == FISH) {
      const int DIVE_DEPTH = 6;
      float tiltAngle = 0.0f; 
      unsigned long now = millis();
      
      if (now - lastFishJumpTrigger >= FISH_JUMP_INTERVAL) {
        lastFishJumpTrigger = now;
      }

      unsigned long elapsed = now - lastFishJumpTrigger;
      if (elapsed < FISH_JUMP_DURATION) {
        float t = (float)elapsed / (float)FISH_JUMP_DURATION;

        if (t < 0.3f) {
          float diveProgress = t / 0.3f;
          int diveOffset = (int)(sin(diveProgress * 3.14159f) * DIVE_DEPTH);
          surfOffsetY += diveOffset;
          tiltAngle = 20.0f; 
        } else {
          float jumpProgress = (t - 0.3f) / 0.7f;
          int jumpArcY = (int)(4.0f * MAX_JUMP_HEIGHT * jumpProgress * (1.0f - jumpProgress));
          surfOffsetY -= jumpArcY;
          tiltAngle = -20.0f; 
        }
      } else {
        if (slopeYDelta > 1) tiltAngle = 12.0f;
        else if (slopeYDelta < -1) tiltAngle = -12.0f;
      }

      drawRotatedBitmap(surfOffsetX, surfOffsetY, surferBitmap, spriteWidth, spriteHeight, tiltAngle, fgColor);

    } else {
      int drawY = surfOffsetY;
      if (slopeYDelta > 1) drawY += 2;
      else if (slopeYDelta < -1) drawY -= 2;

      display.drawBitmap(surfOffsetX, drawY, surferBitmap, spriteWidth, spriteHeight, fgColor);

      if (currentSurfer == BALL) {
        float angle = (millis() % 1000) * (2.0f * 3.14159f / 1000.0f);
        int centerX = surfOffsetX + 7;
        int centerY = drawY + 7;
        int radius = 5;

        int markX = centerX + round(radius * cos(angle));
        int markY = centerY + round(radius * sin(angle));
        display.fillCircle(markX, markY, 1, bgColor);
      }

      if (currentSurfer == BOAT) {
        bool animState = (millis() / SPLASH_INTERVAL) % 2;

        if (animState) {
          display.drawPixel(surfOffsetX + 29, drawY + 12, fgColor);
          display.drawPixel(surfOffsetX + 31, drawY + 12, fgColor);
          display.drawPixel(surfOffsetX + 28, drawY + 13, fgColor);
          display.drawPixel(surfOffsetX + 30, drawY + 13, fgColor);
        } else {
          display.drawPixel(surfOffsetX + 30, drawY + 14, fgColor);
          display.drawPixel(surfOffsetX + 31, drawY + 15, fgColor);
        }

        if (animState) {
          display.drawPixel(surfOffsetX - 1, drawY + 15, fgColor);
          display.drawPixel(surfOffsetX - 2, drawY + 16, fgColor);
          display.drawPixel(surfOffsetX - 4, drawY + 15, fgColor);
          display.drawPixel(surfOffsetX - 6, drawY + 16, fgColor);
        } else {
          display.drawPixel(surfOffsetX - 2, drawY + 15, fgColor);
          display.drawPixel(surfOffsetX - 3, drawY + 16, fgColor);
          display.drawPixel(surfOffsetX - 5, drawY + 16, fgColor);
        }
      }
    }
  }

  for (int p = 0; p < 4; p++) {
    if (p == currentDisplayPage) display.fillCircle(52 + (p * 8), 116, 2, fgColor);
    else display.drawCircle(52 + (p * 8), 116, 2, fgColor);
  }

  display.display();
}

void drawForecastUI(const char* forecast) {
  uint16_t bgColor = (themeMode == DAY) ? SH110X_WHITE : SH110X_BLACK;
  uint16_t fgColor = (themeMode == DAY) ? SH110X_BLACK : SH110X_WHITE;

  display.clearDisplay();
  display.fillRect(0, 0, 128, 128, bgColor);
  display.drawRoundRect(2, 2, 124, 124, 4, fgColor);
  display.drawFastHLine(2, 20, 124, fgColor);

  display.setTextSize(1);
  display.setTextColor(fgColor);
  display.setCursor(6, 6);
  display.print("FORECAST");

  display.drawRoundRect(6, 26, 116, 82, 3, fgColor);

  const unsigned char* selectedIcon = icon_cloud;
  if (strstr(forecast, "Sunny") || strstr(forecast, "Clear")) selectedIcon = icon_sun;
  else if (strstr(forecast, "Rain") || strstr(forecast, "Storms")) selectedIcon = icon_rain;

  display.drawBitmap(56, 32, selectedIcon, 16, 16, fgColor);

  const char* shortForecast = getShortForecast(forecast);
  display.setTextSize(1);
  int xPos = (128 - (strlen(shortForecast) * 6)) / 2;
  display.setCursor(xPos, 52);
  display.print(shortForecast);

  display.drawFastHLine(10, 64, 108, fgColor);
  display.setCursor(10, 70);
  display.print("Status:");

  String fullStatus = String(forecast);
  int spaceIdx = fullStatus.indexOf('/');

  if (spaceIdx != -1) {
    String line1 = fullStatus.substring(0, spaceIdx); line1.trim();
    display.setCursor(10, 80); display.print(line1);

    String line2 = fullStatus.substring(spaceIdx + 1); line2.trim();
    display.setCursor(10, 91); display.print(line2);
  } else {
    display.setCursor(10, 82); display.print(fullStatus);
  }

  for (int p = 0; p < 4; p++) {
    if (p == currentDisplayPage) display.fillCircle(52 + (p * 8), 116, 2, fgColor);
    else display.drawCircle(52 + (p * 8), 116, 2, fgColor);
  }

  display.display();
}

void processPageCommand(String cmd) {
  cmd.trim();
  cmd.toLowerCase();
  Serial.printf("Page Command Received: '%s'\n", cmd.c_str());

  if (cmd.indexOf("auto") != -1) {
    if (!autoRotatePages) {
      autoRotatePages = true;
      lastPageSwitch = millis();
    }
  } else if (cmd.indexOf("temperature") != -1 || cmd.indexOf("temp") != -1) {
    autoRotatePages = false; currentDisplayPage = 0;
  } else if (cmd.indexOf("humidity") != -1 || cmd.indexOf("humi") != -1) {
    autoRotatePages = false; currentDisplayPage = 1;
  } else if (cmd.indexOf("pressure") != -1 || cmd.indexOf("pres") != -1) {
    autoRotatePages = false; currentDisplayPage = 2;
  } else if (cmd.indexOf("forecast") != -1 || cmd.indexOf("fcst") != -1) {
    autoRotatePages = false; currentDisplayPage = 3;
  }
}

void processDisplayCommand(String cmd) {
  cmd.trim();
  cmd.toLowerCase();
  Serial.printf("Display Command Received: '%s'\n", cmd.c_str());

  // --- Day / Night Theme Commands ---
  if (cmd.indexOf("night") != -1 || cmd.indexOf("dark") != -1) {
    themeMode = NIGHT;
  } else if (cmd.indexOf("day") != -1 || cmd.indexOf("light") != -1) {
    themeMode = DAY;
  }

  // --- Surfer Selection Commands ---
  if (cmd.indexOf("off") != -1 || cmd.indexOf("none") != -1 || cmd.indexOf("hide") != -1) {
    userSelectedSurfer = OFF;
  } else if (cmd.indexOf("boat") != -1 || cmd.indexOf("ship") != -1 || cmd.indexOf("sailboat") != -1) {
    userSelectedSurfer = BOAT;
  } else if (cmd.indexOf("ball") != -1) {
    userSelectedSurfer = BALL;
  } else if (cmd.indexOf("fish") != -1 || cmd.indexOf("whale") != -1 || cmd.indexOf("clownfish") != -1 || cmd.indexOf("goldfish") != -1) {
    userSelectedSurfer = FISH;
  } else if (cmd.indexOf("dino") != -1 || cmd.indexOf("dinosaur") != -1) {
    userSelectedSurfer = DINO;
  } else if (cmd.indexOf("dragon") != -1 || cmd.indexOf("wyvern") != -1 || cmd.indexOf("fly") != -1) {
    userSelectedSurfer = DRAGON;
  }

  if (WiFi.status() == WL_CONNECTED) {
    currentSurfer = userSelectedSurfer;
  }
}

void setup() {
  Serial.begin(115200);

  display.begin(0, true);
  display.clearDisplay();
  display.fillRect(0, 0, 128, 128, SH110X_WHITE);
  display.setTextSize(1);
  display.setTextColor(SH110X_BLACK);
  display.setCursor(15, 55);
  display.println("Connecting WiFi...");
  display.display();

  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  trioe.setCACert(TRIOE_ROOT_CA);
  trioe.setReportingInterval(10000); 
  trioe.setChangeThreshold(0.2f);    
  trioe.setStateEndpoint(STATE_URL);
  trioe.setStatePollInterval(2000);

  trioe.onText("value_selector", [](const char* val) { processPageCommand(String(val)); });
  trioe.onText("valueSelector", [](const char* val) { processPageCommand(String(val)); });

  trioe.onText("display command", [](const char* val) { processDisplayCommand(String(val)); });
  trioe.onText("display_command", [](const char* val) { processDisplayCommand(String(val)); });
  trioe.onText("displayCommand", [](const char* val) { processDisplayCommand(String(val)); });
  trioe.onText("theme", [](const char* val) { processDisplayCommand(String(val)); });
  trioe.onText("mode", [](const char* val) { processDisplayCommand(String(val)); });

  trioe.onText("command", [](const char* val) { 
    processPageCommand(String(val)); 
    processDisplayCommand(String(val)); 
  });

  if (!trioe.begin()) Serial.println("TRIOE setup failed.");

  Wire.begin(2, 1); 

  if (!bme.begin(0x76, &Wire)) { 
    Serial.println("Could not find a valid BME280 sensor!");
    display.clearDisplay();
    display.fillRect(0, 0, 128, 128, SH110X_WHITE);
    display.setCursor(15, 55);
    display.println("BME280 Error!");
    display.display();
    while (1) delay(10);
  }

  bme.setSampling(Adafruit_BME280::MODE_NORMAL,
                  Adafruit_BME280::SAMPLING_X2,
                  Adafruit_BME280::SAMPLING_X16,
                  Adafruit_BME280::SAMPLING_X1,
                  Adafruit_BME280::FILTER_X16,
                  Adafruit_BME280::STANDBY_MS_0_5);

  lastPageSwitch = millis();
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    processPageCommand(input);
    processDisplayCommand(input);
  }

  if (WiFi.status() != WL_CONNECTED) {
    currentSurfer = OFF; 
    if (millis() - lastWiFiRetry >= WIFI_RETRY_INTERVAL) {
      lastWiFiRetry = millis();
      WiFi.disconnect();
      WiFi.reconnect();
    }
  } else {
    currentSurfer = userSelectedSurfer; 
  }

  sensors_event_t temp_event, pressure_event, humidity_event;
  bme_temp->getEvent(&temp_event);
  bme_pressure->getEvent(&pressure_event);
  bme_humidity->getEvent(&humidity_event);

  float seaLevelPressure = pressure_event.pressure + 1.8f; 

  currentForecast = predictWeather(seaLevelPressure, humidity_event.relative_humidity);
  
  float rainProb = calculateRainProbability(currentForecast, humidity_event.relative_humidity, seaLevelPressure);
  const char* rainState = getRainStatus(rainProb);

  if (millis() - lastGraphUpdate >= GRAPH_UPDATE_INTERVAL) {
    lastGraphUpdate = millis();
    tempHistory[historyIndex] = temp_event.temperature;
    humiHistory[historyIndex] = humidity_event.relative_humidity;
    presHistory[historyIndex] = pressure_event.pressure;
    historyIndex = (historyIndex + 1) % GRAPH_POINTS;
  }

  if (autoRotatePages && (millis() - lastPageSwitch >= SCREEN_PAGE_DELAY)) {
    lastPageSwitch = millis();
    currentDisplayPage = (currentDisplayPage + 1) % 4;
  }

  const char* activePageName = "TEMPERATURE";
  if (currentDisplayPage == 0) {
    activePageName = "TEMPERATURE";
    drawUI("TEMPERATURE", temp_icon, temp_event.temperature, "C", tempHistory, 10.0f, 40.0f, 2, currentForecast);
  } else if (currentDisplayPage == 1) {
    activePageName = "HUMIDITY";
    drawUI("HUMIDITY", humi_icon, humidity_event.relative_humidity, "%", humiHistory, 20.0f, 90.0f, 2, currentForecast);
  } else if (currentDisplayPage == 2) {
    activePageName = "PRESSURE";
    drawUI("PRESSURE", pres_icon, pressure_event.pressure, "hPa", presHistory, 950.0f, 1050.0f, 1, currentForecast);
  } else if (currentDisplayPage == 3) {
    activePageName = "FORECAST";
    drawForecastUI(currentForecast);
  }

  const char* tempStatus = (temp_event.temperature < 12.0) ? "Cold" : (temp_event.temperature <= 18.0) ? "Cool" : (temp_event.temperature <= 23.0) ? "Room Temp" : (temp_event.temperature <= 29.0) ? "Warm" : (temp_event.temperature <= 37.0) ? "Hot" : "Extreme";
  const char* humiStatus = (humidity_event.relative_humidity < 40.0) ? "Dry" : (humidity_event.relative_humidity <= 60.0) ? "Optimal" : "Humid";
  const char* presStatus = (seaLevelPressure < 1007.0) ? "Low Pressure" : (seaLevelPressure <= 1018.0) ? "Normal" : "High Pressure";

  char overallStatusBuffer[256];
  snprintf(overallStatusBuffer, sizeof(overallStatusBuffer), 
    "Active: %s | Auto: %s | Mode: %s | Temp: %.2f C | Humi: %.2f %% | Pres: %.1f hPa | Forecast: %s", 
    activePageName, autoRotatePages ? "ON" : "OFF", (themeMode == DAY) ? "DAY" : "NIGHT", temp_event.temperature, humidity_event.relative_humidity, pressure_event.pressure, currentForecast
  );

  trioe.addReading("temperature", temp_event.temperature, "C");
  trioe.addReading("humidity", humidity_event.relative_humidity, "%");
  trioe.addReading("pressure", pressure_event.pressure, "hPa");
  trioe.addReading("sealevel_pressure", seaLevelPressure, "hPa");
  trioe.addReading("forecast_prediction", currentForecast);
  trioe.addReading("rain_probability", rainProb, "%"); 
  trioe.addReading("rain_status", rainState);
  trioe.addReading("theme_mode", (themeMode == DAY) ? "DAY" : "NIGHT");
  trioe.addReading("message", overallStatusBuffer);
  trioe.addReading("temp_status", tempStatus);
  trioe.addReading("humidity_status", humiStatus);
  trioe.addReading("pressure_status", presStatus);
  trioe.addReading("led_status", (WiFi.status() == WL_CONNECTED) ? 1 : 0);

  if (WiFi.status() == WL_CONNECTED) {
    trioe.loop();
  }

  delay(50); 
}
