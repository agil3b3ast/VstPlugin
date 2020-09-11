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
    frequencyInHz = 0.1;
    this->sampleRate = sampleRate;
    fScale = WAVETABLE_SIZE/sampleRate;
    createWavetables();
    createFrequencyTable();
    stepValue = frequencyInHz*fScale;

    currentWavetable = sine;
    
}

//-------------------------------------------------------------------------------------------------------
Oscillator::~Oscillator(){
    deleteWavetables();
}

double Oscillator::getFrequencyInHz(){
    return frequencyInHz;
}

//-------------------------------------------------------------------------------------------------------
void Oscillator::setFrequencyInHz(double freq){
    frequencyInHz = freq;
    stepValue = frequencyInHz*fScale;
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
    double numberOfBytes = WAVETABLE_SIZE*sizeof(double);
    
    sawtooth = BufferFactory::createBufferDouble(numberOfBytes);
    pulse = BufferFactory::createBufferDouble(numberOfBytes);
    sine = BufferFactory::createBufferDouble(numberOfBytes);

    
    
    for(int i=0; i<WAVETABLE_SIZE; i++){
        float max_size = pwm*WAVETABLE_SIZE;
        if (i<max_size)
            pulse[i] = 0;
        else
            pulse[i] = 1;
        
        sawtooth[i] = 2.0*((double)i/(double) WAVETABLE_SIZE) - 1.0;
        
        sine[i] = sin(2.0*pi*(double)i/(double) WAVETABLE_SIZE);

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
    
    for(int i=0; i<sampleFrames;i++){
        processOscillatorSingle(&outL[i]);
        processOscillatorSingle(&outR[i]);
        updateCursorTable();
    }
}

//-------------------------------------------------------------------------------------------------------
void Oscillator::processOscillatorSingle(float *input){
    
        if (input == nullptr){
            std::cerr << "An input must be provided!\n";
            return;
        }
        genSignal(input);
}

//-------------------------------------------------------------------------------------------------------
void Oscillator::processOscillatorSingleDouble(double *input){
    
    if (input == nullptr){
        std::cerr << "An input must be provided!\n";
        return;
    }
    genSignalDoubleWithInterp(input);
}

//-------------------------------------------------------------------------------------------------------
void Oscillator::genSignal(float* output){
    *output = currentWavetable[(int) cursorTable];
}

//-------------------------------------------------------------------------------------------------------
void Oscillator::genSignalDouble(double* output){
    *output = currentWavetable[(int) cursorTable];
}

//-------------------------------------------------------------------------------------------------------
void Oscillator::genSignalDoubleWithInterp(double* output){
    int next = ceil(cursorTable);
    int previous = floor(cursorTable);
    
    if(previous < 0){ //realign previous
        previous = WAVETABLE_SIZE -1;
    }
    if(next == WAVETABLE_SIZE){ //realign next
        next = 0;
    }
    *output = NU*currentWavetable[previous] + (1.0-NU)*currentWavetable[next];
}

//-------------------------------------------------------------------------------------------------------
void Oscillator::updateCursorTable(){
    cursorTable = cursorTable + stepValue;
    if (cursorTable > (WAVETABLE_SIZE-1))
        cursorTable = cursorTable - WAVETABLE_SIZE;
}
