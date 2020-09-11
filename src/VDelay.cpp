//
//  VDelay.cpp
//  VstPlugin
//
//  Created by Alessandro Fazio on 09/09/2020.
//

#include "VDelay.hpp"

VDelay::VDelay(float sampleRate): Delay(sampleRate), oscillator(sampleRate), modOperator(&oscillator){
    currentFractDelay = ((double)delayMaxSize)/2.0;
    modOperator.setMinAmount(currentFractDelay);
    writeCursor = 0; //cannot set writeCursor to max/2 due to precision errors
    readCursor = delayMaxSize-currentFractDelay; //it is necessary to start here to avoid writeCursor precision errors
}

float VDelay::getFrequencyInHz(){
    return modOperator.getOscillator()->getFrequencyInHz();
}

void VDelay::setFrequencyInHz(float frequencyInHz){
    modOperator.getOscillator()->setFrequencyInHz(frequencyInHz);
}

void VDelay::realignReadCursor(){
    if (readCursor < 0.0){
        while (readCursor < 0.0){
            readCursor = readCursor + ((double)delayMaxSize);
        }
    }
    else if (readCursor >= ((double)delayMaxSize)){
        while (readCursor >= ((double)delayMaxSize)){
            readCursor = readCursor - ((double)delayMaxSize);
        }
    }
}

void VDelay::calcOldestSample(float *oldestSampleL, float *oldestSampleR){
    int previous = floor(readCursor);
    int next = ceil(readCursor);
    
    if(previous < 0){ //realign previous
        previous = delayMaxSize -1;
    }
    if(next == delayMaxSize){ //realign next
        next = 0;
    }
    *oldestSampleL = NU*bufferDelayL[previous] + (1.0-NU)*bufferDelayL[next];
    *oldestSampleR = NU*bufferDelayR[previous] + (1.0-NU)*bufferDelayR[next];

}

void VDelay::processDelay(float** inputs, float** outputs, VstInt32 sampleFrames){
    float *buffL = inputs[0]; // buffer input left
    float *buffR = inputs[1]; // buffer input right
    
    float *buffOutL = outputs[0]; // buffer output left
    float *buffOutR = outputs[1]; // buffer output right
    
    float wetDryBalance;
    
    double outCurrDelay = 0.0;
    float oldestSampleL = 0.0;
    float oldestSampleR = 0.0;
    
    std::fstream fout;
    fout.open("/Users/alessandro_fazio/Desktop/output.csv", std::ios::out | std::ios::app);
    
    for(int i=0; i<sampleFrames;i++){
        modOperator.updateModOperator();
        modOperator.processModOperator(&currentFractDelay, &outCurrDelay);
        //oscillator.processOscillatorSingle(&buffOutL[i]);
        //oscillator.processOscillatorSingle(&buffOutR[i]);
        //modOperator.processModOperator(&delayCurrentSizeR, &outCurrDelay); TODO estendere a delay stereo
        readCursor = ((double)writeCursor) - outCurrDelay;
        
        fout << std::to_string(readCursor) << '\n';
        
        realignReadCursor();
        
        calcOldestSample(&oldestSampleL, &oldestSampleR);
        
        //float oldestSampleL = bufferDelayL[delayCurrentSizeL];
        //float oldestSampleR = bufferDelayR[delayCurrentSizeR];
        
        
        bufferDelayL[writeCursor] = buffL[i]+oldestSampleR*delFeedbackL;
        bufferDelayR[writeCursor] = buffR[i]+oldestSampleL*delFeedbackR;

        writeCursor++;
        
        //TODO estendere a delay stereo
        if (writeCursor >= delayMaxSize){
            writeCursor = 0;
        }
        
        /*
        if (delayCursorR >= outCurrDelay){
            delayCursorR = 0;
        }*/
        
        wetDryBalance = wetDry*buffL[i]+(1-wetDry)*oldestSampleL;
        gainStereo.processGainL(&wetDryBalance);
        buffOutL[i] = wetDryBalance;
        
        wetDryBalance = wetDry*buffR[i]+(1-wetDry)*oldestSampleR;
        gainStereo.processGainR(&wetDryBalance);
        buffOutR[i] = wetDryBalance;
        
        
    }
    
    fout.close();

}
