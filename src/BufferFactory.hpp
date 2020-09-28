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
        static float* createBuffer(unsigned int numberOfBytes);
        static void deleteBuffer(double *buffer);
        static double* createBufferDouble(unsigned int numberOfBytes);


};
//-----------------------------------------------------------------------------------------

inline void BufferFactory::deleteBuffer(float *buffer){
    
    if (buffer != nullptr) {
        delete buffer;
        buffer = nullptr;
    }
}
//-----------------------------------------------------------------------------------------

inline float* BufferFactory::createBuffer(unsigned int numberOfBytes){
    float *buffer = (float *) malloc(numberOfBytes);
    memset(buffer,0,numberOfBytes);
    return buffer;
}
//-----------------------------------------------------------------------------------------

inline void BufferFactory::deleteBuffer(double *buffer){
    
    if (buffer != nullptr) {
        delete buffer;
        buffer = nullptr;
    }
}
//-----------------------------------------------------------------------------------------

inline double* BufferFactory::createBufferDouble(unsigned int numberOfBytes){
    double *buffer = (double *) malloc(numberOfBytes);
    memset(buffer,0,numberOfBytes);
    return buffer;
}
#endif /* BufferFactory_hpp */
