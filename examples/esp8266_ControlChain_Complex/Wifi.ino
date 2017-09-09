#include "common.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 

IPAddress local_IP(192,168,52,1);
IPAddress gateway(192,168,52,1);
IPAddress subnet(255,255,255,0);

int numConnectedClients = 0;

void setup_wifi() {
    if (setupAccessPoint) {
        WiFi.mode(WIFI_AP);
        debug.printf("[Wifi] Setting up access point '%s': ", ssid);
        if (!WiFi.softAPConfig(local_IP, gateway, subnet)) {
            debug.println("configuration failed!");
            return;
        }
        if (WiFi.softAP(ssid, password)) {
            debug.print("OK, own IP address = ");
            debug.println(WiFi.softAPIP());
        } else {
            debug.println("failed!");
        }
        numConnectedClients = 0;
    } else {
        WiFi.mode(WIFI_STA);
        debug.print("[Wifi] Connecting to >>");
        debug.print(ssid);
        debug.print("<< ");
        WiFi.begin(ssid, password);

        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            debug.print(".");
        }
        debug.println("");

        debug.println("[Wifi] Connected, IP address: ");
        debug.println(WiFi.localIP());
    }
    /* If you want to debug the WiFi connectivity, uncomment the next statement */
//    WiFi.printDiag(debug);
}

void loop_wifi() {
    if (!setupAccessPoint)
        return;

        
    int cc = WiFi.softAPgetStationNum();
    if (cc != numConnectedClients) {
        numConnectedClients = cc;
        debug.printf("[Wifi] %i client(s) connected\n", numConnectedClients);
    }

}
