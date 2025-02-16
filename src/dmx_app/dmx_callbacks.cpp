// #include "dmx_callbacks.h"
// #include "config.h"
// #include <LXESP32DMX.h>

// void receiveDMXCallback(int slots) {
//   if (slots) {
//     xSemaphoreTake(ESP32DMX.lxDataLock, portMAX_DELAY);  
//     bool changed = false;
//     for (int i = 0; i < NUM_LEDS + 1; i++) {
//       uint8_t newSlotVal = ESP32DMX.getSlot(i + 1 + startSlot);
//       if (dmx_led_8bit_data[i] != newSlotVal) {
//         changed = true;
//         break;
//       }
//     }
//     if (changed) {
//       new_dmx = true;
//       new_data = true;
//       for (int i = 0; i < NUM_LEDS + 1; i++) {
//         dmx_led_8bit_data[i] = ESP32DMX.getSlot(i + 1 + startSlot);
//         led_8bit_data[i]     = dmx_led_8bit_data[i];
//       }
//     }
//     xSemaphoreGive(ESP32DMX.lxDataLock);
//   }
// }
