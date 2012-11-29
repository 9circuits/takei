
#include "Common.h"

#include "Sensors.h"

#include "Actuators.h"


ActionSequence cricketSounds[6];
RelayClickAction cricketRelays[6];

ActionSequence cricketSound1;
RelayClickAction cricketRelay1;

MotorRunAction randomMotor1;

ActionSequence frogCroak;
MotorLerpAction frogCroakLerpAction1;
MotorLerpAction frogCroakLerpAction2;
MotorLerpAction frogCroakLerpAction3;

byte offAll = 0;

unsigned long interActivityFactor = 0;

unsigned long interActivityTicker = 0;

ActuationTimer lightL0AT(15000, 1000);
ActuationTimer lightL8AT(25000, 1000);
ActuationTimer lightL12AT(20000, 1000);
// ActuationTimer lightL12AT(10000);

void setup() 
{
    setupMotors();
    
    setupCapsListener();
    
    setupAmbientSoundActions();
    
    registerTouchEvent(touchC3, 5);
    registerTouchEvent(touchC10, 6);
    
    registerAnalogEvent(lightL2, 0, 180, 450);
    registerAnalogEvent(lightL0, 1, 180, 450);
    registerAnalogEvent(lightL7, 2, 180, 450);
    registerAnalogEvent(lightL8, 3, 180, 450);
    registerAnalogEvent(lightL12, 4, 180, 450);
    
    interActivityTicker = millis();
    
#ifdef TAKEI_DEBUG_ON
    // Enable serial port for debugging
    initLog(115200);
#endif
}

void lightL2(uint16_t val) {
    if (offAll) {
        return;
    }
    
    if (val < 360) {
        analogWrite(9, 255);    
        interActivityFactor++;
    } else {
        analogWrite(9, 0);
    }
}

void lightL0(uint16_t val) {
    if (offAll) {
        return;
    }
    
    if (val < 360) {
        analogWrite(2, 255);    
        interActivityFactor++;
        lightL0AT.wasInitiated();
    } else {
        analogWrite(2, 0);
    }
}
// Master reset
void lightL7(uint16_t val) {
    if (val < 360) {
        offAll = 1;
        analogWrite(3, 0);
        cricketSounds[2].stop();
        cricketSounds[4].stop();
        frogCroak.stop();
        
        if (interActivityFactor > 110) {
            interActivityFactor = 0;
        }
    } else {
        offAll = 0;
    }
}
void lightL8(uint16_t val) {
    if (offAll) {
        return;
    }
    
    if (val < 300) {
        cricketSounds[3].run();
        interActivityFactor++;
        lightL8AT.wasInitiated();
    } else {
        cricketSounds[3].stop();
    }
}
void lightL12(uint16_t val) {
    if (offAll) {
        return;
    }

    if (val < 300) {
        cricketSounds[5].run();
        interActivityFactor++;
        lightL12AT.wasInitiated();
    } else {
        cricketSounds[5].stop();
    }
}

// C3 to R3
void touchC3(byte interval) {
    if (offAll) {
        return;
    }
    
    // Serial.println(interval);
    if (interval < CAP5_MAX && interval > CAP5_MIN) {
        cricketSounds[0].run();
        interActivityFactor++;
    } else {
         cricketSounds[0].stop();
     }
}

void touchC10(byte interval)
{
    if (offAll) {
        return;
    }
    // Serial.println(interval);
    if (interval < CAP7_MAX && interval > CAP7_MIN) {
        cricketSounds[1].run();
        interActivityFactor++;
    } else {
        cricketSounds[1].stop();
    }
}

void loop() 
{
#ifdef TAKEI_DEBUG_ON
    // To enable serial commands for on/off of actuators.
    readSerialCommands();
#endif
//    
   if ((millis() - interActivityTicker) > 1000) {
       logLine(interActivityFactor);
       if (!offAll) {
           if (interActivityFactor > 7500 && interActivityFactor < 10000) {
               cricketSounds[2].run();
               cricketSounds[4].stop();
               analogWrite(3, 0);
               frogCroak.stop();
           } else if (interActivityFactor > 10000 && interActivityFactor < 15000) {
               cricketSounds[2].run();
               cricketSounds[4].run();
               analogWrite(3, 0);
               frogCroak.stop();
           } else if (interActivityFactor > 15000) {
               // cricketSounds[1].run();
               cricketSounds[2].run();
               cricketSounds[4].run();
               analogWrite(3, 255);
               frogCroak.run();
           } else {
               cricketSounds[2].stop();
               cricketSounds[4].stop();
               analogWrite(3, 0);
               frogCroak.stop();
           }
       }
       
       if (interActivityFactor > 600) {
           interActivityFactor = interActivityFactor - 500;
       }
       
       interActivityTicker = millis();
   }
   
    // To dispatch touch events
    touchDispatcher();
    
    // To read the analog values
    analogDispatcher();
    
    // These updates should be called to make sure that the action gets updated.
    // It doesn't matter whether the action is running or not. 
    frogCroak.update();
    
    byte j = 0;
    while (j < 6) {
        cricketSounds[j].update();
        j++;
    }
    
    lightL0AT.update();
    if (lightL0AT.isLate()) {
        analogWrite(2, 255);
        lightL0AT.wasInitiated();
    } else {
        analogWrite(2, 0);
    }
    
    lightL8AT.update();
    if (lightL8AT.isLate()) {
        cricketSounds[3].run();
        lightL8AT.wasInitiated();
    } else {
        cricketSounds[3].stop();
    }
    
    lightL12AT.update();
    if (lightL12AT.isLate()) {
        cricketSounds[5].run();
        lightL12AT.wasInitiated();
    } else {
        cricketSounds[5].stop();
    }
}



void setupAmbientSoundActions()
{
    // Frog Actions and sequences
    // randomMotor1.init(2, 250);
    
    frogCroakLerpAction1.init(8, 100, 150, 200);
    frogCroakLerpAction2.init(8, 50, 100, 300);
    frogCroakLerpAction3.init(8, 0, 0, 5000);
    
    frogCroak.addAction(frogCroakLerpAction1, 0, 200);
    frogCroak.addAction(frogCroakLerpAction1, 100, 200);
    frogCroak.addAction(frogCroakLerpAction2, 3500, 300);
    frogCroak.addAction(frogCroakLerpAction2, 100, 300);
    frogCroak.addAction(frogCroakLerpAction1, 3500, 200);
    frogCroak.addAction(frogCroakLerpAction1, 100, 200);
    frogCroak.addAction(frogCroakLerpAction2, 3500, 300);
    frogCroak.addAction(frogCroakLerpAction2, 100, 300);
    frogCroak.addAction(frogCroakLerpAction1, 3500, 200);
    frogCroak.addAction(frogCroakLerpAction1, 100, 200);    
    frogCroak.addAction(frogCroakLerpAction2, 3500, 300);
    frogCroak.addAction(frogCroakLerpAction2, 100, 300);
    frogCroak.addAction(frogCroakLerpAction3, 500, 5000);
    
    byte j = 0;
    while (j < 6) {
        cricketRelays[j].init(14 + j, 10);
        byte i = 0;
        cricketSounds[j].addAction(cricketRelays[j], 0, 200);
        while (i++ < 9) {
            cricketSounds[j].addAction(cricketRelays[j], random(75, 175), random(150, 225));
        }
        j++;
    }
}



#ifdef TAKEI_DEBUG_ON

void readSerialCommands()
{
    // Test code. Start when user sends an input.
    if (Serial.available() > 0) {
        char c = Serial.read();

        if (c == 'q') {
            frogCroak.run();
        }
        
        if (c == 'a') {
            frogCroak.stop();
        }
        
        if (c == 'w') {
            cricketSounds[0].run();
        }
        
        if (c == 's') {
            cricketSounds[0].stop();
        }
        
        if (c == 'e') {
            cricketSounds[1].run();
        }
        
        if (c == 'd') {
            cricketSounds[1].stop();
        }   
        
        if (c == 'r') {
            cricketSounds[2].run();
        }
        
        if (c == 'f') {
            cricketSounds[2].stop();
        }
        
        if (c == 't') {
            cricketSounds[3].run();
        }
        
        if (c == 'g') {
            cricketSounds[3].stop();
        }
        
        if (c == 'y') {
            cricketSounds[4].run();
        }
        
        if (c == 'h') {
            cricketSounds[4].stop();
        }
        
        if (c == 'u') {
            cricketSounds[5].run();
        }
        
        if (c == 'j') {
            cricketSounds[5].stop();
        }
    }    
}

#endif
