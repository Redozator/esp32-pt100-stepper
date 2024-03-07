#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

const char* host = "gon";
const char* ssid = "vlak";
const char* password = "15662747";
WebServer server(80);

IPAddress ip;

void webserver_init();
void handle_OnConnect();
void handle_water_open();
void handle_water_close();
void handle_change_temp();
void handle_change_ten();
void handle_debug_temp();
void handle_NotFound();
void handle_change_etap();

/*
 * Login page
 */

const char* loginIndex =
		"<form name='loginForm'>"
		"<table width='20%' bgcolor='A09F9F' align='center'>"
		"<tr>"
		"<td colspan=2>"
		"<center><font size=4><b>ESP32 Login Page</b></font></center>"
		"<br>"
		"</td>"
		"<br>"
		"<br>"
		"</tr>"
		"<tr>"
		"<td>Username:</td>"
		"<td><input type='text' size=25 name='userid'><br></td>"
		"</tr>"
		"<br>"
		"<br>"
		"<tr>"
		"<td>Password:</td>"
		"<td><input type='Password' size=25 name='pwd'><br></td>"
		"<br>"
		"<br>"
		"</tr>"
		"<tr>"
		"<td><input type='submit' onclick='check(this.form)' value='Login'></td>"
		"</tr>"
		"</table>"
		"</form>"
		"<script>"
		"function check(form)"
		"{"
		"if(form.userid.value=='admin' && form.pwd.value=='admin')"
		"{"
		"window.open('/serverIndex')"
		"}"
		"else"
		"{"
		" alert('Error Password or Username')/*displays error message*/"
		"}"
		"}"
		"</script>";

/*
 * Server Index Page
 */

const char* serverIndex =
		"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
		"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
		"<input type='file' name='update'>"
		"<input type='submit' value='Update'>"
		"</form>"
		"<div id='prg'>progress: 0%</div>"
		"<script>"
		"$('form').submit(function(e){"
		"e.preventDefault();"
		"var form = $('#upload_form')[0];"
		"var data = new FormData(form);"
		" $.ajax({"
		"url: '/update',"
		"type: 'POST',"
		"data: data,"
		"contentType: false,"
		"processData:false,"
		"xhr: function() {"
		"var xhr = new window.XMLHttpRequest();"
		"xhr.upload.addEventListener('progress', function(evt) {"
		"if (evt.lengthComputable) {"
		"var per = evt.loaded / evt.total;"
		"$('#prg').html('progress: ' + Math.round(per*100) + '%');"
		"}"
		"}, false);"
		"return xhr;"
		"},"
		"success:function(d, s) {"
		"console.log('success!')"
		"},"
		"error: function (a, b, c) {"
		"}"
		"});"
		"});"
		"</script>";

void webserver_init() {
	WiFi.begin(ssid, password);
	//check wi-fi is connected to wi-fi network
	while (WiFi.status() != WL_CONNECTED) {
		delay(1000);
		Serial.print(".");
	}
	Serial.println("");
	Serial.println("WiFi connected..!");
	ip = WiFi.localIP();

	/*use mdns for host name resolution*/
	if (!MDNS.begin(host)) { //http://gon.local
		Serial.println("Error setting up MDNS responder!");
		lcd.print(ip);
	} else {
		lcd.print("http://");
		lcd.print(host);
		lcd.print(".local");
	}

	server.on("/", handle_OnConnect);
	server.on("/water_open", handle_water_open);
	server.on("/water_close", handle_water_close);
	server.on("/change_temp", handle_change_temp);
	server.on("/change_ten", handle_change_ten);
	server.on("/change_etap", handle_change_etap);
	server.on("/ota", HTTP_GET, []() {
		server.sendHeader("Connection", "close");
		server.send(200, "text/html", loginIndex);
	});
	server.on("/serverIndex", HTTP_GET, []() {
		server.sendHeader("Connection", "close");
		server.send(200, "text/html", serverIndex);
	});
	/*handling uploading firmware file */
	server.on("/update", HTTP_POST, []() {
		server.sendHeader("Connection", "close");
		server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
		ESP.restart();
	}, []() {
		HTTPUpload& upload = server.upload();
		if (upload.status == UPLOAD_FILE_START) {
			Serial.printf("Update: %s\n", upload.filename.c_str());
			if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
				Update.printError(Serial);
			}
		} else if (upload.status == UPLOAD_FILE_WRITE) {
			/* flashing firmware to ESP*/
			if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
				Update.printError(Serial);
			}
		} else if (upload.status == UPLOAD_FILE_END) {
			if (Update.end(true)) { //true to set the size to the current progress
				Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
			} else {
				Update.printError(Serial);
			}
		}
	});
	server.on("/debug_temp", handle_debug_temp);
	server.onNotFound(handle_NotFound);
	server.begin();
}

void handle_OnConnect() {
	server.send(200, "text/html", SendHTML());
}

void handle_water_open() {
	if (server.hasArg("pos")) {
		server.send(200, "text/html", task_water(1, 0));
	} else {
		server.send(200, "text/html", task_water(1, 1));
	}
}

void handle_water_close() {
	if (server.hasArg("pos")) {
		server.send(200, "text/html", task_water(-1, 0));
	} else {
		server.send(200, "text/html", task_water(-1, 1));
	}
}

void handle_change_temp() {
	if (server.hasArg("min") && server.hasArg("max")) {
		server.send(200, "text/html", change_temp(server.arg("min"), server.arg("max")));
	} else {
		server.send(200, "text/plain", "min and max not found ");
	}
}

void handle_change_ten() {
	if (server.hasArg("dimmer_val")) {
		server.send(200, "text/html", change_ten(server.arg("dimmer_val")));
	} else {
		server.send(200, "text/plain", "dimmer_val not found ");
	}
}

void handle_debug_temp() {
	server.send(200, "text/html", debug_temp());
}

void handle_NotFound() {
	server.send(404, "text/plain", "Not found");
}

void handle_change_etap() {
	if (server.hasArg("etap")) {
		server.send(200, "text/html", change_etap(server.arg("etap")));
	} else {
		server.send(200, "text/plain", "etap not found ");
	}
}
