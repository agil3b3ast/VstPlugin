//
//  Chorus.hpp
//  VstPlugin
//
//  Created by Alessandro Fazio on 13/09/2020.
//

#ifndef Chorus_hpp
#define Chorus_hpp

#include <stdio.h>
#include <fstream>
#include "VDelay.hpp"
#include "GainStereo.hpp"

class Chorus{
    VDelay *vdelay1;
    VDelay *vdelay2;
    VDelay *vdelay3;

    GainStereo gainStereo;
    
    float wetDry;
    
public:
    explicit Chorus(float sampleRate);
    ~Chorus();
    
    void processChorus(float** inputs, float** outputs, VstInt32 sampleFrames);
    void processChorusBySample(float* inputL, float* inputR);
    
    
    //getters
    VDelay *getDelay1();
    VDelay *getDelay2();
    VDelay *getDelay3();
    GainStereo getGain();
    double getFrequencyInHz1();
    double getFrequencyInHz2();
    double getFrequencyInHz3();
    float getWetDry();
    
    //setters
    void setGain(float gainL, float gainR);
    void setFrequencyInHz1(double frequencyInHz);
    void setFrequencyInHz2(double frequencyInHz);
    void setFrequencyInHz3(double frequencyInHz);
    void setWetDry(float wetDry);
};

#endif /* Chorus_hpp */
