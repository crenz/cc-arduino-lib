#include "common.h"

void setup() {
    debug.begin(115200);   
    debug.println("\nBooting ESP8266 Control Chain module\n\n");
    setup_cc();
    setup_wifi();
    setup_osc();
    setup_webserver();
    debug.println("\nSetup finished\n--------------\n");
}

void loop() {
    loop_cc();
    loop_wifi();
    loop_osc();
    loop_webserver();
}
