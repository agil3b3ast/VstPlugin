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
    explicit AutoPan(float sampleRate);
    void setSampleRate(float sampleRate);
    void setFreq(double freq);
    double getFreq();
    double getMaxFreq();
    double getMinFreq();
    void setAmount(double amount);
    double getAmount();

    //templates
    template <typename outL,typename outR> void processAutoPan(outL *left, outR *right){
        
        modOperator.updateModOperator();
        modOperator.processModOperator(&outCurr);
        
        *left = *left*amount + *left*outCurr*(1.0-amount);
        *right = *right*amount + *right*(1.0-outCurr)*(1.0-amount);
        
    }
};
//-----------------------------------------------------------------------------------------

inline AutoPan::AutoPan(float sampleRate) : oscillator(sampleRate), modOperator(&oscillator){
    amount = 1.0;
    outCurr = 0.0;
    freq = 1.0;
    
    modOperator.setMinAmount(0.0);
    modOperator.setMaxAmount(1.0);
    modOperator.setAmount(1.0);
}
//-----------------------------------------------------------------------------------------

inline void AutoPan::setSampleRate(float sampleRate){
    modOperator.getOscillator()->setSampleRate(sampleRate);
}
//-----------------------------------------------------------------------------------------

inline void AutoPan::setFreq(double freq){
    modOperator.getOscillator()->setFrequencyInHz(freq);
}
//-----------------------------------------------------------------------------------------

inline double AutoPan::getFreq(){
    return modOperator.getOscillator()->getFrequencyInHz();
}
//-----------------------------------------------------------------------------------------

inline double AutoPan::getMaxFreq(){
    return MAX_FREQ;
}
//-----------------------------------------------------------------------------------------

inline double AutoPan::getMinFreq(){
    return MIN_FREQ;
}
//-----------------------------------------------------------------------------------------

inline void AutoPan::setAmount(double amount){
    this->amount = amount;
}
//-----------------------------------------------------------------------------------------

inline double AutoPan::getAmount(){
    return amount;
}

#endif /* AutoPan_h */
