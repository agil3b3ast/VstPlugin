//
//  AutoPan.hpp
//  VstPlugin
//
//  Created by Alessandro Fazio on 18/09/2020.
//

#ifndef AutoPan_h
#define AutoPan_h

#include "ModOperator.hpp"

class AutoPan{
    double amount;
    
    double outCurr;
    
    double freq;
    
    const double MAX_FREQ = 10.0;
    const double MIN_FREQ = 0.1;
    
    ModOperator modOperator;
    Oscillator oscillator;
    
    public:
    inline AutoPan(float sampleRate) : oscillator(sampleRate), modOperator(&oscillator){
        amount = 1.0;
        outCurr = 0.0;
        freq = 1.0;
        
        modOperator.setMinAmount(0.0);
        modOperator.setMaxAmount(1.0);
        modOperator.setAmount(1.0);
    }
    
    inline void setSampleRate(float sampleRate){
        modOperator.getOscillator()->setSampleRate(sampleRate);
    }
    
    inline void setFreq(double freq){
        modOperator.getOscillator()->setFrequencyInHz(freq);
    }
    
    inline double getFreq(){
        return modOperator.getOscillator()->getFrequencyInHz();
    }
    
    inline double getMaxFreq(){
        return MAX_FREQ;
    }
    
    inline double getMinFreq(){
        return MIN_FREQ;
    }
    
    inline void setAmount(double amount){
        this->amount = amount;
    }
    
    inline double getAmount(){
        return amount;
    }
    
    //templates
    template <typename outL,typename outR> void processAutoPan(outL *left, outR *right){
        
        modOperator.updateModOperator();
        modOperator.processModOperator(&outCurr);
                
        *left = *left*amount + *left*outCurr*(1.0-amount);
        *right = *right*amount + *right*(1.0-outCurr)*(1.0-amount);
        
    }
};

#endif /* AutoPan_h */
