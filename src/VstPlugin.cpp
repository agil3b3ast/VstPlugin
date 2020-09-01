#include "VstPlugin.hpp"
#include <memory>

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new VstPlugin(audioMaster);
}

//-------------------------------------------------------------------------------------------------------
VstPlugin::VstPlugin(audioMasterCallback audioMaster)
: AudioEffectX(audioMaster, PROGS_COUNT, ParamCOUNT)	// n program, n parameters
{
    setNumInputs(0);		// stereo in
    setNumOutputs(2);		// stereo out
    setUniqueID('vMis');	// identify
    canDoubleReplacing(true);
    isSynth(true);

    initPlugin();

}

//-------------------------------------------------------------------------------------------------------
void VstPlugin::initPlugin()
{
    //Nostra funzione che fa da init, chiamata nel costruttore

    gainL = 1.0;
    gainR = 1.0;

    delFeedbackL = 0.5;
    delFeedbackR = 0.5;

    maxFeedback = 0.99;

    wetDry  = 0.5; // 0 full dry, 1 full wet

    midiNoteNumber = 0;
    midiNoteVelocity = 0;
    midiNoteDelta = 0;
    isNoteOn = false;

    midiControlNumber = 0;
    midiControlValue = 0;
    midiControlDelta = 0;

    createBuffers();

    pwm = 0.3;
    cursorTable = 0;
    fScale = WAVETABLE_SIZE/getSampleRate();
    createWavetables();
    createFrequencyTable();


    initPresets();
    setProgram(0);
}

//Preset
void VstPlugin::initPresets(){
    programs[0].delayTimeL = 0.3;
    programs[0].delayTimeR = 0.3;
    programs[0].feedbackL = 0.3;
    programs[0].feedbackR = 0.3;
    programs[0].wetDry = 0.5;
    strcpy(programs[0].name, "Default");

    programs[1].delayTimeL = 0.5;
    programs[1].delayTimeR = 0.5;
    programs[1].feedbackL = 0.5;
    programs[1].feedbackR = 0.5;
    programs[1].wetDry = 0.5;
    strcpy(programs[1].name, "Half Delay");

    programs[2].delayTimeL = 0.1;
    programs[2].delayTimeR = 0.1;
    programs[2].feedbackL = 0.1;
    programs[2].feedbackR = 0.1;
    programs[2].wetDry = 0.5;
    strcpy(programs[2].name, "Short Delay");

    programs[3].delayTimeL = 0.8;
    programs[3].delayTimeR = 0.8;
    programs[3].feedbackL = 0.8;
    programs[3].feedbackR = 0.8;
    programs[3].wetDry = 0.4;
    strcpy(programs[3].name, "Long Delay");

    programs[4].delayTimeL = 0.8;
    programs[4].delayTimeR = 0;
    programs[4].feedbackL = 0.8;
    programs[4].feedbackR = 0;
    programs[4].wetDry = 0.5;
    strcpy(programs[4].name, "Full Left Delay");
}

//funzioni eventi MIDI

//-------------------------------------------------------------------------------------------------------
void VstPlugin::controlChange(int controlNumber, int controlValue, int deltaEvent){
    midiControlNumber = controlNumber;
    midiControlValue = controlValue;
    midiControlDelta = deltaEvent;
}

//-------------------------------------------------------------------------------------------------------
void VstPlugin::noteOn(int note, int velocity, int deltaEvent){
    midiNoteNumber = note;
    midiNoteVelocity = velocity;
    midiNoteDelta = deltaEvent;
    isNoteOn = true;
    cursorTable = 0;
}


//-------------------------------------------------------------------------------------------------------
void VstPlugin::noteOff(int note, int velocity, int deltaEvent){
    midiNoteNumber = note;
    midiNoteVelocity = velocity;
    midiNoteDelta = deltaEvent;
    isNoteOn = false;
}

//-------------------------------------------------------------------------------------------------------

VstInt32 VstPlugin::processEvents (VstEvents* events)
{   //processa eventi VST
    for (int i=0; i<events->numEvents;i++){
        VstEvent *event = events->events[i];
        if (event->type== kVstMidiType){
            VstMidiEvent *midiEvent = (VstMidiEvent *) event;

            char *midiData = midiEvent->midiData;

            char statusByte = midiData[0];
            char dataByte1 = midiData[1];
            char dataByte2 = midiData[2];


            char channelMidi = (statusByte & 0xF) + 1;
            VstInt32 midiMessageType = statusByte & 0xF0;

            if(midiMessageType == 0x90){ // note on
                int note = dataByte1;
                int velocity = dataByte2;
                int deltaFrame = midiEvent->deltaFrames;

                if (velocity == 0)
                    noteOff(note, velocity, deltaFrame);
                else
                    noteOn(note, velocity, deltaFrame);
            } else if (midiMessageType == 0x80){ // note off
                int note = dataByte1;
                int velocity = dataByte2;
                int deltaFrame = midiEvent->deltaFrames;

                noteOff(note, velocity, deltaFrame);
            } else if (midiMessageType == 0xB0){ // control change
                int controlNumber = dataByte1;
                int controlValue = dataByte2;
                int deltaFrame = midiEvent->deltaFrames;
                controlChange(controlNumber, controlValue, deltaFrame);
            }

        }
    }
    return 1;
}

//-------------------------------------------------------------------------------------------------------
void VstPlugin::createFrequencyTable()
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
void VstPlugin::createWavetables()
{
    float numberOfBytes = WAVETABLE_SIZE*sizeof(float);

    sawtooth = (float *) malloc(numberOfBytes);
    memset(sawtooth,0,numberOfBytes);

    pulse = (float *) malloc(numberOfBytes);
    memset(pulse,0,numberOfBytes);


    for(int i=0; i<WAVETABLE_SIZE; i++){
        float max_size = pwm*WAVETABLE_SIZE;
        if (i<max_size)
            pulse[i] = 0;
        else
            pulse[i] = 1;

        sawtooth[i] = 2*((float)i/(float) WAVETABLE_SIZE) - 1;
    }

}

//-------------------------------------------------------------------------------------------------------
void VstPlugin::deleteWavetables(){
    if (sawtooth != nullptr) {
        delete sawtooth;
        sawtooth = nullptr;
    }

    if (pulse != nullptr) {
        delete pulse;
        pulse = nullptr;
    }
}

//-------------------------------------------------------------------------------------------------------
void VstPlugin::createBuffers()
{
    float delayLineInSec = 1; //1 seconds of audio
    float currentSampleRate = getSampleRate();
    float numberOfSamples = delayLineInSec*currentSampleRate;
    float numberOfBytes = numberOfSamples*sizeof(float);

    bufferDelayL = (float *) malloc(numberOfBytes);
    memset(bufferDelayL,0,numberOfBytes);

    bufferDelayR = (float *) malloc(numberOfBytes);
    memset(bufferDelayR,0,numberOfBytes);

    delayCursorL = 0;
    delayCursorR = 0;
    delayMaxSize = numberOfSamples;
    delayCurrentSizeL = delayMaxSize/2;
    delayCurrentSizeR = delayMaxSize/2;
}

//-----------------------------------------------------------------------------------------
void VstPlugin::processDelay(float** inputs, VstInt32 sampleFrames)
{
    // PROCESS SINGLE PRECISION - processa audio

    float *buffL = inputs[0]; // buffer input left
    float *buffR = inputs[1]; // buffer input right

    for(int i=0; i<sampleFrames;i++){

        float oldestSampleL = bufferDelayL[delayCursorL];
        float oldestSampleR = bufferDelayR[delayCursorR];


        bufferDelayL[delayCursorL] = buffL[i]+oldestSampleR*delFeedbackL;
        bufferDelayR[delayCursorR] = buffR[i]+oldestSampleL*delFeedbackR;

        delayCursorL++;
        delayCursorR++;

        if (delayCursorL >= delayCurrentSizeL){
            delayCursorL = 0;
        }

        if (delayCursorR >= delayCurrentSizeR){
            delayCursorR = 0;
        }

        buffL[i] = gainL*(wetDry*buffL[i]+(1-wetDry)*oldestSampleL);
        buffR[i] = gainR*(wetDry*buffR[i]+(1-wetDry)*oldestSampleR);

    }

}


//-----------------------------------------------------------------------------------------
void VstPlugin::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
{
    // PROCESS SINGLE PRECISION - processa audio

    //float *inL = inputs[0]; // buffer input left
    //float *inR = inputs[1]; // buffer input right

    float *outL = outputs[0]; // buffer output left
    float *outR = outputs[1]; // buffer output right


    //PROCESS SYNTH
    if (isNoteOn){ // NOTE ON
        if(midiNoteDelta > sampleFrames){ //evento è di un buffer successivo
            memset(outL, 0, sampleFrames*sizeof(float));
            memset(outR, 0, sampleFrames*sizeof(float));
            midiNoteDelta = midiNoteDelta - sampleFrames;
            return;
        }

        double frequencyInHz = freqTable[midiNoteNumber];
        double stepValue = frequencyInHz*fScale;
        double gainVelocity = (double) midiNoteVelocity/(double)127;

        int midiNoteDeltaCached = midiNoteDelta;
        if (midiNoteDelta > 0){
            memset(outL, 0, midiNoteDelta*sizeof(float));
            memset(outR, 0, midiNoteDelta*sizeof(float));
            midiNoteDelta = 0;
        }

        for (int i=midiNoteDeltaCached;i<sampleFrames;i++){
            outL[i] = gainVelocity*sawtooth[(int) cursorTable];
            outR[i] = gainVelocity*pulse[(int) cursorTable];

            cursorTable = cursorTable + stepValue;
            if (cursorTable > (WAVETABLE_SIZE-1))
                cursorTable = cursorTable - WAVETABLE_SIZE;
        }
    }
    else{ // NOTE OFF
        if (midiNoteDelta == 0){//se NOTE OFF subito, metto tutto il buffer a zero
            memset(outL, 0, sampleFrames*sizeof(float));
            memset(outR, 0, sampleFrames*sizeof(float));
            return;
        }

        double frequencyInHz = freqTable[midiNoteNumber];
        double stepValue = frequencyInHz*fScale;
        double gainVelocity = (double) midiNoteVelocity/(double)127;

        for (int i=0; i<sampleFrames;i++){
            if (i<midiNoteDelta){// prima di NOTE OFF scrivo segnale
                outL[i] = gainVelocity*sawtooth[(int) cursorTable];
                outR[i] = gainVelocity*pulse[(int) cursorTable];

                cursorTable = cursorTable + stepValue;
                if (cursorTable > (WAVETABLE_SIZE-1))
                    cursorTable = cursorTable - WAVETABLE_SIZE;
            }
            else{// dopo NOTE OFF metto buffer a zero
                outL[i] = 0;
                outR[i] = 0;
            }
        }

        midiNoteDelta = 0;
    }

    //PROCESS EFX
    processDelay(outputs, sampleFrames);


}

//-----------------------------------------------------------------------------------------
void VstPlugin::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames){
    // PROCESS SINGLE PRECISION

    double *inL = inputs[0]; // buffer input left
    double *inR = inputs[1]; // buffer input right

    double *outL = outputs[0]; // buffer output left
    double *outR = outputs[1]; // buffer output right

    for(int i=0; i<sampleFrames;i++){
        outL[i] = inL[i]*gainL;
        outR[i] = inR[i]*gainR;
    }
}

//-------------------------------------------------------------------------------------------------------
void VstPlugin::deleteBuffers(){
    if (bufferDelayL != nullptr) {
        delete bufferDelayL;
        bufferDelayL = nullptr;
    }

    if (bufferDelayR != nullptr) {
        delete bufferDelayR;
        bufferDelayR = nullptr;
    }
}

//-------------------------------------------------------------------------------------------------------
void VstPlugin::setSampleRate (float sampleRate)
{
    //Chiamo setSampleRate definita nella classe parent
    AudioEffect::setSampleRate(sampleRate);
    deleteBuffers();
    createBuffers();

}

//-------------------------------------------------------------------------------------------------------
VstPlugin::~VstPlugin()
{
    deleteBuffers();
    deleteWavetables();
}

//-----------------------------------------------------------------------------------------
void VstPlugin::setParameter (VstInt32 index, float value){
    switch (index) {
        case GainLeft:
            gainL=value;
            break;
        case GainRight:
            gainR=value;
            break;
        case DelaySizeL:
            delayCurrentSizeL=value*delayMaxSize;
            break;
        case DelaySizeR:
            delayCurrentSizeR=value*delayMaxSize;
            break;
        case DelayFeedbackL:
            delFeedbackL=value*maxFeedback;
            break;
        case DelayFeedbackR:
            delFeedbackR=value*maxFeedback;
            break;
        case WetDry:
            wetDry=value;
            break;
        default:
            break;
    }
};

//-----------------------------------------------------------------------------------------
float VstPlugin::getParameter (VstInt32 index){
    float valueToReturn = 0.0;
    switch (index) {
        case GainLeft:
            valueToReturn = gainL;
            break;
        case GainRight:
            valueToReturn = gainR;
            break;
        case DelaySizeL:
            valueToReturn = (float) delayCurrentSizeL/ (float) delayMaxSize;
            break;
        case DelaySizeR:
            valueToReturn = (float) delayCurrentSizeR/ (float) delayMaxSize;
            break;
        case DelayFeedbackL:
            valueToReturn = delFeedbackL/maxFeedback;
            break;
        case DelayFeedbackR:
            valueToReturn = delFeedbackR/maxFeedback;
            break;
        case WetDry:
            valueToReturn = wetDry;
            break;
        default:
            break;
    }
    return valueToReturn;
};

//-----------------------------------------------------------------------------------------

bool VstPlugin::getEffectName (char* name){
    vst_strncpy(name, "Gain Stereo", kVstMaxEffectNameLen);
    return true;
};
//-----------------------------------------------------------------------------------------

bool VstPlugin::getVendorString (char* text){
    vst_strncpy(text, "Master MIS", kVstMaxVendorStrLen);
    return true;
};
//-----------------------------------------------------------------------------------------

bool VstPlugin::getProductString (char* text){
    vst_strncpy(text, "Cool Efx", kVstMaxProductStrLen);
    return true;
};
//-----------------------------------------------------------------------------------------

void VstPlugin::getParameterLabel (VstInt32 index, char* label){

    switch (index) {
        case GainLeft:
            vst_strncpy(label, "dB", kVstMaxParamStrLen);
            break;
        case GainRight:
            vst_strncpy(label, "dB", kVstMaxParamStrLen);
            break;
        case DelaySizeL:
            vst_strncpy(label, " s", kVstMaxParamStrLen);
            break;
        case DelaySizeR:
            vst_strncpy(label, " s", kVstMaxParamStrLen);
            break;
        case DelayFeedbackL:
            vst_strncpy(label, " ", kVstMaxParamStrLen);
            break;
        case DelayFeedbackR:
            vst_strncpy(label, " ", kVstMaxParamStrLen);
            break;
        case WetDry:
            vst_strncpy(label, " ", kVstMaxParamStrLen);
            break;
        default:
            break;
    }

};
//-----------------------------------------------------------------------------------------

void VstPlugin::getParameterDisplay (VstInt32 index, char* text) {


    switch (index) {
        case GainLeft:
            dB2string(gainL, text, kVstMaxParamStrLen);
            break;
        case GainRight:
            dB2string(gainR, text, kVstMaxParamStrLen);
            break;
        case DelaySizeL:
            float2string(delayCurrentSizeL/getSampleRate(), text, kVstMaxParamStrLen);
            break;
        case DelaySizeR:
            float2string(delayCurrentSizeR/getSampleRate(), text, kVstMaxParamStrLen);
            break;
        case DelayFeedbackL:
            float2string(delFeedbackL, text, kVstMaxParamStrLen);
            break;
        case DelayFeedbackR:
            float2string(delFeedbackR, text, kVstMaxParamStrLen);
            break;
        case WetDry:
            float2string(wetDry, text, kVstMaxParamStrLen);
            break;
        default:
            break;
    }
};
//-----------------------------------------------------------------------------------------

void VstPlugin::getParameterName (VstInt32 index, char* text) {
    switch (index) {
        case GainLeft:
            vst_strncpy(text, "Gain L", kVstMaxParamStrLen);
            break;
        case GainRight:
            vst_strncpy(text, "Gain R", kVstMaxParamStrLen);
            break;
        case DelaySizeL:
            vst_strncpy(text, "Delay L", kVstMaxParamStrLen);
            break;
        case DelaySizeR:
            vst_strncpy(text, "Delay R", kVstMaxParamStrLen);
            break;
        case DelayFeedbackL:
            vst_strncpy(text, "DelayFbL", kVstMaxParamStrLen);
            break;
        case DelayFeedbackR:
            vst_strncpy(text, "DelayFbR", kVstMaxParamStrLen);
            break;
        case WetDry:
            vst_strncpy(text, "Wet/Dry", kVstMaxParamStrLen);
            break;
        default:
            break;
    }
};

//-----------------------------------------------------------------------------------------

void VstPlugin::setProgram (VstInt32 program){
    AudioEffect::setProgram(program);


    setParameter(DelaySizeL, programs[curProgram].delayTimeL);
    setParameter(DelaySizeR, programs[curProgram].delayTimeR);

    setParameter(DelayFeedbackL, programs[curProgram].feedbackL);
    setParameter(DelayFeedbackR, programs[curProgram].feedbackR);

    setParameter(WetDry, programs[curProgram].wetDry);

}

//-----------------------------------------------------------------------------------------

void VstPlugin::getProgramName (char* name){
    strcpy(name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------

bool VstPlugin::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text){
    strcpy(text, programs[index].name);
    return true;
}
