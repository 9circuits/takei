
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
    } else {
        analogWrite(2, 0);
    }
}
void lightL7(uint16_t val) {
    if (val < 360) {
        offAll = 1;
        // analogWrite(2, 0);
        // analogWrite(9, 0);
        
        // cricketSounds[1].stop();
        analogWrite(3, 0);
        // analogWrite(8, 0);
        cricketSounds[2].stop();
        cricketSounds[4].stop();
        
        if (interActivityFactor > 110) {
            interActivityFactor = interActivityFactor - 100;
        }
    } else {
        offAll = 0;        
        
        if (interActivityFactor > 7500 && interActivityFactor < 10000) {
            // cricketSounds[1].run();  
            cricketSounds[2].run();
            cricketSounds[4].stop();
            analogWrite(3, 0);
            // analogWrite(8, 0);              
        } else if (interActivityFactor > 10000 && interActivityFactor < 15000) {
            // cricketSounds[1].run();
            cricketSounds[2].run();
            cricketSounds[4].run();
            analogWrite(3, 0);
            // analogWrite(8, 0);
        } else if (interActivityFactor > 15000) {
            // cricketSounds[1].run();
            cricketSounds[2].run();
            cricketSounds[4].run();
            analogWrite(3, 255);
            // analogWrite(8, 255);
            // analogWrite(2, 255);
            // analogWrite(9, 255);
        } else {
            // cricketSounds[1].stop();
            cricketSounds[2].stop();
            cricketSounds[4].stop();
            analogWrite(3, 0);
            // analogWrite(8, 0);
            // analogWrite(2, 0);
            // analogWrite(9, 0);
        }
    }
}
void lightL8(uint16_t val) {
    if (offAll) {
        return;
    }
    
    if (val < 300) {
        cricketSounds[3].run();
        interActivityFactor++;
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
// 
// void touchC21(byte interval) {
//     if (offAll) {
//         return;
//     }
//     Serial.println(interval);
//     if (interval < CAP5_MAX && interval > CAP5_MIN) {
//         // cricketSounds[0].run();
//         // interActivityFactor++;
//     } else {
//          // cricketSounds[0].stop();
//      }
// }

// 
// // CACOPHONY
// void touchCap5(byte interval)
// {
//     if (interval < CAP5_MAX && interval > CAP5_MIN) {
//          if (!offAll) {
//              analogWrite(10, 255);
//              analogWrite(7, 255);
//              analogWrite(11, 255);
//              interActivityFactor++;
//          } else {
//              analogWrite(10, 0);
//              analogWrite(7, 0);
//              analogWrite(11, 0);
//          }
//      } else {
//          analogWrite(10, 0);
//          analogWrite(7, 0);   
//          analogWrite(11, 0);
//      }
// }





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
               // cricketSounds[1].run();  
               cricketSounds[2].run();
               cricketSounds[4].stop();
               analogWrite(3, 0);
               // analogWrite(8, 0);              
           } else if (interActivityFactor > 10000 && interActivityFactor < 15000) {
               // cricketSounds[1].run();
               cricketSounds[2].run();
               cricketSounds[4].run();
               analogWrite(3, 0);
               // analogWrite(8, 0);
           } else if (interActivityFactor > 15000) {
               // cricketSounds[1].run();
               cricketSounds[2].run();
               cricketSounds[4].run();
               analogWrite(3, 255);
               // analogWrite(8, 255);
               // analogWrite(2, 255);
               // analogWrite(9, 255);
           } else {
               // cricketSounds[1].stop();
               cricketSounds[2].stop();
               cricketSounds[4].stop();
               analogWrite(3, 0);
               // analogWrite(8, 0);
               // analogWrite(2, 0);
               // analogWrite(9, 0);
           }
       }
       
       if (interActivityFactor > 600) {
           interActivityFactor = interActivityFactor - 100;
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
    
    // cricketSound1.update();
}



void setupAmbientSoundActions()
{
    // Frog Actions and sequences
    // randomMotor1.init(2, 250);
    
    frogCroakLerpAction1.init(8, 40, 230, 1200);
    frogCroakLerpAction2.init(8, 150, 250, 1000);
    frogCroakLerpAction3.init(8, 0, 0, 100);
    
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
            // analogWrite(8, 255);
        }
        
        if (c == 'a') {
            frogCroak.stop();
            // analogWrite(8, 0);
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
