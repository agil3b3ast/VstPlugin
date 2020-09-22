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
#include "Interp.hpp"

class VDelay : public Delay{
    Oscillator oscillator;
    ModOperator modOperator;
    Interp interp;

    double readCursor;
    int writeCursor;
    //double currentFractDelay;

    //float previousOutL;
    //float previousOutR;

    double outCurrDelay;
    float oldestSampleL;
    float oldestSampleR;
    float minV;

    //double BL;
    //double FF;
    //double FB;

    //double L;
    //double c;

    double nu;

    //const float NU = 0.5;

    public:
        explicit VDelay(float sampleRate);
        void initVDelay();
        float getOldestSampleL();
        float getOldestSampleR();
        void processDelay(float** inputs, float** outputs, VstInt32 sampleFrames) override;
        void processDelayBySample(float *inputL,float *inputR , float *outputL,float *outputR);
        void tick(float *inputL,float *inputR) override;
        void processDelayAlt(float** inputs, float** outputs, VstInt32 sampleFrames);
        void realignReadCursor(); //this function is necessary to align read cursor in range 0-maxDelayLine
        void calcOldestSample(float *oldestSampleL, float *oldestSampleR);

        void setFrequencyInHz(double frequencyInHz);
        void setAmount(double amount);
        void setPhase(float phase);

        double getFrequencyInHz();
        double getMaxFreq();
        double getMinFreq();
        double getMinV();
        double getAmount();
};

#endif /* VDelay_hpp */
