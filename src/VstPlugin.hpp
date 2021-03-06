#ifndef VSTPLUGIN_HPP
#define VSTPLUGIN_HPP

#include "../vstsdk2.4/public.sdk/source/vst2.x/audioeffectx.h"
#include "BufferFactory.hpp"
#include "Oscillator.hpp"
#include "Chorus.hpp"
#include "VDelay.hpp"
#include "Smooth.hpp"
#include "AutoPan.hpp"



#include <memory>

#define PROGS_COUNT 5

const unsigned char numCharDisplay = 4;

struct Preset{
    char name[24];
    float feedback;
    float wetDry;
    float amount1;
    float amount2;
    float amount3;
    float frequencyInHz1;
    float frequencyInHz2;
    float frequencyInHz3;
    float panAmount;
    float panFrequency;
};


//-------------------------------------------------------------------------------------------------------

enum EfxParameter {
    GainLeft = 0,
    GainRight,
    DelayFeedback,
    WetDry,
    Amount1,
    Amount2,
    Amount3,
    FrequencyInHz1,
    FrequencyInHz2,
    FrequencyInHz3,
    PanAmount,
    PanFrequency,
    ParamCOUNT
};

//-------------------------------------------------------------------------------------------------------

struct SmoothParams{
    bool toSmooths[ParamCOUNT];
    Smooth *smooths[ParamCOUNT];
};

//-------------------------------------------------------------------------------------------------------
class VstPlugin : public AudioEffectX
{

    //presets
    Preset programs[PROGS_COUNT];

    //smooth
    SmoothParams smoothParams;

    //chorus (with gain inside)
    Chorus chorus;

    //AutoPan
    AutoPan autoPan;

    void initPresets();
    void initSmoothParams();
    void initSmoothParamValues();
    void initToSmooths();
    
    //plugin
    void initPlugin();

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
