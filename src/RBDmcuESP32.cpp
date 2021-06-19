#include "RBDmcuESP32.h"


int pulseWidth = 1;
volatile int current_dim = 0;
int all_dim = 3;
int rise_fall = true;
char user_zero_cross = '0';

static int toggleCounter = 0;
static int toggleReload = 25;
static unsigned long debug_timer = 0;
static int zeroCounter = 0;
static int zeroReload = 120;

static dimmerLamp* dimmer[ALL_DIMMERS];
volatile uint16_t dimPower[ALL_DIMMERS];
volatile uint16_t dimOutPin[ALL_DIMMERS];
volatile uint16_t dimZCPin[ALL_DIMMERS];
volatile uint16_t zeroCross[ALL_DIMMERS];
volatile DIMMER_MODE_typedef dimMode[ALL_DIMMERS];
volatile ON_OFF_typedef dimState[ALL_DIMMERS];
volatile uint16_t dimCounter[ALL_DIMMERS];
static uint16_t dimPulseBegin[ALL_DIMMERS];
volatile uint16_t togMax[ALL_DIMMERS];
volatile uint16_t togMin[ALL_DIMMERS];
volatile bool togDir[ALL_DIMMERS];

dimmerLamp::dimmerLamp(int user_dimmer_pin, int zc_dimmer_pin) :
dimmer_pin(user_dimmer_pin),
zc_pin(zc_dimmer_pin) {
	current_dim++;
	dimmer[current_dim - 1] = this;
	current_num = current_dim - 1;
	toggle_state = false;

	dimPulseBegin[current_dim - 1] = 1;
	dimOutPin[current_dim - 1] = user_dimmer_pin;
	dimZCPin[current_dim - 1] = zc_dimmer_pin;
	dimCounter[current_dim - 1] = 0;
	zeroCross[current_dim - 1] = 0;
	dimMode[current_dim - 1] = NORMAL_MODE;
	togMin[current_dim - 1] = 0;
	togMax[current_dim - 1] = 1;
	pinMode(user_dimmer_pin, OUTPUT);
}

void dimmerLamp::timer_init(void) {
	hw_timer_t * timer = NULL;
	// Use 1st timer of 4 (counted from zero).
	// Set 80 divider for prescaler (see ESP32 Technical Reference Manual for more info).
	timer = timerBegin(0, 80, true);
	// Attach onTimer function to our timer.
	timerAttachInterrupt(timer, &onTimerISR, true);
	// Set alarm to call onTimer function every second (value in microseconds).
	// Repeat the alarm (third parameter)
	timerAlarmWrite(timer, 200, true);
	// Start an alarm
	timerAlarmEnable(timer);
}

void dimmerLamp::ext_int_init(void) {
	int inPin = dimZCPin[this->current_num];
	// что-то не работает прерывание. по всякому пробовал. Работает только если питание не подавать на диммер.
	// но при этом прерывание вызывается слишком часто. Явно неправильно
//  pinMode(inPin, INPUT_PULLUP); 
  attachInterrupt(inPin, isr_ext, RISING);
}

void dimmerLamp::begin(DIMMER_MODE_typedef DIMMER_MODE, ON_OFF_typedef ON_OFF) {
	dimMode[this->current_num] = DIMMER_MODE;
	dimState[this->current_num] = ON_OFF;
	timer_init();
	ext_int_init();
}

void dimmerLamp::setPower(int power) {
	if (power >= 99) {
		power = 99;
	}
	dimPower[this->current_num] = power;
	dimPulseBegin[this->current_num] = powerBuf[power];
	delay(1);
}

int dimmerLamp::getPower(void) {
	if (dimState[this->current_num] == ON)
		return dimPower[this->current_num];
	else return 0;
}

unsigned long dimmerLamp::getDebug_timer(void) {
	return debug_timer;
}

void dimmerLamp::setState(ON_OFF_typedef ON_OFF) {
	dimState[this->current_num] = ON_OFF;
}

bool dimmerLamp::getState(void) {
	bool ret;
	if (dimState[this->current_num] == ON) ret = true;
	else ret = false;
	return ret;
}

void dimmerLamp::changeState(void) {
	if (dimState[this->current_num] == ON) dimState[this->current_num] = OFF;
	else
		dimState[this->current_num] = ON;
}

DIMMER_MODE_typedef dimmerLamp::getMode(void) {
	return dimMode[this->current_num];
}

void dimmerLamp::setMode(DIMMER_MODE_typedef DIMMER_MODE) {
	dimMode[this->current_num] = DIMMER_MODE;
}

void dimmerLamp::toggleSettings(int minValue, int maxValue) {
	if (maxValue > 99) {
		maxValue = 99;
	}
	if (minValue < 1) {
		minValue = 1;
	}
	dimMode[this->current_num] = TOGGLE_MODE;
	togMin[this->current_num] = powerBuf[maxValue];
	togMax[this->current_num] = powerBuf[minValue];

	toggleReload = 50;
}

void IRAM_ATTR isr_ext() {
//	zeroCounter++;
//	if (zeroCounter > zeroReload) { // что-то через ноль проходим каждые 65 микросекунд. Увеличим
//		zeroCounter = 0;
		for (int i = 0; i < current_dim; i++)
			if (dimState[i] == ON) {
				zeroCross[i] = 1;
				// при нуле отключаем питание
				digitalWrite(dimOutPin[i], LOW);
				debug_timer++;
				dimCounter[i] = 0;
			}
//	}
}

static int k;

void IRAM_ATTR onTimerISR() {
	//	unsigned long time = micros();
	//	debug_timer = time-debug_timer;
	toggleCounter++;
	for (k = 0; k < current_dim; k++) {
		if (zeroCross[k] == 1) {
			dimCounter[k]++;

			if (dimMode[k] == TOGGLE_MODE) {
				/*****
				   TOGGLE DIMMING MODE
				 *****/
				if (dimPulseBegin[k] >= togMax[k]) {
					// if reach max dimming value
					togDir[k] = false; // downcount
				}
				if (dimPulseBegin[k] <= togMin[k]) {
					// if reach min dimming value
					togDir[k] = true; // upcount
				}
				if (toggleCounter == toggleReload) {
					if (togDir[k] == true) dimPulseBegin[k]++;
					else dimPulseBegin[k]--;
				}
			}

			// При нуле выключаем, а через определенное число циклов включаем
			// Чем больше циклов пропустить, тем меньше мощности на потребителя.
			// Соответственно циклов должно быть 100 от зеро до зеро. А это 200 микросекунд (1/5000 секунды)
			if (dimCounter[k] >= dimPulseBegin[k]) {
				if (dimPower[k] <= 0) {
					digitalWrite(dimOutPin[k], LOW); // power=0, вообще никогда не включаем питание
				} else {
					digitalWrite(dimOutPin[k], HIGH);
				}
				zeroCross[k] = 0; // оставляем включенным питание до следующего прохода через ноль
			}
		}
	}
	if (toggleCounter >= toggleReload) toggleCounter = 1;

}
