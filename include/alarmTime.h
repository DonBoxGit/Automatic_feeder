#ifndef _ALARM_TIME_H_
#define _ALARM_TIME_H_

#include <microDS3231.h>
#include "config.h"

extern MicroDS3231 *pRTC;

struct AlarmTime {
  int8_t hours;
  int8_t minutes;
  int8_t seconds; 
};

uint8_t readRegisterDS3231(void);
void writeRegisterDS3231(uint8_t);

#endif /* _ALARM_TIME_H_ */