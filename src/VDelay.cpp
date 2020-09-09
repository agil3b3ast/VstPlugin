//
//  VDelay.cpp
//  VstPlugin
//
//  Created by Alessandro Fazio on 09/09/2020.
//

#include "VDelay.hpp"
#include <fstream>

VDelay::VDelay(float sampleRate): Delay(sampleRate), oscillator(sampleRate), modOperator(&oscillator){
    modOperator.setMinAmount((this->delayMaxSize/2) - 1);
}

void VDelay::processDelay(float** inputs, float** outputs, VstInt32 sampleFrames){
    float *buffL = inputs[0]; // buffer input left
    float *buffR = inputs[1]; // buffer input right
    
    float *buffOutL = outputs[0]; // buffer output left
    float *buffOutR = outputs[1]; // buffer output right
    
    float wetDryBalance;
    
    int outCurrDelay = 0;
    
    std::fstream fout;
    fout.open("/Users/alessandro_fazio/Desktop/output.csv", std::ios::out | std::ios::app);
    
    for(int i=0; i<sampleFrames;i++){
        modOperator.updateModOperator();
        modOperator.processModOperator(&delayCurrentSizeL, &outCurrDelay);
        //modOperator.processModOperator(&delayCurrentSizeR, &outCurrDelay); TODO estendere a delay stereo
        
        fout << std::to_string(outCurrDelay) << '\n';

        
        float oldestSampleL = bufferDelayL[delayCursorL];
        float oldestSampleR = bufferDelayR[delayCursorR];
        
        
        bufferDelayL[delayCursorL] = buffL[i]+oldestSampleR*delFeedbackL;
        bufferDelayR[delayCursorR] = buffR[i]+oldestSampleL*delFeedbackR;

        delayCursorL++;
        delayCursorR++;
        
        //TODO estendere a delay stereo
        if (delayCursorL >= outCurrDelay){
            delayCursorL = 0;
        }
        
        if (delayCursorR >= outCurrDelay){
            delayCursorR = 0;
        }
        
        wetDryBalance = wetDry*buffL[i]+(1-wetDry)*oldestSampleL;
        gainStereo.processGainL(&wetDryBalance);
        buffOutL[i] = wetDryBalance;
        
        wetDryBalance = wetDry*buffR[i]+(1-wetDry)*oldestSampleR;
        gainStereo.processGainR(&wetDryBalance);
        buffOutR[i] = wetDryBalance;
        
        
    }
    
    fout.close();

}
