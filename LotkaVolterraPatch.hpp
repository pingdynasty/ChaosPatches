#ifndef __LotkaVolterraPatch_hpp__
#define __LotkaVolterraPatch_hpp__

#include "StompBox.h"

/*
 * The Lotka Volterra model is a pair of differential equations that describe
 * two interacting species, one as prey, the other as predator (Rabbits and Foxes).
 * The left and right input values affect the current population levels, as does 
 * the birth and death rates of the two species. 
 * Rabbits on the left channel, Foxes on the right.
 * Time: Rate of change
 * Migration: The amount that the population is affected by external input
 * Rabbit/Fox +: Birth rate for prey / predator
 * Rabbit/Fox -: Death rate for prey / predator
 * http://en.wikipedia.org/wiki/Lotka%E2%80%93Volterra_equation
 * http://www.tiem.utk.edu/~gross/bioed/bealsmodules/predator-prey.html
 * http://www.r-bloggers.com/lotka-volterra-model%C2%A0%C2%A0intro/
 */
class LotkaVolterraPatch : public Patch {
private:
  const float minpop = 20.0;// minimum population
  float a = 0.4;            // Rabbit growth
  float b = 0.002;          // Rabbit death
  float c = 0.002;          // Fox growth
  float d = 0.9;            // Fox death
  float step = 0.1;
  float migration = 10.0;
  float x, y;

public:
  LotkaVolterraPatch(){
    registerParameter(PARAMETER_A, "Time");
    registerParameter(PARAMETER_B, "Migration");
    registerParameter(PARAMETER_C, "Birth");
    registerParameter(PARAMETER_D, "Death");
    x = (1-0.2)*1000;
    y = 0.2*1000;
  }

  void processAudio(AudioBuffer &buffer){
    step = getParameterValue(PARAMETER_A);
    step = step * step * step * 3;
    migration = getParameterValue(PARAMETER_B) * getParameterValue(PARAMETER_B) * 200.0;
    a = getParameterValue(PARAMETER_C)*0.8+0.1;
    c = (1-getParameterValue(PARAMETER_C))*0.004+0.0001;
    b = getParameterValue(PARAMETER_D)*0.004+0.0001;
    d = (1-getParameterValue(PARAMETER_D))*1.8+0.1;
    int size = buffer.getSize();
    float* rabbits = buffer.getSamples(0);
    float* foxes = buffer.getSamples(1);
    const float xgain = 2.0/1000.;
    const float ygain = 2.0/1000.;
    float dx, dy;
    for(int i=0; i<size; i++){
      dx = dxdt(x, y);
      dy = dydt(x, y);
      x += step*dx + migration*rabbits[i];
      y += step*dy + migration*foxes[i];
      // prevent extinction
      x = max(minpop, x);
      y = max(minpop, y);
      rabbits[i] = x * xgain - 1.0; // centre around 0.0
      foxes[i] = y * ygain - 1.0;
    }
  }

  float dxdt(float x, float y){
    return a*x - b*y*x;
  }

  float dydt(float x, float y){
    return c*x*y - d*y;
  }
};

#endif // __LotkaVolterraPatch_hpp__
