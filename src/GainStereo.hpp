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
    float getGainL() const;
    float getGainR() const;
    
    //setters
    void setGainL(float gainL);
    void setGainR(float gainR);
    
    //process
    void processGainL(float *sample) const;
    void processGainR(float *sample) const;
    
};
//-------------------------------------------------------------------------------------------------------

inline GainStereo::GainStereo(){
    gainL = 1.0;
    gainR = 1.0;
}

//-------------------------------------------------------------------------------------------------------

inline float GainStereo::getGainL() const{
    return gainL;
}

//-------------------------------------------------------------------------------------------------------

inline float GainStereo::getGainR() const{
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

inline void GainStereo::processGainL(float *sample) const{
    *sample = *sample * this->gainL;
}
//-------------------------------------------------------------------------------------------------------

inline void GainStereo::processGainR(float *sample) const{
    *sample = *sample * this->gainR;
}
#endif /* GainStereo_hpp */
