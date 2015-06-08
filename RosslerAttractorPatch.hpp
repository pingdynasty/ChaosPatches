#ifndef __RosslerAttractorPatch_hpp__
#define __RosslerAttractorPatch_hpp__

#include "StompBox.h"

/*
 * Rössler attractor
 * http://en.wikipedia.org/wiki/R%C3%B6ssler_attractor
 */
class RosslerAttractorPatch : public Patch {
public:
  double x,y,z;
  double a = .1;
  double b = .1;
  double c = 18;

  double dt = 0.01;

  RosslerAttractorPatch(){
    registerParameter(PARAMETER_A, "Rate");
    registerParameter(PARAMETER_B, "Chaos");
    registerParameter(PARAMETER_C, "X");
    registerParameter(PARAMETER_D, "Y");
    reset();
  }

  void reset(){
    x = 0.1;
    y = 0.0;
    z = 0.0;
  }

  double dxdt(double x, double y, double z){
    return -y-z;
  }

  double dydt(double x, double y, double z){
    return x+a*y;
  }

  double dzdt(double x, double y, double z){
    return b+z*(x-c);
  }

  void processAudio(AudioBuffer &buffer){
    if(isButtonPressed(PUSHBUTTON))
      reset();
    dt = getParameterValue(PARAMETER_A)*getParameterValue(PARAMETER_A)*0.002;
    c = getParameterValue(PARAMETER_B)*14.0+4.0;
    double gainL = getParameterValue(PARAMETER_C)*2.0/25.0;
    double gainR = getParameterValue(PARAMETER_D)*2.0/25.0;
    int size = buffer.getSize();
    float* left = buffer.getSamples(0);
    float* right = buffer.getSamples(1);
    double dx, dy, dz;
    for(int i=0;i<size;i++){
      dx = dxdt(x, y, z);
      dy = dydt(x, y, z);
      dz = dzdt(x, y, z);
      x += dx*dt;
      y += dy*dt;
      z += dz*dt;
      // left[i] = (x/z) * gainL;
      // right[i] = (y/z) * gainR;
      left[i] = x * gainL;
      right[i] = y * gainR;
    }
    // debugMessage("x/y/z", x, y, z);
  }

};

#endif // __RosslerAttractorPatch_hpp__
