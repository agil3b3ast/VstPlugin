#include "VstPlugin.hpp"


//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new VstPlugin(audioMaster);
}

//-------------------------------------------------------------------------------------------------------
VstPlugin::VstPlugin(audioMasterCallback audioMaster)
: AudioEffectX(audioMaster, PROGS_COUNT, ParamCOUNT), chorus(getSampleRate()), autoPan(getSampleRate()) 	// n program, n parameters
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

    smoothParams.toSmooths[DelayFeedback] = false;

    smoothParams.toSmooths[WetDry] = true;

    smoothParams.toSmooths[Amount1] = true;
    smoothParams.toSmooths[Amount2] = true;
    smoothParams.toSmooths[Amount3] = true;

    smoothParams.toSmooths[FrequencyInHz1] = false;
    
    smoothParams.toSmooths[FrequencyInHz2] = false;

    smoothParams.toSmooths[FrequencyInHz3] = false;
    
    smoothParams.toSmooths[PanAmount] = false;

    smoothParams.toSmooths[PanFrequency] = false;

}

void VstPlugin::initSmoothParamValues(){
    //smooths[GainLeft]->setStart(delay.getDelayGain().getGainL());

    //smooths[GainRight]->setStart(delay.getDelayGain().getGainR());

    //smooths[DelayFeedbackL]->setStart(delay.getDelFeedbackL()*delay.getMaxFeedback());

    //smooths[DelayFeedbackR]->setStart(delay.getDelFeedbackR()*delay.getMaxFeedback());

    smooths[WetDry]->setStart(chorus.getWetDry());

    smooths[Amount1]->setStart(chorus.getDelay1()->getAmount());
    smooths[Amount2]->setStart(chorus.getDelay2()->getAmount());
    smooths[Amount3]->setStart(chorus.getDelay3()->getAmount());


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
    programs[0].feedback = 0;
    programs[0].wetDry = 0.5;
    programs[0].amount1 = 0.15;
    programs[0].amount2 = 0.16;
    programs[0].amount3 = 0.14;
    programs[0].frequencyInHz1 = 0.08;
    programs[0].frequencyInHz2 = 0.1;
    programs[0].frequencyInHz3 = 0.09;
    programs[0].panAmount = 1.0;
    programs[0].panFrequency = 1.0;
    strcpy(programs[0].name, "Default");

    programs[1].feedback = 0.1;
    programs[1].wetDry = 0.5;
    programs[1].amount1 = 0.29;
    programs[1].amount2 = 0.3;
    programs[1].amount3 = 0.28;
    programs[1].frequencyInHz1 = 0.2;
    programs[1].frequencyInHz2 = 0.19;
    programs[1].frequencyInHz3 = 0.25;
    programs[1].panAmount = 1.0;
    programs[1].panFrequency = 1.0;
    strcpy(programs[1].name, "Medium Chorus");

    programs[2].feedback = 0.3;
    programs[2].wetDry = 0.6;
    programs[2].amount1 = 0.39;
    programs[2].amount2 = 0.4;
    programs[2].amount3 = 0.38;
    programs[2].frequencyInHz1 = 0.45;
    programs[2].frequencyInHz2 = 0.5;
    programs[2].frequencyInHz3 = 0.4;
    programs[2].panAmount = 1.0;
    programs[2].panFrequency = 1.0;
    strcpy(programs[2].name, "High Chorus");

    programs[3].feedback = 0.7;
    programs[3].wetDry = 0.6;
    programs[3].amount1 = 0.79;
    programs[3].amount2 = 0.8;
    programs[3].amount3 = 0.78;
    programs[3].frequencyInHz1 = 0.9;
    programs[3].frequencyInHz2 = 1.0;
    programs[3].frequencyInHz3 = 0.9;
    programs[3].panAmount = 1.0;
    programs[3].panFrequency = 1.0;
    strcpy(programs[3].name, "Chaos");

    programs[4].feedback = 0;
    programs[4].wetDry = 0.5;
    programs[4].amount1 = 0.15;
    programs[4].amount2 = 0.16;
    programs[4].amount3 = 0.14;
    programs[4].frequencyInHz1 = 0.08;
    programs[4].frequencyInHz2 = 0.1;
    programs[4].frequencyInHz3 = 0.09;
    programs[4].panAmount = 0;
    programs[4].panFrequency = 4.0;
    strcpy(programs[4].name, "AutoPan");
}




//-----------------------------------------------------------------------------------------
void VstPlugin::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
{

    // PROCESS SINGLE PRECISION - processa audio

    float *buffL = inputs[0]; // buffer input left
    float *buffR = inputs[1]; // buffer input right

    float *buffOutL = outputs[0]; // buffer output left
    float *buffOutR = outputs[1]; // buffer output right

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
        chorus.processChorusBySample(&buffL[i], &buffR[i]);
        autoPan.processAutoPan(&buffL[i], &buffR[i]);
        buffOutL[i] = buffL[i];
        buffOutR[i] = buffR[i];
    }

    //fout.close();



}

//-----------------------------------------------------------------------------------------
void VstPlugin::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames){
    ;
}



//-------------------------------------------------------------------------------------------------------
void VstPlugin::setSampleRate (float sampleRate)
{
    //Chiamo setSampleRate definita nella classe parent
    AudioEffect::setSampleRate(sampleRate);
    
    chorus.getDelay1()->setSampleRate(sampleRate);
    chorus.getDelay2()->setSampleRate(sampleRate);
    chorus.getDelay3()->setSampleRate(sampleRate);
    
    chorus.getDelay1()->deleteDelayLines();
    chorus.getDelay1()->createDelayLines();
    chorus.getDelay2()->deleteDelayLines();
    chorus.getDelay2()->createDelayLines();
    chorus.getDelay3()->deleteDelayLines();
    chorus.getDelay3()->createDelayLines();
    
    chorus.getDelay1()->initVDelay();
    chorus.getDelay2()->initVDelay();
    chorus.getDelay3()->initVDelay();

    for(int i=0; i<ParamCOUNT;i++){
        smooths[i] != nullptr ? smooths[i]->setSampleRate(sampleRate) : (void) 0;
    }
    
    autoPan.setSampleRate(sampleRate);
}

//-------------------------------------------------------------------------------------------------------

 VstPlugin::~VstPlugin()
{
    for (int i=0;i<ParamCOUNT;i++){
        if (smooths[i] != nullptr){
            delete smooths[i];
            smooths[i] = nullptr;
        }
    }
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
        case DelayFeedback:
            chorus.getDelay1()->setDelFeedbackL(value*chorus.getDelay1()->getMaxFeedback());
            chorus.getDelay2()->setDelFeedbackL(value*chorus.getDelay2()->getMaxFeedback());
            chorus.getDelay3()->setDelFeedbackL(value*chorus.getDelay3()->getMaxFeedback());
            chorus.getDelay1()->setDelFeedbackR(value*chorus.getDelay1()->getMaxFeedback());
            chorus.getDelay2()->setDelFeedbackR(value*chorus.getDelay2()->getMaxFeedback());
            chorus.getDelay3()->setDelFeedbackR(value*chorus.getDelay3()->getMaxFeedback());
            break;
        case WetDry:
            chorus.setWetDry(value);
            break;
        case Amount1:
            chorus.getDelay1()->setAmount(value);
            break;
        case Amount2:
            chorus.getDelay2()->setAmount(value);
            break;
        case Amount3:
            chorus.getDelay3()->setAmount(value);
            break;
        case FrequencyInHz1:
            chorus.setFrequencyInHz1(chorus.getDelay1()->getMinFreq() + \
            value*(chorus.getDelay1()->getMaxFreq()-chorus.getDelay1()->getMinFreq()));
            break;
        case FrequencyInHz2:
            chorus.setFrequencyInHz2(chorus.getDelay2()->getMinFreq() + \
            value*(chorus.getDelay2()->getMaxFreq()-chorus.getDelay2()->getMinFreq()));
            break;
        case FrequencyInHz3:
            chorus.setFrequencyInHz3(chorus.getDelay3()->getMinFreq() + \
            value*(chorus.getDelay3()->getMaxFreq()-chorus.getDelay3()->getMinFreq()));
            break;
        case PanAmount:
            autoPan.setAmount(value);
            break;
        case PanFrequency:
            autoPan.setFreq(autoPan.getMinFreq() + \
            value*(autoPan.getMaxFreq()-autoPan.getMinFreq()));
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
        case DelayFeedback:
            valueToReturn = chorus.getDelay1()->getDelFeedbackL()/chorus.getDelay1()->getMaxFeedback();
            break;
        case WetDry:
            valueToReturn = chorus.getWetDry();
            break;
        case Amount1:
            valueToReturn = chorus.getDelay1()->getAmount();
            break;
        case Amount2:
            valueToReturn = chorus.getDelay2()->getAmount();
            break;
        case Amount3:
            valueToReturn = chorus.getDelay3()->getAmount();
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
        case FrequencyInHz3:
            valueToReturn = (chorus.getFrequencyInHz3() - \
            chorus.getDelay3()->getMinFreq())/(chorus.getDelay3()->getMaxFreq()- \
            chorus.getDelay3()->getMinFreq());
            break;
        case PanAmount:
            valueToReturn = autoPan.getAmount();
            break;
        case PanFrequency:
            valueToReturn = (autoPan.getFreq() - \
            autoPan.getMinFreq())/(autoPan.getMaxFreq()- \
            autoPan.getMinFreq());
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
        case DelayFeedback:
            vst_strncpy(label, " ", kVstMaxParamStrLen);
            break;
        case WetDry:
            vst_strncpy(label, " ", kVstMaxParamStrLen);
            break;
        case Amount1:
            vst_strncpy(label, "ms", kVstMaxParamStrLen);
            break;
        case Amount2:
            vst_strncpy(label, "ms", kVstMaxParamStrLen);
            break;
        case Amount3:
            vst_strncpy(label, "ms", kVstMaxParamStrLen);
            break;
        case FrequencyInHz1:
            vst_strncpy(label, "Hz", kVstMaxParamStrLen);
            break;
        case FrequencyInHz2:
            vst_strncpy(label, "Hz", kVstMaxParamStrLen);
            break;
        case FrequencyInHz3:
            vst_strncpy(label, "Hz", kVstMaxParamStrLen);
            break;
        case PanAmount:
            vst_strncpy(label, " ", kVstMaxParamStrLen);
            break;
        case PanFrequency:
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
        case DelayFeedback:
            //toDisplay = smoothParams.toSmooths[index] ? smooths[DelayFeedbackL]->getEnd() : delay.getDelFeedbackL();
            //float2string(smooths[DelayFeedbackL]->getEnd()*delay.getMaxFeedback(), text, numCharDisplay);
            float2string(getParameter(DelayFeedback)*chorus.getDelay1()->getMaxFeedback(), text, numCharDisplay);
            break;
        case WetDry:
            //float2string(smooths[WetDry]->getEnd(), text, numCharDisplay);
            float2string(getParameter(WetDry), text, numCharDisplay);
            break;
        case Amount1:
            //int2string((int) 1000 * delay.getAmount()*delay.getDelayMaxSize()/(2.0*getSampleRate()), text, kVstMaxParamStrLen);
            //float2string(smooths[Amount]->getEnd(), text, numCharDisplay);
            float2string(1000 * getParameter(Amount1)*chorus.getDelay1()->getDelayMaxSize()/getSampleRate(), text, numCharDisplay);
            break;
        case Amount2:
            //int2string((int) 1000 * delay.getAmount()*delay.getDelayMaxSize()/(2.0*getSampleRate()), text, kVstMaxParamStrLen);
            //float2string(smooths[Amount]->getEnd(), text, numCharDisplay);
            float2string(1000 * getParameter(Amount2)*chorus.getDelay2()->getDelayMaxSize()/getSampleRate(), text, numCharDisplay);
            break;
        case Amount3:
            //int2string((int) 1000 * delay.getAmount()*delay.getDelayMaxSize()/(2.0*getSampleRate()), text, kVstMaxParamStrLen);
            //float2string(smooths[Amount]->getEnd(), text, numCharDisplay);
            float2string(1000 * getParameter(Amount3)*chorus.getDelay3()->getDelayMaxSize()/getSampleRate(), text, numCharDisplay);
            break;
        case FrequencyInHz1:
            float2string(chorus.getDelay1()->getMinFreq() + \
            getParameter(FrequencyInHz1)*(chorus.getDelay1()->getMaxFreq() - \
            chorus.getDelay1()->getMinFreq()), text, numCharDisplay);
            break;
        case FrequencyInHz2:
            float2string(chorus.getDelay2()->getMinFreq() + \
            getParameter(FrequencyInHz2)*(chorus.getDelay2()->getMaxFreq() - \
            chorus.getDelay2()->getMinFreq()), text, numCharDisplay);
            break;
        case FrequencyInHz3:
            float2string(chorus.getDelay3()->getMinFreq() + \
            getParameter(FrequencyInHz3)*(chorus.getDelay3()->getMaxFreq() - \
            chorus.getDelay3()->getMinFreq()), text, numCharDisplay);
            break;
        case PanAmount:
            float2string(getParameter(PanAmount), text, numCharDisplay);
            break;
        case PanFrequency:
            float2string(autoPan.getMinFreq() + \
            getParameter(PanFrequency)*(autoPan.getMaxFreq() - \
            autoPan.getMinFreq()), text, numCharDisplay);
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
        case DelayFeedback:
            vst_strncpy(text, "DelayFb", kVstMaxParamStrLen);
            break;
        case WetDry:
            vst_strncpy(text, "Wet/Dry", kVstMaxParamStrLen);
            break;
        case Amount1:
            vst_strncpy(text, "Amount1", kVstMaxParamStrLen);
            break;
        case Amount2:
            vst_strncpy(text, "Amount2", kVstMaxParamStrLen);
            break;
        case Amount3:
            vst_strncpy(text, "Amount3", kVstMaxParamStrLen);
            break;
        case FrequencyInHz1:
            vst_strncpy(text, "Rate1", kVstMaxParamStrLen);
            break;
        case FrequencyInHz2:
            vst_strncpy(text, "Rate2", kVstMaxParamStrLen);
            break;
        case FrequencyInHz3:
            vst_strncpy(text, "Rate3", kVstMaxParamStrLen);
            break;
        case PanAmount:
            vst_strncpy(text, "Pan Amnt", kVstMaxParamStrLen);
            break;
        case PanFrequency:
            vst_strncpy(text, "Pan Rate", kVstMaxParamStrLen);
            break;
        default:
            break;
    }
};

//-----------------------------------------------------------------------------------------

void VstPlugin::setProgram (VstInt32 program){
    AudioEffect::setProgram(program);

    setSmoothParameter(DelayFeedback, programs[curProgram].feedback);

    setSmoothParameter(WetDry, programs[curProgram].wetDry);

    setSmoothParameter(Amount1, programs[curProgram].amount1);
    setSmoothParameter(Amount2, programs[curProgram].amount2);
    setSmoothParameter(Amount3, programs[curProgram].amount3);


    setSmoothParameter(FrequencyInHz1, programs[curProgram].frequencyInHz1);
    setSmoothParameter(FrequencyInHz2, programs[curProgram].frequencyInHz2);
    setSmoothParameter(FrequencyInHz3, programs[curProgram].frequencyInHz3);

    setSmoothParameter(PanAmount, programs[curProgram].panAmount);
    setSmoothParameter(PanFrequency, programs[curProgram].panFrequency);

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
