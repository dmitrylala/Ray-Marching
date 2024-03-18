#include <iostream>
#include <fstream>
#include <vector>
#include <memory>  // for shared pointers
#include <iomanip> // for std::fixed/std::setprecision
#include <sstream>

#include "example_tracer/example_tracer.h"
#include "Image2d.h"

#ifdef USE_VULKAN
#include "vk_context.h"
std::shared_ptr<RayMarcherExample> CreateRayMarcherExample_generated(vk_utils::VulkanContext a_ctx, size_t a_maxThreadsGenerated);
#endif

int main(int argc, const char** argv) {
    #ifndef NDEBUG
    bool enableValidationLayers = true;
    #else
    bool enableValidationLayers = false;
    #endif

    uint WIN_WIDTH  = 512;
    uint WIN_HEIGHT = 512;

    std::shared_ptr<RayMarcherExample> pImpl = nullptr;
    #ifdef USE_VULKAN
    bool onGPU = true; // TODO: you can read it from command line
    if(onGPU)
    {
    auto ctx = vk_utils::globalContextGet(enableValidationLayers, 0);
    pImpl    = CreateRayMarcherExample_generated(ctx, WIN_WIDTH*WIN_HEIGHT);
    }
    else
    #else
    bool onGPU = false;
    #endif
    pImpl = std::make_shared<RayMarcherExample>();

    pImpl->CommitDeviceData();

    std::vector<uint> pixelData(WIN_WIDTH*WIN_HEIGHT);


    float4x4 mRotX = rotate4x4X(float(-25) * DEG_TO_RAD);
    float4 camPos = mRotX * float4(1, 1, 2, 1);

    // pos, look_at, up
    float4x4 viewMat = lookAt(to_float3(camPos), float3(0, 0, 0), float3(0, 1, 0));

    pImpl->SetWorldViewMatrix(viewMat);
    pImpl->UpdateMembersPlainData();                                            // copy all POD members from CPU to GPU in GPU implementation
    pImpl->RayMarch(pixelData.data(), WIN_WIDTH, WIN_HEIGHT);

    float timings[4] = {0, 0, 0, 0};
    pImpl->GetExecutionTime("RayMarch", timings);

    std::stringstream strOut;
    if (onGPU)
        strOut << std::fixed << std::setprecision(2) << "out_gpu.bmp";
    else
        strOut << std::fixed << std::setprecision(2) << "out_cpu.bmp";
    std::string fileName = strOut.str();

    LiteImage::SaveBMP(fileName.c_str(), pixelData.data(), WIN_WIDTH, WIN_HEIGHT);

    std::cout << "timeRender = " << timings[0] << " ms, timeCopy = " <<  timings[1] + timings[2] << " ms " << std::endl;

    pImpl = nullptr;
    return 0;
}