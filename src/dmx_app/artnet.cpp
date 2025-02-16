#include "../config/config.h"
#include "artnet.h"

/***********************************************************************
 * Callback function invoked by the ArtNetnode library on DMX reception.
 **********************************************************************/
void onArtNetFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data) {
  // Ignore if not our configured universe
  if (universe != Universe) {
    return;
  }

  bool changed = false;
  // We expect 11 channels => brightness + 10 LED channels
  // (If your fixture uses a different number, adjust accordingly.)
  for (int i = 0; i < 11; i++) {
    if (led_8bit_data[i] != data[i + startSlot]) {
      changed = true;
      break;
    }
  }

  if (changed) {
    new_artnet = true;
    new_data   = true;

    for (int i = 0; i < 11; i++) {
      artnet_led_8bit_data[i] = data[i + startSlot];
      led_8bit_data[i]        = artnet_led_8bit_data[i];
    }
  }
}
