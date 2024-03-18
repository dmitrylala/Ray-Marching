#include "ray_marcher.h"


uint32_t MarchOneRay(float3 rayPos, float3 rayDir, float3 light, DistFunc sdf) {

    int max_iterations = 50;

    float4 resColor(0.0f);
    for (int i = 0; i < max_iterations; ++i) {
        float step = sdf(rayPos);
        float3 new_pos = rayPos + rayDir * step;

        if (step <= 1e-5) {
            float3 lightDirection = light - new_pos;
            float3 normal = EstimateNormal(new_pos, sdf);
            float color = max(0.1f, dot(lightDirection, normal));

            for (int i = 0; i < 3; ++i) {
                resColor[i] = color;
            }
            resColor[3] = 1.0f;
            break;
        }

        rayPos = new_pos;
    }

    return RealColorToUint32(resColor);
}

void RayMarcher::kernel2D_RayMarch(
    uint32_t* out_color, uint32_t width, uint32_t height, DistFunc sdf)
{
    // float3 light = float3(-2.0f, 2.5f, 0.0f);
    float3 light = float3(-2.0f, 2.5f, 0.0f);
    // std::cout << "Dist: " << sdf(light) << std::endl;

    for (uint32_t y = 0; y < height; ++y)  {
        for (uint32_t x = 0; x < width; ++x) {
            float3 rayDir = EyeRayDir((float(x) + 0.5f) / float(width), (float(y) + 0.5f) / float(height), m_worldViewProjInv);
            float3 rayPos = float3(0.0f, 0.0f, 0.0f);

            transform_ray3f(m_worldViewInv, &rayPos, &rayDir);

            out_color[y * width + x] = MarchOneRay(rayPos, rayDir, light, sdf);
        }
    }
}

void RayMarcher::RayMarch(uint32_t* out_color, uint32_t width, uint32_t height, DistFunc sdf)
{
    auto start = std::chrono::high_resolution_clock::now();
    kernel2D_RayMarch(out_color, width, height, sdf);
    rayMarchTime = float(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count())/1000.f;
}

void RayMarcher::GetExecutionTime(const std::string &a_funcName, float a_out[4])
{
    if (a_funcName == "RayMarch")
        a_out[0] =  rayMarchTime;
}
