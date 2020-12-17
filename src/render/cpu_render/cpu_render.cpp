#include <ctime>
#include <random>
#include <iostream>
#include <iomanip>

#include "cpu_render.h"
#include "trace.h"
#include "scene/material.h"
#include "utils/parallel_for.h"

/**
 * \brief Render initialization function
 * \param[in] SelectedDeviceId Selected vulkan device
 * \param[in] OneSeed One seed for all sample flag for Vulkan implementation
 */
VOID cpu_render::InitRender( UINT SelectedDeviceId, BOOL OneSeed )
{
}

/**
  * \brief Setup scene function
  * \param[in] NewScene Scene for render
  */
VOID cpu_render::SetScene( scene &NewScene )
{
  Scene = &NewScene;
}

/**
  * \brief Setup camera function
  * \param[in] NewCamera Camera for render
  */
VOID cpu_render::SetCamera( const cam &NewCamera )
{
  Camera = &NewCamera;
}

/**
  * \brief Begin frame function
  * \param[in] W Frame width
  * \param[in] H Fream heigth
  */
VOID cpu_render::StartFrame( INT W, INT H )
{
  /* HDR */
  HDR.SetSize(W, H);
}

/**
  * \brief End frame function
  */
VOID cpu_render::EndFrame( VOID )
{
}

/**
 * \brief Apply HDR correction to image function
 * \param[in, out] Im Image
 */
VOID cpu_render::ProcessHDR( image * Im )
{
  HDR.SetBrighnesLimit(1.5);
  HDR.SetExposure(2);
  HDR.SetNumberOfBloomIterations(100);
  HDR.Process(Im);
}

/**
  * \brief Generate sample image function
  * \param[in, out] Im Image
  */
VOID cpu_render::MakeSample( image *Im )
{
  static std::uniform_real_distribution<FLT> Distr(-0.5f, 0.5f);
  static std::mt19937_64 Gen;
  const environment AirEnvi = environment::Make();
  const kd_tree &Tree = Scene->GetTree();

 // #pragma omp parallel for
  //for (INT y = 0; y < Im->FrameH; y++)
  parallel_for::Run(Im->FrameH, [&]( INT y )
    {
      tracer RayTraceStructure(Tree, AirEnvi, Gen, Scene->RenderPar);

      for (INT x = 0; x < Im->FrameW; x++)
      {
        vec TraceResult = RayTraceStructure.Trace(Camera->ToRay(x + Distr(Gen), y + Distr(Gen)),
                                                  Scene->AirEnvi, vec(1, 1, 1));

        Im->SetPixel(x, y, image_vec(TraceResult.X, TraceResult.Y, TraceResult.Z));
      }
    });
}

/**
 * \brief Render frame function
 * \param[in, out] Im Image for render
 * \param[in] Camera Camera for render
 * \param[in, out] Scene Scene for render
 * \param[in] NumberOfSamples Number of samples
 */
VOID cpu_render::RenderFrame( image *Im, const cam &Camera, scene &Scene, INT NumberOfSamples )
{
  StartFrame(Im->FrameW, Im->FrameH);

  image SampleImg;

  SampleImg.Resize(Im->FrameW, Im->FrameH);
  SampleImg.Clear();
  Im->Clear();

  std::cout << "Generating/Getting tree\n";

  INT64 Time = clock();

  Scene.GetTree();
  std::cout << "Success. Elapsed time: " + std::to_string((clock() - Time) /
                                                          (DBL)CLOCKS_PER_SEC) << "\n\n";

  SetCamera(Camera);
  SetScene(Scene);

  INT SampleCounter;

  for (SampleCounter = 0; SampleCounter < NumberOfSamples; SampleCounter++)
  {
    SampleImg.Clear();

    std::cout << "Generating sample #" + std::to_string(SampleCounter + 1) + "\n";
    Time = clock();
    MakeSample(&SampleImg);
    std::cout << "Success. Elapsed time: " + std::to_string((clock() - Time) /
                                                            (DBL)CLOCKS_PER_SEC) << "\n";

    std::cout << "Applying sample #" + std::to_string(SampleCounter + 1) + "\n";
    Time = clock();
    //  Img->ApplySample(SampleImg, i);
    *Im += SampleImg;
    std::cout << "Success. Elapsed time: " + std::to_string((clock() - Time) /
                                                            (DBL)CLOCKS_PER_SEC) << "\n" << std::endl;
  }

  *Im /= SampleCounter;
  ProcessHDR(Im);

  EndFrame();
}


