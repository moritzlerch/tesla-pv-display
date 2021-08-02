#ifndef TOOLS_H
#define TOOLS_H

#include <Saver.h>

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

  // append everything
  output = zeroString + intString;

  return output;
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


#endif