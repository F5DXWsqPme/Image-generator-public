#include <boost/test/unit_test.hpp>
#include <cmath>

#include "render/render.h"
#include "scene/material.h"
#include "utils/error.h"

/* Hardcode shapes */
shape Cow;
shape Box;
shape Emit;
shape Dragons;

/**
 * \brief Generate scene function (Hardcode)
 * \param[in, out] Scn Scene for generation
 * \warning Temporary function
 */
VOID GenScene( scene &Scn )
{
  INT DefMtl = material::AddToTable(material(vec(1, 1, 1), vec(0, 0, 0), 0.6, 0.3));
  INT DefEnvi = environment::AddToTable(environment::Make());
  INT EmitMtl = material::AddToTable( material(vec(0, 0, 0), vec(5, 5, 5)));
  INT GreenMtl = material::AddToTable(material(vec(0, 1.5, 0), vec(0, 0, 0), 0.8, 0.3));
  INT PinkMetalMtl = material::AddToTable(material(vec(1, 0, 1), vec(0, 0, 0), 0.1, 1));
  INT MetalMtl = material::AddToTable(material(vec(0.5, 0.5, 0.5), vec(0, 0, 0), 0.4, 0.9));
  INT RedMtl = material::AddToTable(material(vec(1.3, 0, 0), vec(0, 0, 0), 0.7, 0.3));
  INT BlueMtl = material::AddToTable(material(vec(0, 0, 3), vec(0, 0, 0), 0.5, 0.5));
  INT IdealMetalMtl = material::AddToTable(material(vec(0.9, 0.9, 0.9), vec(0, 0, 0), 0.02, 0.99));

  Scn.AirEnvi = environment::Make();

  Cow.LoadOBJ("bin/models/cow.obj", MetalMtl, DefEnvi, matr::Scale(0.5, 0.5, 0.5) * matr::Translate(0, 1.5, 0));
  Box.MakeBox(vec(-6, 0, -15), vec(6, 6.5, 9), DefMtl, DefEnvi);
  Box.MakeBox(vec(-5, 1, 8), vec(5, 5, 9), IdealMetalMtl, DefEnvi);
  Box.MakeBox(vec(-5, 0, -14), vec(-3, 5, -12), GreenMtl, DefEnvi);
  Box.MakeBox(vec(0.5f, 0, -13.5), vec(5, 2, -11.5), RedMtl, DefEnvi);
  Emit.MakeBox(vec(-3, 6.4, -3), vec(3, 6.51, 3), EmitMtl, DefEnvi);

  Dragons.LoadOBJ("bin/models/dragon.obj", GreenMtl, DefEnvi,
    matr::Scale(vec(0.75, 0.75, 0.75)) * matr::RotateX(-90) *
    matr::RotateY(35) * matr::Translate(vec(-4, 0, 1.5)));
  Dragons.LoadOBJ("bin/models/dragon.obj", PinkMetalMtl, DefEnvi,
    matr::Scale(vec(0.75, 0.75, 0.75)) * matr::RotateX(-90) *
    matr::RotateY(125) * matr::Translate(vec(4, 0, 1.5)));
  Dragons.LoadOBJ("bin/models/dragon.obj", BlueMtl, DefEnvi,
    matr::Scale(vec(0.75, 0.75, 0.75)) * matr::RotateX(-90) *
    matr::RotateY(-145) * matr::Translate(vec(3, 0, -3)));
  Dragons.LoadOBJ("bin/models/dragon.obj", RedMtl, DefEnvi,
    matr::Scale(vec(0.75, 0.75, 0.75)) * matr::RotateX(-90) *
    matr::RotateY(-55) * matr::Translate(vec(-3, 0, -3)));

  Scn.Objects.push_back(&Cow);
  Scn.Objects.push_back(&Box);
  Scn.Objects.push_back(&Emit);
  Scn.Objects.push_back(&Dragons);

  Scn.IsChanged = TRUE;
}

/**
 * \brief Get distance between images
 * \param Im1 First image
 * \param Im2 Second image
 * \return Distance
 */
DBL ImageDistance( const image &Im1, const image &Im2 )
{
  if (Im1.FrameW != Im2.FrameW)
    return INFINITY;

  if (Im1.FrameH != Im2.FrameH)
    return INFINITY;

  DBL Sum = 0;

  for (INT y = 0; y < Im1.FrameH; y++)
    for (INT x = 0; x < Im1.FrameW; x++)
    {
      const image_vec &Pixel1 = Im1.GetPixel(x, y);
      const image_vec &Pixel2 = Im2.GetPixel(x, y);

      vec Diff(Pixel1.R - Pixel2.R, Pixel1.G - Pixel2.G, Pixel1.B - Pixel2.B);

      Sum += !Diff;
    }

  return Sum / Im1.FrameW / Im1.FrameH;
}

BOOST_AUTO_TEST_SUITE(CpuAndGpuGetEqualTestsSuite)

/**
 * \brief Test cpu and gpu results equal
 */
BOOST_AUTO_TEST_CASE(CpuAndGpuGetEqual)
{
  const INT NumOfSamples = 50;
  const INT W = 100;
  const INT H = 50;

  render RndCPU;
  render RndGPU;
  scene Scn;
  image ImgCPU;
  image ImgGPU;

  cam Camera;

  RndCPU.SetRenderMode(render::MODE::CPU, 0);
  RndGPU.SetRenderMode(render::MODE::VULKAN, 0);

  Camera.SetWH(W, H);
  Camera.SetProj();
  Camera.SetView(vec(0, 5, -10), vec(0, 2.5, 0), vec(0, 1, 0));

  GenScene(Scn);

  RndCPU.MakeFrame(&ImgCPU, Camera, Scn, W, H, NumOfSamples);
  RndGPU.MakeFrame(&ImgGPU, Camera, Scn, W, H, NumOfSamples);

  DBL Dist = ImageDistance(ImgCPU, ImgGPU);

  std::cout << "Distance: " << Dist << std::endl;

  BOOST_CHECK_SMALL(Dist, 0.3);
}

BOOST_AUTO_TEST_SUITE_END()
