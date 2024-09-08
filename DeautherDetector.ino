#include <WiFi.h>
#include <esp_wifi.h>

// Bypass ESPRESSIF security check
extern "C" int ieee80211_raw_frame_sanity_check(int32_t arg, int32_t arg2, int32_t arg3) {
    return 0;
}

// Define the GPIO pin for the LED
#define LED_PIN 2


/*SETTINGS*/
bool useSoloMode = true;  // Default to Solo Mode
const char* softAP_SSID = "DeautherDetector-AP";  // Default SoftAP SSID
const char* softAP_password = "";  // Leave empty for open AP
const char* network_SSID = "NETWORK_SSID";
const char* network_password = "NETWORK_PASS";
/*SETTINGS*/


// Define the callback function for promiscuous mode
void snifferPacketCallback(void* buf, wifi_promiscuous_pkt_type_t type) {
  wifi_promiscuous_pkt_t *snifferPacket = (wifi_promiscuous_pkt_t*)buf;
  const uint8_t* payload = snifferPacket->payload;

  // Get the current time in milliseconds since the ESP32 started
  unsigned long timestamp = millis();

  // Check if the packet is a deauthentication frame
  if (payload[0] == 0xC0) {  // 0xC0 is the frame type for deauthentication
    // Source MAC address is located at byte offset 10 in the deauthentication frame
    const uint8_t *src_mac = payload + 10;  // Source MAC address starts at byte 10
    const uint8_t *bssid = payload + 4;     // BSSID is located at byte 4

    // Print the timestamp and the MAC address of the attacker
    Serial.print("Timestamp: ");
    Serial.print(timestamp);
    Serial.print(" ms - Deauth packet detected! BSSID: ");
    if (bssid[0] == 0xFF && bssid[1] == 0xFF && bssid[2] == 0xFF && bssid[3] == 0xFF && bssid[4] == 0xFF && bssid[5] == 0xFF) {
      Serial.print("Broadcast ");
    } else {
      for (int i = 0; i < 6; i++) {
        if (i > 0) Serial.print(":");
        Serial.print(bssid[i], HEX);
      }
    }
    Serial.print(" | Source MAC: ");
    for (int i = 0; i < 6; i++) {
      if (i > 0) Serial.print(":");
      Serial.print(src_mac[i], HEX);
    }
    Serial.println();

    delay(250);
    digitalWrite(LED_PIN, HIGH);  // Turn on LED
    delay(1000);                  // Keep the LED on for 1 second
    digitalWrite(LED_PIN, LOW);   // Turn off LED
  }
}

void setup() {
  Serial.begin(115200);

  // Set LED pin as output
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Turn LED off initially

  if (useSoloMode) {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(softAP_SSID, softAP_password);  // AP SSID without password
    Serial.println("Access Point Started");
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());  // Print the IP address of the AP
  } else {
    // Initialize the WiFi in Station mode
    WiFi.mode(WIFI_STA);
    WiFi.begin(network_SSID, network_password);  // Connect to the specified network

    Serial.println("Connecting to WiFi...");
    
    // Wait for connection
    for (int attempt = 0; WiFi.status() != WL_CONNECTED && attempt < 10; attempt++) {
      delay(1000);
      Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nConnected to WiFi!");
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());  // Print the IP address assigned by the network
      Serial.print("BSSID: ");
      Serial.println(WiFi.BSSIDstr()); // Print the BSSID of the connected network
    } else {
      Serial.println("\nFailed to connect to WiFi.");
      Serial.print("Connection Status: ");
      Serial.println(WiFi.status()); // Print connection status code
    }
  }
    // Set ESP32 to promiscuous mode
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_promiscuous_rx_cb(&snifferPacketCallback);
}

void loop() {
  // The ESP32 will continue to monitor WiFi packets and log deauthentication frames
  delay(1000);  // Prevents instructions to be spammed.
}
