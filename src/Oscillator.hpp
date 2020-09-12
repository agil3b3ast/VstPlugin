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
    double *sawtooth;
    double *pulse;
    double *sine;

    float sampleRate;
    
    const int WAVETABLE_SIZE = 960000;
    const float NU = 0.5;
    
    double freqTable[128];
    double cursorTable;
    double fScale;
    double frequencyInHz;
    double stepValue;
    
    float pwm; // determina la simmetria dell'onda quadra
    
    double *currentWavetable;
    
    void genSignal(float *output);
    void genSignalDouble(double *output);
    
    public:
        explicit Oscillator(float sampleRate);
        ~Oscillator();
    
        //wavetables
        void createWavetables();
        void deleteWavetables();
        void createFrequencyTable();
        void processOscillator(float** outputs, VstInt32 sampleFrames);
        void processOscillatorSingle(float *input);
        void processOscillatorSingleDouble(double *input);
        void genSignalDoubleWithInterp(double* output);


        //getters
        double getFrequencyInHz();

        //setters
        void setFrequencyInHz(double freq);
        void setCurrentWavetable(unsigned char currentWavetable);
        void updateCursorTable();

};

#endif /* Oscillator_hpp */
