//
// Created by Tommy Burholt on 25/09/2024.
//

#ifndef ADSR_H
#define ADSR_H

enum
{

    stateAttack = 0,
    stateDecay,
    stateSustain,
    stateRelease,
    stateOff,
};

class ADSR {
public:
    ADSR(float sampleRate);

    float getADSRAmp();
    void setAttack(float attack);
    void setDecay(float decay);
    void setSustain(float sustain);
    void setRelease(float release);

    void setState(int state);
    bool isOn();

private:
    int currentState = stateOff;
    float sampleRate;
    int counter = 0;
    float currentValue;
    float incremeant;

    // Measured in ms
    float attackTime;
    float decayTime;
    float sustainLevel;
    float releaseTime;

    void update();
};



#endif //ADSR_H
