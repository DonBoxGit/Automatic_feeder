/**************************************************************
 *                  Automatic feeder for birds                *
 *                         ver. 1.0.0                         *
 *     Used RTC DS3231 and liquidCrystal LCD on I2C wire      *
 **************************************************************/

#include <EncButton.h>
#include <EEPROM.h>
#include "config.h"
#include "drawDisplay.h"
#include "timer_blink.h"

Timer ledIndicatorDuration(LED_INDICATOR_DURATION_MS);
Timer correctionDrawScreen(SCREN_DRAW_DELAY_MS);

/* Initialization of buttons of control */
EncButton<EB_TICK, LEFT_BUTTON_PIN>  left_btn   (INPUT_PULLUP);
EncButton<EB_TICK, RIGHT_BUTTON_PIN> right_btn  (INPUT_PULLUP);
EncButton<EB_TICK, SELECT_BUTTON_PIN> select_btn(INPUT_PULLUP);
EncButton<EB_TICK, CANCEL_BUTTON_PIN> cancel_btn(INPUT_PULLUP);
/* Initialization of rotation sensor as a button */
EncButton<EB_TICK, ROTATION_SENSOR_PIN> rtn_sens(INPUT_PULLUP);

extern LiquidCrystal_I2C *pLCD;
extern MicroDS3231 *pRTC;
extern DateTime time;

static int8_t position = 0;
static uint8_t backlightCounter = 0;
static bool screenStatus = true;
volatile bool isr_sqw_flag = false;

enum class Mode : uint8_t {
  WORK = 0,
  EDITING,
  CALIBRATION,
  ERROR
} modeState;

void ISR_SQW() { isr_sqw_flag = true; }

void setup() {
  Serial.begin(9600);

  /* Get Alarm timers */
  EEPROM.get(ALARM_1_ADDR, alarm_1);
  EEPROM.get(ALARM_2_ADDR, alarm_2);
  /* Checking time's data and if they is incorrect then to change */
  if (!timeDataCheck(&alarm_1)) EEPROM.put(ALARM_1_ADDR, alarm_1);
  if (!timeDataCheck(&alarm_2)) EEPROM.put(ALARM_2_ADDR, alarm_2);

  /* Reading data from Control Register of RTC DS3231 */
  uint8_t value = readRegisterDS3231();
  /* Allows generating square pulse on SQW pin */
  value &= ~(1 << INTCN_BIT); // Reset the INTCN bit
  value &= ~((1 << RS1_BIT) | (1 << RS2_BIT));   // Reset the RS bits
  value &= ~((1 << A1IE_BIT) | (1 << A2IE_BIT)); // Reset the A1IE and A2IE bits
  /* Writing data to control register of RTC DS3231 */
  writeRegisterDS3231(value);
  Serial.println(value);

  /* Allowing an external interrupt on the SQW signal */
  pinMode(2, INPUT_PULLUP); // Input needs to pull up to VCC
  attachInterrupt(0, ISR_SQW, FALLING);  // INT0 attached

  pinMode(MOTOR_RELAY_PIN, OUTPUT);
  digitalWrite(MOTOR_RELAY_PIN, HIGH);

  pinMode(LED_INDICATOR_PIN, OUTPUT);
  digitalWrite(LED_INDICATOR_PIN, LOW);

  pLCD->init();
  pLCD->backlight();

  if (!pRTC->begin()) modeState = Mode::ERROR;
  else modeState = Mode::WORK;

  /* If battery is lost power */
  if (pRTC->lostPower()) {  // return true, if january 1st 2000 year
    modeState = Mode::EDITING;
    menuState = Menu::SET_CLOCK_TIME;
    drawDisplay(menuState, position);
  }
} 

void rotationSepatator() {
  digitalWrite(MOTOR_RELAY_PIN, LOW);
  while (true) {
    rtn_sens.tick();
    if (rtn_sens.release()) break; // This rotational sensor is triggered to open
    /* Need to cheking a terminator sensitivity */
  }
  digitalWrite(MOTOR_RELAY_PIN, HIGH);
}

void loop() {
  left_btn.tick();
  right_btn.tick();
  select_btn.tick();
  cancel_btn.tick();

  if (backlightCounter == DISPLAY_BACKLIGHT_DURATION && screenStatus) {
    pLCD->noBacklight();
    pLCD->noDisplay();
    screenStatus = false;
  }

  switch (modeState) {
    case Mode::WORK:
      if (screenStatus) {
        /* Execute every second */
        if (isr_sqw_flag) {
          if (correctionDrawScreen.ready())
            printMainScreen();
        }
      } else {
        /* Activating the LCD screen by pressing any button */
        if (left_btn.press()  || right_btn.press() ||
            select_btn.press() || cancel_btn.press()) {
          pLCD->backlight();
          pLCD->display();
          backlightCounter = 0;
          screenStatus = true;
        }
      }

      if (isr_sqw_flag) {
        /* Camparison a time of timers */
        if ((pRTC->getHours() == alarm_1.hours &&
           pRTC->getMinutes() == alarm_1.minutes &&
           pRTC->getSeconds() == alarm_1.seconds) ||
           (pRTC->getHours() == alarm_2.hours &&
           pRTC->getMinutes() == alarm_2.minutes &&
           pRTC->getSeconds() == alarm_2.seconds)) rotationSepatator();

        backlightCounter++;
        isr_sqw_flag = false;
        digitalWrite(LED_INDICATOR_PIN, HIGH);
      } else {
        if (ledIndicatorDuration.ready())
          digitalWrite(LED_INDICATOR_PIN, LOW);
      }

      /* Entering the calibration mode */
      if (left_btn.hold() && cancel_btn.hold()) {
        modeState = Mode::CALIBRATION;
        pLCD->clear();
        pLCD->setCursor(2, 0);
        pLCD->print("CALIBRATION");
        pLCD->setCursor(5, 1);
        pLCD->print("MODE");
      }

      /* Entering the settings menu */
      if (select_btn.press()) {
        modeState = Mode::EDITING;
        menuState = Menu::SETTING;
        pLCD->clear();
        drawDisplay(menuState, position);
        if (digitalRead(LED_INDICATOR_PIN))
          digitalWrite(LED_INDICATOR_PIN, LOW);
      }
      break;

    case Mode::EDITING:
      /* Pressing Left button */
      if (left_btn.press()) {
        if (menuState == Menu::SETTING) {
          if (--position < 0) position = 2;
        }
        /* Set time */
        if (menuState == Menu::SET_ACTION_1) {
          if (position == 0 && --alarm_1.hours < 0) alarm_1.hours = 23;
          if (position == 1 && --alarm_1.minutes < 0) alarm_1.minutes = 59;
        }
        if (menuState == Menu::SET_ACTION_2) {
          if (position == 0 && --alarm_2.hours < 0) alarm_2.hours = 23;
          if (position == 1 && --alarm_2.minutes < 0) alarm_2.minutes = 59;
        }
        if (menuState == Menu::SET_CLOCK_TIME) {
          if (position == 0 && --time.hour == 255) time.hour = 23;
          if (position == 1 && --time.minute == 255) time.minute = 59;
          if (position == 2 && --time.second == 255) time.second = 59;
        }

        drawDisplay(menuState, position);
      }
      /* Pressing Right button */
      if (right_btn.press()) {
        if (menuState == Menu::SETTING) {
           if (++position > 2) position = 0;
        }
        /* Set time */
        if (menuState == Menu::SET_ACTION_1) {
          if (position == 0 && ++alarm_1.hours > 23) alarm_1.hours = 0;
          if (position == 1 && ++alarm_1.minutes > 59) alarm_1.minutes = 0;
        }
        if (menuState == Menu::SET_ACTION_2) {
          if (position == 0 && ++alarm_2.hours > 23) alarm_2.hours = 0;
          if (position == 1 && ++alarm_2.minutes > 59) alarm_2.minutes = 0;
        }
        if (menuState == Menu::SET_CLOCK_TIME) {
          if (position == 0 && ++time.hour > 23) time.hour = 0;
          if (position == 1 && ++time.minute > 59) time.minute = 0;
          if (position == 2 && ++time.second > 59) time.second = 0;
        }

        drawDisplay(menuState, position);
      }

      /* Pressing Select button */
      if (select_btn.press()) {
        /* Select timer */
        if (menuState == Menu::SETTING) {
          pLCD->clear();
          if (position == 0) {
            menuState = Menu::SET_ACTION_1;
            position = -1;
          }

          if (position == 1) {
            menuState = Menu::SET_ACTION_2;
            position = -1;
          }

          if (position == 2) {
            menuState = Menu::SET_CLOCK_TIME;
            position = -1;
            time = pRTC->getTime();
          }
        }
          
        if (menuState == Menu::SET_ACTION_1) {
          if (++position == 2) {
            menuState = Menu::SETTING;
            position = 0;
            pLCD->clear();
            pLCD->setCursor(1, 0);
            pLCD->print("TIMER 1 SAVED");
            EEPROM.put(ALARM_1_ADDR, alarm_1);
            _delay_ms(800);
            pLCD->clear();
          }
        }

        if (menuState == Menu::SET_ACTION_2) {
          if (++position == 2) {
            menuState = Menu::SETTING;
            position = 1;
            pLCD->clear();
            pLCD->setCursor(1, 0);
            pLCD->print("TIMER 2 SAVED");
            EEPROM.put(ALARM_2_ADDR, alarm_2);
            _delay_ms(800);
            pLCD->clear();
          }
        }

        if (menuState == Menu::SET_CLOCK_TIME) {
          if (++position == 3) {
            menuState = Menu::SETTING;
            position = 2;
            pLCD->clear();
            pLCD->setCursor(1, 0);
            pLCD->print("CLOCK SAVED");
            pRTC->setTime(time);
            _delay_ms(800);
            pLCD->clear();
          }
        }
        drawDisplay(menuState, position);
      }

      /* Pressing Cancel button */
      if (cancel_btn.press()) {
        /* Exit to  Main screen from Setting */
        if (menuState == Menu::SETTING) {
          modeState = Mode::WORK;
          backlightCounter = 0;
          position = 0;
          pLCD->clear();
        }
        /* Exit to Setting screen */
        if (menuState == Menu::SET_ACTION_1) {
          menuState = Menu::SETTING;
          position = 0;
          pLCD->clear();
          drawDisplay(menuState, position);
        }
        if (menuState == Menu::SET_ACTION_2) {
          menuState = Menu::SETTING;
          position = 1;
          pLCD->clear();
          drawDisplay(menuState, position);
        }
        if (menuState == Menu::SET_CLOCK_TIME) {
          menuState = Menu::SETTING;
          position = 2;
          pLCD->clear();
          drawDisplay(menuState, position);
        }
      }
      break;

    case Mode::CALIBRATION:
      /* Test separator rotation */
      if (left_btn.press()) {
        rotationSepatator();
      }
      if (cancel_btn.press()) {
        modeState = Mode::WORK;
        pLCD->clear();
      }
      break;
    
    case Mode::ERROR:
      pLCD->clear();
      pLCD->setCursor(0, 0);
      pLCD->print("ERROR: RTC NOT FOUND!");
      while (true);
      break;
  }
}