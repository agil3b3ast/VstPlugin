#ifndef VSTPLUGIN_HPP
#define VSTPLUGIN_HPP

#include "../vstsdk2.4/public.sdk/source/vst2.x/audioeffectx.h"
#include "BufferFactory.hpp"
#include "Oscillator.hpp"
#include "VDelay.hpp"



#include <memory>

#define PROGS_COUNT 5

struct Preset{
    char name[24];
    float feedbackL;
    float feedbackR;
    float delayTimeL;
    float delayTimeR;
    float wetDry;
};


//-------------------------------------------------------------------------------------------------------

enum EfxParameter {
    GainLeft = 0,
    GainRight,
    DelaySizeL,
    DelaySizeR,
    DelayFeedbackL,
    DelayFeedbackR,
    WetDry,
    ParamCOUNT
};

//-------------------------------------------------------------------------------------------------------
class VstPlugin : public AudioEffectX
{

    //gain is inside delay


    //Presets
    Preset programs[PROGS_COUNT];

    //plugin
    void initPlugin();

    //oscillators
    Oscillator oscillator;
    
    //delay
    VDelay delay;
    
    //presets
    void initPresets();

public:

	explicit VstPlugin(audioMasterCallback audioMaster);
	~VstPlugin();

	// Processing
	void processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames) override;
    void processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames) override;
    void setParameter (VstInt32 index, float value) override;
    float getParameter (VstInt32 index) override;

    bool getEffectName (char* name) override;
    bool getVendorString (char* text) override;
    bool getProductString (char* text) override;


    void getParameterLabel (VstInt32 index, char* label) override;
    void getParameterDisplay (VstInt32 index, char* text) override;
    void getParameterName (VstInt32 index, char* text) override;

    void setSampleRate (float sampleRate) override;

    void setProgram (VstInt32 program) override;
    void getProgramName (char* name) override;
    bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text) override;
};

#endif
