#include "common.h"

WiFiUDP Udp;
OSCErrorCode error;
IPAddress ipTouchOsc;

int refreshIndex = -1;

void receiveButton1Value(OSCMessage &msg) {
    receiveActuatorValue(0, msg);
}

void receiveButton2Value(OSCMessage &msg) {
    receiveActuatorValue(1, msg);
}

void receiveButton3Value(OSCMessage &msg) {
    receiveActuatorValue(2, msg);
}

void receiveButton4Value(OSCMessage &msg) {
    receiveActuatorValue(3, msg);
}

void receiveButton5Value(OSCMessage &msg) {
    receiveActuatorValue(4, msg);
}

void receiveRotary1Value(OSCMessage &msg) {
    receiveActuatorValue(numButtons + 0, msg);
}

void receiveRotary2Value(OSCMessage &msg) {
    receiveActuatorValue(numButtons + 1, msg);
}

void receiveRotary3Value(OSCMessage &msg) {
    receiveActuatorValue(numButtons + 2, msg);
}

void receiveRotary4Value(OSCMessage &msg) {
    receiveActuatorValue(numButtons + 3, msg);
}

void receiveRotary5Value(OSCMessage &msg) {
    receiveActuatorValue(numButtons + 4, msg);
}

void refresh(OSCMessage &msg) {
    if (refreshIndex < 0) {
        refreshIndex = 0;
    }
}

void receiveActuatorValue(uint actIdx, OSCMessage &msg) {
    float newState = msg.getFloat(0);
    debug.printf("[OSC] Actuator %i new value: ", actIdx);
    debug.println(newState);
    setActuatorValue(actIdx, (float) newState);    
}

void sendActuatorValue(uint actIdx, float value) {
    char buffer[50];
    if (actIdx <= numButtons) {
        sprintf(buffer, "/button/%i/value", actIdx + 1);
    } else {
        sprintf(buffer, "/rotary/%i/value", actIdx + 1 - numButtons);      
    }
    debug.printf("[OSC] >> %s: ", buffer);
    debug.println(value);
    OSCMessage msg(buffer);
    msg.add(value);
    Udp.beginPacket(ipTouchOsc, oscOutgoingPort);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
}

void sendActuatorLabel(int actIdx, char * label) {
    char buffer[50];
    if (actIdx <= numButtons) {
        sprintf(buffer, "/button/%i/label", actIdx + 1);
    } else {
        sprintf(buffer, "/rotary/%i/label", actIdx + 1 - numButtons);      
    }
    debug.printf("[OSC] >> %s: %s\n", buffer, label);
    OSCMessage msg(buffer);
    msg.add(label);
    Udp.beginPacket(ipTouchOsc, oscOutgoingPort);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
}

void setup_osc() {
    Udp.begin(oscIncomingPort);
    debug.print("[OSC] Listening for incoming messages on port ");
    debug.println(Udp.localPort());  
    ipTouchOsc = IPAddress(192, 168, 1, 1);
}

void routeButton(OSCMessage &msg, int addrOffset ){
    char addr[100] = { 0 };
    msg.getAddress(addr, addrOffset + 1);
    int numButton = atoi(addr);
    debug.printf("[OSC] Routing button, offset = %i, parsed = %i, addr = ", numButton, addrOffset);
    debug.println(addr);

    receiveActuatorValue(numButton - 1, msg);
}

void routeRotary(OSCMessage &msg, int addrOffset ){
    char addr[100] = { 0 };
    msg.getAddress(addr, addrOffset + 1);
    int numRotary = atoi(addr);
    debug.printf("[OSC] Routing rotary, offset = %i, parsed = %i, addr = ", numRotary, addrOffset);
    debug.println(addr);

    receiveActuatorValue(numButtons + numRotary - 1, msg);
}

void loop_osc() {
    OSCMessage msg;
    int size = Udp.parsePacket();
    
    if (size > 0) {
        ipTouchOsc = Udp.remoteIP();        
        while (size--) {
          msg.fill(Udp.read());
        }
       if (!msg.hasError()) {
          char addr[100] = { 0 };
          msg.getAddress(addr);
          debug.print("[OSC] << ");
          debug.println(addr);
          msg.route("/button", routeButton);
          msg.route("/rotary", routeRotary);
          msg.dispatch("/refresh", refresh);
       } else {
          error = msg.getError();
          debug.print("[OSC] Error: ");
          debug.println(error);
        }
    }

    if (refreshIndex >= 0) {
          if (refreshIndex < numActuators) {
              sendActuatorLabel(refreshIndex, getActuatorName(refreshIndex));
              sendActuatorValue(refreshIndex, getActuatorValue(refreshIndex));
              refreshIndex++;
          } else {
              // done refreshing
              refreshIndex = -1;
          }
    }
}

/*
 * 
void loop_osc() {
    OSCBundle bundle;
    int size = Udp.parsePacket();

    if (size > 0) {
        while (size--) {
          bundle.fill(Udp.read());
        }
       if (!bundle.hasError()) {
          debug.println("Received a message");
          bundle.dispatch("/toggleA_1", setButton1);
       } else {
          error = bundle.getError();
          debug.print("[OSC] Error: ");
          debug.println(error);
        }
    }
}

 */
