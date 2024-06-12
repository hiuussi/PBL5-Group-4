#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>

const char* ssid = "Toan Anh";
const char* password = "65701526";

const char* serverName = "http://192.168.1.7:8000/classify";

WiFiClient wifiClient;

Servo esc; // Create a Servo object for the ESC
const int escPin = D5; // GPIO pin for ESC signal (D1 on most ESP8266 boards)

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    yield();
  }
  Serial.println("\nConnected to WiFi");

  // Attach the ESC to the escPin
  esc.attach(escPin);
  esc.writeMicroseconds(1000); // Initialize ESC with minimum throttle
  delay(2000); // Wait for the ESC to initialize
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.setTimeout(5000); // Set a timeout for the HTTP request
    http.begin(wifiClient, serverName); // Use the new API with WiFiClient
    http.addHeader("Content-Type", "application/json");

    // Create JSON payload
    String payload = "{\"data\":\"your_input_data\"}";

    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);

      // Parse the response to get the label
      int label = response.toInt(); // Assuming the response is just the label

      // Adjust ESC signal based on label
      int escSignal = 0;
      switch (label) {
        case 0:
          escSignal = 1400; // Light suction
          break;
        case 1:
          escSignal = 1600; // Moderate suction
          break;
        case 2:
          escSignal = 1800; // Strong suction
          break;
        case 3:
          escSignal = 1950; // Maximum suction
          break;
        default:
          escSignal = 1000; // Turn off the motor if label is not 1-4
          break;
      }
      esc.writeMicroseconds(escSignal);

    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected, reconnecting...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
      yield();
    }
    Serial.println("\nReconnected to WiFi");
  }

  delay(10000); // Wait for 10 seconds before sending the next request
  yield();
}
