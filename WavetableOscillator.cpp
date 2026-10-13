//
// Created by Tommy Burholt on 11/09/2024.
//

#include "WavetableOscillator.h"
#include <math.h>
#include <stdio.h>

WavetableOscillator::WavetableOscillator()
{
    createWavetable();
}

void WavetableOscillator::setFrequency(float frequency, float sampleRate)
{
    auto tableSizeOverSampleRate = (float) tableSize / sampleRate;
    tableDelta = frequency * tableSizeOverSampleRate;
}

void WavetableOscillator::createWavetable()
{
    auto angleDelta = (2*M_PI) / (double) (tableSize - 1);
    auto currenAngle = 0.0;
    auto sampleSaw = 1.0;
    auto sampleTri = 0.0;
    auto triAdd = 4.0 / tableSize;

    // Generate Wavetables
    for (unsigned int i = 0; i < tableSize; i++)
    {
        // Sine
        auto sampleSine = sin(currenAngle);
        tables[0][i] = (float) sampleSine;

        // Saw
        tables[1][i] = (float) sampleSaw;
        sampleSaw -= (2.0 / tableSize);

        // Triangle
        tables[2][i] = (float) sampleTri;
        sampleTri += triAdd;
        if (sampleTri > 1.0 || sampleTri < -1.0)
        {
            triAdd *= -1;
        }

        // Square
        if (i < (tableSize / 2.0))
        {
            tables[3][i] = 1.0;
        } else
        {
            tables[3][i] = 0.0;
        }

        currenAngle += angleDelta;
    }
}

void WavetableOscillator::setWavetable(unsigned int table)
{
    tableNum = table;
}

float WavetableOscillator::getNextSample()
{
    if (!isNoteOn)
    {
        return 0.0f;
    }

    auto index0 = (unsigned int) currentIndex;
    auto index1 = index0 == (tableSize - 1) ? (unsigned int) 0 : index0 + 1;

    float frac = currentIndex - (float) index0;

    float value0 = tables[tableNum][index0];
    float value1 = tables[tableNum][index1];

    auto currentSample = value0 + frac* (value1 - value0);
    if((currentIndex += tableDelta) > (float) tableSize)
        currentIndex -= (float) tableSize;

    return currentSample;
}

void WavetableOscillator::setState(const bool state)
{
    isNoteOn = state;
}
