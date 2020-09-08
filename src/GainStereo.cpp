//
//  Gain.cpp
//  VstPlugin
//
//  Created by Alessandro Fazio on 08/09/2020.
//

#include "GainStereo.hpp"

GainStereo::GainStereo(){
    gainL = 1.0;
    gainR = 1.0;
}

//-------------------------------------------------------------------------------------------------------

float GainStereo::getGainL(){
    return gainL;
}

//-------------------------------------------------------------------------------------------------------

float GainStereo::getGainR(){
    return gainR;
}

//-------------------------------------------------------------------------------------------------------

void GainStereo::setGainL(float gainL){
    this->gainL = gainL;
}

//-------------------------------------------------------------------------------------------------------

void GainStereo::setGainR(float gainR){
    this->gainR = gainR;
}

void GainStereo::processGainL(float *sample){
    *sample = *sample * this->gainL;
}

void GainStereo::processGainR(float *sample){
    *sample = *sample * this->gainR;
}
