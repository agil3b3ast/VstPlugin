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
    amount = 0.0;
    coeff = (maxAmount-minAmount)/2.0;
}

ModOperator::~ModOperator(){
//    if(oscillator != nullptr){
//        delete oscillator;
//        oscillator = nullptr;
//    }
}


//-------------------------------------------------------------------------------------------------------

//getters
Oscillator *ModOperator::getOscillator(){
    return oscillator;
}

//-------------------------------------------------------------------------------------------------------

double ModOperator::getAmount(){
    return amount;
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

void ModOperator::setAmount(double amount){
    this->amount = amount;
}

//-------------------------------------------------------------------------------------------------------

//setters
void ModOperator::setMaxAmount(double maxAmount){
    this->maxAmount = maxAmount;
}

//-------------------------------------------------------------------------------------------------------

void ModOperator::setMinAmount(double minAmount){
    this->minAmount = minAmount;
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


