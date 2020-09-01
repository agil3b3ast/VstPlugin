#ifndef VSTPLUGIN_HPP
#define VSTPLUGIN_HPP

#include "../vstsdk2.4/public.sdk/source/vst2.x/audioeffectx.h"


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

    float gainL;
    float gainR;

    //delay
    float *bufferDelayL;
    float *bufferDelayR;

    int delayCursorL;
    int delayCursorR;

    int delayMaxSize;
    int delayCurrentSizeL;
    int delayCurrentSizeR;

    float delFeedbackL;
    float delFeedbackR;

    float maxFeedback;

    float wetDry;

    //MIDI
    int midiNoteNumber;
    int midiNoteVelocity;
    int midiNoteDelta;
    bool isNoteOn;

    int midiControlNumber;
    int midiControlValue;
    int midiControlDelta;

    //Presets
    Preset programs[PROGS_COUNT];

    //wavetable
    float *sawtooth;
    float *pulse;
    const int WAVETABLE_SIZE = 2048;
    float pwm; // determina la simmetria dell'onda quadra
    double freqTable[128];
    double cursorTable;
    double fScale;

    void initPlugin();

    void deleteBuffers();
    void createBuffers();

    void noteOn(int note, int velocity, int deltaEvent);
    void noteOff(int note, int velocity, int deltaEvent);
    void controlChange(int controlNumber, int controlValue, int deltaEvent);

    void createWavetables();
    void deleteWavetables();
    void createFrequencyTable();

    void initPresets();

public:

	explicit VstPlugin(audioMasterCallback audioMaster);
	~VstPlugin();

	// Processing
	void processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames) override;
    void processDelay(float** inputs, VstInt32 sampleFrames);
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

    VstInt32 processEvents (VstEvents* events) override;

    void setProgram (VstInt32 program) override;
    void getProgramName (char* name) override;
    bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text) override;
};

#endif
