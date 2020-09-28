//
//  Delay.hpp
//  VstPlugin
//
//  Created by Alessandro Fazio on 08/09/2020.
//

#ifndef Delay_hpp
#define Delay_hpp

#include "../vstsdk2.4/public.sdk/source/vst2.x/audioeffect.h"
#include "BufferFactory.hpp"
#include "GainStereo.hpp"


class Delay
{
    protected:
        //delay
        float *bufferDelayL;
        float *bufferDelayR;
        
        int delayCursorL;
        int delayCursorR;
        
        int delayMaxSize;
        int delayCurrentSizeL;
        int delayCurrentSizeR;
        
        float delFeedbackL;
        float delFeedbackR;
        
        float maxFeedback;
        
        float wetDry;
        
        float sampleRate;
    
        float oldestSampleL;
        float oldestSampleR;

        //gain
        GainStereo gainStereo;

    
public:
    explicit Delay(float sampleRate);
    ~Delay();
    virtual void processDelay(float** inputs, float** outputs, VstInt32 sampleFrames);
    virtual void processDelayBySample(float *inputL,float *inputR , float *outputL,float *outputR);
    virtual void tick(float *inputL, float *inputR);
    
    //delay
    void deleteDelayLines();
    void createDelayLines();
    
    //getters
    float getOldestSampleL();
    float getOldestSampleR();
    
    int getDelayCurrentSizeL();
    int getDelayCurrentSizeR();
    
    int getDelayMaxSize();
    
    float getDelFeedbackL();
    float getDelFeedbackR();
    
    float getWetDry();
    
    float getMaxFeedback();
    
    GainStereo getDelayGain();

    
    //setters
    
    void setSampleRate(float sampleRate);
    
    void setDelayGain(float gainL, float gainR);

    
    void setDelayCurrentSizeL(int delayCurrentSizeL);
    void setDelayCurrentSizeR(int delayCurrentSizeR);
    
    void setDelayMaxSize(int delayMaxSize);
    
    void setDelFeedbackL(float delFeedbackL);
    void setDelFeedbackR(float delFeedbackR);
    
    void setMaxFeedback(float maxFeedback);
    
    void setWetDry(float wetDry);

    
};

#endif /* Delay_hpp */
