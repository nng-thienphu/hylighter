#include "config.h"

/***********************************************************************
 *                       GLOBAL VARIABLE DEFINITIONS
 **********************************************************************/
// Wi-Fi credentials
const char* ssid     = "SciFi";
const char* password = "2.00231930436";

// Universe for Art-Net
const int Universe   = 0;

// DMX / Art-Net flags
bool slotZero        = true;
int  startSlot       = 0;
bool new_dmx         = false;
bool new_artnet      = false;
bool new_data        = false;

// Wi-Fi & networking
bool            wifiConnected = false;
WebServer       webServer;
AutoConnect     portal(webServer);
AutoConnectConfig portalConfig;
HTTPUpdateServer httpUpdater;
AutoConnectAux  updatePage("/update", "UPDATE");
AutoConnectAux  helloPage;

// Artnet
String         nodeName = "ESP32 Art-Net ";
ArtnetnodeWifi artnetnode;

// LED data arrays
unsigned long dmx_led_8bit_data[NUM_LEDS + 1]    = {0};
unsigned long artnet_led_8bit_data[NUM_LEDS + 1] = {0};
unsigned long led_8bit_data[NUM_LEDS + 1]        = {0};
unsigned long led_duty[NUM_LEDS]                 = {0};
unsigned long stringCount[NUM_LEDS]              = {3,4,4,4,4,3,3,3,3,3};

// Dimming
int   dimming_mode = 1;  // 1 => linear
float curr         = 0.0;

// Timers
unsigned long previousMillis = 0;
unsigned long currentMillis  = 0;
unsigned long lag            = 0;
unsigned long maxlag         = 0;

// Custom JSON pages for AutoConnect (example)
const char HELLO_PAGE[] PROGMEM = R"raw(
[
  {
    "title":"HyLighter Parameter",
    "uri":"/param",
    "menu":true,
    "element":[
      { "name":"input1", "type":"ACInput", "label":"Dimming mode" },
      { "name":"input2", "type":"ACInput", "label":"Starting slot (unused)" },
      { "name":"send",   "type":"ACSubmit", "value":"Send", "uri":"/echo" }
    ]
  },
  {
    "title":"Echo",
    "uri":"/echo",
    "menu":false,
    "element":[
      { "name":"echo", "type":"ACText" }
    ]
  }
]
)raw";
