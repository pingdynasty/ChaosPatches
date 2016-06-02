#ifndef __ChuaCircuitPatch_h__
#define __ChuaCircuitPatch_h__

#include "StompBox.h"

// http://www.chuacircuits.com/matlabsim.php
// %----------Chua.m----------
// function out = chua(t,in)

// x = in(1);
// y = in(2);
// z = in(3);

// alpha  = 15.6;
// beta   = 28; 
// m0     = -1.143;
// m1     = -0.714;

// h = m1*x+0.5*(m0-m1)*(abs(x+1)-abs(x-1));

// xdot = alpha*(y-x-h);
// ydot = x - y+ z;
// zdot  = -beta*y;

// out = [xdot ydot zdot]';
// %----------StartChua.m----------
// [t,y] = ode45(@chua,[0 100],[0.7 0 0]);
// plot3(y(:,1),y(:,2),y(:,3))
// grid

class ChuaCircuitPatch : public Patch {
private:
public:
  ChuaCircuitPatch(){
    registerParameter(PARAMETER_A, "Rate");
    registerParameter(PARAMETER_C, "Gain X");
    registerParameter(PARAMETER_D, "Gain Y");
  }
  float alpha = 15.6;
  float beta = 28;
  const float m0 = -1.143;
  const float m1 = -0.714;
  float x = 0.7;
  float y = 0;
  float z = 0;
  void reset(){
    x = 0.7;
    y = 0;
    z = 0;
  }
  float h(float x){
    return m1*x+0.5*(m0-m1)*(abs(x+1)-abs(x-1));
  }
  float xdot(float x, float y, float z){
    return alpha*(y-x-h(x));
  }
  float ydot(float x, float y, float z){
    return x - y + z;
  }
  float zdot(float x, float y, float z){
    return -beta*y;
  }
  void processAudio(AudioBuffer &buffer){
    float dt = getParameterValue(PARAMETER_A);
    float gainL = getParameterValue(PARAMETER_C)*2.0/25.0;
    float gainR = getParameterValue(PARAMETER_D)*2.0/25.0;
    dt = dt*dt*0.01;
    FloatArray left = buffer.getSamples(LEFT_CHANNEL);
    FloatArray right = buffer.getSamples(RIGHT_CHANNEL);
    for(int i=0; i<buffer.getSize(); ++i){
      float dx = xdot(x, y, z);
      float dy = ydot(x, y, z);
      float dz = zdot(x, y, z);
      // Euler's method
      x += dx*dt;
      y += dy*dt;
      z += dz*dt;
      left[i] = x * gainL;
      right[i] = y * gainR;
    }
  }
};

#endif // __ChuaCircuitPatch_h__
