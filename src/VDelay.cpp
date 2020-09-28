//
//  VDelay.cpp
//  VstPlugin
//
//  Created by Alessandro Fazio on 09/09/2020.
//

#include "VDelay.hpp"

VDelay::VDelay(float sampleRate): Delay(sampleRate), oscillator(sampleRate), modOperator(&oscillator), interp(2){
    initVDelay();
}
//-----------------------------------------------------------------------------------------

void VDelay::initVDelay(){
    minV = 0.001f * sampleRate; //suppose 1ms min delay
    
    
    modOperator.setMinAmount((double) minV);
    modOperator.setMaxAmount((double)delayMaxSize);
    modOperator.getOscillator()->setSampleRate(sampleRate);
    
    writeCursor = 0; //cannot set writeCursor directly to max/2 due to precision errors
    readCursor = 0.0;
    
    outCurrDelay = 0.0;
    
    interp.setMax(delayMaxSize);
}
//-----------------------------------------------------------------------------------------

double VDelay::getMinV(){
    return minV;
}
//-----------------------------------------------------------------------------------------

double VDelay::getAmount(){
    return modOperator.getAmount();
}
//-----------------------------------------------------------------------------------------

void VDelay::setAmount(double amount){
    modOperator.setAmount(amount);
}

//-----------------------------------------------------------------------------------------

double VDelay::getFrequencyInHz(){
    return modOperator.getOscillator()->getFrequencyInHz();
}
//-----------------------------------------------------------------------------------------

double VDelay::getMaxFreq(){
    return modOperator.getOscillator()->getMaxFreq();
}
//-----------------------------------------------------------------------------------------

double VDelay::getMinFreq(){
    return modOperator.getOscillator()->getMinFreq();
}
//-----------------------------------------------------------------------------------------

void VDelay::setPhase(float phase){
    modOperator.getOscillator()->setPhase(phase);
}
//-----------------------------------------------------------------------------------------

void VDelay::setFrequencyInHz(double frequencyInHz){
    modOperator.getOscillator()->setFrequencyInHz(frequencyInHz);
}
//-----------------------------------------------------------------------------------------

void VDelay::realignReadCursor(){
    double dlength = static_cast<double>(delayMaxSize);
    if (readCursor <= 0.0){
        readCursor = readCursor + dlength;
    }
    else if (readCursor == dlength){
        readCursor = readCursor - dlength;
    }
}
//-----------------------------------------------------------------------------------------

void VDelay::calcOldestSample(float *oldestSampleL, float *oldestSampleR){

    interp.setInterp(readCursor);

    interp.interpAllPass(oldestSampleL, 0, bufferDelayL); //left
    interp.interpAllPass(oldestSampleR, 1, bufferDelayR); //right

}

//-----------------------------------------------------------------------------------------
void VDelay::tick(float *inputL,float *inputR){
    modOperator.updateModOperator();
    modOperator.processModOperator(&outCurrDelay);
    
    
    readCursor = writeCursor - outCurrDelay;
    
    
    
    realignReadCursor();
    
    
    calcOldestSample(&oldestSampleL, &oldestSampleR);
    
    
    bufferDelayL[writeCursor] = *inputL+oldestSampleL*delFeedbackL;
    bufferDelayR[writeCursor] = *inputR+oldestSampleR*delFeedbackR;
    
    writeCursor++;
    
    if (writeCursor == delayMaxSize){
        writeCursor = 0;
    }
    
}
