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
    double currentSignal;
    
    double maxAmount; //vale da 0 a 1
    double minAmount;
    double coeff;
    
public:
    explicit ModOperator(Oscillator *oscillator);
    
    void updateModOperator();
    
    //getters
    Oscillator *getOscillator();
    double getMaxAmount();
    double getMinAmount();
    //setters
    void setOscillator(Oscillator *oscillator);
    void setMaxAmount(double maxAmount);
    void setMinAmount(double minAmount);
    void setAmount(double amount);
    
    //templates
    template <typename outputToMod> void processModOperator(outputToMod *output){
        
        if (output == nullptr){
            std::cerr << "An input must be provided!\n";
            return;
        }
        coeff = (maxAmount-minAmount)/2.0;
        *output = minAmount + coeff + (currentSignal * coeff); //mod a signal or parameter (e.g. delay size)
    }
    
    
};


#endif /* ModOperator_hpp */
