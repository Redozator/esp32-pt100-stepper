
#define STEPEN 4 // тут указываем степень полинома + 1
#define POINTS 7 // количество точек

// берем POINTS точек графика температуры и вычисляем следующие две точки
// как? очень просто: делаем апроксимацию полиномом N-й степени методом наименьших квадратов

float tempers[POINTS] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
float a_koef[STEPEN]; // массив для результирующих коэффициентов a в функции f(x) = a1+a2*x+a3*x*x+a4*x*x*x+...
float prognoz1; // рассчитанная следующая точка температуры
float prognoz2; // рассчитанная следующая через одну точка температуры

void calcMatrix() {
  // обнуляем массивы
  float b_koef[STEPEN]; // массив для промежуточных коэффициентов
  float sums[STEPEN][STEPEN];
  for (int i = 0; i < STEPEN; i++) {
    a_koef[i] = 0;
    b_koef[i] = 0;
    for (int j = 0; j < STEPEN; j++) {
      sums[i][j] = 0;
    }
  }
  // вычисляем sums и b_koef
  int N = POINTS;
  for (int i = 0; i < STEPEN; i++) {
    for (int j = 0; j < STEPEN; j++) {
      sums[i][j] = 0;
      for (int k = 0; k < N; k++) {
        sums[i][j] += pow(k + 1, i + j);
      }
    }
  }
  for (int i = 0; i < STEPEN; i++) {
    for (int k = 0; k < N; k++) {
      int x = k+1;
      float y = tempers[k];
      b_koef[i] += pow(x, i) * y;
    }
  }
  //делаем так, чтобы на главной диагонали не было нулей, чтобы не пришлось однажды делить на ноль в процессе приведения системы к треугольному виду
  float temp = 0;
  for (int i = 0; i < STEPEN; i++) {
    if (sums[i][i] == 0) {
      for (int j = 0; j < STEPEN; j++) {
        if (j == i) continue;
        if (sums[j][i] != 0 && sums[i][j] != 0) {
          for (int k = 0; k < STEPEN; k++) {
            temp = sums[j][k];
            sums[j][k] = sums[i][k];
            sums[i][k] = temp;
          }
          temp = b_koef[j];
          b_koef[j] = b_koef[i];
          b_koef[i] = temp;
          break;
        }
      }
    }
  }
  //процесс приведения системы уравнений к треугольному виду и обратная прогонка
  for (int k = 0; k < STEPEN; k++) {
    for (int i = k + 1; i < STEPEN; i++) {
      if (sums[k][k] == 0) {
        //printf("\nSolution is not exist.\n");
        prognoz1 = 0;
        prognoz2 = 0;
        return;
      }
      float M = sums[i][k] / sums[k][k];
      for (int j = k; j < STEPEN; j++) {
        sums[i][j] -= M * sums[k][j];
      }
      b_koef[i] -= M * b_koef[k];
    }
  }
  for (int i = STEPEN - 1; i >= 0; i--) {
    float s = 0;
    for (int j = i; j < STEPEN; j++) {
      s = s + sums[i][j] * a_koef[j];
    }
    a_koef[i] = (b_koef[i] - s) / sums[i][i];
  }
  prognoz1 = a_koef[0];
  prognoz2 = a_koef[0];
  for (int i = 1; i < STEPEN; i++) {
    prognoz1 += a_koef[i] * pow(POINTS+1, i);
    prognoz2 += a_koef[i] * pow(POINTS+2, i);
  }
  return;
}
