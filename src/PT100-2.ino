#include <Arduino.h>
#include <stdlib.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
// #include "RBDmcuESP32.h"
#include "PWMrelay.h"
#include "aprox.h"
#include "variables.h"
PWMrelay relay(outputPin);
//dimmerLamp dimmer(outputPin, zerocross);
#include "etaps.h"
#include "html.h"
#include "stepper.h"

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display

#include "web_server.h"

int _state = 0;
int _cycle = 0;

void IRAM_ATTR onTimerISR() {
  relay.tick();
}

void setup() {
  Serial.begin(115200);
  lcd.init();                      // initialize the lcd
  lcd.backlight();
  lcd.setCursor(0, 0);
  relay.setLevel(LOW);
  relay.setPeriod(1000);
  relay.setPWM(dimmer_val*2.55);
//  dimmer.begin(NORMAL_MODE, ON);
//  dimmer.setPower(dimmer_val);
  webserver_init();
  stepper_setup();
  showtemp();

  hw_timer_t * timer = NULL;
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimerISR, true);
  timerAlarmWrite(timer, 1000, true); // вызов таймера через каждые 1000 микросекунд
  timerAlarmEnable(timer);
}

void loop() {
  server.handleClient();
  delay(200);
  _cycle++;
  if(_cycle>10) {
    _cycle = 0;
    check_temper();
    showtemp();
  }

  if(_state==2) {
    lcd.setCursor(0, 1);
    lcd.print("pause           ");
    return;
  }

}

void showtemp(){
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print(tempers[POINTS-1], 1);
  lcd.print("C D");
  lcd.print(dimmer_val);
  lcd.print(" Z");
  lcd.print(stepper_position);
}
