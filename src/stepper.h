#include <Adafruit_ADS1X15.h>
#include <Stepper_28BYJ.h>
// изменить количество шагов для вашего мотора
#define STEPS 4078

Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */

Stepper_28BYJ stepper(STEPS, 13, 12, 14, 27);

void stepper_setup() {
  ads.setGain(GAIN_SIXTEEN);
  ads.begin(0x48);
  stepper.setSpeed(13); // обороты в минуту
}

void water_open(int n) {
  if (stepper_position + n  > stepper_max) {
    n = stepper_max-stepper_position;
  }
  stepper_position = stepper_position + n;
  stepper.step(n * stepper_d);
  Serial.print("water_open");
  Serial.println(n * stepper_d);
}

void water_close(int n) {
  if (stepper_position - n < 0) {
    n = stepper_position;
  }
  stepper_position = stepper_position - n;
  stepper.step(-1 * n * stepper_d);
  Serial.print("water_close");
  Serial.println(-1 * n * stepper_d);
}

void check_temper() {
  float results;
  float sum = 0;
  float cnt = 0;
  for (int i = 1; i <= 10; i++) {
    sum += ads.readADC_Differential_0_1();
    cnt++;
    delay(10);
  }
  // средняя величина разности на входах 0 и 1 АЦП
  results = sum / cnt;
  for (int i = 1; i < POINTS; i++) {
    tempers[i - 1] = tempers[i];
    Serial.print(tempers[i - 1],1);
    Serial.print(" ");
  }
  tempers[POINTS-1] = (float)round(10 * (results / 11.23 + 15)) / 10;
  tempToChart();
  Serial.print(tempers[POINTS-1],1);
  if(stepper_pause>0) {
    stepper_pause--;
    Serial.println(" ");
    return;
  }
  if (tempers[0] > 0) { // достаточно набрали информации. можно вычислять
    Serial.print(" Будущие значения:");
    calcMatrix();
    Serial.print(prognoz1,1);
    Serial.print(" ");
    Serial.print(prognoz2,1);
    Serial.println(" ");
    check_etaps();
  } else {
    Serial.println(" ");
  }
}
