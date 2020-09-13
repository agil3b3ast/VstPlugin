//
//  Chorus.cpp
//  VstPlugin
//
//  Created by Alessandro Fazio on 13/09/2020.
//

#include "Chorus.hpp"

Chorus::Chorus(float sampleRate, VDelay *vd1, VDelay *vd2): gainStereo(){
    wetDry = 0.0;
    vdelay1 = vd1;
    vdelay2 = vd2;
}

//-------------------------------------------------------------------------------------------------------

double getMaxAmount(){
    return vdelay1->getMaxAmount();
;
}

//-------------------------------------------------------------------------------------------------------

double getMinAmount(){
    return vdelay1->getMinAmount();
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

void Chorus::setWetDry(float wetDry){
    this->wetDry = wetDry;
}

//-------------------------------------------------------------------------------------------------------

void setMaxAmount(double maxAmount){
    vdelay1->setMaxAmount(maxAmount);
    vdelay2->setMaxAmount(maxAmount);
    vdelay3->setMaxAmount(maxAmount);
}
//-------------------------------------------------------------------------------------------------------

void setMinAmount(double minAmount){
    this->minAmount = minAmount;
    vdelay1->setMinAmount(minAmount);
    vdelay2->setMinAmount(minAmount);
    vdelay3->setMinAmount(minAmount);
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

void Chorus::processChorus(float** inputs, float** outputs, VstInt32 sampleFrames){
    float *buffL = inputs[0]; // buffer input left
    float *buffR = inputs[1]; // buffer input right

    float *buffOutL = outputs[0]; // buffer output left
    float *buffOutR = outputs[1]; // buffer output right

    float wetDryBalance;

    float in1L,in1R,in2L,in2R = 0.0;

    //std::fstream fout;
    //std::fstream fout2;
    //fout.open("/Users/alessandro_fazio/Desktop/output_in.csv", std::ios::out | std::ios::app);
    //fout2.open("/Users/alessandro_fazio/Desktop/output_out.csv", std::ios::out | std::ios::app);

    for(int i=0; i<sampleFrames;i++){
        in1L = buffL[i];
        in2L = in1L;
        in1R = buffR[i];
        in2R = in1R;
        vdelay1->tick(&in1L, &in1R);
        vdelay2->tick(&in2L, &in2R);

        wetDryBalance = (1.0-wetDry)*(in1L+in2L)+wetDry*(vdelay1->getOldestSampleL()+vdelay2->getOldestSampleL());
        gainStereo.processGainL(&wetDryBalance);
        buffOutL[i] = wetDryBalance;

        wetDryBalance = (1.0-wetDry)*(in1R+in2R)+wetDry*(vdelay1->getOldestSampleR()+vdelay2->getOldestSampleR());
        gainStereo.processGainR(&wetDryBalance);
        buffOutR[i] = wetDryBalance;

        //fout << std::to_string(buffL[i]) << '\n';
        //fout2 << std::to_string(buffOutL[i]) << '\n';


    }

    //fout.close();
    //fout2.close();

}
