#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

#include "example_tracer/example_tracer.h"
#include "Image2d.h"

//#include "vk_context.h"
//std::shared_ptr<TestClass> CreateTestClass_Generated(vk_utils::VulkanContext a_ctx, size_t a_maxThreadsGenerated);

int main(int argc, const char** argv)
{
  #ifndef NDEBUG
  bool enableValidationLayers = true;
  #else
  bool enableValidationLayers = false;
  #endif

  uint WIN_WIDTH  = 512;
  uint WIN_HEIGHT = 512;

  bool onGPU = false;
  std::shared_ptr<RayMarcherExample> pImpl = nullptr;
  //if(onGPU)
  //{
  //  unsigned int a_preferredDeviceId = args.getOptionValue<int>("--gpu_id", 0);
  //  auto ctx = vk_utils::globalContextGet(enableValidationLayers, a_preferredDeviceId);
  //  pImpl = CreateTestClass_Generated(ctx, WIN_WIDTH*WIN_HEIGHT);
  //}
  //else
    pImpl = std::make_shared<RayMarcherExample>();

  pImpl->CommitDeviceData();

  std::vector<uint> pixelData(WIN_WIDTH*WIN_HEIGHT);  
  pImpl->RayMarch(pixelData.data(), WIN_WIDTH, WIN_HEIGHT);
  
  if(onGPU)
    LiteImage::SaveBMP("out_gpu.bmp", pixelData.data(), WIN_WIDTH, WIN_HEIGHT);
  else
    LiteImage::SaveBMP("out_cpu.bmp", pixelData.data(), WIN_WIDTH, WIN_HEIGHT);

  float timings[4] = {0,0,0,0};
  pImpl->GetExecutionTime("RayMarch", timings);
  std::cout << "RayMarch(exec) = " << timings[0]              << " ms " << std::endl;
  std::cout << "RayMarch(copy) = " << timings[1] + timings[2] << " ms " << std::endl;
  std::cout << "RayMarch(ovrh) = " << timings[3]              << " ms " << std::endl;
  
  pImpl = nullptr;
  return 0;
}