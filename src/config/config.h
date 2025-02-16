#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <AutoConnect.h>
#include <HTTPUpdateServer.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <SSD1306Wire.h>
#include <ArtnetnodeWifi.h>
#include <LXESP32DMX.h>

/***********************************************************************
 *                              CONSTANTS
 **********************************************************************/
// Versioning
#define Version         104
#define hylighterNum    10

// Pin definitions
#define DMX_DIRECTION_PIN     12
#define DMX_SERIAL_INPUT_PIN  14
#define NUM_LEDS              10
#define SDA_PIN               4
#define SCL_PIN               15

// Wi-Fi credentials
extern const char* ssid;
extern const char* password;

// Universe for Art-Net (example)
extern const int Universe;

/***********************************************************************
 *                         GLOBAL VARIABLES
 **********************************************************************/
// DMX / Art-Net control
extern bool     slotZero;
extern int      startSlot;
extern bool     new_dmx;
extern bool     new_artnet;
extern bool     new_data;

// Wi-Fi & networking
extern bool             wifiConnected;
extern WebServer        webServer;
extern AutoConnect      portal;
extern AutoConnectConfig portalConfig;
extern HTTPUpdateServer httpUpdater;
extern AutoConnectAux   updatePage;
extern AutoConnectAux   helloPage;

// Artnet
extern String          nodeName;
extern ArtnetnodeWifi  artnetnode;

// DMX data arrays
extern unsigned long dmx_led_8bit_data[NUM_LEDS + 1];
extern unsigned long artnet_led_8bit_data[NUM_LEDS + 1];
extern unsigned long led_8bit_data[NUM_LEDS + 1];
extern unsigned long led_duty[NUM_LEDS];
extern unsigned long stringCount[NUM_LEDS];

// Dimming
extern int   dimming_mode;   // 1 => linear, 2 => squared
extern float curr;           // track approximate current

// Timers
extern unsigned long previousMillis;
extern unsigned long currentMillis;
extern unsigned long lag;
extern unsigned long maxlag;

// AutoConnect JSON page definition
extern const char HELLO_PAGE[] PROGMEM;

// LED 
extern SSD1306Wire display;

#endif // CONFIG_H
