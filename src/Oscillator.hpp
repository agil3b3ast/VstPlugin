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
#include "Interp.hpp"

const double pi = 3.14159265358979323846;
//-----------------------------------------------------------------------------------------

enum OscFeatures{
    Gen = 0,
    RingMod,
    NumMod
};
//-----------------------------------------------------------------------------------------

enum Waveforms{
    Sine = 0,
    Saw,
    Pulse,
    Triangle
};

//-----------------------------------------------------------------------------------------

class Oscillator{

    //wavetable
    double *sine;
    //double *sawtooth;
    //double *pulse;
    //double *triangle;

    float sampleRate;

    const int WAVETABLE_SIZE = 960000;
    const float NU = 0.5;

    double freqTable[128];
    double cursorTable;
    double fScale;

    double frequencyInHz;
    double minFreq;
    double maxFreq;
    double stepValue;

    float phase; //percentage of 2pi

    float pwm; // pulse width modulation

    double *currentWavetable;
    
    Interp interp;

    void genSignal(float *output);
    void genSignal(double *output);

    public:
        explicit Oscillator(float sampleRate);
        ~Oscillator();

        //wavetables
        void createWavetables();
        void deleteWavetables();
        void processOscillator(float** outputs, VstInt32 sampleFrames);
        void processOscillatorSingle(float *input);
        void processOscillatorSingle(double *input);
        void genSignalWithInterp(double* output);


        //getters
        double getFrequencyInHz();
        double getMinFreq();
        double getMaxFreq();

        float getPhase();

        //setters
        void setSampleRate(float sampleRate);
        void setPhase(float phase);
        void setFrequencyInHz(double freq);
        void setCurrentWavetable(unsigned char currentWavetable);
        void updateCursorTable();

};

#endif /* Oscillator_hpp */
