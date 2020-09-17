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
    minAmount = 0.0;
}

//getters
Oscillator *ModOperator::getOscillator(){
    return oscillator;
}

double ModOperator::getAmount(){
    return amount;
}

//setters
void ModOperator::setAmount(double amount){
    this->amount = amount;
}

//-------------------------------------------------------------------------------------------------------

void ModOperator::setMinAmount(double minAmount){
    this->minAmount = minAmount;
}

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
    oscillator->processOscillatorSingleDouble(&currentSignal);
    oscillator->updateCursorTable();
}

//-------------------------------------------------------------------------------------------------------


