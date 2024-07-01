#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// WiFi credentials
const char* ssid = "Airtel_BhandGogi";
const char* password = "admin@123";

// ThingSpeak settings
unsigned long myChannelNumber =2588939;
const char* myWriteAPIKey = "FCM844RUZRM71ZR7";

// Ultrasonic sensor pins
const int trigPin = D5;
const int echoPin = D6;

WiFiClient client;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize ThingSpeak
  ThingSpeak.begin(client);

  // Initialize ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Read distance from ultrasonic sensor
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Print distance to Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Write data to ThingSpeak
  ThingSpeak.setField(1, distance);
  int responseCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if (responseCode == 200) {
    Serial.println("Data uploaded successfully");
  } else {
    Serial.println("Error uploading data: " + String(responseCode));
  }

  // Wait for 15 seconds (default ThingSpeak rate limit)
  delay(3000);
}

