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
    float *oldestSamples;
    int channels;
    float *previousOuts;
    
    public:
        inline Interp(int channels){
            previousOuts = BufferFactory::createBuffer(channels*sizeof(float));
        }
    
        inline void setMax(int max){
            this->max = max;
        }
    
        //templates
        template <typename toInterp> void setInterp(toInterp tI, bool circular=true){
            previous = floor(tI);
            next = ceil(tI);
            
            int_part = previous;
            fract_part = tI-int_part;
            
            //if(previous < 0){ //realign previous
            //    previous = delayMaxSize -1;
            //}
            if(circular and next == max){ //realign next
                next = 0;
            }
            
        }
    
        //templates
        template <typename toInterp> void interpLinear(toInterp *oldestSample, int channelIndex=0, toInterp *buffer=nullptr){
            if (buffer != nullptr)
                *oldestSample = buffer[previous] + (fract_part*(buffer[next]-buffer[previous]));
            else
                *oldestSample = previous + fract_part*(next-previous); //returns original sample
            //*oldestSampleR = bufferDelayR[previous] + (fract_part*(bufferDelayR[next]-bufferDelayR[previous]));
        }
    
        //templates
        template <typename toInterp> void interpAllPass(toInterp *oldestSample, int channelIndex=0, toInterp *buffer=nullptr){
            
            if (buffer != nullptr)
                *oldestSample = buffer[previous] + (fract_part*(buffer[next]-previousOuts[channelIndex]));// - (1-fract_part)*previousOutL;
            else
                *oldestSample = previous + fract_part*(next-previousOuts[channelIndex]);// - (1-fract_part)*previousOutL;
            //*oldestSampleR = bufferDelayR[previous] + (fract_part*(bufferDelayR[next]-previousOutR));// - (1-fract_part)*previousOutR;
            
            previousOuts[channelIndex] = *oldestSample; //all-pass interp
            //previousOutR = *oldestSampleR; //all-pass interp
        }
};

#endif /* Interp_h */
