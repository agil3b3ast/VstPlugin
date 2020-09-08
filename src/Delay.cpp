//
//  Delay.cpp
//  VstPlugin
//
//  Created by Alessandro Fazio on 08/09/2020.
//

#include "Delay.hpp"

Delay::Delay(float sampleRate): gainStereo(){
    delFeedbackL = 0.5;
    delFeedbackR = 0.5;
    
    maxFeedback = 0.99;
    
    wetDry  = 0.5; // 0 full dry, 1 full wet
    
    this->sampleRate = sampleRate;
    
    // gain
    
    createDelayLines();
}

//-------------------------------------------------------------------------------------------------------
Delay::~Delay(){
    deleteDelayLines();
}

//-------------------------------------------------------------------------------------------------------

void Delay::setDelayGain(float gainL, float gainR){
    gainStereo.setGainL(gainL);
    gainStereo.setGainR(gainR);
}

//-------------------------------------------------------------------------------------------------------

void Delay::setDelayCurrentSizeL(int delayCurrentSizeL){
    this->delayCurrentSizeL = delayCurrentSizeL;
}
//-------------------------------------------------------------------------------------------------------

void Delay::setDelayCurrentSizeR(int delayCurrentSizeR){
    this->delayCurrentSizeR = delayCurrentSizeR;
}

//-------------------------------------------------------------------------------------------------------

void Delay::setDelayMaxSize(int delayMaxSize){
    this->delayMaxSize = delayMaxSize;
}
//-------------------------------------------------------------------------------------------------------

void Delay::setDelFeedbackL(float delFeedbackL){
    this->delFeedbackL = delFeedbackL;
}
//-------------------------------------------------------------------------------------------------------

void Delay::setDelFeedbackR(float delFeedbackR){
    this->delFeedbackR = delFeedbackR;
}
//-------------------------------------------------------------------------------------------------------

void Delay::setMaxFeedback(float maxFeedback){
    this->maxFeedback= maxFeedback;
}

//-------------------------------------------------------------------------------------------------------

void Delay::setWetDry(float wetDry){
    this->wetDry = wetDry;
}

//-------------------------------------------------------------------------------------------------------

GainStereo Delay::getDelayGain(){
    return gainStereo;
}

//-------------------------------------------------------------------------------------------------------
int Delay::getDelayCurrentSizeL(){
    return delayCurrentSizeL;
}

//-------------------------------------------------------------------------------------------------------
int Delay::getDelayCurrentSizeR(){
    return delayCurrentSizeR;
}

//-------------------------------------------------------------------------------------------------------
int Delay::getDelayMaxSize(){
    return delayMaxSize;
}

//-------------------------------------------------------------------------------------------------------
float Delay::getDelFeedbackL(){
    return delFeedbackL;
}

//-------------------------------------------------------------------------------------------------------
float Delay::getDelFeedbackR(){
    return delFeedbackR;
}

//-------------------------------------------------------------------------------------------------------
float Delay::getWetDry(){
    return wetDry;
}

//-------------------------------------------------------------------------------------------------------
float Delay::getMaxFeedback(){
    return maxFeedback;
}


//-------------------------------------------------------------------------------------------------------
void Delay::createDelayLines()
{
    float delayLineInSec = 1; //1 seconds of audio
    float numberOfSamples = delayLineInSec*sampleRate;
    float numberOfBytes = numberOfSamples*sizeof(float);
    
    bufferDelayL = BufferFactory::createBuffer(numberOfBytes);
    bufferDelayR = BufferFactory::createBuffer(numberOfBytes);
    
    delayCursorL = 0;
    delayCursorR = 0;
    delayMaxSize = numberOfSamples;
    delayCurrentSizeL = delayMaxSize/2;
    delayCurrentSizeR = delayMaxSize/2;
}

//-------------------------------------------------------------------------------------------------------
void Delay::deleteDelayLines(){
    BufferFactory::deleteBuffer(bufferDelayL);
    BufferFactory::deleteBuffer(bufferDelayR);
}

//-----------------------------------------------------------------------------------------
void Delay::processDelay(float** inputs, float** outputs, VstInt32 sampleFrames)
{
    // PROCESS SINGLE PRECISION - processa audio
    
    float *buffL = inputs[0]; // buffer input left
    float *buffR = inputs[1]; // buffer input right
    
    float *buffOutL = outputs[0]; // buffer output left
    float *buffOutR = outputs[1]; // buffer output right
    
    float wetDryBalance;
    
    for(int i=0; i<sampleFrames;i++){
        
        float oldestSampleL = bufferDelayL[delayCursorL];
        float oldestSampleR = bufferDelayR[delayCursorR];
        
        
        bufferDelayL[delayCursorL] = buffL[i]+oldestSampleR*delFeedbackL;
        bufferDelayR[delayCursorR] = buffR[i]+oldestSampleL*delFeedbackR;
        
        delayCursorL++;
        delayCursorR++;
        
        if (delayCursorL >= delayCurrentSizeL){
            delayCursorL = 0;
        }
        
        if (delayCursorR >= delayCurrentSizeR){
            delayCursorR = 0;
        }

        wetDryBalance = wetDry*buffL[i]+(1-wetDry)*oldestSampleL;
        gainStereo.processGainL(&wetDryBalance);
        buffOutL[i] = wetDryBalance;
        
        wetDryBalance = wetDry*buffR[i]+(1-wetDry)*oldestSampleR;
        gainStereo.processGainR(&wetDryBalance);
        buffOutR[i] = wetDryBalance;

        
    }
    
}
