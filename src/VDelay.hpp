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
    
    double readCursor;
    int writeCursor;
    //double currentFractDelay;
    
    float previousOutL;
    float previousOutR;
    
    double outCurrDelay;
    float oldestSampleL;
    float oldestSampleR;
    
    //double BL;
    //double FF;
    //double FB;
    
    //double L;
    //double c;
    
    double nu;
    
    //const float NU = 0.5;
    
    public:
        explicit VDelay(float sampleRate);
        void processDelay(float** inputs, float** outputs, VstInt32 sampleFrames) override;
        void tick(float *inputL,float *inputR) override;
        void processDelayAlt(float** inputs, float** outputs, VstInt32 sampleFrames);
        void realignReadCursor(); //this function is necessary to align read cursor in range 0-maxDelayLine
        void calcOldestSample(float *oldestSampleL, float *oldestSampleR);
        void setFrequencyInHz(double frequencyInHz);
        void setMaxAmount(double maxAmount);
        void setMinAmount(double minAmount);
        double getFrequencyInHz();
        double getMaxAmount();
        double getMinAmount();
};

#endif /* VDelay_hpp */
