#include "VstPlugin.hpp"


//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new VstPlugin(audioMaster);
}

//-------------------------------------------------------------------------------------------------------
VstPlugin::VstPlugin(audioMasterCallback audioMaster)
: AudioEffectX(audioMaster, PROGS_COUNT, ParamCOUNT), oscillator(getSampleRate()), delay(getSampleRate())	// n program, n parameters
{
    setNumInputs(2);		// stereo in
    setNumOutputs(2);		// stereo out
    setUniqueID('vMis');	// identify
    canDoubleReplacing(true);
    isSynth(false);

    initPlugin();

}

//-------------------------------------------------------------------------------------------------------
void VstPlugin::initPlugin()
{
    //Nostra funzione che fa da init, chiamata nel costruttore

    //gain is inside delay

    // INIT DELAY

    
    initPresets();
    setProgram(0);
}

//Preset
void VstPlugin::initPresets(){
    programs[0].delayTimeL = 0.3;
    programs[0].delayTimeR = 0.3;
    programs[0].feedbackL = 0;
    programs[0].feedbackR = 0;
    programs[0].wetDry = 0.5;
    programs[0].minAmount = 0.3;
    programs[0].maxAmount = 0.1;
    programs[0].frequencyInHz = 1.0;
    strcpy(programs[0].name, "Default");

    programs[1].delayTimeL = 0.5;
    programs[1].delayTimeR = 0.5;
    programs[1].feedbackL = 0.5;
    programs[1].feedbackR = 0.5;
    programs[1].wetDry = 0.5;
    programs[1].minAmount = 0.1;
    programs[1].maxAmount = 0.3;
    programs[1].frequencyInHz = 0.1;
    strcpy(programs[1].name, "Half Delay");

    programs[2].delayTimeL = 0.1;
    programs[2].delayTimeR = 0.1;
    programs[2].feedbackL = 0.1;
    programs[2].feedbackR = 0.1;
    programs[2].wetDry = 0.5;
    programs[2].minAmount = 0.1;
    programs[2].maxAmount = 0.3;
    programs[2].frequencyInHz = 0.1;
    strcpy(programs[2].name, "Short Delay");

    programs[3].delayTimeL = 0.8;
    programs[3].delayTimeR = 0.8;
    programs[3].feedbackL = 0.8;
    programs[3].feedbackR = 0.8;
    programs[3].wetDry = 0.4;
    programs[3].minAmount = 0.1;
    programs[3].maxAmount = 0.3;
    programs[3].frequencyInHz = 0.1;
    strcpy(programs[3].name, "Long Delay");

    programs[4].delayTimeL = 0.8;
    programs[4].delayTimeR = 0;
    programs[4].feedbackL = 0.8;
    programs[4].feedbackR = 0;
    programs[4].wetDry = 0.5;
    programs[4].minAmount = 0.1;
    programs[4].maxAmount = 0.3;
    programs[4].frequencyInHz = 0.1;
    strcpy(programs[4].name, "Full Left Delay");
}




//-----------------------------------------------------------------------------------------
void VstPlugin::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
{
    // PROCESS SINGLE PRECISION - processa audio

    //float *inL = inputs[0]; // buffer input left
    //float *inR = inputs[1]; // buffer input right

    //float *outL = outputs[0]; // buffer output left
    //float *outR = outputs[1]; // buffer output right

    //PROCESS EFX
    delay.processDelay(inputs, outputs, sampleFrames);
    

}

//-----------------------------------------------------------------------------------------
void VstPlugin::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames){
    // PROCESS SINGLE PRECISION

    double *inL = inputs[0]; // buffer input left
    double *inR = inputs[1]; // buffer input right

    double *outL = outputs[0]; // buffer output left
    double *outR = outputs[1]; // buffer output right

    for(int i=0; i<sampleFrames;i++){
        outL[i] = inL[i]*delay.getDelayGain().getGainL();
        outR[i] = inR[i]*delay.getDelayGain().getGainR();
    }
}



//-------------------------------------------------------------------------------------------------------
void VstPlugin::setSampleRate (float sampleRate)
{
    //Chiamo setSampleRate definita nella classe parent
    AudioEffect::setSampleRate(sampleRate);
    delay.deleteDelayLines();
    delay.createDelayLines();

}

//-------------------------------------------------------------------------------------------------------

 VstPlugin::~VstPlugin()
{
    /* NON PIU NECESSARIO VISTO CHE OGNI CLASSE HA IL SUO DISTRUTTORE CHE VIENE INVOCATO
    delay.~Delay(); //de-allocate delay lines
    oscillator.~Oscillator(); //de-allocate wavetable
     */
}


//-----------------------------------------------------------------------------------------
void VstPlugin::setParameter (VstInt32 index, float value){
    switch (index) {
        case GainLeft:
            delay.setDelayGain(value,delay.getDelayGain().getGainR());
            break;
        case GainRight:
            delay.setDelayGain(delay.getDelayGain().getGainL(),value);
            break;
        case DelaySizeL:
            delay.setDelayCurrentSizeL(value*delay.getDelayMaxSize());
            break;
        case DelaySizeR:
            delay.setDelayCurrentSizeR(value*delay.getDelayMaxSize());
            break;
        case DelayFeedbackL:
            delay.setDelFeedbackL(value*delay.getMaxFeedback());
            break;
        case DelayFeedbackR:
            delay.setDelFeedbackR(value*delay.getMaxFeedback());
            break;
        case WetDry:
            delay.setWetDry(value);
            break;
        case MinAmount:
            delay.setMinAmount(value);
            break;
        case MaxAmount:
            delay.setMaxAmount(value);
            break;
        case FrequencyInHz:
            delay.setFrequencyInHz(value);
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
            valueToReturn = delay.getDelayGain().getGainL();
            break;
        case GainRight:
            valueToReturn = delay.getDelayGain().getGainR();
            break;
        case DelaySizeL:
            valueToReturn = (float) delay.getDelayCurrentSizeL()/ (float) delay.getDelayMaxSize();
            break;
        case DelaySizeR:
            valueToReturn = (float) delay.getDelayCurrentSizeR()/ (float) delay.getDelayMaxSize();
            break;
        case DelayFeedbackL:
            valueToReturn = delay.getDelFeedbackL()/delay.getMaxFeedback();
            break;
        case DelayFeedbackR:
            valueToReturn = delay.getDelFeedbackR()/delay.getMaxFeedback();
            break;
        case WetDry:
            valueToReturn = delay.getWetDry();
            break;
        case MinAmount:
            valueToReturn = delay.getMinAmount()/delay.getDelayMaxSize();
            break;
        case MaxAmount:
            valueToReturn = delay.getMaxAmount()/delay.getDelayMaxSize();
            break;
        case FrequencyInHz:
            valueToReturn = delay.getFrequencyInHz();
            break;
        default:
            break;
    }
    return valueToReturn;
};

//-----------------------------------------------------------------------------------------

bool VstPlugin::getEffectName (char* name){
    vst_strncpy(name, "Vst Plugin", kVstMaxEffectNameLen);
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
        case MinAmount:
            vst_strncpy(label, "ms", kVstMaxParamStrLen);
            break;
        case MaxAmount:
            vst_strncpy(label, "ms", kVstMaxParamStrLen);
            break;
        case FrequencyInHz:
            vst_strncpy(label, "Hz", kVstMaxParamStrLen);
            break;
        default:
            break;
    }

};

//-----------------------------------------------------------------------------------------

void VstPlugin::getParameterDisplay (VstInt32 index, char* text) {


    switch (index) {
        case GainLeft:
            dB2string(delay.getDelayGain().getGainL(), text, kVstMaxParamStrLen);
            break;
        case GainRight:
            dB2string(delay.getDelayGain().getGainR(), text, kVstMaxParamStrLen);
            break;
        case DelaySizeL:
            float2string(delay.getDelayCurrentSizeL()/getSampleRate(), text, kVstMaxParamStrLen);
            break;
        case DelaySizeR:
            float2string(delay.getDelayCurrentSizeR()/getSampleRate(), text, kVstMaxParamStrLen);
            break;
        case DelayFeedbackL:
            float2string(delay.getDelFeedbackL(), text, kVstMaxParamStrLen);
            break;
        case DelayFeedbackR:
            float2string(delay.getDelFeedbackR(), text, kVstMaxParamStrLen);
            break;
        case WetDry:
            float2string(delay.getWetDry(), text, kVstMaxParamStrLen);
            break;
        case MinAmount:
            int2string((int) 1000 * delay.getMinAmount()/getSampleRate(), text, kVstMaxParamStrLen);
            break;
        case MaxAmount:
            int2string((int) 1000 * delay.getMaxAmount()/getSampleRate(), text, kVstMaxParamStrLen);
            break;
        case FrequencyInHz:
            float2string(delay.getFrequencyInHz(), text, kVstMaxParamStrLen);
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
        case MinAmount:
            vst_strncpy(text, "MinDelay", kVstMaxParamStrLen);
            break;
        case MaxAmount:
            vst_strncpy(text, "MaxDelay", kVstMaxParamStrLen);
            break;
        case FrequencyInHz:
            vst_strncpy(text, "Rate", kVstMaxParamStrLen);
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
    
    setParameter(FrequencyInHz, programs[curProgram].frequencyInHz);
    
    setParameter(MinAmount, programs[curProgram].minAmount);
    setParameter(MaxAmount, programs[curProgram].maxAmount);

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
