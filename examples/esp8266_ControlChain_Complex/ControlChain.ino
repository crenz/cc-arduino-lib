#include "common.h"
#include <ControlChain.h>

ControlChain cc;

float actuatorValue[numActuators];
cc_actuator_t *actuator[numActuators];

void setActuatorValue(uint actIdx, float newValue) {
    if (actIdx >= numActuators) {
        return;
    }
    actuatorValue[actIdx] = newValue;
}

char * getActuatorName(uint actIdx) {
    if (actIdx >= numActuators) {
        return "";
    }

    return actuator[actIdx]->name.text;
}

float getActuatorValue(uint actIdx) {
    if (actIdx >= numActuators) {
        return 0;
    }

    return actuatorValue[actIdx];
}

void eventsCallback(void *arg) {
     cc_event_t *event = (cc_event_t *)arg;
     uint8_t actIdx = *((uint8_t *) event->data);
     debug.printf("[CC] Received event %i\n", event->id);
     switch(event->id) {
        case CC_EV_HANDSHAKE_FAILED:
            debug.printf("[CC] Handshake status: %i\n", *((int *) arg));
            break;
        case CC_EV_UNASSIGNMENT:
//            debug.printf("[CC] Received unassignment event for actuator %i / %i\n", *((uint8_t *) arg));
            debug.printf("[CC] Received unassignment event for actuator %i\n", actIdx);
            sendActuatorValue(actIdx, 0);
            sendActuatorLabel(actIdx, "");
            break;
        case CC_EV_ASSIGNMENT:
        case CC_EV_UPDATE:            
            debug.println("[CC] Received assignment/update event");
            cc_assignment_t *assignment = (cc_assignment_t *)event->data;
            debug.printf("id=%i, actuator_id=%i, value=", assignment->id, assignment->actuator_id);
            debug.print(assignment->value);
            debug.print(", min=");
            debug.print(assignment->min);
            debug.print(", max=");
            debug.print(assignment->max);
            debug.print(", def=");
            debug.print(assignment->def);
            debug.printf(", mode=%i, steps=%i, list_count=%i, list_index=%i, label=",
              assignment->mode, assignment->steps, assignment->list_count, assignment->list_index);
            debug.print(assignment->label.text);
            debug.print(", unit=");
            debug.print(assignment->unit.text);
            debug.println();

            actIdx = assignment->actuator_id;
            
            sendActuatorValue(actIdx, assignment->value);
            sendActuatorLabel(actIdx, assignment->label.text);
            break;
     }
}

void setup_cc() {
    debug.println("[CC] Initializing Control Chain");
    cc.init();
    cc.setEventsCallback(eventsCallback);

    cc_device_t *device = cc.newDevice("ESP8266", cc_uri);

    int actIndex = 0;
    // Configure OSC Button actuators
    for (int i = 0; i < numButtons; i++) {
        cc_actuator_config_t actuator_config;
        actuator_config.type = CC_ACTUATOR_MOMENTARY;
        char buffer[50];
        sprintf(buffer, "Button #%i", i + 1);
        actuator_config.name = buffer;
        actuator_config.value = &actuatorValue[actIndex];
        actuator_config.min = 0.0;
        actuator_config.max = 1.0;
        actuator_config.supported_modes = CC_MODE_TOGGLE | CC_MODE_TRIGGER;
        actuator_config.max_assignments = 1;

        // create and add actuator to device
        actuator[actIndex] = cc.newActuator(&actuator_config);
        cc.addActuator(device, actuator[actIndex]);
        actIndex++;
    }
    // Configure OSC rotary actuators
    for (int i = 0; i < numRotary; i++) {
        cc_actuator_config_t actuator_config;
        actuator_config.type = CC_ACTUATOR_CONTINUOUS;
        char buffer[50];
        sprintf(buffer, "Rotary #%i", i + 1);
        actuator_config.name = buffer;
        actuator_config.value = &actuatorValue[actIndex];
        actuator_config.min = 0.0;
        actuator_config.max = 1.0;
        actuator_config.supported_modes = CC_MODE_REAL | CC_MODE_INTEGER;
        actuator_config.max_assignments = 1;

        // create and add actuator to device
        actuator[actIndex] = cc.newActuator(&actuator_config);
        cc.addActuator(device, actuator[actIndex]);
        actIndex++;
    }
    // Configure expression pedals
    for (int i = 0; i < numExpression; i++) {
        cc_actuator_config_t actuator_config;
        actuator_config.type = CC_ACTUATOR_CONTINUOUS;
        char buffer[50];
        sprintf(buffer, "Exp Pedal #%i", i + 1);
        actuator_config.name = buffer;
        actuator_config.value = &actuatorValue[actIndex];
        actuator_config.min = 0.0;
        actuator_config.max = 1023.0;
        actuator_config.supported_modes = CC_MODE_REAL | CC_MODE_INTEGER;
        actuator_config.max_assignments = 1;

        // create and add actuator to device
        actuator[actIndex] = cc.newActuator(&actuator_config);
        cc.addActuator(device, actuator[actIndex]);
        actIndex++;
    }
}

void loop_cc() {
    updatePedalValue();
    cc.run();
}
