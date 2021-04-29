#include <Arduino.h>
#include <stdlib.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "aprox.h"
#include "variables.h"
#include "etaps.h"
#include "html.h"
#include "stepper.h"
#include "web_server.h"


LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x3F for a 16 chars and 2 line display

int _state = 0;
int _cycle = 0;


void setup() {
  Serial.begin(115200);
  webserver_init();
  stepper_setup();


  lcd.init();                      // initialize the lcd
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(ip);
  showtemp();
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
  lcd.print("T: ");
  lcd.print(tempers[POINTS-1], 1);
  lcd.print(" C     ");
}
