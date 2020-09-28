//
//  Interp.hpp
//  VstPlugin
//
//  Created by Alessandro Fazio on 19/09/2020.
//

#ifndef Interp_h
#define Interp_h
#include "BufferFactory.hpp"

class Interp{
    int previous;
    int next;
    int int_part;
    double fract_part;
    int max;
    float *previousOuts;
    
    public:
        explicit Interp(int channels);
        void setMax(int max);
    
//-------------------------------------------------------------------------------------------------------
        template <typename toInterp> void setInterp(toInterp tI, bool circular=true){
            previous = floor(tI);
            next = ceil(tI);
            
            int_part = previous;
            fract_part = tI-int_part;
            
            if(circular and next == max){ //realign next
                next = 0;
            }
            
        }
    
//-------------------------------------------------------------------------------------------------------
        template <typename toInterp> void interpLinear(toInterp *oldestSample, toInterp *buffer=nullptr) const{
            if (buffer != nullptr)
                *oldestSample = buffer[previous] + (fract_part*(buffer[next]-buffer[previous]));
            else
                *oldestSample = previous + fract_part*(next-previous); //returns original sample
        }
    
//-------------------------------------------------------------------------------------------------------
        template <typename toInterp> void interpAllPass(toInterp *oldestSample, int channelIndex=0, toInterp *buffer=nullptr){
            
            if (buffer != nullptr)
                *oldestSample = buffer[previous] + (fract_part*(buffer[next]-previousOuts[channelIndex]));
            else
                *oldestSample = previous + fract_part*(next-previousOuts[channelIndex]);
            
            previousOuts[channelIndex] = *oldestSample; //all-pass interp
        }
};

//-------------------------------------------------------------------------------------------------------
inline Interp::Interp(int channels){
    previousOuts = BufferFactory::createBuffer((unsigned int) channels*sizeof(float));
    previous = 0;
    next = 0;
    int_part = 0;
    fract_part = 0.0;
    max = 0;
}
//-------------------------------------------------------------------------------------------------------
inline void Interp::setMax(int max){
    this->max = max;
}

#endif /* Interp_h */
