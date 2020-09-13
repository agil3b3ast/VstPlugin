#ifndef VSTPLUGIN_HPP
#define VSTPLUGIN_HPP

#include "../vstsdk2.4/public.sdk/source/vst2.x/audioeffectx.h"
#include "BufferFactory.hpp"
#include "Oscillator.hpp"
#include "Chorus.hpp"
#include "VDelay.hpp"
#include "Smooth.hpp"



#include <memory>

#define PROGS_COUNT 5

const unsigned char numCharDisplay = 4;
struct Preset{
    char name[24];
    float feedbackL;
    float feedbackR;
    float delayTimeL;
    float delayTimeR;
    float wetDry;
    float minAmount;
    float maxAmount;
    float frequencyInHz1;
    float frequencyInHz2;
};


//-------------------------------------------------------------------------------------------------------

enum EfxParameter {
    GainLeft = 0,
    GainRight,
    DelayFeedbackL,
    DelayFeedbackR,
    WetDry,
    Amount,
    MinAmount,
    MaxAmount,
    FrequencyInHz1,
    FrequencyInHz2,
    ParamCOUNT
};
//-------------------------------------------------------------------------------------------------------

struct Preset{
    char name[24];
    float feedbackL;
    float feedbackR;
    float wetDry;
    float amount;
    float frequencyInHz;
};
//-------------------------------------------------------------------------------------------------------

struct SmoothParams{
    bool toSmooths[ParamCOUNT];
};

//-------------------------------------------------------------------------------------------------------
class VstPlugin : public AudioEffectX
{

    //gain is inside chorus


    //Presets
    Preset programs[PROGS_COUNT];

    //plugin
    void initPlugin();

    //oscillators
    Oscillator oscillator;

    //smooth
    Smooth *smooths[ParamCOUNT];
    SmoothParams smoothParams;
    

    //chorus
    Chorus chorus;
    VDelay vd1;
    VDelay vd2;

    //presets
    void initPresets();
    void initSmoothParams();
    void initSmoothParamValues();
    void initToSmooths();

public:

	explicit VstPlugin(audioMasterCallback audioMaster);
	~VstPlugin();

	// Processing
	void processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames) override;
    void processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames) override;
    void setParameter (VstInt32 index, float value) override;
    void setSmoothParameter(VstInt32 index, float value);

    float getParameter (VstInt32 index) override;
    float getSmoothParameter(VstInt32 index);

    bool getEffectName (char* name) override;
    bool getVendorString (char* text) override;
    bool getProductString (char* text) override;


    void getParameterLabel (VstInt32 index, char* label) override;

    void getParameterDisplay (VstInt32 index, char* text) override;
    void getSmoothParameterDisplay (VstInt32 index, char* text);

    void getParameterName (VstInt32 index, char* text) override;

    void setSampleRate (float sampleRate) override;

    void setProgram (VstInt32 program) override;
    void getProgramName (char* name) override;
    bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text) override;
};

#endif
