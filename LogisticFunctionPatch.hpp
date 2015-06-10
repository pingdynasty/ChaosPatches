#ifndef __LogisticFunctionPatch_hpp__
#define __LogisticFunctionPatch_hpp__

#include "StompBox.h"

/*
 * Iterated logistic map
 * http://en.wikipedia.org/wiki/Logistic_map
 * http://en.wikipedia.org/wiki/Feigenbaum_constants
 */
class LogisticFunctionPatch : public Patch {
public:
  const float minR = 2.95;
  const float maxR = 3.95;
  const float maxI = 60;

  LogisticFunctionPatch(){
    registerParameter(PARAMETER_B, "r");
    registerParameter(PARAMETER_C, "Iterations");
    registerParameter(PARAMETER_D, "Gain");
  }

  float calc(float x, float r){
    return r*x*(1-x);
  }

  void processAudio(AudioBuffer &buffer){
    float r = getParameterValue(PARAMETER_B)*(maxR-minR) + minR;
    int iterations = getParameterValue(PARAMETER_C)*maxI+1;
    float gain = getParameterValue(PARAMETER_D)*2.0;
    int size = buffer.getSize();
    float* samples = buffer.getSamples(0);
    for(int i=0; i<size; i++){
      float x = samples[i];
      for(int j=0; j<iterations; ++j)
	x = calc(x, r);
      samples[i] = x*gain;
    }
  }

};

#endif // __LogisticFunctionPatch_hpp__
