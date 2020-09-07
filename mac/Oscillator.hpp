//
//  Oscillator.hpp
//  VstPlugin
//
//  Created by Alessandro Fazio on 07/09/2020.
//

#ifndef Oscillator_hpp
#define Oscillator_hpp

#include "BufferFactory.hpp"
#include "audioeffect.h"

#include <cmath>

const double pi = 3.14159265358979323846;



class Oscillator{
    
    //wavetable
    float *sawtooth;
    float *pulse;
    float *sine;

    
    const int WAVETABLE_SIZE = 2048;
    float pwm; // determina la simmetria dell'onda quadra
    double freqTable[128];
    double cursorTable;
    double fScale;
    float sampleRate;
    float frequencyInHz;
    
    
    public:
        explicit Oscillator(float sampleRate);
        ~Oscillator();
        //wavetables
        void createWavetables();
        void deleteWavetables();
        void createFrequencyTable();
        void processOscillator(float** outputs, VstInt32 sampleFrames);
        void genSignal(float *sample, float *wavetable);
    

    

};

#endif /* Oscillator_hpp */
