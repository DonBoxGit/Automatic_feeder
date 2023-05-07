#include "alarmTime.h"

MicroDS3231 *pRTC = new MicroDS3231(RTC_I2C_ADDR);

uint8_t readRegisterDS3231() {
  Wire.begin();
  Wire.beginTransmission(RTC_I2C_ADDR);
  Wire.write(CONTROL_REGISTER);
  Wire.requestFrom(RTC_I2C_ADDR, 1);
  while(!Wire.available()) {}
  return  Wire.read();
  Wire.endTransmission();
}

void writeRegisterDS3231(uint8_t val) {
  Wire.begin();
  Wire.beginTransmission(RTC_I2C_ADDR);
  Wire.write(CONTROL_REGISTER);
  Wire.write(val);
  Wire.endTransmission();
}