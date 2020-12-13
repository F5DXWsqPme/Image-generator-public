#include <iostream>

#include "render/render.h"
#include "scene/material.h"
#include "utils/error.h"
#include "scene_loader/scene_loader.h"

/* Hardcode shapes */
/*shape Cow;
shape Box;
shape Emit;
shape Dragons; */

/**
 * \brief Generate scene function (Hardcode)
 * \param[in, out] Scn Scene for generation
 * \warning Temporary function
 */
/*VOID GenScene( scene &Scn )
{
  INT DefMtl = material::AddToTable(material(vec(1, 1, 1), vec(0, 0, 0), 0.6, 0.3)); // First in table - default
  INT DefEnvi = environment::AddToTable(environment::Make());                       // First in table - default
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
}   */

/**
 * \brief Main function
 * \param[in] ArgC Number of arguments
 * \param[in] ArgV Arguments array
 * \return Error code
 */
INT main( INT ArgC, CHAR **ArgV )
{
  /*try
  {
    const INT NumOfSamples = 1;
    const INT W = 1920;
    const INT H = 1080;
    const std::string ResPath = "res/";

    render Rnd;
    scene Scn;
    image Img;

    cam Camera;

    Rnd.SetRenderMode(render::MODE::CPU, 0);

    Camera.SetWH(W, H);
    Camera.SetProj();
    Camera.SetView(vec(0, 5, -10), vec(0, 2.5, 0), vec(0, 1, 0));

    GenScene(Scn);

    Rnd.MakeFrame(&Img, Camera, Scn, W, H, NumOfSamples);
    Img.SavePNG(ResPath + "frame1.png");
  }
  catch ( const error &Err )
  {
    std::cout << "Error:\n    " << Err.what() << "\n";
  }

  std::cout << "Finished\n";  */

  try
  {
    scene_loader Loader;

    if (ArgC != 2)
    {
      std::cout << "Wrong number of arguments";
      return 1;
    }

    Loader.LoadXML(ArgV[1]);

    render Rnd;

    Rnd.SetRenderMode(Loader.RenderMode, Loader.DeviceId);

    image Img;

    Rnd.MakeFrame(&Img, Loader.Camera, Loader.Scene,
                  Loader.Width, Loader.Height, Loader.NumberOfSamples);

    Img.Save(Loader.OutputPath, Loader.OutputFormat);
  }
  catch ( const error &Err )
  {
    std::cerr << "Error:\n    " << Err.what() << "\n";
    return 2;
  }
  catch ( ... )
  {
    std::cerr << "All is very very bad!\n";
    return 3;
  }

  return 0;
}
