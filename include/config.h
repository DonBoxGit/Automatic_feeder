#ifndef _CONFIG_H_
#define _CONFIG_H_

/*-------------------------| RTC DS3231 settings |---------------------------*/
#define RTC_I2C_ADDR     0x68
#define CONTROL_REGISTER 0x0E
/* Bits of Control register */
#define INTCN_BIT        2
#define RS1_BIT          3
#define RS2_BIT          4

/*--------------------------| Display settings |-----------------------------*/
#define DISPLAY_I2C_ADDR            0x3F
#define DISPLAY_WIDTH               20
#define DISPLAY_HEIGHT              4
#define DISPLAY_BACKLIGHT_DURATION  20  // Measured in seconds

/*------------------| Button and sensor connection pins |--------------------*/
#define LEFT_BUTTON_PIN         9
#define RIGHT_BUTTON_PIN        10
#define SELECT_BUTTON_PIN       11
#define CANCEL_BUTTON_PIN       12
#define MOTOR_RELAY_PIN         15
#define ROTATION_SENSOR_PIN     7

/*---------------------| Addreses of alarms in EEPROM |----------------------*/
#define ALARM_1_ADDR    0x00
#define ALARM_2_ADDR    0x03

#endif /* _CONFIG_H_ */