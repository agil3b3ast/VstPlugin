//
//  BufferFactory.cpp
//  VstPlugin
//
//  Created by Alessandro Fazio on 07/09/2020.
//

#include "BufferFactory.hpp"

void BufferFactory::deleteBuffer(float *buffer){
    
    if (buffer != nullptr) {
        delete buffer;
        buffer = nullptr;
    }
}

float* BufferFactory::createBuffer(float numberOfBytes){
    float *buffer = (float *) malloc(numberOfBytes);
    memset(buffer,0,numberOfBytes);
    return buffer;
}

void BufferFactory::deleteBufferDouble(double *buffer){
    
    if (buffer != nullptr) {
        delete buffer;
        buffer = nullptr;
    }
}

double* BufferFactory::createBufferDouble(double numberOfBytes){
    double *buffer = (double *) malloc(numberOfBytes);
    memset(buffer,0,numberOfBytes);
    return buffer;
}
