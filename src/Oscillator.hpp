//
//  Oscillator.hpp
//  VstPlugin
//
//  Created by Alessandro Fazio on 07/09/2020.
//

#ifndef Oscillator_hpp
#define Oscillator_hpp

#include "BufferFactory.hpp"
#include "../vstsdk2.4/public.sdk/source/vst2.x/audioeffect.h"
#include <iostream>
#include <cmath>

const double pi = 3.14159265358979323846;

enum OscFeatures{
    Gen = 0,
    RingMod,
    NumMod
};

enum Waveforms{
    Sine = 0,
    Saw,
    Pulse
};


class Oscillator{
    
    //wavetable
    float *sawtooth;
    float *pulse;
    float *sine;

    
    const int WAVETABLE_SIZE = 88200;
    float pwm; // determina la simmetria dell'onda quadra
    double freqTable[128];
    double cursorTable;
    double fScale;
    float sampleRate;
    float frequencyInHz;
    double stepValue;
    
    
    float *currentWavetable;
    
    void genSignal(float *output);
    
    public:
        explicit Oscillator(float sampleRate);
        ~Oscillator();
    
        //wavetables
        void createWavetables();
        void deleteWavetables();
        void createFrequencyTable();
        void processOscillator(float** outputs, VstInt32 sampleFrames);
        void processOscillatorSingle(float *input);
    
        //setters
        void setFrequencyInHz(float freq);
        void setMaxAmount(int maxAmount);
        void setCurrentWavetable(unsigned char currentWavetable);
        void updateCursorTable();

};

#endif /* Oscillator_hpp */
