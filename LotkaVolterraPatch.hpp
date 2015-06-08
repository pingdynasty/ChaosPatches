#ifndef __LotkaVolterraPatch_hpp__
#define __LotkaVolterraPatch_hpp__

#include "StompBox.h"

/*
 * http://en.wikipedia.org/wiki/Lotka%E2%80%93Volterra_equation
 * http://www.tiem.utk.edu/~gross/bioed/bealsmodules/predator-prey.html
 * http://www.r-bloggers.com/lotka-volterra-model%C2%A0%C2%A0intro/
 */

class LotkaVolterraPatch : public Patch {
private:
  float a = 0.4;   // Rabbit growth
  float b = 0.002; // Rabbit death
  float c = 0.002; // Fox growth
  float d = 0.9;   // Fox death

  float step = 0.1;
  float x0;
  float y0;

public:
  LotkaVolterraPatch(){
    // registerParameter(PARAMETER_A, "Rate");
    // registerParameter(PARAMETER_B, "Rabbit/Fox -");
    // registerParameter(PARAMETER_C, "Rabbit/Fox +");
    registerParameter(PARAMETER_A, "Rabbit +");
    registerParameter(PARAMETER_B, "Rabbit -");
    registerParameter(PARAMETER_C, "Fox +");
    registerParameter(PARAMETER_D, "Fox -");
    reset();
  }

  void reset(){
    x0 = (1-0.2)*1000;
    y0 = 0.2*1000;
    setButton(PUSHBUTTON, false);
  }

  void processAudio(AudioBuffer &buffer){
    // a = getParameterValue(PARAMETER_C)*0.8+0.1;
    // c = (1-getParameterValue(PARAMETER_C))*0.004+0.0001;
    // b = getParameterValue(PARAMETER_B)*0.004+0.0001;
    // d = (1-getParameterValue(PARAMETER_B))*1.8+0.1;
    // a += getParameterValue(PARAMETER_D) ;

    a = getParameterValue(PARAMETER_A)*1.8+0.01;
    b = getParameterValue(PARAMETER_B)*0.004+0.0001;
    c = getParameterValue(PARAMETER_C)*0.004+0.0001;
    d = getParameterValue(PARAMETER_D)*1.8+0.001;

    if(isButtonPressed(PUSHBUTTON))
      reset();

    int size = buffer.getSize();
    float* left = buffer.getSamples(0);
    float* right = buffer.getSamples(1);
    // step = getParameterValue(PARAMETER_A)*.6;

    const float xgain = 2.0/1000.;
    const float ygain = 2.0/1000.;
    float dx, dy;
    for(int i=0; i<size; i++){
      left[i] = x0 * xgain - 1.0;
      right[i] = y0 * ygain - 1.0;
      dx = dxdt(x0, y0);
      dy = dydt(x0, y0);
      x0 += step*dx;
      y0 += step*dy;
      // prevent exctinction
      x0 = max(10, x0);
      y0 = max(10, y0);
      // left[i] = dx;
      // right[i] = dy;
    }
    debugMessage("x/y", x0, y0);
    // debugMessage("dx/dy", dx, dy);
    // debugMessage("xgain/xavg", xgain, xavg);
    // debugMessage("x/y", x0, y0);
  }

  float dxdt(float x, float y){
    return a*x - b*y*x;
  }

  float dydt(float x, float y){
    return c*x*y - d*y;
  }
};

#endif // __LotkaVolterraPatch_hpp__
