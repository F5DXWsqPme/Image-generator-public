#ifndef __cpu_hdr_h_
#define __cpu_hdr_h_

#include "utils/image.h"

/**
 * \brief HDR correction 
 */
class cpu_hdr
{
private:
  /** Exposure coefficient */
  FLT Exposure = 1.5f;

  /** Number of bluum iterations */
  INT NumberOfBlumIterations = 5;

  /** Buffers for bloom  */
  image PingPongBuffers[2];

  /** Brighnes limit for bloom */
  FLT BrighnessLimit = 1;

  /** Blur coefficients for bloom */
  const FLT BlurCoefs[5] = {0.227027f, 0.1945946f, 0.1216216f, 0.054054f, 0.016216f};

  /**
   * \brief Tone mapping function
   * \param[in] Src Vector before tone compression
   * \return Compressed color
   */
  image_vec ToneCompression( const image_vec &Src ) const;
  
  /**
   * \brief Bloom function
   * \param[in, out] Img Image
   */
  VOID Bloom( image *Img );

  /**
   * \brief Blur vertiacal iteration function
   * \param[in] Src Source image
   * \param[in, out] Dest Destination image
   */
  VOID BlurVerticalIteration( const image &Src, image *Dest );

  /**
   * \brief Blur horisontal iteration function
   * \param[in] Src Source image
   * \param[in, out] Dest Destination image
   */
  VOID BlurHorisontalIteration( const image &Src, image *Dest );

public:
  /**
   * \brief Setup exposure for tone mapping function
   * \param[in] NewExposure Exposure
   */
  VOID SetExposure( FLT NewExposure );

  /**
   * \brief Setup image size function
   * \param[in] W Width
   * \param[in] H Heigth
   */
  VOID SetSize( const INT W, const INT H );

  /**
   * \brief Setup number of bloom iterations function
   * \param[in] N Number of bloom iterations
   */
  VOID SetNumberOfBloomIterations( const INT N );

  /**
   * \brief Setup brighness limit for bloom function
   * \param[in] BrL Brighness limit (minimum color in grayscale)
   */
  VOID SetBrighnesLimit( const FLT BrL );

  /**
   * \brief Apply HDR correction to image function
   * \param[in, out] Img Image for processing
   */
  VOID Process( image *Img );
};

#endif /* __cpu_hdr_h_ */
