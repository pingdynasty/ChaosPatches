#ifndef __HenonMapPatch_hpp__
#define __HenonMapPatch_hpp__

#include "StompBox.h"

/*
 * Non-linear oscillator based on the Hénon map, a discrete-time dynamical system 
 * which exhibits chaotic behavior.
 * http://en.wikipedia.org/wiki/H%C3%A9non_map
 * Interpolations from here
 * http://paulbourke.net/miscellaneous/interpolation/
 */

class HenonMapPatch : public Patch {
public:
  double y[4];
  double dt;
  double a = 1.4;
  double b = 0.3;

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

  double calc(double yn1, double yn2){
    return a * yn1*yn1 + b * yn2 + 1;
  }

  void iterate(){
    double yn = calc(y[0], y[1]);
    y[3] = y[2];
    y[2] = y[1];
    y[1] = y[0];
    y[0] = yn;
  }

  double cosineInterpolate(double y1,double y2, double mu){
    double mu2;
    mu2 = (1-cos(mu*PI))/2;
    return(y1*(1-mu2)+y2*mu2);
  }

  double cubicInterpolate(double y0, double y1, double y2, double y3, double mu){
    double a0,a1,a2,a3,mu2;
    mu2 = mu*mu;
    a0 = y3 - y2 - y0 + y1;
    a1 = y0 - y1 - a0;
    a2 = y2 - y0;
    a3 = y1;
    return(a0*mu*mu2+a1*mu2+a2*mu+a3);
  }

  double splineInterpolate(double y0, double y1, double y2, double y3, double mu){
    double a0,a1,a2,a3,mu2;
    mu2 = mu*mu;
    a0 = -0.5*y0 + 1.5*y1 - 1.5*y2 + 0.5*y3;
    a1 = y0 - 2.5*y1 + 2*y2 - 0.5*y3;
    a2 = -0.5*y0 + 0.5*y2;
    a3 = y1;
    return(a0*mu*mu2+a1*mu2+a2*mu+a3);
  }

  float incr = 0.0;
  void processAudio(AudioBuffer &buffer){
    if(isButtonPressed(PUSHBUTTON))
      reset();
    a = getParameterValue(PARAMETER_B)*-0.4 - 1.2;
    b = getParameterValue(PARAMETER_C)*0.45 - 0.26;
    double gain = getParameterValue(PARAMETER_D)*2.0;
    int size = buffer.getSize();
    float* left = buffer.getSamples(0);
    float* right = buffer.getSamples(1);
    float step = getParameterValue(PARAMETER_A)*getParameterValue(PARAMETER_A)*0.3;
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
      	// incr -= 1.0;
      }
    }
    // debugMessage("x/y", x, y);
    // debugMessage("a/b", (float)a, (float)b);
  }

};

#endif // __HenonMapPatch_hpp__
