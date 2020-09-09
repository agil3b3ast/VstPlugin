//
//  ModOperator.cpp
//  VstPlugin
//
//  Created by Alessandro Fazio on 09/09/2020.
//

#include "ModOperator.hpp"

ModOperator::ModOperator(Oscillator *oscillator){
    this->oscillator = oscillator;
    currentSignal = 0.0;
    amount = 1.0;
    maxAmount = 1;
}

//getters
Oscillator *ModOperator::getOscillator(){
    return oscillator;
}
//setters
//-------------------------------------------------------------------------------------------------------
void ModOperator::setMaxAmount(int maxAmount){
    this->maxAmount = maxAmount;
}

//-------------------------------------------------------------------------------------------------------

void ModOperator::setOscillator(Oscillator *oscillator){
    this->oscillator = oscillator;
}

//-------------------------------------------------------------------------------------------------------

void ModOperator::updateModOperator(){
    oscillator->processOscillatorSingle(&currentSignal);
    oscillator->updateCursorTable();
}

//-------------------------------------------------------------------------------------------------------

template <typename inputToMod> void ModOperator::processModOperator(inputToMod input){
    
    if (input == nullptr){
        std::cerr << "An input must be provided!\n";
        return;
    }

    *input = *input * currentSignal * amount * maxAmount; //mod a signal or parameter (e.g. delay size)

    
}
