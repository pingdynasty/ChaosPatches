#ifndef __HenonMapPatch_hpp__
#define __HenonMapPatch_hpp__

#include "StompBox.h"

/*
 * Non-linear oscillator based on the Hénon map, 
 * a discrete-time dynamical system which exhibits chaotic behavior.
 * http://en.wikipedia.org/wiki/H%C3%A9non_map
 * Interpolations from here
 * http://paulbourke.net/miscellaneous/interpolation/
 */
class HenonMapPatch : public Patch {
public:
  float y[4];
  float dt;
  float a = 1.4;
  float b = 0.3;
  const float minRate = 0.000001;
  const float maxRate = 0.0025; // try 0.5

  HenonMapPatch(){
    registerParameter(PARAMETER_A, "Rate");
    registerParameter(PARAMETER_B, "Alpha");
    registerParameter(PARAMETER_C, "Beta");
    registerParameter(PARAMETER_D, "Gain");
    reset();
  }

  void reset(){
    y[0] = 0.631354477;
    y[1] = 0.189406343;
    y[2] = 0;
    y[3] = 0;
  }

  float calc(float yn1, float yn2){
    return a * yn1*yn1 + b * yn2 + 1;
  }

  void iterate(){
    float yn = calc(y[0], y[1]);
    y[3] = y[2];
    y[2] = y[1];
    y[1] = y[0];
    y[0] = yn;
  }

  float incr = 0.0;
  void processAudio(AudioBuffer &buffer){
    if(isButtonPressed(PUSHBUTTON))
      reset();
    float step = getParameterValue(PARAMETER_A)*getParameterValue(PARAMETER_A)*maxRate + minRate;
    a = getParameterValue(PARAMETER_B)*-0.4 - 1.2;
    b = getParameterValue(PARAMETER_C)*0.45 - 0.26;
    float gain = getParameterValue(PARAMETER_D)*2.0;
    int size = buffer.getSize();
    float* left = buffer.getSamples(0);
    float* right = buffer.getSamples(1);
    for(int i=0; i<size; i++){
      left[i] = y[0]*gain;
      // right[i] = (y[0]*incr + y[1]*(1-incr))*gain;
      // right[i] = cosineInterpolate(y[0], y[1], incr)*gain;
      // right[i] = cubicInterpolate(y[0], y[1], y[2], y[3], incr)*gain;
      right[i] = splineInterpolate(y[0], y[1], y[2], y[3], incr)*gain;
      incr += step;
      if(incr > 1.0){
	iterate();
      	incr = .0;
      }
    }
    // debugMessage("x/y", x, y);
  }

  float cosineInterpolate(float y1,float y2, float mu){
    float mu2;
    mu2 = (1-cos(mu*PI))/2;
    return(y1*(1-mu2)+y2*mu2);
  }

  float cubicInterpolate(float y0, float y1, float y2, float y3, float mu){
    float a0,a1,a2,a3,mu2;
    mu2 = mu*mu;
    a0 = y3 - y2 - y0 + y1;
    a1 = y0 - y1 - a0;
    a2 = y2 - y0;
    a3 = y1;
    return(a0*mu*mu2+a1*mu2+a2*mu+a3);
  }

  float splineInterpolate(float y0, float y1, float y2, float y3, float mu){
    float a0,a1,a2,a3,mu2;
    mu2 = mu*mu;
    a0 = -0.5*y0 + 1.5*y1 - 1.5*y2 + 0.5*y3;
    a1 = y0 - 2.5*y1 + 2*y2 - 0.5*y3;
    a2 = -0.5*y0 + 0.5*y2;
    a3 = y1;
    return(a0*mu*mu2+a1*mu2+a2*mu+a3);
  }

};

#endif // __HenonMapPatch_hpp__
