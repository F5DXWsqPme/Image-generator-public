#include <cmath>

#include "cpu_hdr.h"
#include "utils/parallel_for.h"

/**
  * \brief Tone mapping function
  * \param[in] Src Vector before tone compression
  * \return Compressed color
  */
image_vec cpu_hdr::ToneCompression( const image_vec &Src ) const
{
  return image_vec(1.0 - exp(-Src.R * Exposure),
                   1.0 - exp(-Src.G * Exposure),
                   1.0 - exp(-Src.B * Exposure));
}

/**
  * \brief Bloom function
  * \param[in, out] Img Image
  */
VOID cpu_hdr::Bloom( image * Img )
{
  //#pragma omp parallel for
  //for (INT y = 0; y < Img->FrameH; y++)
  parallel_for::Run(Img->FrameH, [&]( INT y )
  {
    for (INT x = 0; x < Img->FrameW; x++)
    {
      image_vec SrcColor(Img->GetPixel(x, y));
      image_vec Color(0, 0, 0);

      if (SrcColor.R * 0.3 + SrcColor.G * 0.59 + SrcColor.B * 0.11 > BrighnessLimit)
      {
        Color = SrcColor;
      }

      PingPongBuffers[1].SetPixel(x, y, Color);
    }
  });

  for (INT i = 0; i < NumberOfBlumIterations; i++)
  {
    BlurHorisontalIteration(PingPongBuffers[1], &PingPongBuffers[0]);
    BlurVerticalIteration(PingPongBuffers[0], &PingPongBuffers[1]);
  }

  //#pragma omp parallel for
  //for (INT y = 0; y < Img->FrameH; y++)
  parallel_for::Run(Img->FrameH, [&]( INT y )
  {
    for (INT x = 0; x < Img->FrameW; x++)
    {
      image_vec Color(Img->GetPixel(x, y));
      image_vec BloomResult(PingPongBuffers[1].GetPixel(x, y));

      Color.R += BloomResult.R;
      Color.G += BloomResult.G;
      Color.B += BloomResult.B;

      Img->SetPixel(x, y, Color);
    }
  });
}

/**
  * \brief Blur vertiacal iteration function
  * \param[in] Src Source image
  * \param[in, out] Dest Destination image
  */
VOID cpu_hdr::BlurVerticalIteration( const image & Src, image * Dest )
{
  //#pragma omp parallel for
  //for (INT y = 0; y < Dest->FrameH; y++)
  parallel_for::Run(Dest->FrameH, [&]( INT y )
  {
    for (INT x = 0; x < Dest->FrameW; x++)
    {
      image_vec Color(0, 0, 0);

      for (INT i = -4; i < 5; i++)
      {
        const image_vec SrcColor = Src.GetPixelSave(x, y + i);
        const FLT Coef = BlurCoefs[std::abs(i)];

        Color.R += Coef * SrcColor.R;
        Color.G += Coef * SrcColor.G;
        Color.B += Coef * SrcColor.B;
      }

      Dest->SetPixel(x, y, Color);
    }
  });
}

/**
  * \brief Blur horisontal iteration function
  * \param[in] Src Source image
  * \param[in, out] Dest Destination image
  */
VOID cpu_hdr::BlurHorisontalIteration( const image & Src, image * Dest )
{
  //#pragma omp parallel for
  //for (INT y = 0; y < Dest->FrameH; y++)
  parallel_for::Run(Dest->FrameH, [&]( INT y )
  {
    for (INT x = 0; x < Dest->FrameW; x++)
    {
      image_vec Color(0, 0, 0);

      for (INT i = -4; i < 5; i++)
      {
        const image_vec SrcColor = Src.GetPixelSave(x + i, y);
        const FLT Coef = BlurCoefs[std::abs(i)];

        Color.R += Coef * SrcColor.R;
        Color.G += Coef * SrcColor.G;
        Color.B += Coef * SrcColor.B;
      }

      Dest->SetPixel(x, y, Color);
    }
  });
}

/**
  * \brief Setup exposure for tone mapping function
  * \param[in] NewExposure Exposure
  */
VOID cpu_hdr::SetExposure( FLT NewExposure )
{
  Exposure = NewExposure;
}

/**
  * \brief Setup image size function
  * \param[in] W Width
  * \param[in] H Heigth
  */
VOID cpu_hdr::SetSize(const INT W, const INT H)
{
  PingPongBuffers[0].Resize(W, H);
  PingPongBuffers[1].Resize(W, H);
}

/**
  * \brief Setup number of bloom iterations function
  * \param[in] N Number of bloom iterations
  */
VOID cpu_hdr::SetNumberOfBloomIterations( const INT N )
{
  NumberOfBlumIterations = N;
}

/**
  * \brief Setup brighness limit for bloom function
  * \param[in] BrL Brighness limit (minimum color in grayscale)
  */
VOID cpu_hdr::SetBrighnesLimit( const FLT BrL )
{
  BrighnessLimit = BrL;
}

/**
  * \brief Apply HDR correction to image function
  * \param[in, out] Img Image for processing
  */
VOID cpu_hdr::Process( image *Img )
{
  // Bloom
  Bloom(Img);

  // Tone mapping
  //#pragma omp parallel for
  //for (INT y = 0; y < Img->FrameH; y++)
  parallel_for::Run(Img->FrameH, [&]( INT y )
  {
    for (INT x = 0; x < Img->FrameW; x++)
      Img->SetPixel(x, y, ToneCompression(Img->GetPixel(x, y)));
  });
}
