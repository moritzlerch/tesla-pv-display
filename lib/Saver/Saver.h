#ifndef Saver_h
#define Saver_h

#include <Arduino.h>
#include <EEPROM.h>

const int locDisID = 0;  // Location of the Display-ID in the EEPROM storage

class Saver {
   private:
    byte readByteRaw(int address);
    void writeByteRaw(int address, byte byteval);

   public:
    Saver();
    void deleteEverything();
    void saveDisplayID(int id);
    int readDisplayID();
    bool isAvailableDisplayID();
};

Saver::Saver() {
    EEPROM.begin(4096);
}

byte Saver::readByteRaw(int address) {
    byte byteval = EEPROM.read(address);
    return byteval;
}

void Saver::writeByteRaw(int address, byte byteval) {
    EEPROM.write(address, byteval);
    EEPROM.commit();
}

void Saver::deleteEverything() {
    for (int i = 0; i < 4096; i++) {
        EEPROM.write(i, 255);
    }
    EEPROM.commit();
}

void Saver::saveDisplayID(int id) {
    if (id >= 0 && id <= 255) {
        this->writeByteRaw(locDisID, id);
    }
}

int Saver::readDisplayID() {
    return this->readByteRaw(locDisID);
}

bool Saver::isAvailableDisplayID() {
    byte value = this->readByteRaw(locDisID);

    if (value >= 1 && value <= 99) {
        return true;
    } else {
        return false;
    }
}

#endif