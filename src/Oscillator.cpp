//
//  Oscillator.cpp
//  VstPlugin
//
//  Created by Alessandro Fazio on 07/09/2020.
//

#include "Oscillator.hpp"


Oscillator::Oscillator(float sampleRate){
    pwm = 0.3;
    cursorTable = 0;
    frequencyInHz = 1.0;
    this->sampleRate = sampleRate;
    fScale = WAVETABLE_SIZE/sampleRate;
    createWavetables();
    createFrequencyTable();
    stepValue = frequencyInHz*fScale;
    amount = 1.0;
    maxAmount = 1;

    currentWavetable = sine;
    
}

//-------------------------------------------------------------------------------------------------------
Oscillator::~Oscillator(){
    deleteWavetables();
}

//-------------------------------------------------------------------------------------------------------
void Oscillator::setMaxAmount(int maxAmount){
    this->maxAmount = maxAmount;
}

//-------------------------------------------------------------------------------------------------------
void Oscillator::setCurrentWavetable(unsigned char currentWavetable){
    switch (currentWavetable) {
        case Sine:
            this->currentWavetable = sine;
            break;
        case Saw:
            this->currentWavetable = sawtooth;
            break;
        case Pulse:
            this->currentWavetable = pulse;
            break;
        default:
            break;
    }
}

//-------------------------------------------------------------------------------------------------------
void Oscillator::createFrequencyTable()
{
    const double k = 1.059463094359;
    double a = 6.875; // A
    a *= k; // Bb
    a *= k; // B
    a *= k; // C
    
    for (int i = 0; i<128; i++){
        freqTable[i] = a;
        a *= k; //next frequency note
    }
    
}

//-------------------------------------------------------------------------------------------------------
void Oscillator::createWavetables()
{
    float numberOfBytes = WAVETABLE_SIZE*sizeof(float);
    
    sawtooth = BufferFactory::createBuffer(numberOfBytes);
    pulse = BufferFactory::createBuffer(numberOfBytes);
    sine = BufferFactory::createBuffer(numberOfBytes);

    
    
    for(int i=0; i<WAVETABLE_SIZE; i++){
        float max_size = pwm*WAVETABLE_SIZE;
        if (i<max_size)
            pulse[i] = 0;
        else
            pulse[i] = 1;
        
        sawtooth[i] = 2*((float)i/(float) WAVETABLE_SIZE) - 1;
        
        sine[i] = sin(2*pi*(float)i/(float) WAVETABLE_SIZE);

    }
    
}

//-------------------------------------------------------------------------------------------------------
void Oscillator::deleteWavetables(){
    if (sawtooth != nullptr) {
        delete sawtooth;
        sawtooth = nullptr;
    }
    
    if (pulse != nullptr) {
        delete pulse;
        pulse = nullptr;
    }
    
    if (sine != nullptr) {
        delete sine;
        sine = nullptr;
    }
}

//-------------------------------------------------------------------------------------------------------
void Oscillator::processOscillator(float** outputs, unsigned char feature, VstInt32 sampleFrames){
    float *outL = outputs[0]; // buffer output left
    float *outR = outputs[1]; // buffer output right
    
    for(int i=0; i<sampleFrames;i++){
        processOscillatorSingle(&outL[i], feature, sampleFrames);
        processOscillatorSingle(&outR[i], feature, sampleFrames);
    }

}

//-------------------------------------------------------------------------------------------------------
template <typename inputToMod> void Oscillator::processOscillatorSingle(inputToMod input, unsigned char feature, VstInt32 sampleFrames){

        if (input == nullptr){
            std::cerr << "An input must be provided!\n";
            return;
        }

        switch (feature) {
            case Gen:
                genSignal(input, currentWavetable);
                break;
            //TODO add to modulator
            /*case RingMod:
                ringModSignal(input, currentWavetable);
                break;
            case NumMod:
                numMod(input, currentWavetable);
                break;*/
            default:
                break;
        }
    
}

//-------------------------------------------------------------------------------------------------------
void Oscillator::genSignal(float* output, float *wavetable){
    *output = wavetable[(int) cursorTable];
    incrementCursorTable();
}

//-------------------------------------------------------------------------------------------------------
//TODO add to modulator
/*void Oscillator::ringModSignal(float *sample, float *wavetable){
    *sample = *sample * wavetable[(int) cursorTable] * amount; //ring mod
    incrementCursorTable();
}

//-------------------------------------------------------------------------------------------------------
void Oscillator::numMod(int *number, float *wavetable){
    *number = *number * wavetable[(int) cursorTable] * amount * maxAmount; //mod a numeric parameter
    incrementCursorTable();
}*/

//-------------------------------------------------------------------------------------------------------
void Oscillator::incrementCursorTable(){
    cursorTable = cursorTable + stepValue;
    if (cursorTable > (WAVETABLE_SIZE-1))
        cursorTable = cursorTable - WAVETABLE_SIZE;
}
