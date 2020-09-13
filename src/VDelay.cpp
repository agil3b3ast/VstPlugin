//
//  VDelay.cpp
//  VstPlugin
//
//  Created by Alessandro Fazio on 09/09/2020.
//

#include "VDelay.hpp"

VDelay::VDelay(float sampleRate): Delay(sampleRate), oscillator(sampleRate), modOperator(&oscillator){
    currentFractDelay = ((double)delayMaxSize)/2.0;
    modOperator.setMinAmount(currentFractDelay);
    writeCursor = 0; //cannot set writeCursor to max/2 due to precision errors
    readCursor = delayMaxSize-currentFractDelay; //it is necessary to start here to avoid writeCursor precision errors
    previousOutL = 0.0;
    previousOutR = 0.0;
    //BL = 0.7;
    //FF = 0.7;
    //FB = 0.7;
    //L=1.0/(1.0-abs(FF)); //L_inf
    //c = 1/L;
    nu=0.0;
}

double VDelay::getFrequencyInHz(){
    return modOperator.getOscillator()->getFrequencyInHz();
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
    int previous = floor(readCursor);
    int next = ceil(readCursor);
    
    int int_part = previous;
    double fract_part = readCursor-int_part;
    
    //if(previous < 0){ //realign previous
    //    previous = delayMaxSize -1;
    //}
    if(next == delayMaxSize){ //realign next
        next = 0;
    }

    //*oldestSampleL = bufferDelayL[previous] + (fract_part*(bufferDelayL[next]-bufferDelayL[previous]));
    //*oldestSampleR = bufferDelayR[previous] + (fract_part*(bufferDelayR[next]-bufferDelayR[previous]));

    *oldestSampleL = bufferDelayL[previous] + (fract_part*(bufferDelayL[next]-previousOutL));// - (1-fract_part)*previousOutL;
    *oldestSampleR = bufferDelayR[previous] + (fract_part*(bufferDelayR[next]-previousOutR));// - (1-fract_part)*previousOutR;
    
    //float lPrev = bufferDelayL[previous];//just for debug
    //float lNext = bufferDelayL[next];//just for debug
    
    //float rPrev = bufferDelayR[previous];//just for debug
    //float rNext = bufferDelayR[next];//just for debug
    
    
    //*oldestSampleL = lPrev + (nu*(lNext-previousOutL));// just for debug
    //*oldestSampleR = rPrev + (nu*(rNext-previousOutR));// just for debug
    
    previousOutL = *oldestSampleL; //all-pass interp
    previousOutR = *oldestSampleR; //all-pass interp
}


void VDelay::processDelay(float** inputs, float** outputs, VstInt32 sampleFrames){
    float *buffL = inputs[0]; // buffer input left
    float *buffR = inputs[1]; // buffer input right
    
    float *buffOutL = outputs[0]; // buffer output left
    float *buffOutR = outputs[1]; // buffer output right
    
    float wetDryBalance;
    
    double outCurrDelay = 0.0;
    float oldestSampleL = 0.0;
    float oldestSampleR = 0.0;
    
    //std::fstream fout;
    //std::fstream fout2;
    //fout.open("/Users/alessandro_fazio/Desktop/output_in.csv", std::ios::out | std::ios::app);
    //fout2.open("/Users/alessandro_fazio/Desktop/output_out.csv", std::ios::out | std::ios::app);
    
    for(int i=0; i<sampleFrames;i++){
        modOperator.updateModOperator();
        modOperator.processModOperator(&currentFractDelay, &outCurrDelay);
        //outCurrDelay = currentFractDelay + currentFractDelay*sin(2*pi*getFrequencyInHz());
        
        //oscillator.processOscillatorSingle(&buffOutL[i]);
        //oscillator.processOscillatorSingle(&buffOutR[i]);
        //modOperator.processModOperator(&delayCurrentSizeR, &outCurrDelay); TODO estendere a delay stereo
        
        //fout << "Iteration " << i << ":\n\t-Actual read cursor: " << std::to_string(readCursor)
        //<< "\n\t-Actual write cursor: " << std::to_string(writeCursor)
        //<< "\n\t-Actual out delay: " << std::to_string(outCurrDelay);
        
        readCursor = writeCursor - outCurrDelay;
        
        //fout << ":\n\t-Actual updated read cursor: " << std::to_string(readCursor) << '\n';
        
        
        realignReadCursor();
        
        nu = (1.0-outCurrDelay)/(1.0+outCurrDelay);
        
        calcOldestSample(&oldestSampleL, &oldestSampleR);
        
        //float oldestSampleL = bufferDelayL[delayCurrentSizeL];
        //float oldestSampleR = bufferDelayR[delayCurrentSizeR];
        
        
        bufferDelayL[writeCursor] = buffL[i]+oldestSampleL*delFeedbackL;
        bufferDelayR[writeCursor] = buffR[i]+oldestSampleR*delFeedbackR;

        writeCursor++;
        
        //TODO estendere a delay stereo
        if (writeCursor == delayMaxSize){
            writeCursor = 0;
        }
        
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

        
        
        
        //to add if want to debug
        /*bufferDelayL[writeCursor] = static_cast<float>(buffL[i]
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
        buffOutR[i] = wetDryBalance;*/
        
    }
    
    //fout.close();
    
}
