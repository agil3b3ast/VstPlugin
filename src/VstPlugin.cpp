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

    // Init smooths array
    for (int i=0;i<ParamCOUNT;i++){smoothParams.smooths[i] = nullptr;}

    initPresets();
    initToSmooths();

    setProgram(0);
    initSmoothParams();
}
//-----------------------------------------------------------------------------------------

void VstPlugin::initToSmooths(){
    smoothParams.toSmooths[GainLeft] = true;
    smoothParams.toSmooths[GainRight] = true;

    smoothParams.toSmooths[DelayFeedback] = true;

    smoothParams.toSmooths[WetDry] = true;

    smoothParams.toSmooths[Amount1] = true;
    smoothParams.toSmooths[Amount2] = true;
    smoothParams.toSmooths[Amount3] = true;

    smoothParams.toSmooths[FrequencyInHz1] = false;
    smoothParams.toSmooths[FrequencyInHz2] = false;
    smoothParams.toSmooths[FrequencyInHz3] = false;
    
    smoothParams.toSmooths[PanAmount] = true;

    smoothParams.toSmooths[PanFrequency] = false;

}
//-----------------------------------------------------------------------------------------

void VstPlugin::initSmoothParamValues(){
    smoothParams.smooths[GainLeft]->setStart(chorus.getGain().getGainL());
    smoothParams.smooths[GainRight]->setStart(chorus.getGain().getGainR());

    smoothParams.smooths[DelayFeedback]->setStart(chorus.getDelay1()->getDelFeedbackL()*chorus.getDelay1()->getMaxFeedback());

    smoothParams.smooths[WetDry]->setStart(chorus.getWetDry());

    smoothParams.smooths[Amount1]->setStart(chorus.getDelay1()->getAmount());
    smoothParams.smooths[Amount2]->setStart(chorus.getDelay2()->getAmount());
    smoothParams.smooths[Amount3]->setStart(chorus.getDelay3()->getAmount());


    //smooths[FrequencyInHz]->setStart((delay.getFrequencyInHz() - delay.getMinFreq())/(delay.getMaxFreq()-delay.getMinFreq()));

    smoothParams.smooths[PanAmount]->setStart(autoPan.getAmount());
}
//-----------------------------------------------------------------------------------------

void VstPlugin::initSmoothParams(){
    float smoothDelayTime = 0.01; //suppose 10ms delay

    for (int i=0;i<ParamCOUNT;i++){
        smoothParams.smooths[i] = smoothParams.toSmooths[i] ? new Smooth(smoothDelayTime,getSampleRate()) : nullptr;
    }

    initSmoothParamValues();

    for (int i=0;i<ParamCOUNT;i++){
        if(smoothParams.toSmooths[i]){
            smoothParams.smooths[i]->setEnd(smoothParams.smooths[i]->getStart());
            smoothParams.smooths[i]->setIsSmooth(false);
        }
    }
}

//-----------------------------------------------------------------------------------------
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
    programs[1].amount1 = 0.15;
    programs[1].amount2 = 0.16;
    programs[1].amount3 = 0.14;
    programs[1].frequencyInHz1 = 0.45;
    programs[1].frequencyInHz2 = 0.5;
    programs[1].frequencyInHz3 = 0.42;
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
    programs[2].frequencyInHz3 = 0.42;
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
    programs[4].panFrequency = 1.0;
    strcpy(programs[4].name, "AutoPan");
}




//-----------------------------------------------------------------------------------------
void VstPlugin::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
{

    float *buffL = inputs[0]; // buffer input left
    float *buffR = inputs[1]; // buffer input right

    float *buffOutL = outputs[0]; // buffer output left
    float *buffOutR = outputs[1]; // buffer output right


    for(int i=0; i<sampleFrames;i++){
        //update params
        for (int j=0;j<ParamCOUNT;j++){
            float toSmooth = 0.0;
            smoothParams.toSmooths[j] ? (smoothParams.smooths[j]->process(&toSmooth) ? setSmoothParameter(j, toSmooth) : (void) 0) : (void) 0;
        }

        //process efx
        chorus.processChorusBySample(&buffL[i], &buffR[i]);
        autoPan.processAutoPan(&buffL[i], &buffR[i]);
        buffOutL[i] = buffL[i];
        buffOutR[i] = buffR[i];
    }

}

//-----------------------------------------------------------------------------------------
void VstPlugin::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames){
    ;
}



//-------------------------------------------------------------------------------------------------------
void VstPlugin::setSampleRate (float sampleRate)
{
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
        smoothParams.smooths[i] != nullptr ? smoothParams.smooths[i]->setSampleRate(sampleRate) : (void) 0;
    }
    
    autoPan.setSampleRate(sampleRate);
}

//-------------------------------------------------------------------------------------------------------

 VstPlugin::~VstPlugin()
{
    for (int i=0;i<ParamCOUNT;i++){
        if (smoothParams.smooths[i] != nullptr){
            delete smoothParams.smooths[i];
            smoothParams.smooths[i] = nullptr;
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
    ((smoothParams.smooths[index] != nullptr) and smoothParams.toSmooths[index]) ? smoothParams.smooths[index]->setOtherSmoothPath(value) : setSmoothParameter(index, value);
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
    return ((smoothParams.smooths[index] != nullptr) and smoothParams.toSmooths[index]) ?  smoothParams.smooths[index]->getEnd() : getSmoothParameter(index);
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
            dB2string(getParameter(GainLeft), text, numCharDisplay);
            break;
        case GainRight:
            dB2string(getParameter(GainRight), text, numCharDisplay);
            break;
        case DelayFeedback:
            float2string(getParameter(DelayFeedback)*chorus.getDelay1()->getMaxFeedback(), text, numCharDisplay);
            break;
        case WetDry:
            float2string(getParameter(WetDry), text, numCharDisplay);
            break;
        case Amount1:
            float2string(1000 * getParameter(Amount1)*(chorus.getDelay1()->getDelayMaxSize()-chorus.getDelay1()->getMinV())/getSampleRate(), text, numCharDisplay);
            break;
        case Amount2:
            float2string(1000 * getParameter(Amount2)*(chorus.getDelay2()->getDelayMaxSize()-chorus.getDelay2()->getMinV())/getSampleRate(), text, numCharDisplay);
            break;
        case Amount3:
            float2string(1000 * getParameter(Amount3)*(chorus.getDelay3()->getDelayMaxSize()-chorus.getDelay3()->getMinV())/getSampleRate(), text, numCharDisplay);
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

    setParameter(DelayFeedback, programs[curProgram].feedback);

    setParameter(WetDry, programs[curProgram].wetDry);

    setParameter(Amount1, programs[curProgram].amount1);
    setParameter(Amount2, programs[curProgram].amount2);
    setParameter(Amount3, programs[curProgram].amount3);


    setParameter(FrequencyInHz1, programs[curProgram].frequencyInHz1);
    setParameter(FrequencyInHz2, programs[curProgram].frequencyInHz2);
    setParameter(FrequencyInHz3, programs[curProgram].frequencyInHz3);

    setParameter(PanAmount, programs[curProgram].panAmount);
    setParameter(PanFrequency, programs[curProgram].panFrequency);

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
