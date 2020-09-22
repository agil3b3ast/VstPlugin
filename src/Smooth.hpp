//
//  Smooth.h
//  VstPlugin-test
//
//  Created by Alessandro Fazio on 16/09/2020.
//  Copyright © 2020 Alessandro Fazio. All rights reserved.
//

#ifndef Smooth_h
#define Smooth_h

class Smooth { //ramp from start to end in delaytime
    float start;
    float end;
    float delaytime;
    float sampleRate;
    bool isSmooth;
    int samples;
    int currentIndex;
    float currentSampleInPath;
    float delta;
    //bool minBound;
    //float minEnd;
    
public:
    explicit Smooth(float delaytime, float sampleRate);
    ~Smooth(){};
    void setSampleRate(float sampleRate);
    float smooth(float input);
    bool process(float *input);
    void setStart(float input);
    void setEnd(float input);
    void startSmoothPath(float start, float end);
    void setOtherSmoothPath(float input);
    bool getIsSmooth() const;
    void setIsSmooth(bool isSmooth);
    float getStart() const;
    float getEnd() const;
    //void setMinEnd(float minEnd);
    //void setMinBound(bool minBound);

    
};

inline Smooth::Smooth(float delaytime, float sampleRate) {
    this->delaytime = delaytime;
    this->sampleRate=sampleRate;
    
    start=0.0;
    end=0.0;
    isSmooth=false;
    samples=delaytime*sampleRate;
    currentIndex=1;
    currentSampleInPath = 0.0;
    delta = 0.0;
    //minBound = false;
    //minEnd = 0.0;
};

//-------------------------------------------------------------------------------------------------------

inline void Smooth::setSampleRate(float sampleRate){
    this->sampleRate = sampleRate;
    samples = delaytime*sampleRate;
}

//-------------------------------------------------------------------------------------------------------

inline void Smooth::setStart(float input){
    //minBound ? (input < minEnd ? start=minEnd : start=input) : start=input;
    start=input;
    currentSampleInPath = start;
}
//-------------------------------------------------------------------------------------------------------

inline void Smooth::setEnd(float input){
    //minBound ? (input < minEnd ? end=minEnd : end = input) : end = input;
    end = input;
    currentIndex = 1;
    isSmooth = true;
    delta = abs(end - start); //this is the absolute distance between start and end
}
//-------------------------------------------------------------------------------------------------------

/*inline void Smooth::setMinEnd(float minEnd){
    this->minEnd = minEnd;
}*/
//-------------------------------------------------------------------------------------------------------

/*inline void Smooth::setMinBound(bool minBound){
    this->minBound = minBound;
}*/
//-------------------------------------------------------------------------------------------------------

inline void Smooth::setIsSmooth(bool isSmooth){
    this->isSmooth = isSmooth;
}
//-------------------------------------------------------------------------------------------------------

inline bool Smooth::getIsSmooth() const{
    return isSmooth;
}
//-------------------------------------------------------------------------------------------------------

inline float Smooth::getStart() const{
    return start;
}
//-------------------------------------------------------------------------------------------------------

inline float Smooth::getEnd() const{
    return end;
}
//-------------------------------------------------------------------------------------------------------

inline void Smooth::startSmoothPath(float start, float end){
    setStart(start);
    setEnd(end);
}
//-------------------------------------------------------------------------------------------------------

inline void Smooth::setOtherSmoothPath(float input){ //continue smoooth path with other end
    setStart(currentSampleInPath);
    setEnd(input);
}
//-------------------------------------------------------------------------------------------------------

inline float Smooth::smooth(float input) {
    float currentStep = currentIndex/(float)samples;
    
    currentIndex++;
    
    currentSampleInPath = end < start ? start - currentStep*(delta) : start + currentStep*(delta);
    return currentSampleInPath;
}
//-------------------------------------------------------------------------------------------------------

inline bool Smooth::process(float *input) {
    bool processed = isSmooth;
    
    if(isSmooth){
        *input = smooth(end);
        if (currentIndex == samples+1){
            isSmooth=false;
        }
    }
    return processed;
}

#endif /* Smooth_h */
