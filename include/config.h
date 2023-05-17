#ifndef _CONFIG_H_
#define _CONFIG_H_

/*-------------------------| RTC DS3231 settings |---------------------------*/
#define RTC_I2C_ADDR     0x68
#define CONTROL_REGISTER 0x0E
/* Bits of Control register */
#define INTCN_BIT        2  // INTerrupt CoNtrol. 0 - SQW(by Meandr), 1 -INT(by Alarm).
#define RS1_BIT          3  // Rate Select. Controls the frequency of the meandr.
#define RS2_BIT          4  // RS1 = 0 and RS2 = 0 -> 1kHz.
#define A1IE_BIT         0  // Alarm 1 Interrupt Enable.
#define A2IE_BIT         1  // Alarm 2 Interrupt Enable.

/*--------------------------| Display settings |-----------------------------*/
#define DISPLAY_I2C_ADDR            0x3F
#define DISPLAY_WIDTH               20
#define DISPLAY_HEIGHT              4
#define DISPLAY_BACKLIGHT_DURATION  20  // Measured in seconds.

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

/*--------------------------| Indicator settings |---------------------------*/
#define LED_INDICATOR_PIN         3
#define LED_INDICATOR_DURATION_MS 200  // Measured in milliseconds.

/*------------------------| Miscellaneous settings |-------------------------*/
#define SCREN_DRAW_DELAY_MS     200 // Measured in milliseconds.

#endif /* _CONFIG_H_ */