#ifndef __TAKEI_ACTUATORS__
#define __TAKEI_ACTUATORS__

#include "Common.h"



class DeviceAction 
{
protected:
    byte isRunning;    
    
    byte pinNumber;
    
    unsigned long old_millis;
public:
    DeviceAction() : isRunning(0) {
    };
    
    virtual void runAction();
    virtual void stopAction();
    
    virtual void update();
};



class MotorRunAction : public DeviceAction 
{
private:
    byte frequency;    
public:
    void init(byte pin, byte f);
    void setFrequency(byte f);
    
    void runAction();
    void stopAction();
    
    void update();
};



class MotorLerpAction : public DeviceAction 
{
private:
    byte startFrequency, endFrequency;
    
    byte currentFrequency;
    
    int multiplier;
    
    unsigned int lerpDuration;
    
    unsigned int timeDivision;
    
    byte loop;
    
public:
    void init(byte pin, byte sf = 0, byte ef = 255, unsigned int duration = 0, byte loopBack = 0);
    void setFrequencyRange(byte sf, byte ef);
    
    void runAction();
    void stopAction();
    
    void update();
};



class RelayClickAction : public DeviceAction 
{
private:    
    unsigned int clickDuration;
        
    byte state;
    
public:    
    void init(byte pin, unsigned int duration);
    void setRelayClickDuration(unsigned int duration);
    
    void runAction();
    void stopAction();
    
    void update();
};



class ActionSequence
{
private:
    DeviceAction *sequence[10];
    
    unsigned int intervals[10];
    unsigned int durations[10];
    
    unsigned long old_millis;
    
    byte isRunning;
    
    byte currentAction;
    
    byte numActions;
    
    byte isCurrentActionRunning;
    
    byte loop;
    
public:    
    ActionSequence(byte l = 1);
    
    void addAction(DeviceAction &d, unsigned int interval, unsigned int duration);
    
    void run(byte index = 0);
    void stop();

    void clear();
    
    byte isComplete();
    
    void update();
};



void setupMotors();

class ActuationTimer
{
private:
    unsigned long timeOut;
    unsigned long waitTime;
    
    unsigned long lastInitiationTime;
    
    byte isInWaitTime;
public:
    ActuationTimer(unsigned long, unsigned long);
    byte isLate();
    void wasInitiated();
    void update();
};

#endif // __TAKEI_ACTUATORS__