#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "Arjun";       // Replace with your Wi-Fi SSID
const char* password = ""; // Replace with your Wi-Fi password
// const char* serverUrl = "http://192.X.X.X:3000/"; // Replace with your server's IP and endpoint
const char* serverUrl = "192.X.X.X"; // Replace with your server's IP and endpoint
const int serverPort = 3000;               // Replace with your server's port

WiFiClient client;
HTTPClient http;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");

  // Establish a persistent connection to the server
  if (client.connect(serverUrl, serverPort)) {
    Serial.println("Connected to server");
  } else {
    Serial.println("Failed to connect to server");
  }
}

void loop() {
  // Check if the server connection is still alive
  if (client.connected()) {
    // Read input from the Serial Monitor
    if (Serial.available() > 0) {
      String message = Serial.readStringUntil('\n'); // Read the message
      message.trim(); // Remove any extra whitespace

      // Send the message to the server
      http.begin(client, serverUrl, serverPort, "/", true); // Use the persistent connection
      http.addHeader("Content-Type", "application/json");

      // Create a JSON payload
      String payload = "{\"message\":\"" + message + "\"}";

      // Send POST request
      int httpResponseCode = http.POST(payload);

      // Check the response
      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("HTTP Response code: " + String(httpResponseCode));
        Serial.println("Response: " + response);
      } else {
        Serial.println("Error in HTTP request");
      }

      http.end(); // End the HTTP request but keep the TCP connection alive
    }
  } else {
    // Reconnect if the connection is lost
    Serial.println("Server connection lost. Reconnecting...");
    if (client.connect(serverUrl, serverPort)) {
      Serial.println("Reconnected to server");
    } else {
      Serial.println("Failed to reconnect to server");
    }
  }

  delay(100); // Small delay to avoid flooding the Serial Monitor
}

// void setup() {
//   Serial.begin(115200);
//   WiFi.begin(ssid, password);

//   Serial.print("Connecting to Wi-Fi");
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(1000);
//     Serial.print(".");
//   }
//   Serial.println("\nConnected to Wi-Fi");
// }

// void loop() {
//   if (WiFi.status() == WL_CONNECTED) {
//     HTTPClient http;
//     WiFiClient client; // Create a WiFiClient object
//     http.begin(client, serverUrl); // Use the updated API
//     http.addHeader("Content-Type", "application/json");

//     // Create a JSON payload
//     String payload = "{\"key\":\"value\"}";

//     // Send POST request
//     int httpResponseCode = http.POST(payload);

//     // Check the response
//     if (httpResponseCode > 0) {
//       String response = http.getString();
//       Serial.println("HTTP Response code: " + String(httpResponseCode));
//       Serial.println("Response: " + response);
//     } else {
//       Serial.println("Error in HTTP request");
//     }

//     http.end();
//   } else {
//     Serial.println("Wi-Fi disconnected");
//   }

//   delay(5000); // Wait 5 seconds before sending the next request
// }