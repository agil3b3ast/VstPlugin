#include "VstPlugin.hpp"


//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new VstPlugin(audioMaster);
}

//-------------------------------------------------------------------------------------------------------
VstPlugin::VstPlugin(audioMasterCallback audioMaster)
: AudioEffectX(audioMaster, PROGS_COUNT, ParamCOUNT), vd1(sampleRate), vd2(sampleRate), chorus(getSampleRate(),&vd1,&vd2) 	// n program, n parameters
{
    setNumInputs(2);		// stereo in
    setNumOutputs(2);		// stereo out
    setUniqueID('vMis');	// identify
    canDoubleReplacing(false);
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
    initToSmooths();
    initSmoothParams();
}

void VstPlugin::initToSmooths(){
    smoothParams.toSmooths[GainLeft] = false;

    smoothParams.toSmooths[GainRight] = false;

    smoothParams.toSmooths[DelayFeedbackL] = false;

    smoothParams.toSmooths[DelayFeedbackR] = false;

    smoothParams.toSmooths[WetDry] = true;

    smoothParams.toSmooths[Amount] = true;

    smoothParams.toSmooths[FrequencyInHz] = false;
}

void VstPlugin::initSmoothParamValues(){
    //smooths[GainLeft]->setStart(delay.getDelayGain().getGainL());

    //smooths[GainRight]->setStart(delay.getDelayGain().getGainR());

    //smooths[DelayFeedbackL]->setStart(delay.getDelFeedbackL()*delay.getMaxFeedback());

    //smooths[DelayFeedbackR]->setStart(delay.getDelFeedbackR()*delay.getMaxFeedback());

    smooths[WetDry]->setStart(delay.getWetDry());

    smooths[Amount]->setStart(delay.getAmount());

    //smooths[FrequencyInHz]->setStart((delay.getFrequencyInHz() - delay.getMinFreq())/(delay.getMaxFreq()-delay.getMinFreq()));

}

void VstPlugin::initSmoothParams(){
    float smoothDelayTime = 0.01; //suppose 10ms delay

    for (int i=0;i<ParamCOUNT;i++){
        smooths[i] = smoothParams.toSmooths[i] ? new Smooth(smoothDelayTime,getSampleRate()) : nullptr;
    }

    initSmoothParamValues();

    for (int i=0;i<ParamCOUNT;i++){
        if(smoothParams.toSmooths[i]){
            smooths[i]->setEnd(smooths[i]->getStart());
            smooths[i]->setIsSmooth(false);
        }
    }
}

//Preset
void VstPlugin::initPresets(){
    programs[0].feedbackL = 0;
    programs[0].feedbackR = 0;
    programs[0].wetDry = 0.5;
    programs[0].amount = 1.0;
    programs[0].frequencyInHz1 = 0.1;
    programs[0].frequencyInHz2 = 0.2;
    strcpy(programs[0].name, "Default");

    programs[1].feedbackL = 0.5;
    programs[1].feedbackR = 0.5;
    programs[1].wetDry = 0.5;
    programs[1].amount = 0.3;
    programs[1].frequencyInHz1 = 0.1;
    programs[1].frequencyInHz2 = 0.12;
    strcpy(programs[1].name, "Half Delay");

    programs[2].feedbackL = 0.1;
    programs[2].feedbackR = 0.1;
    programs[2].wetDry = 0.5;
    programs[2].amount = 0.3;
    programs[2].frequencyInHz1 = 0.1;
    programs[2].frequencyInHz2 = 0.12;
    strcpy(programs[2].name, "Short Delay");

    programs[3].feedbackL = 0.8;
    programs[3].feedbackR = 0.8;
    programs[3].wetDry = 0.4;
    programs[3].amount = 0.3;
    programs[3].frequencyInHz1 = 0.1;
    programs[3].frequencyInHz2 = 0.12;
    strcpy(programs[3].name, "Long Delay");

    programs[4].feedbackL = 0.8;
    programs[4].feedbackR = 0;
    programs[4].wetDry = 0.5;
    programs[4].amount = 0.3;
    programs[4].frequencyInHz1 = 0.1;
    programs[4].frequencyInHz2 = 0.12;
    strcpy(programs[4].name, "Full Left Delay");
}




//-----------------------------------------------------------------------------------------
void VstPlugin::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
{

    // PROCESS SINGLE PRECISION - processa audio

    float *inL = inputs[0]; // buffer input left
    float *inR = inputs[1]; // buffer input right

    float *outL = outputs[0]; // buffer output left
    float *outR = outputs[1]; // buffer output right

    //update params


    //PROCESS EFX

    //std::fstream fout;
    //fout.open("/Users/alessandro_fazio/Desktop/output_in.csv", std::ios::out | std::ios::app);


    for(int i=0; i<sampleFrames;i++){
        for (int j=0;j<ParamCOUNT;j++){
            float toSmooth = 0.0;
            smoothParams.toSmooths[j] ? (smooths[j]->process(&toSmooth) ? setSmoothParameter(j, toSmooth) : (void) 0) : (void) 0;
            //fout << std::to_string(toSmooth) << '\n';
        }

        //delay.processDelayBySample(&inL[i], &inR[i], &outL[i], &outR[i]);
        chorus.processChorus(inputs, outputs, sampleFrames);
    }

    //fout.close();


}

//-----------------------------------------------------------------------------------------
void VstPlugin::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames){
    // PROCESS SINGLE PRECISION

    /*double *inL = inputs[0]; // buffer input left
    double *inR = inputs[1]; // buffer input right

    double *outL = outputs[0]; // buffer output left
    double *outR = outputs[1]; // buffer output right

    for(int i=0; i<sampleFrames;i++){
        outL[i] = inL[i]*delay.getDelayGain().getGainL();
        outR[i] = inR[i]*delay.getDelayGain().getGainR();
    }*/
}



//-------------------------------------------------------------------------------------------------------
void VstPlugin::setSampleRate (float sampleRate)
{
    //Chiamo setSampleRate definita nella classe parent
    AudioEffect::setSampleRate(sampleRate);
    chorus.getDelay1()->deleteDelayLines();
    chorus.getDelay1()->createDelayLines();
    chorus.getDelay2()->deleteDelayLines();
    chorus.getDelay2()->createDelayLines();

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
void VstPlugin::setSmoothParameter(VstInt32 index, float value){
    switch (index) {
        case GainLeft:
            chorus.setGain(value,chorus.getGain().getGainR());
            break;
        case GainRight:
            chorus.setGain(chorus.getGain().getGainL(),value);
            break;
        case DelayFeedbackL:
            chorus.getDelay1()->setDelFeedbackL(value*chorus.getDelay1()->getMaxFeedback());
            chorus.getDelay2()->setDelFeedbackL(value*chorus.getDelay2()->getMaxFeedback());
            break;
        case DelayFeedbackR:
            chorus.getDelay1()->setDelFeedbackR(value*chorus.getDelay1()->getMaxFeedback());
            chorus.getDelay2()->setDelFeedbackR(value*chorus.getDelay2()->getMaxFeedback());
            break;
        case WetDry:
            chorus.setWetDry(value);
            break;
        case Amount:
            chorus.getDelay1()->setAmount(value);
            chorus.getDelay2()->setAmount(value);
            break;
        case FrequencyInHz1:
            chorus.setFrequencyInHz1(chorus.getDelay1()->getMinFreq() + \
            value*(chorus.getDelay1()->getMaxFreq()-chorus.getDelay1()->getMinFreq()));
            break;
        case FrequencyInHz2:
            chorus.setFrequencyInHz2(chorus.getDelay2()->getMinFreq() + \
            value*(chorus.getDelay2()->getMaxFreq()-chorus.getDelay2()->getMinFreq()));
            break;
        default:
            break;
    }
};

//-----------------------------------------------------------------------------------------
void VstPlugin::setParameter (VstInt32 index, float value){
    //float paramToChange = 0.0;
    smoothParams.toSmooths[index] ? smooths[index]->setOtherSmoothPath(value) : setSmoothParameter(index, value);
};

//-----------------------------------------------------------------------------------------
float VstPlugin::getSmoothParameter (VstInt32 index){
    float valueToReturn = 0.0;
    switch (index) {
        case GainLeft:
            valueToReturn = chorus.getGain().getGainL();
            break;
        case GainRight:
            valueToReturn = chorus.getGain().getGainR();
            break;
        case DelaySizeL:
            valueToReturn = (float) chorus.getDelay1()->getDelayCurrentSizeL()/ (float) chorus.getDelay1()->getDelayMaxSize();
            break;
        case DelaySizeR:
            valueToReturn = (float) chorus.getDelay1()->getDelayCurrentSizeR()/ (float) chorus.getDelay1()->getDelayMaxSize();
            break;
        case DelayFeedbackL:
            valueToReturn = chorus.getDelay1()->getDelFeedbackL()/chorus.getDelay1()->getMaxFeedback();
            break;
        case DelayFeedbackR:
            valueToReturn = chorus.getDelay1()->getDelFeedbackR()/chorus.getDelay1()->getMaxFeedback();
            break;
        case WetDry:
            valueToReturn = chorus.getWetDry();
            break;
        case Amount:
            valueToReturn = chorus.getDelay1()->getAmount();
            break;
        case FrequencyInHz1:
            valueToReturn = (chorus.getFrequencyInHz1() - \
            chorus.getDelay1()->getMinFreq())/(chorus.getDelay1()->getMaxFreq()- \
            chorus.getDelay1()->getMinFreq());
            break;
        case FrequencyInHz2:
            valueToReturn = (chorus.getFrequencyInHz2() - \
            chorus.getDelay2()->getMinFreq())/(chorus.getDelay2()->getMaxFreq()- \
            chorus.getDelay2()->getMinFreq());
            break;
        default:
            break;
    }
    return valueToReturn;
};

//-----------------------------------------------------------------------------------------
float VstPlugin::getParameter (VstInt32 index){
    return smoothParams.toSmooths[index] ?  smooths[index]->getEnd() : getSmoothParameter(index);
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
        case DelayFeedbackL:
            vst_strncpy(label, " ", kVstMaxParamStrLen);
            break;
        case DelayFeedbackR:
            vst_strncpy(label, " ", kVstMaxParamStrLen);
            break;
        case WetDry:
            vst_strncpy(label, " ", kVstMaxParamStrLen);
            break;
        case Amount:
            vst_strncpy(label, "ms", kVstMaxParamStrLen);
            break;
        case FrequencyInHz1:
            vst_strncpy(label, "Hz", kVstMaxParamStrLen);
            break;
        case FrequencyInHz2:
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
            //toDisplay = smoothParams.toSmooths[index] ? smooths[GainLeft]->getEnd() : delay.getDelayGain().getGainL();
            //dB2string(smooths[GainLeft]->getEnd(), text, numCharDisplay);
            dB2string(getParameter(GainLeft), text, numCharDisplay);
            break;
        case GainRight:
            //toDisplay = smoothParams.toSmooths[index] ? smooths[GainRight]->getEnd() : delay.getDelayGain().getGainR();
            //dB2string(smooths[GainRight]->getEnd(), text, numCharDisplay);
            dB2string(getParameter(GainRight), text, numCharDisplay);
            break;
        case DelayFeedbackL:
            //toDisplay = smoothParams.toSmooths[index] ? smooths[DelayFeedbackL]->getEnd() : delay.getDelFeedbackL();
            //float2string(smooths[DelayFeedbackL]->getEnd()*delay.getMaxFeedback(), text, numCharDisplay);
            float2string(getParameter(DelayFeedbackL)*chorus.getDelay1()->getMaxFeedback(), text, numCharDisplay);
            break;
        case DelayFeedbackR:
            //float2string(smooths[DelayFeedbackR]->getEnd()*delay.getMaxFeedback(), text, numCharDisplay);
            float2string(getParameter(DelayFeedbackR)*chorus.getDelay1()->getMaxFeedback(), text, numCharDisplay);
            break;
        case WetDry:
            //float2string(smooths[WetDry]->getEnd(), text, numCharDisplay);
            float2string(getParameter(WetDry), text, numCharDisplay);
            break;
        case Amount:
            //int2string((int) 1000 * delay.getAmount()*delay.getDelayMaxSize()/(2.0*getSampleRate()), text, kVstMaxParamStrLen);
            //float2string(smooths[Amount]->getEnd(), text, numCharDisplay);
            float2string(1000 * getParameter(Amount)*chorus.getDelay1()->getDelayMaxSize()/getSampleRate(), text, numCharDisplay);
            break;
        case FrequencyInHz1:
            float2string(chorus.getDelay1()->getMinFreq() + \
            getParameter(FrequencyInHz1)*(chorus.getDelay1()->getMaxFreq() - \
            chorus.getDelay1()->getMinFreq()), text, kVstMaxParamStrLen);
            break;
        case FrequencyInHz2:
            float2string(chorus.getDelay2()->getMinFreq() + \
            getParameter(FrequencyInHz2)*(chorus.getDelay2()->getMaxFreq() - \
            chorus.getDelay2()->getMinFreq()), text, kVstMaxParamStrLen);
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
        case DelayFeedbackL:
            vst_strncpy(text, "DelayFbL", kVstMaxParamStrLen);
            break;
        case DelayFeedbackR:
            vst_strncpy(text, "DelayFbR", kVstMaxParamStrLen);
            break;
        case WetDry:
            vst_strncpy(text, "Wet/Dry", kVstMaxParamStrLen);
            break;
        case Amount:
            vst_strncpy(text, "Amount", kVstMaxParamStrLen);
            break;
        case FrequencyInHz1:
            vst_strncpy(text, "Rate1", kVstMaxParamStrLen);
            break;
        case FrequencyInHz2:
            vst_strncpy(text, "Rate2", kVstMaxParamStrLen);
            break;
        default:
            break;
    }
};

//-----------------------------------------------------------------------------------------

void VstPlugin::setProgram (VstInt32 program){
    AudioEffect::setProgram(program);

    setSmoothParameter(DelayFeedbackL, programs[curProgram].feedbackL);
    setSmoothParameter(DelayFeedbackR, programs[curProgram].feedbackR);

    setSmoothParameter(WetDry, programs[curProgram].wetDry);

    setSmoothParameter(Amount, programs[curProgram].amount);

    setSmoothParameter(FrequencyInHz1, programs[curProgram].frequencyInHz1);
    setSmoothParameter(FrequencyInHz2, programs[curProgram].frequencyInHz2);

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
