//
// Created by Tommy Burholt on 25/09/2024.
//

#ifndef MIDIPARSER_H
#define MIDIPARSER_H
#include <math.h>



class MidiParser {
public:

};

inline float noteOn(unsigned char noteNumber)
{
    return powf(2.0, (noteNumber - 69)/12.0) * 440.0;
};

#endif //MIDIPARSER_H
