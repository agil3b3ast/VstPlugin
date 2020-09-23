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
    
    double maxAmount;
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
    
    //templates, only gen
    template <typename outputMod> void processModOperator(outputMod *output){
        
        if (output == nullptr){
            std::cerr << "An input must be provided!\n";
            return;
        }
        coeff = (maxAmount-minAmount)/2.0;
        //std::fstream fout2;
        //fout2.open("~/Desktop/output_out.csv", std::ios::out | std::ios::app);

        *output = minAmount + coeff + (currentSignal * amount * coeff); //mod a signal or parameter (e.g. delay size)
        //fout2 << std::to_string(*output) << '\n';

        //fout2.close();
    }
    
    //templates, mod signal
    template <typename outputMod, typename inputMod> void processModOperator(outputMod *output, inputMod *input){
        if (output == nullptr or input ==nullptr){
            std::cerr << "An output/input must be provided!\n";
            return;
        }
        
        processModOperator(output);
        *output = *output * *input;
    }
    
};


#endif /* ModOperator_hpp */
