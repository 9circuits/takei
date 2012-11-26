
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
    
    registerTouchEvent(touchCap2, 1);
    //registerTouchEvent(touchCap3, 2);
    // registerTouchEvent(touchCap4, 3);
    registerTouchEvent(touchCap5, 4);
    registerTouchEvent(touchCap6, 5);
    
    registerAnalogEvent(LDRCallback7, 7, 180, 4550);
    //registerAnalogEvent(LDRCallback6, 6, 100, 550);
    //registerAnalogEvent(LDRCallback5, 5, 180, 390);    
    registerAnalogEvent(LDRCallback6, 6, 100, 450);    
    //registerAnalogEvent(LDRCallback3, 3, 100, 450);   
    
    //cricketSounds[1].run();
    //cricketSounds[2].run();
    //     frogCroak.run();
    
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
        analogWrite(7, 0);
        analogWrite(10, 0);
        analogWrite(11, 0);
        
        cricketSounds[1].stop();
        cricketSounds[2].stop();
        cricketSounds[3].stop();
        
        if (interActivityFactor > 110) {
            interActivityFactor = interActivityFactor - 100;
        }
        //frogCroak.stop();
    } else {
        offAll = 0;        
        
        if (interActivityFactor > 7500 && interActivityFactor < 10000) {
            cricketSounds[1].run();                
            cricketSounds[2].stop();
            cricketSounds[3].stop();
        } else if (interActivityFactor > 10000 && interActivityFactor < 15000) {
            cricketSounds[1].run();
            cricketSounds[2].run();
            cricketSounds[3].stop();
        } else if (interActivityFactor > 15000) {
            cricketSounds[1].run();
            cricketSounds[2].run();
            cricketSounds[3].run();
        } else {
            cricketSounds[1].stop();
            cricketSounds[2].stop();
            cricketSounds[3].stop();
        }
        //cricketSounds[1].run();
        //cricketSounds[2].run();
        //frogCroak.run();
    }
}



void LDRCallback6(uint16_t val) {
    if (offAll) {
        return;
    }
    
    if (val < 360) {
        analogWrite(11, 255);
        interActivityFactor++;
    } else {
        analogWrite(11, 0);
    }
    //analogWrite(8, 255 - map(val, 100, 450, 100, 255));    
}

void LDRCallback3(uint16_t val) {
    if (offAll) {
        return;
    }
    
    if (val < 360) {
        cricketSounds[4].run();
        interActivityFactor++;
    } else {
        cricketSounds[4].stop();        
    }
}


void touchCap2(byte interval) {
    if (offAll) {
        return;
    }

     if (interval < CAP2_MAX && interval > CAP2_MIN) {
         cricketSounds[0].run();
         // interActivityFactor++;
     } else {
         cricketSounds[0].stop();
     }
}

void touchCap3(byte interval)
{
    if (offAll) {
        return;
    }
    
    if (interval < CAP3_MAX && interval > CAP3_MIN) {
        analogWrite(5, 255);
        interActivityFactor++;
    } else {
        analogWrite(5, 0);
    }    
}

// CACOPHONY
void touchCap5(byte interval)
{
    if (interval < CAP5_MAX && interval > CAP5_MIN) {
         if (!offAll) {
             analogWrite(10, 255);
             analogWrite(7, 255);
             analogWrite(11, 255);
             interActivityFactor++;
         } else {
             analogWrite(10, 0);
             analogWrite(7, 0);
             analogWrite(11, 0);
         }
     } else {
         analogWrite(10, 0);
         analogWrite(7, 0);   
         analogWrite(11, 0);
     }
}


void touchCap6(byte interval)
{
    if (offAll) {
        return;
    }
    
    if (interval < CAP6_MAX && interval > CAP6_MIN) {
        analogWrite(7, 255);
        interActivityFactor++;
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

    if ((millis() - interActivityTicker) > 1000) {
        logLine(interActivityFactor);
        if (!offAll) {
            if (interActivityFactor > 7500 && interActivityFactor < 10000) {
                cricketSounds[1].run();                
                cricketSounds[2].stop();
                cricketSounds[3].stop();
            } else if (interActivityFactor > 10000 && interActivityFactor < 15000) {
                cricketSounds[1].run();
                cricketSounds[2].run();
                cricketSounds[3].stop();
            } else if (interActivityFactor > 15000) {
                cricketSounds[1].run();
                cricketSounds[2].run();
                cricketSounds[3].run();
            } else {
                cricketSounds[1].stop();
                cricketSounds[2].stop();
                cricketSounds[3].stop();
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
    
    // logLine(interActivityFactor);
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
