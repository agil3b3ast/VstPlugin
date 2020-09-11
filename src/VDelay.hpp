//
//  VDelay.hpp
//  VstPlugin
//
//  Created by Alessandro Fazio on 09/09/2020.
//

#ifndef VDelay_hpp
#define VDelay_hpp

#include <stdio.h>
#include <fstream>
#include "Delay.hpp"
#include "Oscillator.hpp"
#include "ModOperator.hpp"

class VDelay : public Delay{
    Oscillator oscillator;
    ModOperator modOperator;
    
    float readCursor;
    int writeCursor;
    float currentFractDelay;
    
    const float NU = 0.5;
    
    public:
        explicit VDelay(float sampleRate);
        void processDelay(float** inputs, float** outputs, VstInt32 sampleFrames) override;
        void realignReadCursor(); //this function is necessary to align read cursor in range 0-maxDelayLine
        void calcOldestSample(float *oldestSampleL, float *oldestSampleR);
        void setFrequencyInHz(float frequencyInHz);
        float getFrequencyInHz();
};

#endif /* VDelay_hpp */
