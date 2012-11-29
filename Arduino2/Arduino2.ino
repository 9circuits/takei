
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

void setup() 
{
    setupMotors();

    setupCapsListener();

    setupAmbientSoundActions();
        // 
    registerTouchEvent(touchC7, 0);
    registerTouchEvent(touchC12, 1);
    registerTouchEvent(touchC2, 2);
        // registerTouchEvent(touchCap7, 6);
        // registerTouchEvent(touchCap8, 7);
        // 
    registerAnalogEvent(lightL1, 0, 180, 4550);
    registerAnalogEvent(lightL10, 1, 100, 550);
    registerAnalogEvent(lightL3, 2, 180, 390);    
        // registerAnalogEvent(LDRCallback4, 4, 100, 450);    
        // registerAnalogEvent(LDRCallback3, 3, 100, 450);        
        // 
    //cricketSounds[0].run();
    //frogCroak.run();
    //cricketSounds[1].run();

    interActivityTicker = millis();


#ifdef TAKEI_DEBUG_ON
    // Enable serial port for debugging
    initLog(115200);
#endif
}

// OFF SWITCH | CTRL + ALT + DEL
void LDRCallback7(uint16_t val) {
    if (val < 400) {
        offAll = 1;
        analogWrite(2, 0);
        analogWrite(6, 0);
        analogWrite(7, 0);
        analogWrite(8, 0);

        cricketSounds[0].stop();
        cricketSounds[1].stop();
        frogCroak.stop();

        if (interActivityFactor > 110) {
            interActivityFactor = interActivityFactor - 100;
        }
    } else {
        offAll = 0;        

        if (interActivityFactor > 5000 && interActivityFactor < 7500) {
            cricketSounds[0].run();
            cricketSounds[1].stop();
            frogCroak.stop();
            analogWrite(2, 0);

        } else if (interActivityFactor > 7500 && interActivityFactor < 10000) {
            cricketSounds[0].run();
            cricketSounds[1].run();
            frogCroak.stop();
            analogWrite(2, 0);
        } else if (interActivityFactor > 10000) {
            cricketSounds[0].run();
            cricketSounds[1].run();
            frogCroak.run();
        } else {
            cricketSounds[0].stop();
            cricketSounds[1].stop();
            analogWrite(2, 0);
            frogCroak.stop();
        }
    }
}

void lightL1(uint16_t val) {
    if (offAll) {
        return;
    }
    
    if (val < 360) {
        // analogWrite(9, 255);    
        // interActivityFactor++;
    } else {
        // analogWrite(9, 0);
    }
}

void lightL10(uint16_t val) {
    if (offAll) {
        return;
    }
    
    if (val < 360) {
        // analogWrite(9, 255);    
        // interActivityFactor++;
    } else {
        // analogWrite(9, 0);
    }
}

void lightL3(uint16_t val) {
    if (offAll) {
        return;
    }
    
    if (val < 360) {
        // analogWrite(9, 255);    
        // interActivityFactor++;
    } else {
        // analogWrite(9, 0);
    }
}


void touchC7(byte interval)
{
    if (offAll) {
        return;
    }
    
    // Serial.println(interval);
    if (interval < CAP1_MAX && interval > CAP1_MIN) {
        interActivityFactor++;
        cricketSounds[3].run();
    } else {
        cricketSounds[3].stop();
    }    
}

void touchC12(byte interval)
{
    if (offAll) {
        return;
    }
    
    // Serial.println(interval);
    if (interval < CAP2_MAX && interval > CAP2_MIN) {
        interActivityFactor++;
        cricketSounds[3].run();
    } else {
        cricketSounds[3].stop();
    }    
}
void touchC2(byte interval) {
    if (offAll) {
        return;
    }

    // Serial.println(interval);
    if (interval < CAP3_MAX && interval > CAP3_MIN) {
        interActivityFactor++;
        analogWrite(7, 255 - map(interval, CAP3_MIN, CAP3_MAX, 100, 255));
    } else {
        analogWrite(7, 0);
    }
}




void loop() 
{
#ifdef TAKEI_DEBUG_ON
    // To enable serial commands for on/off of actuators.
    readSerialCommands();
#endif
    // 
    // if ((millis() - interActivityTicker) > 1000) {
    //      logLine(interActivityFactor);
    //      if (!offAll) {
    //          if (interActivityFactor > 5000 && interActivityFactor < 7500) {
    //              cricketSounds[0].run();
    //              cricketSounds[1].stop();
    //              analogWrite(2, 0);
    //              frogCroak.stop();
    //          } else if (interActivityFactor > 7500 && interActivityFactor < 10000) {
    //              cricketSounds[0].run();
    //              cricketSounds[1].run();
    //              analogWrite(2, 0);
    //              frogCroak.stop();
    //          } else if (interActivityFactor > 10000) {
    //              cricketSounds[0].run();
    //              cricketSounds[1].run();
    //              frogCroak.run();
    //          } else {
    //              cricketSounds[0].stop();
    //              cricketSounds[1].stop();
    //              analogWrite(2, 0);
    //              frogCroak.stop();
    //          }
    //      }
    // 
    //      if (interActivityFactor > 600) {
    //          interActivityFactor = interActivityFactor - 500;
    //      }
    // 
    //      interActivityTicker = millis();
    //  }

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

    // cricketSound1.update();
}



void setupAmbientSoundActions()
{
    // Frog Actions and sequences
    randomMotor1.init(2, 250);

    frogCroakLerpAction1.init(2, 40, 230, 1200);
    frogCroakLerpAction2.init(2, 150, 250, 1000);
    // frogCroakLerpAction3.init(2, 0, 0, 100);

    frogCroak.addAction(frogCroakLerpAction1, 0, 1200);
    frogCroak.addAction(frogCroakLerpAction2, 1000, 1000);
    frogCroak.addAction(frogCroakLerpAction2, 1000, 1000);
    frogCroak.addAction(frogCroakLerpAction2, 1000, 1000);
    frogCroak.addAction(frogCroakLerpAction3, 150, 100);

    // frogCroak.addAction(frogCroakMotorAction1, 100, 400);
    // frogCroak.addAction(frogCroakMotorAction1, 2000, 500);
    // frogCroak.addAction(frogCroakMotorAction1, 100, 400);
    // frogCroak.addAction(frogCroakMotorAction1, 2000, 500);
    // frogCroak.addAction(frogCroakMotorAction1, 100, 400);

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
