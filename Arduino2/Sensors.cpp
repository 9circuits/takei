
#include "Sensors.h"

static byte wasTouched[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void (*touchCallbacks[8]) (byte val);
byte hasTouchCallback[8] = {0, 0, 0, 0, 0, 0, 0, 0};

static unsigned long cap_old_millis[8] = {0, 0, 0, 0, 0, 0, 0, 0};
static unsigned long cap_new_millis[8] = {0, 0, 0, 0, 0, 0, 0, 0};
static unsigned long temp_millis;

static byte cap_port_snapshot_new = 0x00;
static byte cap_port_snapshot_old = 0x00;
static byte non_masked_pins_changed = 0x00;

void setupCapsListener() 
{
    DDRK &= ~(0xff);
    PORTK |= 0xff;
    
#ifdef CAP1
    PCMSK2 |= (1 << PCINT16);
#endif

#ifdef CAP2
    PCMSK2 |= (1 << PCINT17);
#endif

#ifdef CAP3
    PCMSK2 |= (1 << PCINT18);
#endif

#ifdef CAP4
    PCMSK2 |= (1 << PCINT19);
#endif

#ifdef CAP5
    PCMSK2 |= (1 << PCINT20);
#endif

#ifdef CAP6
    PCMSK2 |= (1 << PCINT21);
#endif

#ifdef CAP7
    PCMSK2 |= (1 << PCINT22);
#endif

#ifdef CAP8
    PCMSK2 |= (1 << PCINT23);
#endif

    PCICR |= (1 << PCIE2);
}

ISR(PCINT2_vect) 
{
    cap_port_snapshot_new = PINK;
    cap_port_snapshot_new = cap_port_snapshot_new & PCMSK2;
    
    temp_millis = millis();
    
    non_masked_pins_changed = (cap_port_snapshot_new) ^ (cap_port_snapshot_old);
    
    if (non_masked_pins_changed) {
#ifdef CAP1
        if (non_masked_pins_changed & (1 << PCINT16)) {
            wasTouched[0] = temp_millis - cap_old_millis[0];
            cap_old_millis[0] = temp_millis;
        }
#endif
        
#ifdef CAP2
        if (non_masked_pins_changed & (1 << PCINT17)) {            
            wasTouched[1] = temp_millis - cap_old_millis[1];
            cap_old_millis[1] = temp_millis;
            // logLine(wasTouched[1]);
        }
#endif
        
#ifdef CAP3
        if (non_masked_pins_changed & (1 << PCINT18)) {
            wasTouched[2] = temp_millis - cap_old_millis[2];
            cap_old_millis[2] = temp_millis;
        }
#endif
        
#ifdef CAP4
        if (non_masked_pins_changed & (1 << PCINT19)) {
            wasTouched[3] = temp_millis - cap_old_millis[3];
            cap_old_millis[3] = temp_millis;
        }
#endif
        
#ifdef CAP5
        if (non_masked_pins_changed & (1 << PCINT20)) {
            wasTouched[4] = temp_millis - cap_old_millis[4];
            cap_old_millis[4] = temp_millis;
        }
#endif
        
#ifdef CAP6
        if (non_masked_pins_changed & (1 << PCINT21)) {
            wasTouched[5] = temp_millis - cap_old_millis[5];
            cap_old_millis[5] = temp_millis;
        }
#endif
        
#ifdef CAP7
        if (non_masked_pins_changed & (1 << PCINT22)) {            
            wasTouched[6] = temp_millis - cap_old_millis[6];
            cap_old_millis[6] = temp_millis;     
        }
#endif
        
#ifdef CAP8
        if (non_masked_pins_changed & (1 << PCINT23)) {
            wasTouched[7] = temp_millis - cap_old_millis[7];
            cap_old_millis[7] = temp_millis;
        }
#endif
    }
    
    cap_port_snapshot_old = cap_port_snapshot_new;
}

void registerTouchEvent(void (*func) (byte val), byte num) 
{
    touchCallbacks[num] = func;

    hasTouchCallback[num] = 1;
}

void touchDispatcher() {
    byte k = 0;
    while (k < 8) {
        if (hasTouchCallback[k] && wasTouched[k]) {
            (*touchCallbacks[k]) (wasTouched[k]);
        }
        k++;
    }
}



// Define variables to hold the analog callback data
void (*analogCallbacks[8]) (uint16_t  val);
uint16_t analogRanges[8][2];   
byte hasCallback[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void registerAnalogEvent(void (*func) (uint16_t  val), byte pinNumber, uint16_t low, uint16_t high) 
{
    analogCallbacks[pinNumber] = func;

    analogRanges[pinNumber][0] = low;
    analogRanges[pinNumber][1] = high;

    hasCallback[pinNumber] = 1;
}

void analogDispatcher() 
{
    // Loop to read analog pins
    byte pinNumber = 0;
    
    while (pinNumber < 8) {    
        if (hasCallback[pinNumber]) {
            uint16_t value = analogRead(pinNumber);
            
            if (analogRanges[pinNumber][0] < value && analogRanges[pinNumber][1] > value) {
                (*analogCallbacks[pinNumber]) (value);    
            }
        }
        
        pinNumber++;
    }    
}
