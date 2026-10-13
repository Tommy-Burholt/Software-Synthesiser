//
// Created by Tommy Burholt on 25/09/2024.
//

#include "ADSR.h"
#include <iostream>

ADSR::ADSR(float sampleRate_)
{
    sampleRate = sampleRate_;
    currentValue = 0.0f;
    incremeant = 0.0f;

    attackTime = 0.05f;
    decayTime = 0.05f;
    sustainLevel = 0.8f;
    releaseTime = 500.f;
}

// Retrieves current value and updates count
float ADSR::getADSRAmp()
{
    currentValue += incremeant;
    counter--;

    switch (currentState)
    {
    case stateAttack:
        if (counter == 0)
        {
            //std::cout << "Switching to decay: \n";
            currentState = stateDecay;
            update();
        }
        return currentValue;

    case stateDecay:
        if (counter == 0)
        {
            //std::cout << "Switching to sustain: \n";
            currentState = stateSustain;
            update();
        }
        return currentValue;

    case stateSustain:
        return sustainLevel;

    case stateRelease:
        if (counter == 0)
        {
            //std::cout << "Switching to off: \n";
            currentState = stateOff;
            update();
        }
        return currentValue;

    case stateOff:
        return 0.0f;

    default:
        return 0.0f;
    }
}

// Updates Attack, Decay, Sustain, Release values
void ADSR::setAttack(const float attack)
{
    attackTime = attack;
}

void ADSR::setDecay(const float decay)
{
    decayTime = decay;
}

void ADSR::setSustain(const float sustain)
{
    sustainLevel = sustain;
}

void ADSR::setRelease(const float release)
{
    releaseTime = release;
}

// Used to enter Attack or Release State
void ADSR::setState(int state)
{
    currentState = state;
    update();
}

bool ADSR::isOn()
{
    if (currentState != stateOff)
    {
        return true;
    } return false;
}

// Updates counter and increment when state changes
void ADSR::update()
{
    //std::cout << "update called\n";
    switch (currentState)
    {
    case stateAttack:
        counter = (int)(sampleRate * attackTime);
        incremeant = 1.0f / (sampleRate * attackTime);

    case stateDecay:
        counter = (int)(sampleRate * decayTime);
        incremeant = (sustainLevel - 1.0f) / (sampleRate * decayTime);


    case stateRelease:
        counter = (int)(sampleRate * releaseTime);
        incremeant = (0.0f - sustainLevel) / (sampleRate * releaseTime);
    }
    //std::cout << "Counter: " << counter << "\n";
}
