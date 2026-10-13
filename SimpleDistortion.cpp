//
// Created by Tommy Burholt on 17/09/2024.
//

#include "SimpleDistortion.h"
#include <cmath>
#include <__system_error/errc.h>

SimpleDistortion::SimpleDistortion()
{

}

float SimpleDistortion::getNextSample(float input)
{
    float sample;
    switch (mode)
    {
    // Soft clipping
    case 0:
        if (input >=1.0f)
        {
            return 1.0f;
        } if (input <= -1.0f)
        {
            return -1.0f;
        }
        sample = (3.0f/2.0f)*(input - (powf(input, 3)/3));

    // Hard clipping
    case 1:
        if (input >= 1.0f)
        {
            return 1.0f;
        }
        sample = fmax(input, -1.0f);

    // Asym
    case 2:
        const float squareRoot = sqrtf(4 * input * input);
        sample = input * (input * (1.0f / squareRoot) + 1) - 0.3f;

    // Default case
    default:
        sample =  input;
    }

    // Add DC offset removal (Highpass filter at 10hz)

    return sample;
}

// Gain inputted is in db
void SimpleDistortion::setGain(const float dB)
{
    // Converts dB to Volume
    gain  = powf(10.0f, 0.05f * dB);
}

void SimpleDistortion::setMode(const unsigned int choice)
{
    if (choice > 2 || choice < 0)
    {
        throw std::errc::invalid_argument("Mode must be between 0 and 2");
    }
    mode = choice;
}

void SimpleDistortion::setDCOffset(const bool choice)
{
    DCOffset = choice;
}
