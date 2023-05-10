#include "alarmTimer.h"

MicroDS3231 *pRTC = new MicroDS3231(RTC_I2C_ADDR);
AlarmTime  alarm_1, alarm_2;

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

void timeDataCheck(AlarmTime *pAlarmTime) {
  if (pAlarmTime->hours > 23 || pAlarmTime->minutes > 59 ||
      pAlarmTime->seconds > 59) {
    pAlarmTime->hours = 0;
    pAlarmTime->minutes = 0;
    pAlarmTime->seconds = 0;
    /* If wrong data then to reset and to put in EEPROM */
    if (pAlarmTime == ALARM_1_ADDR) EEPROM.put(ALARM_1_ADDR, pAlarmTime);
    else EEPROM.put(ALARM_2_ADDR, pAlarmTime);
  }
}