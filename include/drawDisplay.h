#ifndef _DRAW_DISPLAY_H_
#define _DRAW_DISPLAY_H_

#include <LiquidCrystal_I2C.h>
#include "config.h"
#include "alarmTime.h"

extern MicroDS3231 *pRTC;
extern LiquidCrystal_I2C *pLCD;
extern AlarmTime alarm_1;
extern AlarmTime alarm_2;

enum class Menu : uint8_t {
  SETTING = 0,
  SET_ACTION_1,
  SET_ACTION_2,
  SET_CLOCK_TIME
};
extern Menu menuState;

void printMainScreen();
void drawDisplay(const Menu mState = Menu::SETTING, uint8_t item = 0);
void drawSetActionTime(AlarmTime &alarm, uint8_t item);

#endif /* _DRAW_DISPLAY_H_ */