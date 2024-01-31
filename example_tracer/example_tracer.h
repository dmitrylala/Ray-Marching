#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <cstdint>

#include "LiteMath.h"
using namespace LiteMath;

class RayMarcherExample // : public DataClass
{
public:

  RayMarcherExample()
  {
    const float4x4 proj = perspectiveMatrix(90.0f, 1.0f, 0.1f, 100.0f);
    m_worldViewProjInv  = inverse4x4(proj);
  }

  virtual void kernel2D_RayMarch(uint32_t* out_color, uint32_t width, uint32_t height);
  virtual void RayMarch(uint32_t* out_color, uint32_t width, uint32_t height);  

  virtual void CommitDeviceData() {}                                       // will be overriden in generated class
  virtual void GetExecutionTime(const char* a_funcName, float a_out[4]) {} // will be overriden in generated class

protected:

  float4x4 m_worldViewProjInv;

};
