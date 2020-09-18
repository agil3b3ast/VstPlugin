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
    maxAmount = 1.0;
    minAmount = 0.0;
    coeff = (maxAmount-minAmount)/2.0;
}

//-------------------------------------------------------------------------------------------------------

//getters
Oscillator *ModOperator::getOscillator(){
    return oscillator;
}

//-------------------------------------------------------------------------------------------------------

double ModOperator::getMinAmount(){
    return minAmount;
}

//-------------------------------------------------------------------------------------------------------

double ModOperator::getMaxAmount(){
    return maxAmount;
}

//-------------------------------------------------------------------------------------------------------

//setters
void ModOperator::setMaxAmount(double maxAmount){
    if (maxAmount < minAmount){
        this->maxAmount = minAmount;
    }
    else{
        this->maxAmount = maxAmount;
    }
}

//-------------------------------------------------------------------------------------------------------

void ModOperator::setMinAmount(double minAmount){
    if (minAmount > maxAmount){
        this->minAmount = maxAmount;
    }
    else{
        this->minAmount = minAmount;
    }
}

//-------------------------------------------------------------------------------------------------------

void ModOperator::setOscillator(Oscillator *oscillator){
    this->oscillator = oscillator;
}

//-------------------------------------------------------------------------------------------------------

void ModOperator::updateModOperator(){
    oscillator->processOscillatorSingleDouble(&currentSignal);
    oscillator->updateCursorTable();
}

//-------------------------------------------------------------------------------------------------------


