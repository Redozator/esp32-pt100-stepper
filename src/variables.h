#define CNT_CHART 100
#define outputPin  16
// #define zerocross  17

double chart_y[CNT_CHART];
int chart_stepper[CNT_CHART];
int chart_dimmer[CNT_CHART];

int stepper_position = 0; //при включении подача воды должна быть в крайнем положении (перекрыта)
int stepper_max = 20; //на сколько шагов максимально открываем воду
int stepper_pause = 0; //сюда записываем количество циклов, которые нужно пропустить и не менять

int limit_min = 75; //нижний предел температуры
int limit_max = 78; //верхний предел температуры
int stepper_d = 1000;  //количество минишагов в одном шаге

int current_etap = 0; //текущий этап

int dimmer_val = 50; //начальное значение диммера (мощность тэна)
int dimmer_low = 20; //низкое значение диммера
int dimmer_critic = 95; // критическое значение, при котором температура превышает пределы. Определяется в процессе

int debug_temper[100];