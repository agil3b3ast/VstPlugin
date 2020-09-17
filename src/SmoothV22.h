//
//  Smooth.h
//  VstPlugin
//
//  Created by Alessandro Fazio on 14/09/2020.
//
/*
#ifndef Smooth_h
#define Smooth_h

class Smooth { //one-pole lp
    float start;
    float end;
    bool isSmooth;
    float delay;
    
public:
    Smooth() {a0 = 1.0; b1 = 0.0; z1 = 0.0; start=0.0; end=0.0;isSmooth=false;};
    Smooth(double cutoff) {z1 = 0.0; setCutoff(cutoff);};
    ~Smooth();
    float smooth(float input);
    void setCutoff(double cutoff);
    bool process(float *input);
    void setStart(float input);
    void setEnd(float input);
    float getStart();
    float getEnd();
    
    
protected:
    double a0, b1, z1;
};

inline void Smooth::setCutoff(double cutoff) {
    //b1 = exp(-2.0 * M_PI * cutoff);
    //a0 = 1.0 - b1;
    delay = (float)cutoff;
}

inline float Smooth::smooth(float input) {
    //z1 = input * a0 + z1 * b1;
    //return z1;
    
}

inline void Smooth::setStart(float input){
    start = input;
}

inline void Smooth::setEnd(float input){
    end = input;
    isSmooth = true;
}

inline bool Smooth::process(float *input) {
    if(isSmooth){
        start = smooth(end);
        *input=start;
        if (abs(start) >= abs(end)){start = end;isSmooth=false;}
    }
    return isSmooth;
}

inline float Smooth::getStart(){
    return start;
}

inline float Smooth::getEnd(){
    return end;
}
 */

//#endif /* Smooth_h */
