//
// Created by Tommy Burholt on 11/09/2024.
//

#ifndef WAVETABLEOSCILLATOR_H
#define WAVETABLEOSCILLATOR_H



class WavetableOscillator {
public:
    WavetableOscillator();
    void setFrequency(float frequency, float sampleRate);
    void createWavetable();
    void setWavetable(unsigned int table);
    float getNextSample();
    void setState(bool state);


private:
    float currentIndex = 0.0f, tableDelta = 0.0f;
    unsigned int tableNum = 0;

    const unsigned int tableSize = 1024;
    float tables[4][1024];
    bool isNoteOn = false;
};

#endif //WAVETABLEOSCILLATOR_H
