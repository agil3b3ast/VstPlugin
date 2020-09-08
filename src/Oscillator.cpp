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
}

//-------------------------------------------------------------------------------------------------------
Oscillator::~Oscillator(){
    deleteWavetables();
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
void Oscillator::processOscillator(float** outputs, VstInt32 sampleFrames){
    float *outL = outputs[0]; // buffer output left
    float *outR = outputs[1]; // buffer output right

    double stepValue = frequencyInHz*fScale;
    
    for(int i=0; i<sampleFrames;i++){

        genSignal(&outL[i], sawtooth);
        genSignal(&outR[i], pulse);

        cursorTable = cursorTable + stepValue;
        if (cursorTable > (WAVETABLE_SIZE-1))
            cursorTable = cursorTable - WAVETABLE_SIZE;
    }

}

//-------------------------------------------------------------------------------------------------------
void Oscillator::genSignal(float* sample, float *wavetable){
    *sample = wavetable[(int) cursorTable];
}


