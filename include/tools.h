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

String ip2Str(IPAddress ip){
  String s = "";
  for (int i = 0; i < 4; i++) {
    s += i ? "." + String(ip[i]) : String(ip[i]);
  }
  return s;
}

#endif