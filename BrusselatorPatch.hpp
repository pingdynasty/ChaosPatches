#ifndef __BrusselatorPatch_hpp__
#define __BrusselatorPatch_hpp__

#include "StompBox.h"

/* 
 * The BrusselatorPatch is a non-linear oscillator exhibiting chaotic behaviour.
 * It is based on a theoretical model of an autocatalytic chemical reaction.
 * The Brusselator model was conceived by Ilya Prigogine, the brilliant Russian 
 * chemist and Nobel Laureate.
 * http://en.wikipedia.org/wiki/Brusselator
 * Computations borrowed from https://gist.github.com/FreeCX/9368108 
 */
class BrusselatorPatch : public Patch {
private:
  float x00 = 1;
  float y00 = 1;
  float xv00 = 3;
  float yv00 = 3;
  float a = 1;
  float b = 3;
  float h = 0.02;
  float xr, xv, yr, yv;
  float k[4], l[4], n[4], m[4];

public:
  BrusselatorPatch(){
    registerParameter(PARAMETER_A, "h");
    registerParameter(PARAMETER_B, "B");
    registerParameter(PARAMETER_C, "X Gain");
    registerParameter(PARAMETER_D, "Y Gain");
    reset();
  }

  void processAudio(AudioBuffer &buffer){
    if(isButtonPressed(PUSHBUTTON))
      reset();
    int size = buffer.getSize();
    float* left = buffer.getSamples(0);
    float* right = buffer.getSamples(1);
    h = getParameterValue(PARAMETER_A)*0.07;
    b = getParameterValue(PARAMETER_B)*3+1;
    float xgain = getParameterValue(PARAMETER_C)*0.1;
    float ygain = getParameterValue(PARAMETER_D)*2;
    ASSERT(buffer.getChannels() == 2, "Patch requires stereo out");
    for(int i=0; i<size; i+=2){
      left[i] = xr * xgain;
      right[i] = yr * ygain;
      iterate();
      left[i+1] = (left[i] + xr * xgain)*0.5;
      right[i+1] = (right[i] + yr * ygain)*0.5;
    }
  }

  void reset(){
    xr = x00;
    xv = xv00;
    yr = y00;
    yv = yv00;
  }

  float X(float x, float y){
    return a - ( b + 1 ) * x + (x*x) * y;
  }

  float Y(float x, float y){
    return b * x - (x*x) * y;
  }

  void iterate(){
    k[0] = xv * h;
    l[0] = X( xr, yr ) * h;
    m[0] = yv * h;
    n[0] = Y( xr, yr ) * h;
    k[1] = ( xv + 0.5 * l[0] ) * h;
    l[1] = X( xr + 0.5 * k[0], yr + 0.5 * m[0] ) * h;
    m[1] = ( yr + 0.5 * n[0] ) * h;
    n[1] = Y( xr + 0.5 * k[0], yr + 0.5 * m[0] ) * h;
    k[2] = ( xv + 0.5 * l[1] ) * h;
    l[2] = X( xr + 0.5 * k[1], yr + 0.5 * m[1] ) * h;
    m[2] = ( yv + 0.5 * n[1] ) * h;
    n[2] = Y( xr + 0.5 * k[1], yr + 0.5 * m[1] ) * h;
    k[3] = ( xv + l[2] ) * h;
    l[3] = X( xr + k[2], yr + m[2] ) * h;
    m[3] = ( yv + n[2] ) * h;
    n[3] = Y( xr + k[2], yr + m[2] ) * h;
    xr = xr + ( 1.0 / 6.0 ) * ( k[0] + 2*k[1] + 2*k[2] + k[3] );
    xv = xv + ( 1.0 / 6.0 ) * ( l[0] + 2*l[1] + 2*l[2] + l[3] );
    yr = yr + ( 1.0 / 6.0 ) * ( m[0] + 2*m[1] + 2*m[2] + m[3] );
    yv = yv + ( 1.0 / 6.0 ) * ( n[0] + 2*n[1] + 2*n[2] + n[3] );
  }
};

#endif // __BrusselatorPatch_hpp__
