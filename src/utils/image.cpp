#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "ext/stb/stb_image.h"
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "ext/stb/stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "ext/stb/stb_image_resize.h"



#include "def.h"
#include "image.h"
#include "error.h"
#include "utils/parallel_for.h"

/**
 * \brief Clear image function
 */
VOID image::Clear( VOID )
{
  for (std::vector<image_vec>::iterator It = Buffer.begin();
       It != Buffer.end(); It++)
  {
    *It = image_vec();
  }
}

/**
 * \brief Get pixel from image function
 * \param[in] X First coordinate
 * \param[in] Y Second coordinate
 * \return Pixel color
 */
const image_vec& image::GetPixel( const INT X, const INT Y ) const
{
  return Buffer[X + (UINT64)Y * W];
}

/**
 * \brief Get pixel from image function (with width and height test)
 * \param[in] X First coordinate
 * \param[in] Y Second coordinate
 * \return Pixel color
 */
const image_vec & image::GetPixelSave( const INT X, const INT Y ) const
{
  INT
    CorrectX = X,
    CorrectY = Y;

  CorrectX = fmaxf(CorrectX, 0);
  CorrectX = fminf(CorrectX, FrameW - 1);
  CorrectY = fmaxf(CorrectY, 0);
  CorrectY = fminf(CorrectY, FrameH - 1);

  return Buffer[CorrectX + (UINT64)CorrectY * W];
}

/**
 * \brief Set pixel function
 * \param[in] X First coordinate
 * \param[in] Y Second coordinate
 * \param[in] Color Pixel color
 */
VOID image::SetPixel( const INT X, const INT Y, const image_vec &Color )
{
  Buffer[X + (UINT64)Y * W] = Color;
}

/**
 * \brief Resize image function
 * \param[in] NewW New image width
 * \param[in] NewH New image height
 */
VOID image::Resize( const INT NewW, const INT NewH )
{
  W = NewW;
  H = NewH;
  Buffer.resize((UINT64)W * H);
}

//  VOID image::ApplySample( const image &Img, const INT NumOfSamples )
//  {
//    #pragma omp parallel for
//    for (INT y = 0; y < H; y++)
//      for (INT x = 0; x < W; x++)
//      {
//        const image_vec OldPixel = GetPixel(x, y);
//        const image_vec NewPixel = Img.GetPixel(x, y);
//        const DBL Mult = NumOfSamples;
//        const DBL Den = (NumOfSamples + 1);
//  
//        const DBL R = (OldPixel.R * Mult + NewPixel.R) / Den;
//        const DBL G = (OldPixel.G * Mult + NewPixel.G) / Den;
//        const DBL B = (OldPixel.B * Mult + NewPixel.B) / Den;
//  
//        SetPixel(x, y, image_vec(R, G, B));
//      }
//  }

/**
 * \brief Addition images function
 * \param[in] Img Image for addition
 */
VOID image::operator+=( const image & Img )
{
  //#pragma omp parallel for
  //for (INT y = 0; y < H; y++)
  parallel_for::Run(Img.FrameH, [&]( INT y )
  {
    for (INT x = 0; x < W; x++)
    {
      const image_vec OldPixel = GetPixel(x, y);
      const image_vec NewPixel = Img.GetPixel(x, y);
  
      const DBL R = OldPixel.R + NewPixel.R;
      const DBL G = OldPixel.G + NewPixel.G;
      const DBL B = OldPixel.B + NewPixel.B;
  
      SetPixel(x, y, image_vec(R, G, B));
    }
  });
}

/**
 * \brief Division image by number function
 * \param[in] N Number
 */
VOID image::operator/=(const DBL N)
{
  //#pragma omp parallel for
  //for (INT y = 0; y < H; y++)
  parallel_for::Run(H, [&]( INT y )
  {
    for (INT x = 0; x < W; x++)
    {
      const image_vec OldPixel = GetPixel(x, y);
  
      const DBL R = OldPixel.R / N;
      const DBL G = OldPixel.G / N;
      const DBL B = OldPixel.B / N;
  
      SetPixel(x, y, image_vec(R, G, B));
    }
  });
}

/**
 * \brief Save image function (tga format)
 * \param[in] FileName Name of file
 */
VOID image::SaveTGA( const std::string &FileName ) const
{
  std::vector<DWORD> Data;
  
  Data.reserve(W * H);
  for (std::vector<image_vec>::const_iterator It = Buffer.cbegin(); It != Buffer.cend(); It++)
    Data.push_back(It->ToDWORD());
  
  stbi_write_tga(FileName.c_str(), W, H, 4, (VOID *)Data.data());
}

/**
 * \brief Save image function (png format)
 * \param[in] FileName Name of file
 */
VOID image::SavePNG( const std::string &FileName ) const
{
  std::vector<DWORD> Data;
  
  Data.reserve(W * H);
  for (std::vector<image_vec>::const_iterator It = Buffer.cbegin(); It != Buffer.cend(); It++)
    Data.push_back(It->ToDWORD());
  
  stbi_write_png(FileName.c_str(), W, H, 4, (VOID *)Data.data(), 0);
}

/**
 * \brief Save image function (jpg format)
 * \param[in] FileName Name of file
 */
VOID image::SaveJPEG( const std::string &FileName ) const
{
  std::vector<DWORD> Data;
  
  Data.reserve(W * H);
  for (std::vector<image_vec>::const_iterator It = Buffer.cbegin(); It != Buffer.cend(); It++)
    Data.push_back(It->ToDWORD());
  
  stbi_write_jpg(FileName.c_str(), W, H, 4, (VOID *)Data.data(), 100);
}

/**
 * \brief Save image function
 * \param[in] FileName Name of file
 * \param[in] Format File format
 */
VOID image::Save( const std::string &FileName, FORMAT Format ) const
{
  switch (Format)
  {
  case FORMAT::JPG:
    SaveJPEG(FileName);
    break;
  case FORMAT::PNG:
    SavePNG(FileName);
    break;
  case FORMAT::TGA:
    SaveTGA(FileName);
    break;
  }
}
