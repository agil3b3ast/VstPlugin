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
    float minAmount;
    
public:
    explicit ModOperator(Oscillator *oscillator);
    
    void updateModOperator();
    
    //getters
    Oscillator *getOscillator();
    //setters
    void setOscillator(Oscillator *oscillator);
    void setMaxAmount(int maxAmount);
    void setMinAmount(float minAmount);
    
    //templates
    template <typename inputToMod, typename outputToMod> void processModOperator(inputToMod input, outputToMod *output){
        
        if (input == nullptr){
            std::cerr << "An input must be provided!\n";
            return;
        }
        *output = minAmount + *input * currentSignal * amount * maxAmount; //mod a signal or parameter (e.g. delay size)
    }
    
    
};


#endif /* ModOperator_hpp */
