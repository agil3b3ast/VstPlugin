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
#include <fstream>

class ModOperator{
    
    Oscillator *oscillator;
    double currentSignal;
    
    double maxAmount; //vale da 0 a 1
    double minAmount;
    double amount;
    double coeff;
    
public:
    explicit ModOperator(Oscillator *oscillator);
    ~ModOperator();
    
    void updateModOperator();
    
    //getters
    Oscillator *getOscillator();
    double getMaxAmount();
    double getMinAmount();
    double getAmount();
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
        //std::fstream fout2;
        //fout2.open("/Users/alessandro_fazio/Desktop/output_out.csv", std::ios::out | std::ios::app);

        *output = minAmount + coeff + (currentSignal * amount * coeff); //mod a signal or parameter (e.g. delay size)
        //fout2 << std::to_string(*output) << '\n';

        //fout2.close();
    }
    
    
};


#endif /* ModOperator_hpp */
