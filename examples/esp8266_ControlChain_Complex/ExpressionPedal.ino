#include "common.h"

const int pedalPin = A0;
const float pedalRange = 1023.0;

float physMin = 500;
float physMax = 501;
float physMinDelta = 3;
float physPrev = -1;
float pedalPrev = -1;
float pedalMinDelta = 0.25;
float rangePercent = 0.98; /* shorten range by two percent to make it easier to reach the full 100% */

void updatePedalValue() {
    float physValue = analogRead(pedalPin);
    if (abs(physValue - physPrev) < physMinDelta) {
        return;    
    }
    physPrev = physValue;

    /* Calibrate min/max range of pedal */
    if (physValue > physMax) {
        physMax = physValue;
    }
    if (physValue < physMin) {
        physMin = physValue;
    }
    float physRange = (physMax - physMin) * rangePercent; 

    /* Norm value to end up within 0..pedalRange */
    float pedalValue = (physValue - physMin) * pedalRange / physRange;
    if (pedalValue > pedalRange) {
        pedalValue = pedalRange;
    }

    if (abs(pedalValue - pedalPrev) > pedalMinDelta) {
        pedalPrev = pedalValue;
        debug.print("[EXP] Updating to ");
        debug.print(pedalValue);
        debug.print(", phys=");
        debug.println(physValue);
        setActuatorValue(expressionPedalId, pedalValue);
    }
}

