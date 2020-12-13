#include <iostream>
#include <ctime>

#include "render.h" 
#include "utils/error.h"

/**
  * \brief Render constructor
  * \param[in] DeviceId Vulkan device identifier
  */
render::render( UINT DeviceId ) : RndVulkan(DeviceId)
{
  RndPtr->InitRender(DeviceId, FALSE);
}

/**
  * \brief Setup render mode function
  * \param[in] RndMode New render mode
  * \param[in] DeviceId Vulkan device identifier
  */
VOID render::SetRenderMode( MODE RndMode, UINT DeviceId )
{
  switch (RndMode)
  {
  case MODE::CPU:
    RndPtr = &RndCPU;
    break;
  case MODE::VULKAN:
  case MODE::VULKAN_ONE_SEED:
    RndPtr = &RndVulkan;
    break;
  default:
    error("Unknown render mode");
  }
  RndPtr->InitRender(DeviceId, RndMode == MODE::VULKAN_ONE_SEED);
}

/**
  * \brief Make one frame function
  * \param[in, out] Img Image for render
  * \param[in] Camera Camera for render
  * \param[in] Scene Scene for render
  * \param[in] W Image width
  * \param[in] H Image heigth
  * \param[in] NumOfSamples Number of samples
  */
VOID render::MakeFrame( image *Img, const cam &Camera, scene &Scn, const INT W, const INT H,
                        const INT NumOfSamples ) const
{
  INT64 TotalTimeStart = clock();

  std::cout << "**********************\n** Generating frame **\n**********************\n\n";

  Img->Resize(W, H);
  RndPtr->RenderFrame(Img, Camera, Scn, NumOfSamples);

  std::cout << "*************\n** Success **\n*************\n";
  std::cout << "Total elapsed time: " << std::to_string((clock() - TotalTimeStart) /
    (DBL)CLOCKS_PER_SEC) << "\n\n";
}
