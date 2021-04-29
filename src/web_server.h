#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "vlak";
const char* password = "15662747";
WebServer server(80);

IPAddress ip;

void webserver_init();
void handle_OnConnect();
void handle_water_open();
void handle_water_close();
void handle_change_temp();
void handle_NotFound();


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

  server.on("/", handle_OnConnect);
  server.on("/water_open", handle_water_open);
  server.on("/water_close", handle_water_close);
  server.on("/change_temp", handle_change_temp);
  server.onNotFound(handle_NotFound);
  server.begin();
}

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML());
}
void handle_water_open() {
  server.send(200, "text/html", task_water(1));
}
void handle_water_close() {
  server.send(200, "text/html", task_water(-1));
}
void handle_change_temp() {
  if (server.hasArg("min") && server.hasArg("max")) {
    server.send(200, "text/html", change_temp(server.arg("min"), server.arg("max")));
  } else {
    server.send(200, "text/plain", "min and max not found ");
  }
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
