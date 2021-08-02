#include <Arduino.h>

#include <LiquidCrystal_I2C.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
// #include <WiFiClient.h>
#include <ArduinoJson.h>

#include <WiFiClientSecureBearSSL.h>

// custom imports
#include <Saver.h>
#include <Powerwall.h>
#include <tools.h>
#include <math_tools.h>

// include config
#include <secrets.h>
const char *wifi_ssid = WIFI_SSID_SECRETS;
const char *wifi_password = WIFI_PASSWORD_SECRETS;

// Objects
Saver saverEEPROM = Saver();
Powerwall powerwall = Powerwall();

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("   PV-Display");
  lcd.setCursor(0,1);
  lcd.print("Watch for Power!");

  delay(4000);

  String hostname = generateHostname(saverEEPROM);

  WiFi.hostname(hostname);
  WiFi.begin(wifi_ssid, wifi_password);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   Connecting");
  lcd.setCursor(0,1);
  lcd.print("   to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   Connected!");
  delay(1000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(hostname);
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());
  delay(1000);

}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(powerwall.getAuthCookie());
    String result = powerwall.powerwallGetRequest("/api/status");

    const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
    DynamicJsonDocument doc(bufferSize);
    deserializeJson(doc, result);

    String version = doc["version"];
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(version);
  }

  delay(30000);
}