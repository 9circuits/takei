
#include "Actuators.h"

void MotorRunAction::init(byte pin, byte f) 
{
    pinNumber = pin;
    isRunning = 0;
    
    setFrequency(f);
    
    pinMode(pinNumber, OUTPUT);
    //logWrite(pinNumber, 0);  
};

void MotorRunAction::setFrequency(byte f) 
{
    frequency = f;
};

void MotorRunAction::runAction() 
{
    // Action is already running. Do not start again.
    if (isRunning) {
        return;
    }
    
    isRunning = 1;
    
    old_millis = millis();
    
    //logWrite(pinNumber, frequency);
    
    //log("Running motor run action...");
};

void MotorRunAction::stopAction() 
{
    // Action is already stopped. Do not stop again.
    if (!isRunning) {
        return;
    }
    
    isRunning = 0;
    
    //logWrite(pinNumber, 0);
    
    //logLine("");
    //logLine("Stopping motor run action...");
};

void MotorRunAction::update() 
{
    if(!isRunning) {
        return;
    }
};



void MotorLerpAction::init(byte pin, byte sf, byte ef, unsigned int duration, byte loopBack) 
{
    pinNumber = pin;
    isRunning = 0;
    
    setFrequencyRange(sf, ef);
    
    currentFrequency = startFrequency;
    
    lerpDuration = duration;
    
    timeDivision = lerpDuration / ((endFrequency - startFrequency) * multiplier);
    
    loop = loopBack;
            
    pinMode(pinNumber, OUTPUT);
    //logWrite(pinNumber, 0);  
};

void MotorLerpAction::setFrequencyRange(byte sf, byte ef) 
{
    startFrequency = sf;
    endFrequency = ef;
    
    if (sf < ef) {
        multiplier = 1;
    } else {
        multiplier = -1;
    }
};

void MotorLerpAction::runAction() 
{
    // Action is already running. Do not start again.
    if (isRunning) {
        return;
    }
    
    isRunning = 1;
    
    old_millis = millis();
    
    //log("Running motor lerp action...");
};

void MotorLerpAction::stopAction() 
{
    // Action is already stopped. Do not stop again.
    if (!isRunning) {
        return;
    }
    
    isRunning = 0;
    
    currentFrequency = startFrequency;
    
    //logWrite(pinNumber, 0);

    //logLine("");
    //logLine("Stopping motor lerp action...");
};

void MotorLerpAction::update() 
{
    if(!isRunning) {
        return;
    }
    
    if( (currentFrequency > endFrequency && multiplier == 1) || \
        (currentFrequency < endFrequency + 1 && multiplier == -1) )
    {
        if (!loop) {
            setFrequencyRange(endFrequency, startFrequency);
            currentFrequency = startFrequency;
        } else {
            currentFrequency = startFrequency;
        }
    }
    
    if ((millis() - old_millis) > timeDivision) {
        currentFrequency = currentFrequency + 1 * multiplier;
        
        //logWrite(pinNumber, currentFrequency);
        
        old_millis = millis();
        
        //log(currentFrequency);
        //log(", ");
    }
};



void RelayClickAction::init(byte pin, unsigned int duration) 
{
    pinNumber = pin;
    isRunning = 0;
            
    setRelayClickDuration(duration);
    
    pinMode(pinNumber, OUTPUT);
    state = LOW;
    digitalWrite(pinNumber, state);  
};

void RelayClickAction::setRelayClickDuration(unsigned int duration) 
{
    clickDuration = duration;
};

void RelayClickAction::runAction() {
    // Action is already running. Do not start again.
    if (isRunning) {
        return;
    }
    
    isRunning = 1;
    
    old_millis = millis();
    
    //log("Running relay action...");
};

void RelayClickAction::stopAction() 
{
    // Action is already stopped. Do not stop again.
    if (!isRunning) {
        return;
    }
    
    isRunning = 0;

    state = LOW;
    digitalWrite(pinNumber, state);
    
    //logLine("");
    //logLine("Stopping relay action...");
};

void RelayClickAction::update() 
{
    if(!isRunning) {
        return;
    }
    
    if ((millis() - old_millis) > clickDuration) {                
        //log(".");
        
        state = state == LOW ? HIGH : LOW;
        digitalWrite(pinNumber, state);
        old_millis = millis();
    }
};



ActionSequence::ActionSequence(byte l) 
{
    isRunning = 0;
    currentAction = 0;
    numActions = 0;
    
    loop = l;
    
    isCurrentActionRunning = 0;
};

void ActionSequence::addAction(DeviceAction &d, unsigned int interval, unsigned int duration) 
{
    // Max number of supported actions reached. Let us be fair here. This is
    // just an 8 bit processor! TROLOLOLOLOLOLOLOL... Mood kharab ho gaya...
    if (numActions > 9) {
        return;
    }
    
    sequence[numActions] = &d;
    
    intervals[numActions] = interval;
    durations[numActions] = duration;
    
    numActions++;
};

void ActionSequence::run(byte index) 
{
    // If the sequence is already running then do nothing.
    if (isRunning) {
        return;
    }
    
    //logLine("Starting sequence run!");
    
    old_millis = millis();
    
    isRunning = 1;
    currentAction = index;
};

void ActionSequence::stop() 
{
    // Do not stop if not already running.
    if (!isRunning) {
        return;
    }
    
    isRunning = 0;
    currentAction = 0;
};

void ActionSequence::clear() 
{
    // If running then do not do such drastic stuff.
    if (isRunning) {
        return;
    }

    currentAction = 0;
    numActions = 0;
};

byte ActionSequence::isComplete() 
{
    return isRunning;
};

void ActionSequence::update() 
{
    // If the sequence is not in run state then do nothing.
    if (!isRunning) {
        return;
    }
    
    unsigned long time_elapsed = millis() - old_millis;
    
    if (!isCurrentActionRunning) {
        // If no action is running then check if it is time to begin
        if (time_elapsed > intervals[currentAction]) {
            if (currentAction >= numActions) {
                
                if (loop) {
                    currentAction = 0;
                    //logLine("Repeating the whole sequence...");
                } else {
                    isRunning = 0;
                    //logLine("All tasks are done...");
                    return;
                }
            }
                            
            //log("Current action = ");
            //log(currentAction);
            //logLine(" | Beginning action...");

            sequence[currentAction]->runAction();
                                            
            isCurrentActionRunning = 1;
            
            old_millis = millis();
        }
    } else {
        // If an action is running then check if time has become over
        if (time_elapsed > durations[currentAction]) {                
            isCurrentActionRunning = 0;

            sequence[currentAction]->stopAction();

            //log("Current action = ");
            //log(currentAction);
            //logLine(" | Action complete...");

            currentAction++;

            old_millis = millis();                
        } else {
            sequence[currentAction]->update();
        }
        
    }
};



void setupMotors() 
{
    // Set these up so the motors don't boot up immediately
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);

    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
}

