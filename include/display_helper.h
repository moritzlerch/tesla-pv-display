#ifndef DISPLAY_HELPER_H
#define MATH_TOOLS_H

#include <Arduino.h>

byte charRequestingDotTop[] = {
    B00011,
    B00011,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000
};

byte charRequestingDotMiddle[] = {
    B00000,
    B00000,
    B00000,
    B00011,
    B00011,
    B00000,
    B00000,
    B00000
};

byte charRequestingDotBottom[] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00011,
    B00011
};

byte charWifiIssue[] = {
    B00000,
    B00000,
    B00000,
    B00111,
    B01000,
    B10011,
    B10100,
    B10101
};

byte charClear[] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000
};

/**
 * This function prints the current API request states in the upper right corner
 * `state == 0` = clear
 * `state == 1` = dot top
 * `state == 2` = dot middle
 * `state == 3` = dot bottom
 * `state == 4` = connection issue
 * 
 * @param lcd instance of the LiquidCrystal_I2C
 * @param state see description
 */
void displayRequestingState(LiquidCrystal_I2C lcd, int state) {
    lcd.createChar(0, charClear);
    lcd.createChar(1, charRequestingDotTop);
    lcd.createChar(2, charRequestingDotMiddle);
    lcd.createChar(3, charRequestingDotBottom);
    lcd.createChar(4, charWifiIssue);

    if (state == 0) {
        lcd.setCursor(19, 0);
        lcd.write(0);
    } else if (state == 1) {
        lcd.setCursor(19, 0);
        lcd.write(1);
    } else if (state == 2) {
        lcd.setCursor(19, 0);
        lcd.write(2);
    } else if (state == 3) {
        lcd.setCursor(19, 0);
        lcd.write(3);
    } else if (state == 4) {
        lcd.setCursor(18, 0);
        lcd.print("F");
        lcd.setCursor(19, 0);
        lcd.write(4);
    }
}


#endif