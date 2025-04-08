// tester.ino
// A single class that tests multiple modules.
// You can adapt these stubs (Wi-Fi, DMX, ArtNet, Display)
// to your actual code.

class SystemTester {
public:
  // Constructor
  SystemTester();

  // Run all tests in sequence
  void runAllTests();

  // Individual test routines (you can expand or rename them)
  void testWiFi();
  void testDMX();
  void testArtnet();
  void testDisplay();

private:
  // Store pass/fail for each test
  bool wifiPassed;
  bool dmxPassed;
  bool artnetPassed;
  bool displayPassed;

  // Utility to print a final summary
  void printSummary();
};

// -- Implementations --

SystemTester::SystemTester()
: wifiPassed(false)
, dmxPassed(false)
, artnetPassed(false)
, displayPassed(false)
{
}

void SystemTester::runAllTests() {
  Serial.println("--- Running All Tests ---");
  testWiFi();
  testDMX();
  testArtnet();
  testDisplay();
  printSummary();
}

void SystemTester::testWiFi() {
  Serial.println("[WiFi Test] Starting...");
  // TODO: Insert your real Wi-Fi testing logic here
  // e.g., call WiFi.begin() or your WifiSetup() function
  // For demo, let's pretend it passed:
  wifiPassed = true;

  Serial.print("[WiFi Test] -> ");
  Serial.println(wifiPassed ? "PASS" : "FAIL");
}

void SystemTester::testDMX() {
  Serial.println("[DMX Test] Starting...");
  // TODO: Insert real DMX check
  // e.g., read a DMX slot or confirm data is received
  // For demo, let's pretend it passed:
  dmxPassed = true;

  Serial.print("[DMX Test] -> ");
  Serial.println(dmxPassed ? "PASS" : "FAIL");
}

void SystemTester::testArtnet() {
  Serial.println("[ArtNet Test] Starting...");
  // TODO: Insert real ArtNet check
  // e.g., confirm a packet arrived or try sending
  // For demo, let's pretend it passed:
  artnetPassed = true;

  Serial.print("[ArtNet Test] -> ");
  Serial.println(artnetPassed ? "PASS" : "FAIL");
}

void SystemTester::testDisplay() {
  Serial.println("[Display Test] Starting...");
  // TODO: Insert real display check
  // e.g., write something to SSD1306 and see if it draws
  // For demo, let's pretend it passed:
  displayPassed = true;

  Serial.print("[Display Test] -> ");
  Serial.println(displayPassed ? "PASS" : "FAIL");
}


void SystemTester::printSummary() {
  Serial.println("\n--- Test Summary ---");
  Serial.printf("WiFi    : %s\n", wifiPassed    ? "PASS" : "FAIL");
  Serial.printf("DMX     : %s\n", dmxPassed     ? "PASS" : "FAIL");
  Serial.printf("ArtNet  : %s\n", artnetPassed  ? "PASS" : "FAIL");
  Serial.printf("Display : %s\n", displayPassed ? "PASS" : "FAIL");
  Serial.println("---------------------\n");
}

// Create a global instance here, so it's visible in main.ino via extern
SystemTester tester;
