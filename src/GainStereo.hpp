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

#endif /* GainStereo_hpp */
