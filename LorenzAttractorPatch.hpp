#ifndef __LorenzAttractorPatch_hpp__
#define __LorenzAttractorPatch_hpp__

#include "StompBox.h"

class LorenzAttractorPatch : public Patch {
public:
  double x, y, z, dt;
  double a = 10.0;
  double b = 8.0/3.0;
  double c = 28.0;

  LorenzAttractorPatch(){
    registerParameter(PARAMETER_A, "Rate");
    registerParameter(PARAMETER_C, "X");
    registerParameter(PARAMETER_D, "Y");
    reset();
  }

  void reset(){
    x = 0.1;
    y = 0.0;
    z = 0.0;
  }

  void processAudio(AudioBuffer &buffer){
    if(isButtonPressed(PUSHBUTTON))
      reset();
    dt = getParameterValue(PARAMETER_A)*getParameterValue(PARAMETER_A)*0.0004;
    double gainL = getParameterValue(PARAMETER_C)*2/25.0;
    double gainR = getParameterValue(PARAMETER_D)*2/25.0;
    int size = buffer.getSize();
    float* left = buffer.getSamples(0);
    float* right = buffer.getSamples(1);
    double dx, dy, dz;
    for(int i=0;i<size;i++){
      dx = a*(y - x);
      dy = (x * (c - z) - y);
      dz = (x*y - b * z);
      x += dx*dt;
      y += dy*dt;
      z += dz*dt;
      left[i] = x * gainL;
      right[i] = y * gainR;
    }
  }

};

#endif // __LorenzAttractorPatch_hpp__
