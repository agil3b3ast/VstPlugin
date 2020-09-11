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


#endif /* BufferFactory_hpp */
