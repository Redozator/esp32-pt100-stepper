
//#include "RBDmcuESP32.h"


// температуру в график

void tempToChart() {
	for (int i = 1; i < CNT_CHART; i++) {
		if (chart_y[i] > 0) {
			chart_y[i - 1] = chart_y[i];
			chart_stepper[i - 1] = chart_stepper[i];
			chart_dimmer[i - 1] = chart_dimmer[i];
		} else {
			chart_y[i - 1] = 0;
			chart_stepper[i - 1] = 0;
			chart_dimmer[i - 1] = 0;
		}
		chart_y[CNT_CHART - 1] = tempers[POINTS - 1];
		chart_stepper[CNT_CHART - 1] = stepper_position;
		chart_dimmer[CNT_CHART - 1] = dimmer_val;
	}
}

String task_water(int n, int pos) {
	if (pos == 1) {
		if (n > 0) {
			water_open(n);
		} else {
			water_close(-n);
		}
	} else {
		if (n > 0) {
			water_open_p();
		} else {
			water_close_p();
		}
	}
	String ptr = "<!DOCTYPE html> <html>\n\
  <meta http-equiv=\"Content-type\" content=\"text/html; charset=utf-8\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n\
   <head>\n\
      <link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap@4.6.0/dist/css/bootstrap.min.css\" integrity=\"sha384-B0vP5xmATw1+K9KRQjQERJvTumQW0nPEzvF6L/Z6nronJ3oUOFUFpCjEUQouq2+l\" crossorigin=\"anonymous\">\n\
      <script src=\"https://code.jquery.com/jquery-3.6.0.min.js\" integrity=\"sha256-/xUj+3OJU5yExlq6GSYGSHk7tPXikynS7ogEvDej/m4=\" crossorigin=\"anonymous\"></script>\n\
      <script src=\"https://cdn.jsdelivr.net/npm/bootstrap@4.6.0/dist/js/bootstrap.min.js\" integrity=\"sha384-+YQ4JLhjyBLPDQt//I+STsc9iw4uQqACwlvpslubQzn4u2UU2UFM80nGisd026JF\" crossorigin=\"anonymous\"></script>\n\
      <script src=\"https://cdn.jsdelivr.net/npm/bootstrap@4.6.0/dist/js/bootstrap.bundle.min.js\" integrity=\"sha384-Piv4xVNRyMGpqkS2by6br4gNJ7DXjqk09RmUpJ8jgGtD7zP9yug3goQfGII0yAns\" crossorigin=\"anonymous\"></script>\n\
      <title>Task</title>\n\
   </head>\n\
\n\
   <body>\n\
      <script language = 'JavaScript'>\n\
        window.location.href = \"/\";\n\
      </script>\n\
   </body>\n\
</html>\n\
";
	return ptr;
}

String change_temp(String min, String max) {
	limit_min = atoi(min.c_str());
	limit_max = atoi(max.c_str());
	dimmer_critic = 95; // при изменении пределов температуры, инициализируем dimmer_critic
	String ptr = "<!DOCTYPE html> <html>\n\
  <meta http-equiv=\"Content-type\" content=\"text/html; charset=utf-8\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n\
   <head>\n\
      <link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap@4.6.0/dist/css/bootstrap.min.css\" integrity=\"sha384-B0vP5xmATw1+K9KRQjQERJvTumQW0nPEzvF6L/Z6nronJ3oUOFUFpCjEUQouq2+l\" crossorigin=\"anonymous\">\n\
      <script src=\"https://code.jquery.com/jquery-3.6.0.min.js\" integrity=\"sha256-/xUj+3OJU5yExlq6GSYGSHk7tPXikynS7ogEvDej/m4=\" crossorigin=\"anonymous\"></script>\n\
      <script src=\"https://cdn.jsdelivr.net/npm/bootstrap@4.6.0/dist/js/bootstrap.min.js\" integrity=\"sha384-+YQ4JLhjyBLPDQt//I+STsc9iw4uQqACwlvpslubQzn4u2UU2UFM80nGisd026JF\" crossorigin=\"anonymous\"></script>\n\
      <script src=\"https://cdn.jsdelivr.net/npm/bootstrap@4.6.0/dist/js/bootstrap.bundle.min.js\" integrity=\"sha384-Piv4xVNRyMGpqkS2by6br4gNJ7DXjqk09RmUpJ8jgGtD7zP9yug3goQfGII0yAns\" crossorigin=\"anonymous\"></script>\n\
      <title>Task</title>\n\
   </head>\n\
\n\
   <body>\n\
      <script language = 'JavaScript'>\n\
        window.location.href = \"/\";\n\
      </script>\n\
   </body>\n\
</html>\n\
";
	return ptr;
}

String change_ten(String s_dimmer_val) {
	dimmer_val = atoi(s_dimmer_val.c_str());
	// dimmer.setPower(dimmer_val);
	relay.setPWM(dimmer_val*2.55);
	String ptr = "<!DOCTYPE html> <html>\n\
  <meta http-equiv=\"Content-type\" content=\"text/html; charset=utf-8\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n\
   <head>\n\
      <link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap@4.6.0/dist/css/bootstrap.min.css\" integrity=\"sha384-B0vP5xmATw1+K9KRQjQERJvTumQW0nPEzvF6L/Z6nronJ3oUOFUFpCjEUQouq2+l\" crossorigin=\"anonymous\">\n\
      <script src=\"https://code.jquery.com/jquery-3.6.0.min.js\" integrity=\"sha256-/xUj+3OJU5yExlq6GSYGSHk7tPXikynS7ogEvDej/m4=\" crossorigin=\"anonymous\"></script>\n\
      <script src=\"https://cdn.jsdelivr.net/npm/bootstrap@4.6.0/dist/js/bootstrap.min.js\" integrity=\"sha384-+YQ4JLhjyBLPDQt//I+STsc9iw4uQqACwlvpslubQzn4u2UU2UFM80nGisd026JF\" crossorigin=\"anonymous\"></script>\n\
      <script src=\"https://cdn.jsdelivr.net/npm/bootstrap@4.6.0/dist/js/bootstrap.bundle.min.js\" integrity=\"sha384-Piv4xVNRyMGpqkS2by6br4gNJ7DXjqk09RmUpJ8jgGtD7zP9yug3goQfGII0yAns\" crossorigin=\"anonymous\"></script>\n\
      <title>Task</title>\n\
   </head>\n\
\n\
   <body>\n\
      <script language = 'JavaScript'>\n\
        window.location.href = \"/\";\n\
      </script>\n\
   </body>\n\
</html>\n\
";
	return ptr;
}

String SendHTML() {
	char TempString[10];
	String ptr = "<!DOCTYPE html> <html>\n\
  <meta http-equiv=\"Content-type\" content=\"text/html; charset=utf-8\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n\
   <head>\n\
      <link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap@4.6.0/dist/css/bootstrap.min.css\" integrity=\"sha384-B0vP5xmATw1+K9KRQjQERJvTumQW0nPEzvF6L/Z6nronJ3oUOFUFpCjEUQouq2+l\" crossorigin=\"anonymous\">\n\
      <script src=\"https://code.jquery.com/jquery-3.6.0.min.js\" integrity=\"sha256-/xUj+3OJU5yExlq6GSYGSHk7tPXikynS7ogEvDej/m4=\" crossorigin=\"anonymous\"></script>\n\
      <script src=\"https://cdn.jsdelivr.net/npm/bootstrap@4.6.0/dist/js/bootstrap.min.js\" integrity=\"sha384-+YQ4JLhjyBLPDQt//I+STsc9iw4uQqACwlvpslubQzn4u2UU2UFM80nGisd026JF\" crossorigin=\"anonymous\"></script>\n\
      <script src=\"https://cdn.jsdelivr.net/npm/bootstrap@4.6.0/dist/js/bootstrap.bundle.min.js\" integrity=\"sha384-Piv4xVNRyMGpqkS2by6br4gNJ7DXjqk09RmUpJ8jgGtD7zP9yug3goQfGII0yAns\" crossorigin=\"anonymous\"></script>\n\
      <script type = 'text/javascript' src = 'https://www.gstatic.com/charts/loader.js'>\n\
      </script>\n";
	ptr += "<title>График температуры</title>\n\
      <script type = 'text/javascript'>\n\
         google.charts.load('current', {packages: ['corechart','line']});  \n\
      </script>\n\
   </head>\n\
\n\
   <body>\n\
      <div class=\"container-fluid py-3\">\n\
      <a href='/water_open' class=\"btn btn-success\">Открыть воду</a><a href='/water_close' class=\"btn btn-danger mx-3\">Прикрыть воду</a>\n\
      <a href='/water_open?pos=0' class=\"btn btn-success\">Открыть воду без изменения номера позиции</a><a href='/water_close?pos=0' class=\"btn btn-danger mx-3\">Прикрыть воду без изменения номера позиции</a>\n\
      <div class=\"container-fluid py-3\">\n\
      Этап: " + String(current_etap) + " \n\
			<form action='/change_etap'>\n\
      Изменить этап, (1-5) <input type='text' name='etap' value=''> (этап 5 - ручной режим) \n\
      <input type='submit' value='Сохранить'>\n\
      </form>\n\
      <form action='/change_ten'>\n\
      Мощность нагрева, % (1-95) <input type='text' name='dimmer_val' value='" + String(dimmer_val) + "'> \n\
      <input type='submit' value='Сохранить'>\n\
      </form>\n\
      <form action='/change_temp'>\n\
      Температура от <input type='text' name='min' value='" + String(limit_min) + "'> \n\
      Температура до <input type='text' name='max' value='" + String(limit_max) + "'> \n\
      <input type='submit' value='Сохранить'>\n\
      </form>\n\
      </div>\n\
      <div class=\"row\">\n\
      <div id = 'container' class=\"col\" >\n\
      </div>\n\
      </div>\n\
      </div>\n\
      <script language = 'JavaScript'>\n\
         function drawChart() {\n\
            \n\
            var data = new google.visualization.DataTable();\n\
            data.addColumn('number', 'Время');\n\
            data.addColumn('number', 'Текущая');\n\
            data.addColumn('number', 'Тэн');\n\
            data.addColumn('number', 'Заслонка');\n";
	for (int i = 0; i < CNT_CHART; i++) {
		if (chart_y[i] > 0) {
			ptr += "data.addRow([" + String(i) + "," + dtostrf(chart_y[i], 3, 1, TempString) + "," + String(chart_dimmer[i]) + "," + String(chart_stepper[i]) + "]);\n";
		}
	}

	ptr += "var options = {'title' : 'Температура',\n\
              series: { \n\
//                0: {targetAxisIndex: 1},\n\
                0: {targetAxisIndex: 1, curveType: 'function'},\n\
                1: {targetAxisIndex: 0}\n\
              },\n\
               hAxis: {\n\
                  title: 'Время'\n\
               },\n\
               vAxes: {\n\
                  0: {title: 'Мощность'},\n\
                  1: {title: 'Температура', minValue: "+String(round(chart_y[CNT_CHART-1])-10)+", maxValue: "+String(round(chart_y[CNT_CHART-1])+10)+"}\n\
               },   \n\
//               trendlines: {\n\
//               0:{type: 'polynomial', degree: 5, color: '#333', opacity: 1, visibleInLegend: false}\n\
//               },\n\
               'width':800,\n\
                     'height':400,\n\
                     pointsVisible: false \n\
                  };\n\
\n\
            var chart = new google.visualization.LineChart(document.getElementById('container'));\n\
            chart.draw(data, options);\n\
         }\n\
         google.charts.setOnLoadCallback(drawChart);\n\
      </script>\n\
   </body>\n\
</html>\n\
";
	return ptr;
}

String debug_temp() {
	// char TempString[10];
	String ptr = "<!DOCTYPE html> <html>\n\
  <meta http-equiv=\"Content-type\" content=\"text/html; charset=utf-8\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n\
   <head>\n\
      <link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap@4.6.0/dist/css/bootstrap.min.css\" integrity=\"sha384-B0vP5xmATw1+K9KRQjQERJvTumQW0nPEzvF6L/Z6nronJ3oUOFUFpCjEUQouq2+l\" crossorigin=\"anonymous\">\n\
      <script src=\"https://code.jquery.com/jquery-3.6.0.min.js\" integrity=\"sha256-/xUj+3OJU5yExlq6GSYGSHk7tPXikynS7ogEvDej/m4=\" crossorigin=\"anonymous\"></script>\n\
      <script src=\"https://cdn.jsdelivr.net/npm/bootstrap@4.6.0/dist/js/bootstrap.min.js\" integrity=\"sha384-+YQ4JLhjyBLPDQt//I+STsc9iw4uQqACwlvpslubQzn4u2UU2UFM80nGisd026JF\" crossorigin=\"anonymous\"></script>\n\
      <script src=\"https://cdn.jsdelivr.net/npm/bootstrap@4.6.0/dist/js/bootstrap.bundle.min.js\" integrity=\"sha384-Piv4xVNRyMGpqkS2by6br4gNJ7DXjqk09RmUpJ8jgGtD7zP9yug3goQfGII0yAns\" crossorigin=\"anonymous\"></script>\n\
      <title>Task</title>\n\
   </head>\n\
\n\
   <body>\n ";
	/*
	unsigned long a = dimmer.getDebug_timer();
	if (a == 0) {
		a = 1;
	}
	unsigned long time = micros();
	int av = time / a;
	ptr += "<div>debug_timer=" + String(a) + " (среднее время в микросекундах: " + String(av) + ". Частота " + String(1000000 / av) + " в секунду)</div>\n";
	for (int i = 0; i < 100; i++) {
		if (debug_temper[i] > 0) {
			double t = (double) round(10 * (debug_temper[i] / 7.15 + 15.4)) / 10;
			ptr += "<div>" + String(debug_temper[i]) + " " + dtostrf(t, 4, 2, TempString) + "</div>\n";
		}
	} */
	ptr += "</body>\n\
</html>\n\
";
	return ptr;
}

String change_etap(String s_etap) {
	int etap = atoi(s_etap.c_str());
	if (etap>0 && etap<6) {
		current_etap = etap;
	}
	String ptr = "<!DOCTYPE html> <html>\n\
  <meta http-equiv=\"Content-type\" content=\"text/html; charset=utf-8\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n\
   <head>\n\
      <title>change etap</title>\n\
   </head>\n\
\n\
   <body>\n\
      <script language = 'JavaScript'>\n\
        window.location.href = \"/\";\n\
      </script>\n\
   </body>\n\
</html>\n\
";
	return ptr;
}
