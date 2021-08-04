#ifndef TOOLS_H
#define TOOLS_H

#include <Saver.h>
#include <math_tools.h>

int countDigits(int x) {
    int length = 1;

    while (x /= 10) {
        length++;
    }
    
    return length;
}


String padInt(int n, int digits, String fill) {
    String zeroString;
    String intString = String(int(n));
    String fillString = fill;
    String output;
    
    int paddingZeros = digits - countDigits(n);
  
    for (int i = paddingZeros; i > 0; i--) {
        zeroString += fillString;
    }
  
    // append everything and return
    return zeroString + intString;
}


String generateHostname(Saver saverEEPROM) {
    int id = 0;
    
    if (!saverEEPROM.isAvailableDisplayID()) {
        id = random(1, 99);
        saverEEPROM.saveDisplayID(id);
    } else {
        id = saverEEPROM.readDisplayID();
    }
    
    String idStr = padInt(int(id), 2, "0");
  
    return("pvDisplay-" + idStr);
}


/**
 * This function returns the current power depending if the power is greater than 10kW. 
 * Negative 10kW is treated the same. Returned is a string with "W" / "kW".
 * @param powerRaw double
 * @return string with power in kW / W
 */
String generatePowerOutputString(double powerRaw) {
    if ((powerRaw >= 10000) || (powerRaw <= -10000)) {
        String output = removeDecimalPlaces(round_nplaces(powerRaw / 1000.0, 1), 1) + "kW";
        return output;
    } else {
        String output = String(trunc_double(powerRaw)) + "W";
        return output;
    }
}

#endif