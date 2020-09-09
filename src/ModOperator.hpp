//
//  ModOperator.hpp
//  VstPlugin
//
//  Created by Alessandro Fazio on 09/09/2020.
//

#ifndef ModOperator_hpp
#define ModOperator_hpp

#include <iostream>
#include "Oscillator.hpp"

class ModOperator{
    
    Oscillator *oscillator;
    float currentSignal;
    
    float amount; //vale da 0 a 1
    int maxAmount;
    
public:
    explicit ModOperator(Oscillator *oscillator);
    
    void updateModOperator();
    
    //getters
    Oscillator *getOscillator();
    //setters
    void setOscillator(Oscillator *oscillator);
    void setMaxAmount(int maxAmount);
    
    //templates
    template <typename inputToMod> void processModOperator(inputToMod input);
    
    
};


#endif /* ModOperator_hpp */
