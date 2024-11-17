#define BLYNK_TEMPLATE_ID "****" // Enter your Blynk Template ID
#define BLYNK_TEMPLATE_NAME "****" // Enter your Blynk Template Name
#define BLYNK_AUTH_TOKEN "*****" // Enter your Blynk Auth Token

#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <BlynkSimpleEsp32.h>

// Wi-Fi credentials
#define WIFI_SSID "*****" //Your SSID
#define WIFI_PASSWORD "*****" //Your Password

// Firebase credentials
#define FIREBASE_HOST "*******" //Your Firebase Host
#define FIREBASE_AUTH "*******" //Your Firebase Auth

#define MQ135_PIN 34 // GPIO Pin for MQ135 sensor

#define RED_LED 14 // GPIO Pin for LED

// Firebase objects
FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

#define BLYNK_VPIN V1 // Blynk virtual pin

void setup() {
  Serial.begin(115200);
  pinMode(MQ135_PIN, INPUT);
  pinMode(RED_LED, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");

  // Configure Firebase
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  // Initialize Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Serial.println("Firebase Initialized");

  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASSWORD); // Initialize Blynk
  Serial.println("Blynk Initialized");
}

void loop() {
  int gasConcentration = analogRead(MQ135_PIN); // Read gas concentration from MQ135 sensor

  Serial.print("Gas Concentration (ppm): ");
  Serial.println(gasConcentration);

  if(gasConcentration>100){
    digitalWrite(RED_LED, HIGH);
  }
  else{
    digitalWrite(RED_LED, LOW);
  }
  
  if (Firebase.RTDB.setFloat(&firebaseData, "/Air_data", gasConcentration))  {
    Serial.println("Data sent to Firebase");
  } else {
    Serial.print("Firebase error: ");
    Serial.println(firebaseData.errorReason());
  }

  Blynk.virtualWrite(BLYNK_VPIN, gasConcentration); // Send data to Blynk

  delay(5000);
  Blynk.run();
}
