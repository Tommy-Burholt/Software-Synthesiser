//
// Created by Tommy Burholt on 17/09/2024.
//

#ifndef SIMPLEDISTORTION_H
#define SIMPLEDISTORTION_H



class SimpleDistortion {
public:
    SimpleDistortion();
    float getNextSample(float input);
    void setGain(float value);
    void setMode(unsigned int choice);
    void setDCOffset(bool choice);

private:
    unsigned int mode = 0;
    float gain = 0.0f;
    bool DCOffset = true;
};



#endif //SIMPLEDISTORTION_H
