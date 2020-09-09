//
//  VDelay.hpp
//  VstPlugin
//
//  Created by Alessandro Fazio on 09/09/2020.
//

#ifndef VDelay_hpp
#define VDelay_hpp

#include <stdio.h>
#include "Delay.hpp"
#include "Oscillator.hpp"
#include "ModOperator.hpp"

class VDelay : public Delay{
    Oscillator oscillator;
    ModOperator modOperator;
    
    public:
        explicit VDelay(float sampleRate);
        void processDelay(float** inputs, float** outputs, VstInt32 sampleFrames) override;
};

#endif /* VDelay_hpp */
