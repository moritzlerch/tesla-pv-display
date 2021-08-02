#ifndef Powerwall_h
#define Powerwall_h

// include librarys
#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

#include <math_tools.h>

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
        String getAuthCookie();
        String powerwallGetRequest(String url, String authCookie);
        String powerwallGetRequest(String url);
        double currBattPerc(String authCookie);
        double * currPower(String authCookie);
};

Powerwall::Powerwall() {
    powerwall_ip = POWERWALL_IP_CONFIG;
    tesla_email = TESLA_EMAIL;
    tesla_password = TESLA_PASSWORD;
    authCookie = "";
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

    String result = authJSON["token"];

    if (result == NULL) {
        getAuthCookie();
    }

    authCookie = result;
    return result;
}

String Powerwall::powerwallGetRequest(String url, String authCookie) {
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

    Serial.println(authCookie);

    httpsClient.print(String("GET ") + url + " HTTP/1.1" + "\r\n" +
                      "Host: " + powerwall_ip + "\r\n" +
                      "Cookie: " + "AuthCookie" + "=" + authCookie + "\r\n" +
                      "Connection: close\r\n\r\n");

    while (httpsClient.connected()) {
        String response = httpsClient.readStringUntil('\n');
        if (response == "\r") {
            break;
        }
    }

    return httpsClient.readStringUntil('\n');
}

String Powerwall::powerwallGetRequest(String url){
    return(this->powerwallGetRequest(url, this->getAuthCookie()));
}

double Powerwall::currBattPerc(String authCookie = "") {
    String tempAuthCookie = this->getAuthCookie();
    String socJson = this->powerwallGetRequest("/api/system_status/soe", tempAuthCookie);

    Serial.println(socJson);

    StaticJsonDocument<48> socJsonDoc;

    DeserializationError error = deserializeJson(socJsonDoc, socJson);

    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return 0.0;
    }

    double output = socJsonDoc["percentage"];
    output = round_down(output, 2);

    return output;
}

/**
 * This function returns the current power consumption of several endpoints as an array. 
 * Included are Power from Battery, Home, Grid and Solar. Have in mind that some of the
 * values might be negative, but keep in mind that you have solar :-)
 * @param authCookie
 * @return array of current power flows
 */
double * Powerwall::currPower(String authCookie) {
    
}

// double Powerwall::currPowerHome(String authCookie) {
    
// }

// double Powerwall::currPowerGrid(String authCookie) {
    
// }

// double Powerwall::currPowerSolar(String authCookie) {
    
// }


#endif
