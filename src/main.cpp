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

// Auth-Stuff
int reUseAuthToken = 10;
int reUsedAuthToken = 0;
String authCookie = "";

LiquidCrystal_I2C lcd(0x27,20,4);

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,1);
  lcd.print("     PV-Display");
  lcd.setCursor(0,2);
  lcd.print("Monitor Solar Power!");

  delay(4000);

  String hostname = generateHostname(saverEEPROM);

  WiFi.hostname(hostname);
  WiFi.begin(wifi_ssid, wifi_password);
  
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("     Connecting");
  lcd.setCursor(0,2);
  lcd.print("     to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("     Connected!");
  delay(1000);

  Serial.print(hostname);
  Serial.print("@");
  Serial.println(WiFi.localIP());

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(hostname);
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());
  delay(1000);

}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {

    Serial.println(""); // space between console output

    if (reUsedAuthToken < reUseAuthToken && reUsedAuthToken != 0) {
      reUsedAuthToken++;
    } else {
      Serial.println("Getting new authToken!");
      authCookie = powerwall.getAuthCookie();

      if (reUsedAuthToken == 0) {
        reUsedAuthToken++;
      } else {
        reUsedAuthToken = 1;
      }
    }

    double soc = powerwall.currBattPerc(authCookie);
    double * powers = powerwall.currPowers(authCookie);

    String pwr_grid = generatePowerOutputString(powers[0]);
    String pwr_batt = generatePowerOutputString(powers[1]);
    String pwr_home = generatePowerOutputString(powers[2]);
    String pwr_solar = generatePowerOutputString(powers[3]);
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SOC: " + String(soc) + "%");
    lcd.setCursor(0,2);
    lcd.print("G:" + pwr_grid);
    lcd.setCursor(11,2);
    lcd.print("B:" + pwr_batt);
    lcd.setCursor(0,3);
    lcd.print("H:" + pwr_home);
    lcd.setCursor(11,3);
    lcd.print("S:" + pwr_solar);
  }

  delay(1000);
}