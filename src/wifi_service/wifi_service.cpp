#include "wifi_service.h"
#include "../config/config.h"

/***********************************************************************
 * @brief  Sets up Wi-Fi using the AutoConnect library.
 *         - Launches the AP or tries to connect to known networks.
 *         - Sets up the HTTP update server.
 *         - Sets up an mDNS entry if successful.
 *
 * @return  true if Wi-Fi connected (or AP started), false otherwise.
 **********************************************************************/
bool WifiServiceSetup() {
  delay(1000);

  // Set up the HTTP Update server
  httpUpdater.setup(&webServer);

  // The AP name if no known Wi-Fi is found
  portalConfig.apid = "HyLighter_" + String(hylighterNum); 
  // Optional AP password (uncomment if desired):
  // portalConfig.psk = "12345678";

  // Configure the AutoConnect portal
  portal.config(portalConfig);

  // Load the custom JSON page from 'HELLO_PAGE'
  helloPage.load(HELLO_PAGE);

  // Attach any custom pages to the portal
  portal.join({ helloPage, updatePage });

  // Callback for form submissions at /echo
  portal.on("/echo", [](AutoConnectAux& aux, PageArgument& args) {
    String val = args.arg("input1");  // user input for dimming mode
    dimming_mode = val.toInt();

    // Reflect the input on the 'echo' text element
    AutoConnectText& ac_echo = aux.getElement<AutoConnectText>("echo");
    ac_echo.value = val;

    return String();
  });

  // Attempt to connect; if no known Wi-Fi, will spawn the config portal
  if (portal.begin()) {
    Serial.print("Connected! IP: ");
    Serial.println(WiFi.localIP());
    display.println("Wi-Fi: " + WiFi.localIP().toString());
    display.drawLogBuffer(0,0);
    display.display();

    wifiConnected = true;

    // Optionally start mDNS
    if (MDNS.begin("esp-webupdate")) {
      MDNS.addService("http", "tcp", 80);
    }
  } else {
    Serial.println("AutoConnect failed or timed out.");
    display.println("AutoConnect failed.");
    display.drawLogBuffer(0,0);
    display.display();
    wifiConnected = false;
  }

  return wifiConnected;
}
