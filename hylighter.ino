/**************************************************************************/
/*!
    Updated, modular HyLighter code.
    Works with:
      - Arduino ESP32 core 2.x or later
      - AutoConnect 1.2.x / 1.3.x / 1.4.x
      - ArtnetnodeWifi from https://github.com/rstephan/ArtnetnodeWifi
      - LXESP32DMX from https://github.com/claudeheintz/LXESP32DMX
      - SSD1306Wire, etc.
*/
/**************************************************************************/


// Import library
#include <LXESP32DMX.h>         
#include "src/dmx_app/artnet.h"
#include "src/config/config.h"
#include "src/wifi_service/wifi_service.h"
#include "test/tester.h"


// ------TESTING MODE INIT--------
// Comment out these lines to switch to production mode.
// These are for unit testing, ensuring the code follows the correct logic and initialization.
// This will remove all test-related code before compilation, reducing memory usage
// #define RUN_TESTS
// class SystemTester;
// extern SystemTester tester;
// ------TESTING MODE INIT -------- 

SSD1306Wire display(0x3c, SDA_PIN, SCL_PIN); // Definition


/**
 * @brief Utility to set up a specific PWM channel for LED dimming.
 * 
 * @param pin     The GPIO pin to attach to a PWM channel.
 * @param channel The ledc channel index (0..15 on ESP32).
 */
static void setupPWMChannel(uint8_t pin, uint8_t channel) {
  const int PWM_FREQ = 1200;  // 1.2 kHz
  const int PWM_RES  = 16;    // 16-bit resolution

  ledcAttachPin(pin, channel);
  ledcSetup(channel, PWM_FREQ, PWM_RES);
  ledcWrite(channel, 0);
}

/**
 * @brief Optional example function for gamma correction (not used in main code).
 * 
 * @param channel The ledc channel to write to.
 * @param level   The 8-bit brightness value.
 */
static void gammaCorrectedWrite(uint8_t channel, uint8_t level) {
  // Example of a simple gamma correction: level^2
  uint32_t corrected = (uint32_t)level * (uint32_t)level; // 16-bit range
  ledcWrite(channel, corrected);
}

/**
 * @brief Callback for DMX data (inline for convenience).
 */
static void receiveCallback(int slots) {
  if (slots) {
    xSemaphoreTake(ESP32DMX.lxDataLock, portMAX_DELAY);
    bool changed = false;
    for (int i = 0; i < NUM_LEDS + 1; i++) {
      uint8_t val = ESP32DMX.getSlot(i + 1 + startSlot);
      if (dmx_led_8bit_data[i] != val) {
        changed = true;
        break;
      }
    }
    if (changed) {
      new_dmx = true;
      new_data = true;
      for (int i = 0; i < NUM_LEDS + 1; i++) {
        dmx_led_8bit_data[i] = ESP32DMX.getSlot(i + 1 + startSlot);
        led_8bit_data[i]     = dmx_led_8bit_data[i];
      }
    }
    xSemaphoreGive(ESP32DMX.lxDataLock);
  }
}

int test_ch = 0; 
int test_pin = 11; 

void channel6Setup() {
  Serial.println("Testing setup code...");
  display.init();
  display.setContrast(255);
  display.setLogBuffer(5, 30);
  display.clear();
  display.println("HyLighter V3 " + 
        String(Version/100) + "." +
        String((Version%100)/10) + "." +
        String(Version%10));
    display.drawLogBuffer(0,0);
    display.display();
    delay(1000);

    setupPWMChannel(test_pin, test_ch); 
}

void channel6Loop() {
  ledcWrite(test_ch, 1000);
  // ledcWrite(3, led_ch3_duty);
}


/************************************************************************
 * Arduino Setup
 ************************************************************************/
void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("ESP32 IS RUNNING !");

  #ifdef RUN_TESTS
    channel6Setup();
    
  #else
    Serial.println("=== Starting HyLighter ===");

    /***********************************************************************
     * Initialization of the OLED display
     **********************************************************************/
    display.init();
    display.setContrast(255);
    display.setLogBuffer(5, 30);

    display.clear();
    display.println("HyLighter V3 " + 
        String(Version/100) + "." +
        String((Version%100)/10) + "." +
        String(Version%10));
    display.drawLogBuffer(0,0);
    display.display();
    delay(1000);

    // ESP32DMX.setDirectionPin(DMX_DIRECTION_PIN);

    // const uint8_t ledPins[NUM_LEDS] = {32, 22, 33, 21, 25, 19, 26, 17, 27, 16};
    // for (int i = 0; i < NUM_LEDS; i++) {
    //   setupPWMChannel(ledPins[i], i + 1);
    // }

    /***********************************************************************
     * DMX Setup
     **********************************************************************/
    if (!slotZero) {
      // If slotZero is false, compute a "startSlot" from hylighterNum
      startSlot = (hylighterNum - 1) * 16;
    }
    Serial.print("DMX Start Slot: ");
    Serial.println(startSlot);

    ESP32DMX.setDirectionPin(DMX_DIRECTION_PIN);

    // Assign a PWM channel to each LED pin
    const uint8_t ledPins[NUM_LEDS] = {32, 22, 33, 21, 25, 19, 26, 17, 27, 16};
    for (int i = 0; i < NUM_LEDS; i++) {
      setupPWMChannel(ledPins[i], i + 1);
    }

    // Start DMX input and register callback
    ESP32DMX.setDataReceivedCallback(&receiveCallback);
    ESP32DMX.startInput(DMX_SERIAL_INPUT_PIN);

    /***********************************************************************
     * Wi-Fi + AutoConnect Setup
     **********************************************************************/
    wifiConnected = WifiServiceSetup();
    
    /***********************************************************************
     * Art-Net Setup (only if Wi-Fi is connected)
     **********************************************************************/
    if (wifiConnected) {
      nodeName += hylighterNum;                // e.g. "ESP32 Art-Net 10"
      artnetnode.setName(nodeName.c_str());
      artnetnode.setNumPorts(1);
      artnetnode.begin();
      artnetnode.setArtDmxCallback(onArtNetFrame);
    }

    delay(500);
    Serial.println("=== Setup Complete ===");
  #endif
}

/************************************************************************
 * Arduino Main Loop
 ************************************************************************/
void loop() {
  #ifdef RUN_TESTS
    channel6Loop();
  #else
      // If connected, handle any incoming web requests or Art-Net data
  if (wifiConnected) {
    portal.handleClient();
    artnetnode.read();
  }

  // If new DMX or new Art-Net data arrived, process it
  if (new_data) {
    new_data = false;  // Reset the flag

    currentMillis = millis();
    lag = currentMillis - previousMillis;
    previousMillis = currentMillis;
    if (lag > maxlag) {
      maxlag = lag;
    }

    Serial.print("Lag: ");
    Serial.print(lag);
    Serial.print(" ms; Max Lag: ");
    Serial.println(maxlag);
    Serial.print("Dimming mode: ");
    Serial.println(dimming_mode);

    // Clear the display
    display.clear();
    display.print(String(maxlag) + " " + String(lag) + " ");

    // 1) Linear dimming
    if (dimming_mode == 1) {
      for (int j = 0; j < NUM_LEDS; j++) {
        // led_8bit_data[0] = overall brightness
        // led_8bit_data[j+1] = channel brightness
        // duty = brightness8 * colorChannel8
        led_duty[j] = led_8bit_data[0] * led_8bit_data[j + 1];
      }
    }
    // 2) Squared dimming (perceived brightness)
    else if (dimming_mode == 2) {
      for (int j = 0; j < NUM_LEDS; j++) {
        unsigned long combined = led_8bit_data[0] * led_8bit_data[j + 1];
        // combined is up to 65535 if both are 255
        // we then square it or do something approximate:
        unsigned long squaredVal = combined * combined; // up to 4,294,901,225
        // scale it back to 16-bit range if needed
        squaredVal >>= 16;  
        led_duty[j] = squaredVal;
      }
    }

    // // Estimate total current
    curr = 0.0f;
    for (int k = 0; k < NUM_LEDS; k++) {
      // 0.033 A per string at full scale, scale by (led_duty[k] / 65535)
      curr += (float)led_duty[k] * (float)stringCount[k] * 0.033f / 65535.0f;
    }

    // If total current > ~1.5 A, scale everything down
    if (curr > 1.5f) {
      float scale = 1.5f / curr;
      display.print(" " + String(scale * 100, 0) + "% ");
      for(int l = 0; l < NUM_LEDS; l++) {
        led_duty[l] = (unsigned long)((float)led_duty[l] * scale);
      }
    } else {
      display.print("100% ");
    }

    // Write final duty cycles to the LED PWM channels
    // Channels start at 1 for us, so add 1
    // Changes: for safety purpose we reduce to 10% only
    for (int j = 0; j < NUM_LEDS; j++) {
      ledcWrite(j + 1, led_duty[j]*0.1f);
      display.print(led_duty[j]);
      display.print(" ");
    }

    display.println();
    display.drawLogBuffer(0,0);
    display.display();
  }

  // Prevent watchdog issues
  delay(1);

  #endif
}



