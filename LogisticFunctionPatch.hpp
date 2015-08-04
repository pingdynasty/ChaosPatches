#ifndef __LogisticFunctionPatch_hpp__
#define __LogisticFunctionPatch_hpp__

#include "StompBox.h"

/*
 * Chaotic waveshaper based on iterated logistic map
 * http://en.wikipedia.org/wiki/Logistic_map
 * http://en.wikipedia.org/wiki/Feigenbaum_constants
 */
class LogisticFunctionPatch : public Patch {
private:
  // BiquadFilter* lowpass;
  const float minR = 2.95;
  const float maxR = 3.95;
  const float maxI = 40.0;
public:

  LogisticFunctionPatch(){
    // lowpass = BiquadFilter::create(2); // four-pole filter
    // lowpass->setLowPass(0.99, FilterStage::BUTTERWORTH_Q); // Fc near Nyquist

    registerParameter(PARAMETER_A, "Pregain");
    registerParameter(PARAMETER_B, "r");
    registerParameter(PARAMETER_C, "Iterations");
    registerParameter(PARAMETER_D, "");
  }

  /* the logistic map
   * https://en.wikipedia.org/wiki/Logistic_map
   */
  float lf(float x, float r){
    return r*x*(1-x);
  }

  float ilf(float nf, float x, float r){
    unsigned int n = floor(nf);
    nf = nf - n;
    while(n--)
      x = lf(x, r);
    float xn = lf(x, r);
    return (x*(1.0-nf) + xn*nf)/2;
    // return x;
  }

  void processAudio(AudioBuffer &buffer){
    float gain = getParameterValue(PARAMETER_A);
    gain = gain*gain*2.0;
    float r = getParameterValue(PARAMETER_B)*(maxR-minR) + minR;
    float iterations = getParameterValue(PARAMETER_C);
    iterations = iterations*iterations*maxI;
    int size = buffer.getSize();
    float* left = buffer.getSamples(0);
    float* right = buffer.getSamples(1);
    for(int i=0; i<size; i++){
      float x = left[i] * gain;
      if(x < 0.0)
	left[i] = -ilf(iterations, -x, r);
      else
	left[i] = ilf(iterations, x, r);
      right[i] = left[i];
    }
    // right.copyFrom(left, size);
    // lowpass->process(right);
  }

};

#endif // __LogisticFunctionPatch_hpp__
