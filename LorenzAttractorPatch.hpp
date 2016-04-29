#ifndef __LorenzAttractorPatch_hpp__
#define __LorenzAttractorPatch_hpp__

#include "StompBox.h"

/*
 * The Lorenz attractor is a set of chaotic solutions of the Lorenz system.
 * http://en.wikipedia.org/wiki/Lorenz_system
 */
class LorenzAttractorPatch : public Patch {
public:
  float x, y, z;
  float dt;
  const float a = 10.0;
  const float b = 8.0/3.0;
  const float c = 28.0;
  float P[3];
  float Pprime[3];
  float R[3][3];
  LorenzAttractorPatch(){
    registerParameter(PARAMETER_A, "Rate");
    registerParameter(PARAMETER_B, "Rotate X");
    registerParameter(PARAMETER_C, "Rotate Y");
    registerParameter(PARAMETER_D, "Zoom");
    registerParameter(PARAMETER_E, "Rotate Z");
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
    dt = getParameterValue(PARAMETER_A)*getParameterValue(PARAMETER_A)*0.0250;
    float rotateX = getParameterValue(PARAMETER_B)*M_PI;
    float rotateY = getParameterValue(PARAMETER_C)*M_PI;
    float rotateZ = getParameterValue(PARAMETER_E)*M_PI;
    float gainL, gainR;
    gainL = gainR = getParameterValue(PARAMETER_D)*2/25.0;
    int size = buffer.getSize();
    float* left = buffer.getSamples(0);
    float* right = buffer.getSamples(1);
    float dx, dy, dz;
    updateMatrix(rotateX, rotateY, rotateZ);
    for(int i=0;i<size;i++){
      dx = a*(y - x);
      dy = (x * (c - z) - y);
      dz = (x*y - b * z);
      x += dx*dt;
      y += dy*dt;
      z += dz*dt;
      P[0] = x;
      P[1] = y;
      P[2] = z-25; // centre on z axis
      rotateP();
      left[i] = Pprime[0] * gainL;
      right[i] = Pprime[1] * gainR;
    }
    // debugMessage("x/y/z", x, y, z);
  }
  
  void updateMatrix(float rotateX, float rotateY, float rotateZ){
  // rotation around the x axis
    float alpha = rotateX;
  // rotation around the y axis
    float beta = rotateY;
  // rotation around the z axis
    float gamma = rotateZ;
    float sa = sinf(alpha);
    float sb = sinf(beta);
    float sg = sinf(gamma);
    float ca = cosf(alpha);
    float cb = cosf(beta);
    float cg = cosf(gamma);

    // compute rotation matrix.
    // We rotate about x, then y, then z
    R[0][0] = cb * cg;
    R[0][1] = cg * sa * sb - ca * sg;
    R[0][2] = ca * cg * sb + sa * sg;
    
    R[1][0] = cb * sg;
    R[1][1] = ca * cg + sa * sb * sg;
    R[1][2] = -cg * sa + ca * sb * sg;
   
    R[2][0] = -sb;
    R[2][1] = cb * sa;
    R[2][2] = ca * cb;
  }
  
  void rotateP(){
   // apply rotation matrix 
    for(int n = 0; n < 3; ++n){
      float value = 0;
      for(int k = 0; k < 3; ++k){
        value += R[n][k] * P[k];
      }
      Pprime[n] = value;
    }
  }

};

#endif // __LorenzAttractorPatch_hpp__
