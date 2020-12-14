#include <boost/test/unit_test.hpp>

#include "render/render.h"
#include "scene/material.h"
#include "utils/error.h"
#include "scene_loader/scene_loader.h"

/**
 * \brief Generate scene function (Hardcode)
 * \param[in, out] Scn Scene for generation
 * \warning Temporary function
 */
VOID GenScene( scene &Scn );

/**
 * \brief Get distance between images
 * \param Im1 First image
 * \param Im2 Second image
 * \return Distance
 */
DBL ImageDistance( const image &Im1, const image &Im2 );

/**
 * \brief Test cpu and gpu results equal with scene loader
 */
BOOST_AUTO_TEST_CASE(CpuAndGpuGetEqualWithLoader)
{
  scene_loader Loader;

  Loader.LoadXML("tests/scenes/frame.xml");

  render RndGPU;

  RndGPU.SetRenderMode(Loader.RenderMode, Loader.DeviceId);

  image ImgGPU;

  RndGPU.MakeFrame(&ImgGPU, Loader.Camera, Loader.Scene,
                   Loader.Width, Loader.Height, Loader.NumberOfSamples);

  INT NumOfSamples = 30;
  INT W = 500;
  INT H = 300;

  render RndCPU;
  scene Scn;
  image ImgCPU;
  cam Camera;

  RndCPU.SetRenderMode(render::MODE::CPU, 0);

  Camera.SetWH(W, H);
  Camera.SetProj();
  Camera.SetView(vec(0, 5, -10), vec(0, 2.5, 0), vec(0, 1, 0));

  GenScene(Scn);

  RndCPU.MakeFrame(&ImgCPU, Camera, Scn, W, H, NumOfSamples);

  DBL Dist = ImageDistance(ImgCPU, ImgGPU);

  std::cout << "Distance: " << Dist << std::endl;

  BOOST_CHECK_SMALL(Dist, 0.3);
}
