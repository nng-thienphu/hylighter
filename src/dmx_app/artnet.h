#ifndef ARTNET_H
#define ARTNET_H

#include <Arduino.h>

/**
 * @brief Callback function when an Art-Net DMX packet is received.
 * 
 * @param universe   The DMX universe number.
 * @param length     The length of the DMX data.
 * @param sequence   The packet sequence number.
 * @param data       Pointer to the incoming DMX data bytes.
 */
void onArtNetFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data);

#endif // ARTNET_H
