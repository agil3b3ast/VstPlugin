//
//  VDelay.cpp
//  VstPlugin
//
//  Created by Alessandro Fazio on 09/09/2020.
//

#include "VDelay.hpp"

VDelay::VDelay(float sampleRate): Delay(sampleRate), oscillator(sampleRate), modOperator(&oscillator), interp(2){
    initVDelay();
}

void VDelay::initVDelay(){
    minV = 0.001f * sampleRate; //suppose 1ms min delay
    //minV = 0.0;
    
    
    modOperator.setMinAmount((double) minV);
    modOperator.setMaxAmount((double)delayMaxSize);
    modOperator.getOscillator()->setSampleRate(sampleRate);
    //currentFractDelay = (modOperator.getMaxAmount()-modOperator.getMinAmount())/2.0;
    writeCursor = 0; //cannot set writeCursor to max/2 due to precision errors
    readCursor = 0.0;
    //previousOutL = 0.0;
    //previousOutR = 0.0;
    
    outCurrDelay = 0.0;
    oldestSampleL = 0.0;
    oldestSampleR = 0.0;
    
    //BL = 0.7;
    //FF = 0.7;
    //FB = 0.7;
    //L=1.0/(1.0-abs(FF)); //L_inf
    //c = 1/L;
    //nu=0.0;
    
    interp.setMax(delayMaxSize);
}

double VDelay::getMinV(){
    return minV;
}

double VDelay::getAmount(){
    return modOperator.getAmount();
}

void VDelay::setAmount(double amount){
    modOperator.setAmount(amount);
}

float VDelay::getOldestSampleL(){
    return oldestSampleL;
}

float VDelay::getOldestSampleR(){
    return oldestSampleR;
}

double VDelay::getFrequencyInHz(){
    return modOperator.getOscillator()->getFrequencyInHz();
}

double VDelay::getMaxFreq(){
    return modOperator.getOscillator()->getMaxFreq();
}

double VDelay::getMinFreq(){
    return modOperator.getOscillator()->getMinFreq();
}

void VDelay::setPhase(float phase){
    modOperator.getOscillator()->setPhase(phase);
}

void VDelay::setFrequencyInHz(double frequencyInHz){
    modOperator.getOscillator()->setFrequencyInHz(frequencyInHz);
}

void VDelay::realignReadCursor(){
    double dlength = static_cast<double>(delayMaxSize);
    if (readCursor <= 0.0){
        //while (readCursor < 0.0){
        readCursor = readCursor + dlength;
        //}
    }
    else if (readCursor == dlength){
        //while (readCursor >= delayMaxSize){
        readCursor = readCursor - dlength;
        //}
    }
}

void VDelay::calcOldestSample(float *oldestSampleL, float *oldestSampleR){

    interp.setInterp(readCursor);

    interp.interpAllPass(oldestSampleL, 0, bufferDelayL); //left
    interp.interpAllPass(oldestSampleR, 1, bufferDelayR); //right

}


void VDelay::processDelay(float** inputs, float** outputs, VstInt32 sampleFrames){
    float *buffL = inputs[0]; // buffer input left
    float *buffR = inputs[1]; // buffer input right

    float *buffOutL = outputs[0]; // buffer output left
    float *buffOutR = outputs[1]; // buffer output right

    float wetDryBalance;

    //std::fstream fout;
    //std::fstream fout2;
    //fout.open("/Users/alessandro_fazio/Desktop/output_in.csv", std::ios::out | std::ios::app);
    //fout2.open("/Users/alessandro_fazio/Desktop/output_out.csv", std::ios::out | std::ios::app);

    for(int i=0; i<sampleFrames;i++){
        tick(&buffL[i], &buffR[i]);

        /*
        if (delayCursorR >= outCurrDelay){
            delayCursorR = 0;
        }*/

        wetDryBalance = wetDry*buffL[i]+(1.0-wetDry)*oldestSampleL;
        //wetDryBalance = wetDryBalance*c;
        gainStereo.processGainL(&wetDryBalance);
        buffOutL[i] = wetDryBalance;

        wetDryBalance = wetDry*buffR[i]+(1.0-wetDry)*oldestSampleR;
        //wetDryBalance = wetDryBalance*c;
        gainStereo.processGainR(&wetDryBalance);
        buffOutR[i] = wetDryBalance;

        //fout << std::to_string(buffL[i]) << '\n';
        //fout2 << std::to_string(buffOutL[i]) << '\n';


    }

    //fout.close();
    //fout2.close();

}

void VDelay::processDelayBySample(float *inputL,float *inputR , float *outputL,float *outputR){

    float wetDryBalance;

    tick(inputL, inputR);

    wetDryBalance = *inputL*wetDry+(1.0-wetDry)*oldestSampleL;
    //wetDryBalance = wetDryBalance*c;
    gainStereo.processGainL(&wetDryBalance);
    *outputL = wetDryBalance;

    wetDryBalance = *inputR*wetDry+(1.0-wetDry)*oldestSampleR;
    //wetDryBalance = wetDryBalance*c;
    gainStereo.processGainR(&wetDryBalance);
    *outputR = wetDryBalance;


}

void VDelay::tick(float *inputL,float *inputR){
    modOperator.updateModOperator();
    modOperator.processModOperator(&outCurrDelay);


    readCursor = writeCursor - outCurrDelay;



    realignReadCursor();

    //nu = (1.0-outCurrDelay)/(1.0+outCurrDelay);

    calcOldestSample(&oldestSampleL, &oldestSampleR);


    bufferDelayL[writeCursor] = *inputL+oldestSampleL*delFeedbackL;
    bufferDelayR[writeCursor] = *inputR+oldestSampleR*delFeedbackR;

    writeCursor++;

    //TODO estendere a delay stereo
    if (writeCursor == delayMaxSize){
        writeCursor = 0;
    }

}


/*
void VDelay::processDelayAlt(float** inputs, float** outputs, VstInt32 sampleFrames){
    float *buffL = inputs[0]; // buffer input left
    float *buffR = inputs[1]; // buffer input right

    float *buffOutL = outputs[0]; // buffer output left
    float *buffOutR = outputs[1]; // buffer output right

    float wetDryBalance;

    double outCurrDelay = 0.0;
    float oldestSampleL = 0.0;
    float oldestSampleR = 0.0;

    //std::fstream fout;
    //fout.open("/Users/alessandro_fazio/Desktop/output.csv", std::ios::out | std::ios::app);

    for(int i=0; i<sampleFrames;i++){
        modOperator.updateModOperator();
        modOperator.processModOperator(&currentFractDelay, &outCurrDelay);


        double dlength = static_cast<double>(delayMaxSize);
        // read pointer is vlength ~behind~ write pointer
        double vlength = dlength -  outCurrDelay;
        // clip vdelayb to max del length
        vlength = vlength < dlength ? vlength : dlength;

        double readpos = writeCursor + vlength;
        unsigned long base_readpos = static_cast<unsigned long>(readpos);
        if(base_readpos >= delayMaxSize)
            base_readpos -= delayMaxSize;

        unsigned long next_index = base_readpos + 1;
        if(next_index >= delayMaxSize)
            next_index -= delayMaxSize;
        // basic interp of variable delay pos
        double frac = readpos - static_cast<int>(readpos);
        double outputL = bufferDelayL[base_readpos]+((bufferDelayL[next_index]
                                             - bufferDelayL[base_readpos]) * frac);
        double outputR = bufferDelayR[base_readpos]+((bufferDelayR[next_index]
                                                      - bufferDelayR[base_readpos]) * frac);




        bufferDelayL[writeCursor] = static_cast<float>(buffL[i]
                                                  + (FB * outputL));
        bufferDelayR[writeCursor] = static_cast<float>(buffR[i]
                                                         + (FB * outputR));
        writeCursor++;
        if(writeCursor == delayMaxSize)
            writeCursor = 0;


        wetDryBalance = BL*buffL[i]+FF*static_cast<float>(outputL);
        wetDryBalance=wetDryBalance*c;
        gainStereo.processGainL(&wetDryBalance);
        buffOutL[i] = wetDryBalance;

        wetDryBalance = BL*buffR[i]+FF*static_cast<float>(outputR);
        wetDryBalance=wetDryBalance*c;
        gainStereo.processGainR(&wetDryBalance);
        buffOutR[i] = wetDryBalance;

    }

    //fout.close();

}
*/
