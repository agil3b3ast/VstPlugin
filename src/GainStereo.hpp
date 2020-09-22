//
//  GainStereo.hpp
//  VstPlugin
//
//  Created by Alessandro Fazio on 08/09/2020.
//

#ifndef GainStereo_hpp
#define GainStereo_hpp

#include <stdio.h>

class GainStereo{
    float gainL;
    float gainR;
    
public:
    explicit GainStereo();
    
    //getters
    float getGainL();
    float getGainR();
    
    //setters
    void setGainL(float gainL);
    void setGainR(float gainR);
    
    //process
    void processGainL(float *sample);
    void processGainR(float *sample);
    
};
//-------------------------------------------------------------------------------------------------------

inline GainStereo::GainStereo(){
    gainL = 1.0;
    gainR = 1.0;
}

//-------------------------------------------------------------------------------------------------------

inline float GainStereo::getGainL(){
    return gainL;
}

//-------------------------------------------------------------------------------------------------------

inline float GainStereo::getGainR(){
    return gainR;
}

//-------------------------------------------------------------------------------------------------------

inline void GainStereo::setGainL(float gainL){
    this->gainL = gainL;
}

//-------------------------------------------------------------------------------------------------------

inline void GainStereo::setGainR(float gainR){
    this->gainR = gainR;
}
//-------------------------------------------------------------------------------------------------------

inline void GainStereo::processGainL(float *sample){
    *sample = *sample * this->gainL;
}
//-------------------------------------------------------------------------------------------------------

inline void GainStereo::processGainR(float *sample){
    *sample = *sample * this->gainR;
}
#endif /* GainStereo_hpp */
