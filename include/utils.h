#pragma once

#include <functional>

#include "LiteMath.h"
using namespace LiteMath;

using DistFunc = std::function<float(float3)>;


float3 EstimateNormal(float3 z, DistFunc sdf, float eps=1e-4);
float3 EyeRayDir(float x, float y, float4x4 a_mViewProjInv);
void transform_ray3f(float4x4 a_mWorldViewInv, float3* ray_pos, float3* ray_dir) ;

float4 RayMarchConstantFog(float tmin, float tmax, float& alpha);
uint32_t RealColorToUint32(float4 real_color);
