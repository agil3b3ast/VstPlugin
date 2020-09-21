//
//  Oscillator.cpp
//  VstPlugin
//
//  Created by Alessandro Fazio on 07/09/2020.
//

#include "Oscillator.hpp"


Oscillator::Oscillator(float sampleRate){
    pwm = 0.3;
    cursorTable = 0.0;

    frequencyInHz = 0.0;
    minFreq = 0.1;
    maxFreq = 10.0;

    phase = 0.0; //0 degrees
    this->sampleRate = sampleRate;
    fScale = WAVETABLE_SIZE/sampleRate;
    createWavetables();
    stepValue = frequencyInHz*fScale;

    currentWavetable = sine;

}

//-------------------------------------------------------------------------------------------------------
Oscillator::~Oscillator(){
    deleteWavetables();
}

//-------------------------------------------------------------------------------------------------------

double Oscillator::getMaxFreq(){
    return maxFreq;
}

//-------------------------------------------------------------------------------------------------------

double Oscillator::getMinFreq(){
    return minFreq;
}

//-------------------------------------------------------------------------------------------------------

float Oscillator::getPhase(){
    return phase;
}
//-------------------------------------------------------------------------------------------------------

double Oscillator::getFrequencyInHz(){
    return frequencyInHz;
}

//-------------------------------------------------------------------------------------------------------
void Oscillator::setSampleRate(float sampleRate){
    this->sampleRate = sampleRate;
    this->fScale = WAVETABLE_SIZE/sampleRate;
    stepValue = frequencyInHz*fScale;

}

//-------------------------------------------------------------------------------------------------------
void Oscillator::setPhase(float phase){
    this->phase = phase;
}

//-------------------------------------------------------------------------------------------------------
void Oscillator::setFrequencyInHz(double freq){
    freq < 0.1 ? frequencyInHz = 0.1 : frequencyInHz = freq;
    stepValue = frequencyInHz*fScale;
}


//-------------------------------------------------------------------------------------------------------
void Oscillator::setCurrentWavetable(unsigned char currentWavetable){
    switch (currentWavetable) {
        case Sine:
            this->currentWavetable = sine;
            break;
        /*case Saw:
            this->currentWavetable = sawtooth;
            break;
        case Pulse:
            this->currentWavetable = pulse;
            break;
        case Triangle:
            this->currentWavetable = triangle;
            break;*/
        default:
            break;
    }
}

//-------------------------------------------------------------------------------------------------------
void Oscillator::createWavetables()
{
    double numberOfBytes = WAVETABLE_SIZE*sizeof(double);

    //sawtooth = BufferFactory::createBufferDouble(numberOfBytes);
    //pulse = BufferFactory::createBufferDouble(numberOfBytes);
    sine = BufferFactory::createBufferDouble(numberOfBytes);
    //triangle = BufferFactory::createBufferDouble(numberOfBytes);



    for(int i=0; i<WAVETABLE_SIZE; i++){
        /*float max_size = pwm*WAVETABLE_SIZE;
        if (i<max_size)
            pulse[i] = 0;
        else
            pulse[i] = 1;*/

        double currentI = (double)i/(double) WAVETABLE_SIZE;

        //sawtooth[i] = 2.0*currentI - 1.0;*/

        sine[i] = sin(2.0*pi*currentI);

        //i<=(int)((float)WAVETABLE_SIZE/2.0) ? triangle[i] = 4.0*currentI - 1.0: triangle[i] = 3.0 - 4.0*currentI; //halfRamp : 2 - halfRamp
    }

}

//-------------------------------------------------------------------------------------------------------
void Oscillator::deleteWavetables(){
    if (sine != nullptr) {
        delete sine;
        sine = nullptr;
    }
    /*
    if (sawtooth != nullptr) {
        delete sawtooth;
        sawtooth = nullptr;
    }

    if (pulse != nullptr) {
        delete pulse;
        pulse = nullptr;
    }

    if (triangle != nullptr) {
        delete triangle;
        triangle = nullptr;
    }*/
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
    double index = cursorTable + phase*WAVETABLE_SIZE;
    *output = currentWavetable[(int) index];
}

//-------------------------------------------------------------------------------------------------------
void Oscillator::genSignalDouble(double* output){
    double index = cursorTable + phase*WAVETABLE_SIZE;
    *output = currentWavetable[(int) index];
}

//-------------------------------------------------------------------------------------------------------
void Oscillator::genSignalDoubleWithInterp(double* output){
    double index = cursorTable+phase*WAVETABLE_SIZE;
    int next = ceil(index);
    int previous = floor(index);

    int int_part = previous;
    double fract_part = index-int_part;

    //if(previous < 0){ //realign previous
    //    previous = WAVETABLE_SIZE -1;
    //}
    if(next == WAVETABLE_SIZE){ //realign next
        next = 0;
    }
    //*output = NU*currentWavetable[previous] + (1.0-NU)*currentWavetable[next]; fixed NU
    *output = currentWavetable[previous] + (fract_part*(currentWavetable[next]-currentWavetable[previous]));

}

//-------------------------------------------------------------------------------------------------------
void Oscillator::updateCursorTable(){
    cursorTable = cursorTable + stepValue;
    double index = cursorTable+phase*WAVETABLE_SIZE;
    if (index > (WAVETABLE_SIZE-1))
        cursorTable = cursorTable - WAVETABLE_SIZE;
}
