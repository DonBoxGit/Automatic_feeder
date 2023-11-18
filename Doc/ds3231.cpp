#include <Wire.h>
#include <avr/io.h>
#include <config.h>

void writeRegister8(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(RTC_I2C_ADDR);
    #if ARDUINO >= 100
        Wire.write(reg);
        Wire.write(value);
    #else
        Wire.send(reg);
        Wire.send(value);
    #endif
    Wire.endTransmission();
}

uint8_t readRegister8(uint8_t reg) {
    uint8_t value;
    Wire.beginTransmission(RTC_I2C_ADDR);
    #if ARDUINO >= 100
        Wire.write(reg);
    #else
        Wire.send(reg);
    #endif
    Wire.endTransmission();

    Wire.requestFrom(RTC_I2C_ADDR, 1);
    while(!Wire.available()) {};
    #if ARDUINO >= 100
        value = Wire.read();
    #else
        value = Wire.receive();
    #endif;
    Wire.endTransmission();

    return value;
}