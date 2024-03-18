#include <iostream>
#include <fstream>
#include <vector>
#include <memory>   // for shared pointers
#include <iomanip>  // for std::fixed/std::setprecision
#include <sstream>

#include "ray_marcher.h"
#include "Image2d.h"

#ifdef USE_VULKAN
#include "vk_context.h"
std::shared_ptr<RayMarcherExample> CreateRayMarcherExample_Generated(vk_utils::VulkanContext a_ctx, size_t a_maxThreadsGenerated);
#endif

float sphere(float3 p) {
    float3 center = float3(0.0f, 0.0f, 0.0f);
    float radiusSquared = 2;
    return length(p - center) - radiusSquared;
}

float3 vecmax(float3 p, float value) {
    return float3(max(p.x, value), max(p.y, value), max(p.z, value));
}

float box(float3 p) {
    float3 b = float3(1.0f, 1.0f, 1.0f);
    float3 q = abs(p) - b;
    return length(vecmax(q, 0.0f)) + min(max(q.x, max(q.y, q.z)), 0.0f);
}

float tetrahedron(float3 z)
{
	float3 a1 = float3(1, 1, 1);
	float3 a2 = float3(-1, -1, 1);
	float3 a3 = float3(1, -1, -1);
	float3 a4 = float3(-1, 1, -1);
	float3 c;

    float Scale = 5.0f;
	int n = 0;
	float dist, d;
	while (n < 3) {
        c = a1;
        dist = length(z - a1);
        d = length(z - a2); if (d < dist) { c = a2; dist=d; }
        d = length(z - a3); if (d < dist) { c = a3; dist=d; }
        d = length(z - a4); if (d < dist) { c = a4; dist=d; }
		z = Scale * z - c * (Scale - 1.0);
		++n;
	}

	return length(z) * pow(Scale, float(-n));
}

int main(int argc, const char** argv) {
    #ifndef NDEBUG
    bool enableValidationLayers = true;
    #else
    bool enableValidationLayers = false;
    #endif

    uint WIN_WIDTH  = 512;
    uint WIN_HEIGHT = 512;

    std::shared_ptr<RayMarcher> pImpl = nullptr;
    #ifdef USE_VULKAN
    bool onGPU = true; // TODO: you can read it from command line
    if (onGPU) {
        auto ctx = vk_utils::globalContextGet(enableValidationLayers, 0);
        pImpl    = CreateRayMarcher_Generated(ctx, WIN_WIDTH*WIN_HEIGHT);
    }
    else
    #else
        bool onGPU = false;
    #endif
    pImpl = std::make_shared<RayMarcher>();

    pImpl->CommitDeviceData();

    std::vector<uint> pixelData(WIN_WIDTH*WIN_HEIGHT);

    for (int angleY = 0; angleY < 350; angleY += 10) {
        float4x4 mRot    = rotate4x4Y(float(angleY) * DEG_TO_RAD);

        // rotate and then translate camera position
        float4   camPos  = mRot * float4(0, 0, -3, 0) + float4(0, 1.5f, 0, 1);

        // pos, look_at, up
        float4x4 viewMat = lookAt(to_float3(camPos), float3(0, 0, 0), float3(0, 1, 0));

        pImpl->SetWorldViewMatrix(viewMat);

        // copy all POD members from CPU to GPU in GPU implementation
        pImpl->UpdateMembersPlainData();
        pImpl->RayMarch(pixelData.data(), WIN_WIDTH, WIN_HEIGHT, sphere);

        float timings[4] = {0, 0, 0, 0};
        pImpl->GetExecutionTime("RayMarch", timings);

        std::stringstream strOut;
        if (onGPU)
            strOut << std::fixed << std::setprecision(2) << "out_gpu_" << angleY << ".bmp";
        else
            strOut << std::fixed << std::setprecision(2) << "out_cpu_" << angleY << ".bmp";
        std::string fileName = strOut.str();

        LiteImage::SaveBMP(fileName.c_str(), pixelData.data(), WIN_WIDTH, WIN_HEIGHT);

        std::cout << "angl = " << angleY << ", timeRender = " << timings[0] << " ms, timeCopy = " <<  timings[1] + timings[2] << " ms " << std::endl;
    }

    pImpl = nullptr;
    return 0;
}
