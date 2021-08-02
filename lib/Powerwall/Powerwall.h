#ifndef Powerwall_h
#define Powerwall_h

// include librarys
#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

// import config files
#include <config.h>
#include <secrets.h>


class Powerwall {
    private:
        const char* powerwall_ip;
        String tesla_email;
        String tesla_password;
        String authCookie;

    public:
        Powerwall();
        String powerwallGetRequest(String url);
        String getAuthCookie();
};

Powerwall::Powerwall() {
    powerwall_ip = POWERWALL_IP_CONFIG;
    tesla_email = TESLA_EMAIL;
    tesla_password = TESLA_PASSWORD;
}

String Powerwall::getAuthCookie() {
    String apiLoginURL = "/api/login/Basic";

    WiFiClientSecure httpsClient;
    httpsClient.setInsecure();
    httpsClient.setTimeout(10000);
    int retry = 0;

    while ((!httpsClient.connect(powerwall_ip, 443)) && (retry < 15)) {
        delay(100);
        Serial.print(".");
        retry++;
    }

    if (retry >= 15) {
        return("CONN-FAIL");
    }

    String dataString;
    StaticJsonDocument<192> authJsonDoc;

    authJsonDoc["username"] = "customer";
    authJsonDoc["email"] = tesla_email;
    authJsonDoc["password"] = tesla_password;

    serializeJson(authJsonDoc, dataString);

    // dataString = "{\"username\": \"customer\",\"email\": \"" + tesla_email + "\",\"password\":\"" + tesla_password + "\"}"; // got replaced by ArduinoJson stuff above
    // '{"username": "customer","email": "' + tesla_email + '","password":"' + tesla_password + '"}'
    

    httpsClient.print(String("POST ") + apiLoginURL + " HTTP/1.1\r\n" +
                      "Host: " + powerwall_ip + "\r\n" +
                      "Connection: close" + "\r\n" +
                      "Content-Type: application/json" + "\r\n" +
                      "Content-Length: " + dataString.length() + "\r\n" +
                      "\r\n" + dataString + "\r\n\r\n");

    while (httpsClient.connected()) {
        String response = httpsClient.readStringUntil('\n');
        if (response == "\r") {
            break;
        }
    }

    String jsonInput = httpsClient.readStringUntil('\n');

    StaticJsonDocument<384> authJSON;

    DeserializationError error = deserializeJson(authJSON, jsonInput);

    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return "ERROR (getAuthCookie())";
    }

    return authJSON["token"];
}

String Powerwall::powerwallGetRequest(String url) {
    WiFiClientSecure httpsClient;
    httpsClient.setInsecure();
    httpsClient.setTimeout(10000);

    int retry = 0;

    while ((!httpsClient.connect(powerwall_ip, 443)) && (retry < 15)) {
        delay(100);
        Serial.print(".");
        retry++;
    }

    if (retry >= 15) {
        return("CONN-FAIL");
    }

    httpsClient.print(String("GET ") + url + " HTTP/1.1\r\n" +
                      "Host: " + powerwall_ip +
                      "\r\n" + "Connection: close\r\n\r\n");

    while (httpsClient.connected()) {
        String response = httpsClient.readStringUntil('\n');
        if (response == "\r") {
            break;
        }
    }

    return httpsClient.readStringUntil('\n');
}

#endif
