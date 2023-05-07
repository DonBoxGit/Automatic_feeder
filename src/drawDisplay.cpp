#include "drawDisplay.h"

AlarmTime  alarm_1, alarm_2;
Menu menuState;
DateTime time;
LiquidCrystal_I2C *pLCD = new LiquidCrystal_I2C(DISPLAY_I2C_ADDR,
                                                DISPLAY_WIDTH, 
                                                DISPLAY_HEIGHT);

void printMainScreen() {
  /* Print current time from RTC */
  pLCD->setCursor(6, 0);
  pLCD->print(pRTC->getTimeString());

  /* Print first action time */
  pLCD->setCursor(4, 1);
  if (alarm_1.hours < 10) pLCD->print("0");
  pLCD->print(alarm_1.hours);
  pLCD->print(":");
  if (alarm_1.minutes < 10) pLCD->print("0");
  pLCD->print(alarm_1.minutes);
  
  /* Print second action time */
  pLCD->setCursor(11, 1);
  if (alarm_2.hours < 10) pLCD->print("0");
  pLCD->print(alarm_2.hours);
  pLCD->print(":");
  if (alarm_2.minutes < 10) pLCD->print("0");
  pLCD->print(alarm_2.minutes);
}

void drawDisplay(const Menu mState, uint8_t item) {
  switch (mState) {
    case Menu::SETTING:
      pLCD->setCursor(3, 0);
      pLCD->print("SETTINGS");

      pLCD->setCursor(1, 1);
      switch (item) {
        case 0:
          pLCD->print("[T1]  T2   ST ");
          break;
    
        case 1:
          pLCD->print(" T1  [T2]  ST ");
          break;

        case 2:
          pLCD->print(" T1   T2  [ST]");
          break;
      }
      break;

      case Menu::SET_ACTION_1:
        pLCD->setCursor(3, 0);
        pLCD->print("SET TIMER 1");

        drawSetActionTime(alarm_1, item);
        break;
      
      case Menu::SET_ACTION_2:
        pLCD->setCursor(3, 0);
        pLCD->print("SET TIMER 2");

        drawSetActionTime(alarm_2, item);
        break;
      
      case Menu::SET_CLOCK_TIME:
        pLCD->setCursor(5, 0);
        pLCD->print("SET CLOCK");

        pLCD->setCursor(2, 1);
        if (item == 0) pLCD->print("[");
        else pLCD->print(" ");
        if (time.hour < 10) pLCD->print("0");
        pLCD->print(time.hour);
        if (item == 0) pLCD->print("]");
        else pLCD->print(" ");
        pLCD->print(":");
        if (item == 1) pLCD->print("[");
        else pLCD->print(" ");
        if (time.minute < 10) pLCD->print("0");
        pLCD->print(time.minute);
        if (item == 1) pLCD->print("]");
        else pLCD->print(" ");
        pLCD->print(":");
        if (item == 2) pLCD->print("[");
        else pLCD->print(" ");
        if (time.second < 10) pLCD->print("0");
        pLCD->print(time.second);
        if (item == 2) pLCD->print("]");
        else pLCD->print(" ");
        break;
  }
}

void drawSetActionTime(AlarmTime &alarm, uint8_t item) {
  /* Print hours */
  pLCD->setCursor(4, 1);
  if (item == 0) pLCD->print("[");
  else pLCD->print(" ");
  if (alarm.hours < 10) pLCD->print("0");
  pLCD->print(alarm.hours);
  if (item == 0) pLCD->print("]");
  else pLCD->print(" ");
  pLCD->print(":");
  /* Print minutes */
  if (item == 1) pLCD->print("[");
  else pLCD->print(" ");
  if (alarm.minutes < 10) pLCD->print("0");
  pLCD->print(alarm.minutes);
  if (item == 1) pLCD->print("]");
  else pLCD->print(" ");
}
