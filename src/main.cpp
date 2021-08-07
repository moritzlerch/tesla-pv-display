#include <Arduino.h>

#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>

// custom imports
#include <Displayer.h>
#include <Saver.h>
#include <Powerwall.h>
#include <tools.h>
#include <math_tools.h>
#include <enums.h>

// include config
#include <secrets.h>
const char* wifi_ssid     = WIFI_SSID_SECRETS;
const char* wifi_password = WIFI_PASSWORD_SECRETS;

// Objects
Displayer display   = Displayer(0x27, 20, 4);
Saver saverEEPROM   = Saver();
Powerwall powerwall = Powerwall();

// Auth-Stuff
int reUseAuthToken  = 10;
int reUsedAuthToken = 0;
String authCookie   = "";

// custom chars for displaying API requests
bool showRequestState = true;

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
    Serial.begin(9600);

    display.start();
    display.welcomeScreen();
    delay(4000);

    // lcd.init();
    // lcd.backlight();

    WiFi.softAPdisconnect(true);

    String hostname = generateHostname(saverEEPROM);
    WiFi.hostname(hostname);
    WiFi.begin(wifi_ssid, wifi_password);

    display.connectingWifiScreen();

    int counterReconnectWifi = 0;

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting...");
        counterReconnectWifi++;

        if (counterReconnectWifi >= 30) {
            display.wifiConnectionFailedScreen();
            while (true) { delay(1000); }  // endless loop ;-)
        }
    }

    display.wifiConnectionSuccessScreen();
    delay(1000);

    String localIP = ip2Str(WiFi.localIP());
    Serial.println(hostname + "@" + localIP);
    display.wifiClientDetailsScreen(hostname, localIP);
    delay(1000);
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("");  // space between console output

        // check if new authToken should get requested
        if (reUsedAuthToken < reUseAuthToken && reUsedAuthToken != 0) {
            reUsedAuthToken++;
        } else {
            Serial.println("Getting new authToken!");
            if (showRequestState) { display.displayRequestState(REQUESTSTATE_GET_AUTHTOKEN); }

            authCookie = powerwall.getAuthCookie();

            if (reUsedAuthToken == 0) {
                reUsedAuthToken++;
            } else {
                reUsedAuthToken = 1;
            }
        }

        if (showRequestState) { display.displayRequestState(REQUESTSTATE_GET_SOC); }
        double soc = powerwall.currBattPerc(authCookie);  // Request SOC

        if (showRequestState) { display.displayRequestState(REQUESTSTATE_GET_POWERFLOWS); }
        double* powers = powerwall.currPowers(authCookie);  // Request power flows

        display.informationScreen(soc, powers);

    } else {
        display.displayRequestState(REQUESTSTATE_CONN_ISSUE);
        delay(2000);
    }

    // start over
    // delay(1000);
}