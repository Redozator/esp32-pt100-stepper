#define PRED_LIMIT 50

void water_open(int n);
void water_close(int n);
void water_open_p();
void water_close_p();
double temp_temp; // временное значение температуры. нужно на некоторых этапах

// реагирование на температуру по этапу1
// ждем когда температура вырастет до 50 градусов и переходим на этап2
// предполагается, что заслонка почти закрыта

void etap1() {
	if (tempers[POINTS - 1] > PRED_LIMIT) {
		// диммер - на низкое значение
		dimmer_val = dimmer_low;
		// dimmer.setPower(dimmer_val);
		relay.setPWM(dimmer_val*2.55);
		// открываем заслонку на максимум
		water_open(stepper_max / 2);
		stepper_pause = 10;
		current_etap = 2;
		Serial.println("Переход на этап 2");
	}
}

// реагирование на температуру по этапу2
// увеличиваем диммер, пока температура не начнет увеличиваться
// когда температура будет увеличиваться и перейдет 50 градусов, переходим на следующий этап
//

void etap2() {
	double d_temp = tempers[POINTS - 1] - tempers[POINTS - 2];
	if (tempers[POINTS - 1] > PRED_LIMIT) {
		if (dimmer_val <= dimmer_low) {
			// видимо еще не среагировала система
			// но на всякий случай проверим, не сильно ли поднялась температура
			if (tempers[POINTS - 1] > limit_min) {
				dimmer_val -= 5;
				if (dimmer_val < 0) {
					dimmer_val = 0;
				}
				// dimmer.setPower(dimmer_val);
				relay.setPWM(dimmer_val*2.55);
			}
			return;
		}
		dimmer_val--;
		if (dimmer_val < 0) {
			dimmer_val = 0;
		}
		// dimmer.setPower(dimmer_val);
		relay.setPWM(dimmer_val*2.55);
		stepper_pause = 5;
		current_etap = 3;
		Serial.println("Переход на этап 3");
	}
	if (d_temp < 0 && prognoz1 < tempers[POINTS - 1]) {
		// температура понижается, а должна повышаться
		dimmer_val++;
		if (dimmer_val > 95) {
			dimmer_val = 95;
		}
		// dimmer.setPower(dimmer_val);
		relay.setPWM(dimmer_val*2.55);
		stepper_pause = 4;
		return;
	}
	// если температура не двигается, ждем
}

// реагирование на температуру по этапу3
// температура должна медленно подняться до минимального предела
// после чего переходим на этап4

void etap3() {
	if (tempers[POINTS - 1] > limit_min) {
		current_etap = 4;
		temp_temp = 0;
		Serial.println("Переход на этап 4");
		return;
	}
	if (tempers[POINTS - 1] < PRED_LIMIT) {
		current_etap = 2;
		Serial.println("Возвращаемся на этап 2");
		return;
	}
	double d_temp = tempers[POINTS - 1] - tempers[POINTS - 2];
	// если температура никуда не движется, нужно добавить температуру
	if (d_temp < 0.2 && d_temp>-0.2 && tempers[POINTS - 1] - prognoz2 <= 0.2 && tempers[POINTS - 1] - prognoz2 >= -0.2) {
		dimmer_val++;
		if (dimmer_val > 95) {
			dimmer_val = 95;
			water_close(1); // прикроем заслонку
		}
		// dimmer.setPower(dimmer_val);
		relay.setPWM(dimmer_val*2.55);
		stepper_pause = 4;
	}
}

// реагирование на температуру по этапу4
// поддержание заданной температуры между допустимыми пределами

void etap4() {
	double seredina = (limit_min + limit_max) / 2;
	if (tempers[POINTS - 1]<(limit_min - 1)) {
		if (tempers[POINTS - 1] < tempers[POINTS - 2]) { // падает температура
			dimmer_val++; // увеличим диммер
			if (dimmer_val > dimmer_critic) {
				dimmer_val = dimmer_critic;
			}
			// dimmer.setPower(dimmer_val);
			relay.setPWM(dimmer_val*2.55);
			stepper_pause = 4;
			if (temp_temp == 0 || temp_temp < tempers[POINTS - 1]) {
				temp_temp = tempers[POINTS - 1]; // пока не переходим на этап 3, но готовимся
			} else {
				if (temp_temp > tempers[POINTS - 1]) { // температура продолжает падать, уходим на этап 3
					dimmer_critic = 95;
					temp_temp = 0;
					current_etap = 3;
					Serial.println("Возвращаемся на этап 3");
				}
			}
			return;
		}
	}
	if (tempers[POINTS - 1] > seredina) {
		if (tempers[POINTS - 1] > limit_max) {
			// превышен максимум, уменьшаем тэн
			dimmer_critic = dimmer_val;
			dimmer_val -= 1;
			if (dimmer_val < 0) {
				dimmer_val = 0;
			}
			// dimmer.setPower(dimmer_val);
			relay.setPWM(dimmer_val*2.55);
			stepper_pause = 2;
			return;
		}
		if (prognoz2 - tempers[POINTS - 1] > 3) {
			// тенденция к увеличению. нужно уменьшить диммер
			dimmer_val--;
			if (dimmer_val < 0) {
				dimmer_val = 0;
			}
			// dimmer.setPower(dimmer_val);
			relay.setPWM(dimmer_val*2.55);
			stepper_pause = 3;
			return;
		}
	}
	if (tempers[POINTS - 1] < limit_min) {
		dimmer_val++; // увеличим диммер
		if (dimmer_val > dimmer_critic) {
			dimmer_val = dimmer_critic;
		}
		if (dimmer_val > 95) {
			dimmer_val = 95;
			water_close(1); // прикроем заслонку
		}
		// dimmer.setPower(dimmer_val);
		relay.setPWM(dimmer_val*2.55);
		stepper_pause = 3;
		return;
	}

}

// реагирование на температуру по этапу5

void etap5() {
}

void check_etaps() {
	if (current_etap == 0 || current_etap == 1) {
		current_etap = 1;
		etap1();
		return;
	}
	if (current_etap == 2) {
		etap2();
		return;
	}
	if (current_etap == 3) {
		etap3();
		return;
	}
	if (current_etap == 4) {
		etap4();
		return;
	}
	if (current_etap == 5) {
		etap5();
		return;
	}
}
