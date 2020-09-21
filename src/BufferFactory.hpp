//
//  BufferFactory.hpp
//  VstPlugin
//
//  Created by Alessandro Fazio on 07/09/2020.
//

#ifndef BufferFactory_hpp
#define BufferFactory_hpp

#include <memory>

class BufferFactory{
    
    public:
        static void deleteBuffer(float *buffer);
        static float* createBuffer(float numberOfBytes);
        static void deleteBufferDouble(double *buffer);
        static double* createBufferDouble(double numberOfBytes);


};

inline void BufferFactory::deleteBuffer(float *buffer){
    
    if (buffer != nullptr) {
        delete buffer;
        buffer = nullptr;
    }
}

inline float* BufferFactory::createBuffer(float numberOfBytes){
    float *buffer = (float *) malloc(numberOfBytes);
    memset(buffer,0,numberOfBytes);
    return buffer;
}

inline void BufferFactory::deleteBufferDouble(double *buffer){
    
    if (buffer != nullptr) {
        delete buffer;
        buffer = nullptr;
    }
}

inline double* BufferFactory::createBufferDouble(double numberOfBytes){
    double *buffer = (double *) malloc(numberOfBytes);
    memset(buffer,0,numberOfBytes);
    return buffer;
}
#endif /* BufferFactory_hpp */
