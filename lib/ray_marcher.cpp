#include <vector>
#include <chrono>
#include <string>

#include "ray_marcher.h"


float2 RayBoxIntersection(float3 ray_pos, float3 ray_dir, float3 boxMin, float3 boxMax)
{
  ray_dir.x = 1.0f/ray_dir.x; // may precompute if intersect many boxes
  ray_dir.y = 1.0f/ray_dir.y; // may precompute if intersect many boxes
  ray_dir.z = 1.0f/ray_dir.z; // may precompute if intersect many boxes

  float lo = ray_dir.x*(boxMin.x - ray_pos.x);
  float hi = ray_dir.x*(boxMax.x - ray_pos.x);
  
  float tmin = std::min(lo, hi);
  float tmax = std::max(lo, hi);

  float lo1 = ray_dir.y*(boxMin.y - ray_pos.y);
  float hi1 = ray_dir.y*(boxMax.y - ray_pos.y);

  tmin = std::max(tmin, std::min(lo1, hi1));
  tmax = std::min(tmax, std::max(lo1, hi1));

  float lo2 = ray_dir.z*(boxMin.z - ray_pos.z);
  float hi2 = ray_dir.z*(boxMax.z - ray_pos.z);

  tmin = std::max(tmin, std::min(lo2, hi2));
  tmax = std::min(tmax, std::max(lo2, hi2));
  
  return float2(tmin, tmax);
}

static inline float3 EyeRayDir(float x, float y, float4x4 a_mViewProjInv)
{
  float4 pos = float4(2.0f*x - 1.0f, 2.0f*y - 1.0f, 0.0f, 1.0f );
  pos = a_mViewProjInv * pos;
  pos /= pos.w;
  return normalize(to_float3(pos));
}

static inline void transform_ray3f(float4x4 a_mWorldViewInv, float3* ray_pos, float3* ray_dir) 
{
  float4 rayPosTransformed = a_mWorldViewInv*to_float4(*ray_pos, 1.0f);
  float4 rayDirTransformed = a_mWorldViewInv*to_float4(*ray_dir, 0.0f);
  
  (*ray_pos) = to_float3(rayPosTransformed);
  (*ray_dir) = to_float3(normalize(rayDirTransformed));
}

float4 RayMarchConstantFog(float tmin, float tmax, float& alpha)
{
  float dt = 0.05f;
	float t  = tmin;
	
	alpha = 1.0f;
	float4 color = float4(0.0f);
	
	while(t < tmax && alpha > 0.01f)
	{
	  float a = 0.025f;
	  color += a*alpha*float4(1.0f,1.0f,0.0f,0.0f);
	  alpha *= (1.0f-a);
	  t += dt;
	}
	
	return color;
}

static inline uint32_t RealColorToUint32(float4 real_color)
{
  float  r = real_color[0]*255.0f;
  float  g = real_color[1]*255.0f;
  float  b = real_color[2]*255.0f;
  float  a = real_color[3]*255.0f;

  uint32_t red   = (uint32_t)r;
  uint32_t green = (uint32_t)g;
  uint32_t blue  = (uint32_t)b;
  uint32_t alpha = (uint32_t)a;

  return red | (green << 8) | (blue << 16) | (alpha << 24);
}

void RayMarcher::kernel2D_RayMarch(uint32_t* out_color, uint32_t width, uint32_t height) 
{
  for(uint32_t y=0;y<height;y++) 
  {
    for(uint32_t x=0;x<width;x++) 
    {
      float3 rayDir = EyeRayDir((float(x) + 0.5f) / float(width), (float(y) + 0.5f) / float(height), m_worldViewProjInv); 
      float3 rayPos = float3(0.0f, 0.0f, 0.0f);

      transform_ray3f(m_worldViewInv, &rayPos, &rayDir);
      
      float2 tNearAndFar = RayBoxIntersection(rayPos, rayDir, float3(-1,-1,-1), float3(1,1,1));
      
      float4 resColor(0.0f);
      if(tNearAndFar.x < tNearAndFar.y && tNearAndFar.x > 0.0f)
      {
        float alpha = 1.0f;
	      resColor = RayMarchConstantFog(tNearAndFar.x, tNearAndFar.y, alpha);
      }
      
      out_color[y*width+x] = RealColorToUint32(resColor);
    }
  }
}

void RayMarcher::RayMarch(uint32_t* out_color, uint32_t width, uint32_t height)
{ 
  auto start = std::chrono::high_resolution_clock::now();
  kernel2D_RayMarch(out_color, width, height);
  rayMarchTime = float(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count())/1000.f;
}  

void RayMarcher::GetExecutionTime(const char* a_funcName, float a_out[4])
{
  if(std::string(a_funcName) == "RayMarch")
    a_out[0] =  rayMarchTime;
}
