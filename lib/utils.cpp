#include "utils.h"


float3 EstimateNormal(float3 z, DistFunc sdf, float eps)
{
    float d = sdf(z);
    // float3 z1 = z + float3(eps, 0, 0);
    // float3 z2 = z - float3(eps, 0, 0);
    // float3 z3 = z + float3(0, eps, 0);
    // float3 z4 = z - float3(0, eps, 0);
    // float3 z5 = z + float3(0, 0, eps);
    // float3 z6 = z - float3(0, 0, eps);
    // float dx = sdf(z1) - sdf(z2);
    // float dy = sdf(z3) - sdf(z4);
    // float dz = sdf(z5) - sdf(z6);
    // return normalize(float3(dx, dy, dz) / (2.0 * eps));
    return normalize(float3(
        sdf(float3(z.x + eps, z.y, z.z)) - d,
        sdf(float3(z.x, z.y + eps, z.z)) - d,
        sdf(float3(z.x, z.y, z.z + eps)) - d
    ));
}

float3 EyeRayDir(float x, float y, float4x4 a_mViewProjInv)
{
    float4 pos = float4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 0.0f, 1.0f);
    pos = a_mViewProjInv * pos;
    pos /= pos.w;
    return normalize(to_float3(pos));
}

void transform_ray3f(float4x4 a_mWorldViewInv, float3* ray_pos, float3* ray_dir)
{
    float4 rayPosTransformed = a_mWorldViewInv * to_float4(*ray_pos, 1.0f);
    float4 rayDirTransformed = a_mWorldViewInv * to_float4(*ray_dir, 0.0f);

    (*ray_pos) = to_float3(rayPosTransformed);
    (*ray_dir) = to_float3(normalize(rayDirTransformed));
}

float4 RayMarchConstantFog(float tmin, float tmax, float& alpha)
{
    float dt = 0.05f;
    float t  = tmin;

    alpha = 1.0f;
    float4 color = float4(0.0f);

    while (t < tmax && alpha > 0.01f) {
        float a = 0.025f;
        color += a * alpha * float4(1.0f, 1.0f, 0.0f, 0.0f);
        alpha *= (1.0f - a);
        t += dt;
    }

  return color;
}

uint32_t RealColorToUint32(float4 real_color)
{
    float r = real_color[0] * 255.0f;
    float g = real_color[1] * 255.0f;
    float b = real_color[2] * 255.0f;
    float a = real_color[3] * 255.0f;

    uint32_t red   = (uint32_t)r;
    uint32_t green = (uint32_t)g;
    uint32_t blue  = (uint32_t)b;
    uint32_t alpha = (uint32_t)a;

    return red | (green << 8) | (blue << 16) | (alpha << 24);
}
