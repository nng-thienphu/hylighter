#ifndef WIFI_SERVICE_H
#define WIFI_SERVICE_H

#include <Arduino.h>

/**
 * @brief Initializes Wi-Fi with AutoConnect.  
 *        If credentials are saved, it connects automatically.
 *        Otherwise, it spawns an AP for configuration.
 * 
 * @return true if connected successfully (or AP started), false if failed.
 */
bool WifiServiceSetup();

#endif // WIFI_SERVICE_H
