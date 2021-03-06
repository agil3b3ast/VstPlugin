//
//  Chorus.cpp
//  VstPlugin
//
//  Created by Alessandro Fazio on 13/09/2020.
//

#include "Chorus.hpp"

Chorus::Chorus(float sampleRate): gainStereo(){
    wetDry = 0.0;
    vdelay1 = new VDelay(sampleRate);
    vdelay2 = new VDelay(sampleRate);
    vdelay3 = new VDelay(sampleRate);
    
    vdelay1->setPhase(0.25);
    vdelay3->setPhase(0.75);
    
    
}
//-----------------------------------------------------------------------------------------

Chorus::~Chorus(){
    if(vdelay1 != nullptr){
        delete vdelay1;
        vdelay1 = nullptr;
    }
    else if(vdelay2 != nullptr){
        delete vdelay2;
        vdelay2 = nullptr;
    }
    else if(vdelay3 != nullptr){
        delete vdelay3;
        vdelay3 = nullptr;
    }
}

//-------------------------------------------------------------------------------------------------------
float Chorus::getWetDry(){
    return wetDry;
}

//-------------------------------------------------------------------------------------------------------

VDelay *Chorus::getDelay1(){
    return vdelay1;
}

//-------------------------------------------------------------------------------------------------------

VDelay *Chorus::getDelay2(){
    return vdelay2;
}

//-------------------------------------------------------------------------------------------------------

VDelay *Chorus::getDelay3(){
    return vdelay3;
}

//-------------------------------------------------------------------------------------------------------

GainStereo Chorus::getGain(){
    return gainStereo;
}

//-------------------------------------------------------------------------------------------------------


double Chorus::getFrequencyInHz1(){
    return vdelay1->getFrequencyInHz();
}

//-------------------------------------------------------------------------------------------------------

double Chorus::getFrequencyInHz2(){
    return vdelay2->getFrequencyInHz();
}

//-------------------------------------------------------------------------------------------------------

double Chorus::getFrequencyInHz3(){
    return vdelay3->getFrequencyInHz();
}

//-------------------------------------------------------------------------------------------------------

void Chorus::setWetDry(float wetDry){
    this->wetDry = wetDry;
}

//-------------------------------------------------------------------------------------------------------

void Chorus::setGain(float gainL, float gainR){
    gainStereo.setGainL(gainL);
    gainStereo.setGainR(gainR);
}

//-------------------------------------------------------------------------------------------------------

void Chorus::setFrequencyInHz1(double frequencyInHz){
    vdelay1->setFrequencyInHz(frequencyInHz);
}

//-------------------------------------------------------------------------------------------------------

void Chorus::setFrequencyInHz2(double frequencyInHz){
    vdelay2->setFrequencyInHz(frequencyInHz);
}

//-------------------------------------------------------------------------------------------------------

void Chorus::setFrequencyInHz3(double frequencyInHz){
    vdelay3->setFrequencyInHz(frequencyInHz);
}

//-------------------------------------------------------------------------------------------------------

void Chorus::processChorus(float** inputs, float** outputs, VstInt32 sampleFrames){
    float *buffL = inputs[0]; // buffer input left
    float *buffR = inputs[1]; // buffer input right
    
    float *buffOutL = outputs[0]; // buffer output left
    float *buffOutR = outputs[1]; // buffer output right
    
    
    for(int i=0; i<sampleFrames;i++){
        processChorusBySample(&buffL[i],&buffR[i]);
        buffOutL[i] = buffL[i];
        buffOutR[i] = buffR[i];
    }
    
    
}

//-------------------------------------------------------------------------------------------------------

void Chorus::processChorusBySample(float* inputL, float* inputR){
    
    float wetDryBalanceL;
    float wetDryBalanceR;
    
    float chorusOutL;
    float chorusOutR;
    
    float in1L,in1R,in2L,in2R,in3L,in3R= 0.0;
    
    in1L = *inputL;
    in2L = in1L;
    in3L = in1L;

    in1R = *inputR;
    in2R = in1R;
    in3R = in1R;

    vdelay1->tick(&in1L, &in1R);
    
    float inSum = 0.5*in2L+0.5*in2R;
    
    vdelay2->tick(&(inSum), &(inSum));
    
    vdelay3->tick(&in3L, &in3R);
    
    
    chorusOutL = 0.5*vdelay1->getOldestSampleL() + 0.5*vdelay2->getOldestSampleL();
    
    wetDryBalanceL = (1.0-wetDry)*in1L + wetDry*(chorusOutL);
    gainStereo.processGainL(&wetDryBalanceL);
    *inputL = wetDryBalanceL;
    
    
    chorusOutR = 0.5*vdelay3->getOldestSampleR() + 0.5*vdelay2->getOldestSampleR();
    
    wetDryBalanceR = (1.0-wetDry)*in1R + wetDry*(chorusOutR);
    gainStereo.processGainR(&wetDryBalanceR);
    *inputR = wetDryBalanceR;
    
}
