#include "common.h"
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

void handleRoot() {
    String out = "<h1>ESP8266 ControlChain</h1>";
    char * name = getActuatorName(0);

    out += "</p>";
    out += name;
    out += "</p>";

    debug.print("[http] ");
    debug.println(out);
  server.send(200, "text/html", out);
}

void setup_webserver() {
    debug.print("[http] Starting HTTP server: ");
    server.on("/", handleRoot);
    server.begin();
    debug.println("OK");
}

void loop_webserver() {
    server.handleClient();
}
