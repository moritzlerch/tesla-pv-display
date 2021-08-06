#ifndef Displayer_h
#define Displayer_h

// include libraries
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <math_tools.h>
#include <enums.h>

class Displayer {
    private:
        LiquidCrystal_I2C _lcd; 
        uint8_t _lcdCols;
        void registerCustomChars();
        String alignLeft(String input, int width);
        String alignLeft(String input);
        String alignRight(String input, int width);
        String alignRight(String input);
        String alignCenter(String input, int width);
        String alignCenter(String input);
        String generatePowerOutputString(double powerRaw);

    public:
        Displayer(uint8_t addr, uint8_t cols, uint8_t rows);
        void start();
        void backlight();
        void noBacklight();
        void clear();
        void print(String input, int col, int row);
        void print(String input);
        void write(int input);
        void setCursor(int col, int row);
        void createChar(uint8_t location, uint8_t charmap[]);
        void welcomeScreen();
        void connectingWifiScreen();
        void wifiConnectionFailedScreen();
        void wifiConnectionSuccessScreen();
        void wifiClientDetailsScreen(String hostname, String localIP);
        void displayRequestState(RequestState state);
        void informationScreen(double soc, double* powers);
};

Displayer::Displayer(uint8_t addr, uint8_t cols, uint8_t rows) : _lcd(addr, cols, rows) {
    this->_lcdCols = cols;
}

void Displayer::start() {
    this->_lcd.init();
    this->backlight();
    this->registerCustomChars();
}

void Displayer::backlight() {
    this->_lcd.backlight();
}

void Displayer::noBacklight() {
    this->_lcd.noBacklight();
}

void Displayer::clear() {
    this->_lcd.clear();
}

void Displayer::print(String input, int col, int row) {
    this->_lcd.setCursor(col, row);
    this->_lcd.print(input);
}

void Displayer::print(String input) {
    this->_lcd.print(input);
}

void Displayer::write(int input) {
    this->_lcd.write(input);
}

void Displayer::setCursor(int col, int row) {
    this->_lcd.setCursor(col, row);
}

void Displayer::createChar(uint8_t location, uint8_t charmap[]) {
    this->_lcd.createChar(location, charmap);
}

void Displayer::registerCustomChars() {
    byte charClear[]     = {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00000};
    byte charDotTop[]    = {B00011, B00011, B00000, B00000, B00000, B00000, B00000, B00000};
    byte charDotMiddle[] = {B00000, B00000, B00000, B00011, B00011, B00000, B00000, B00000};
    byte charDotBottom[] = {B00000, B00000, B00000, B00000, B00000, B00000, B00011, B00011};
    byte charConnectionIssue[] = {B00000, B00000, B00000, B00111, B01000, B10011, B10100, B10101};

    this->createChar(CC_CLEAR, charClear);
    this->createChar(CC_DOT_TOP, charDotTop);
    this->createChar(CC_DOT_MIDDLE, charDotMiddle);
    this->createChar(CC_DOT_BOTTOM, charDotBottom);
    this->createChar(CC_CONN_ISSUE, charConnectionIssue);
}

void Displayer::welcomeScreen() {
    this->clear();
    this->print(this->alignCenter("PV-Display"), 0, 1);
    this->print(this->alignCenter("Monitor Power!"), 0, 2);
}

void Displayer::connectingWifiScreen() {
    this->clear();
    this->print(this->alignCenter("Connecting"), 0, 1);
    this->print(this->alignCenter("to WiFi..."), 0, 2);
}

void Displayer::wifiConnectionFailedScreen() {
    this->clear();
    this->print(this->alignCenter("Connection"), 0, 1);
    this->print(this->alignCenter("failed"), 0, 2);

    delay(3000);

    this->clear();
    this->print(this->alignCenter("Check your"), 0, 0);
    this->print(this->alignCenter("configuration!"), 0, 1);
    this->print(this->alignCenter("Please reboot device"), 0, 3);
}

void Displayer::wifiConnectionSuccessScreen() {
    this->clear();
    this->print(this->alignCenter("Connected!"), 0, 1);
}

void Displayer::wifiClientDetailsScreen(String hostname, String localIP) {
    this->clear();
    this->print(this->alignLeft(hostname), 0, 0);
    this->print(this->alignLeft(localIP), 0, 1);
}

/**
 * This function prints the current API request states in the upper right corner
 * @param state enums.h
 */
void Displayer::displayRequestState(RequestState state) {
    // write an "F" before actual character if state means a WiFi-Connection-Issue
    if (state == REQUESTSTATE_CONN_ISSUE) { 
        this->setCursor(18, 0);
        this->print("F");
    }

    // now write the actual character
    this->setCursor(19, 0);
    switch (state) {
        case REQUESTSTATE_GET_AUTHTOKEN:
            this->write(CC_DOT_TOP);
            break;
        case REQUESTSTATE_GET_SOC:
            this->write(CC_DOT_MIDDLE);
            break;
        case REQUESTSTATE_GET_POWERFLOWS:
            this->write(CC_DOT_BOTTOM);
            break;
        case REQUESTSTATE_CONN_ISSUE:
            this->write(CC_CONN_ISSUE);
            break;
        case REQUESTSTATE_NONE:
        default:
            this->write(CC_CLEAR);
    }
}

/**
 * This function displays the main information screen (SOC + 4xPower)
 * 
 * @param soc state of charge
 * @param powers pointer to array with power values as doubles
 */
void Displayer::informationScreen(double soc, double* powers) {
    String str_soc = String(removeDecimalPlaces(soc, 2));
    String pwr_grid = this->generatePowerOutputString(powers[0]);
    String pwr_batt = this->generatePowerOutputString(powers[1]);
    String pwr_home = this->generatePowerOutputString(powers[2]);
    String pwr_solar = this->generatePowerOutputString(powers[3]);

    this->clear();
    this->print("SOC: " + str_soc + "%", 0, 0);
    this->print("G:" + this->alignRight(pwr_grid, 7), 0, 2);
    this->print("B:" + this->alignRight(pwr_batt, 7), 11, 2);
    this->print("H:" + this->alignRight(pwr_home, 7), 0, 3);
    this->print("S:" + this->alignRight(pwr_solar, 7), 11, 3);
}


/** --------------- UTILS --------------- */

String Displayer::alignLeft(String input, int width) {
    return input.substring(0, width); // if string is too long, just cut everything after the param width
}

String Displayer::alignLeft(String input) {
    return(this->alignLeft(input, this->_lcdCols));
}

String Displayer::alignRight(String input, int width) {
    int inputLength = input.length();

    if (inputLength >= width) {
        return input.substring(0, width); // if string is too long, just cut everything after the param width
    } else {
        String padding; 
        for (int i = ((width - inputLength)); i > 0; i--) {
            padding += " ";
        }

        return padding + input;
    }
}

String Displayer::alignRight(String input) {
    return(this->alignRight(input, this->_lcdCols));
}

String Displayer::alignCenter(String input, int width) {
    int inputLength = input.length();

    if (inputLength >= width) {
        return input.substring(0, width); // if string is too long, just cut everything after the param width
    } else {
        String padding; 
        for (int i = ((width - inputLength) / 2); i > 0; i--) {
            padding += " ";
        }

        return padding + input;
    }
}

String Displayer::alignCenter(String input) {
    return(this->alignCenter(input, this->_lcdCols));
}

/**
 * This function returns the current power depending if the power is greater than 10kW. 
 * Negative 10kW is treated the same. Returned is a string with "W" / "kW".
 * @param powerRaw double
 * @return string with power in kW / W
 */
String Displayer::generatePowerOutputString(double powerRaw) {
    if ((powerRaw >= 10000) || (powerRaw <= -10000)) {
        String output = removeDecimalPlaces(round_nplaces(powerRaw / 1000.0, 1), 1) + "kW";
        return output;
    } else {
        String output = String(trunc_double(powerRaw)) + "W";
        return output;
    }
}

#endif
