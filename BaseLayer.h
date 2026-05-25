#ifndef BASELAYER_H
#define BASELAYER_H

#include "Matrix.h"

class BaseLayer {
public:

    virtual ~BaseLayer() {} 
    
    virtual Matrix forward(const Matrix& input) = 0; 
};

#endif