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


//-------------------------------------------------------------------------------------------------------
class Delay
{
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

    //gain
    GainStereo gainStereo;

    
public:
    explicit Delay(float sampleRate);
    ~Delay();
    void processDelay(float** inputs, float** outputs, VstInt32 sampleFrames);
    
    //delay
    void deleteDelayLines();
    void createDelayLines();
    
    //getters
    
    int getDelayCurrentSizeL();
    int getDelayCurrentSizeR();
    
    int getDelayMaxSize();
    
    float getDelFeedbackL();
    float getDelFeedbackR();
    
    float getWetDry();
    
    float getMaxFeedback();
    
    GainStereo getDelayGain();

    
    //setters
    
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
