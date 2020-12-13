#ifndef __image_h_
#define __image_h_

#include <vector>
#include <string>

#include "math/vec.h"

/**
 * \brief Image structure
 */
class image
{
private:
  /** Buffer with image */
  std::vector<image_vec> Buffer;
 
  /** Image width */
  INT W = 0;

  /** Image height */
  INT H = 0;

public:
  /** Reference to image width */
  const INT &FrameW = W;
  
  /** Reference to image height */
  const INT &FrameH = H;

  /**
   * \brief Image format enumeration
   */
  enum class FORMAT
  {
    /** PNG image format */
    PNG,

    /** JPG image format */
    JPG,

    /** TGA image format */
    TGA
  };

  /**
   * \brief Image constructor
   */
  image( VOID ) = default;

  /**
   * \brief Clear image function
   */
  VOID Clear( VOID );

  /**
   * \brief Get pixel from image function
   * \param[in] X First coordinate
   * \param[in] Y Second coordinate
   * \return Pixel color
   */
  const image_vec & GetPixel( const INT X, const INT Y ) const;
  
  /**
   * \brief Get pixel from image function (with width and height test)
   * \param[in] X First coordinate
   * \param[in] Y Second coordinate
   * \return Pixel color
   */
  const image_vec & GetPixelSave( const INT X, const INT Y ) const;

  /**
   * \brief Set pixel function
   * \param[in] X First coordinate
   * \param[in] Y Second coordinate
   * \param[in] Color Pixel color
   */
  VOID SetPixel( const INT X, const INT Y, const image_vec &Color );
  
  /**
   * \brief Resize image function
   * \param[in] NewW New image width
   * \param[in] NewH New image height
   */
  VOID Resize( const INT NewW, const INT NewH );

  //  VOID ApplySample( const image &Img, const INT NumOfSamples );

  /**
   * \brief Addition images function
   * \param[in] Img Image for addition
   */
  VOID operator+=( const image &Img );
  
  /**
   * \brief Division image by number function
   * \param[in] N Number
   */
  VOID operator/=( const DBL N );

  /**
   * \brief Save image function (tga format)
   * \param[in] FileName Name of file
   */
  VOID SaveTGA( const std::string &FileName ) const;

  /**
   * \brief Save image function (png format)
   * \param[in] FileName Name of file
   */
  VOID SavePNG( const std::string &FileName ) const;
  
  /**
   * \brief Save image function (jpg format)
   * \param[in] FileName Name of file
   */
  VOID SaveJPEG( const std::string &FileName ) const;

  /**
   * \brief Save image function
   * \param[in] FileName Name of file
   * \param[in] Format File format
   */
  VOID Save( const std::string &FileName, FORMAT Format ) const;

  /**
   * \brief Removed copy function
   * \param[in] Img Other image
   */
  VOID operator=( const image &Img ) = delete;

  /**
   * \brief Removed copy function
   * \param[in] Img Other image
   */
  image( const image &Img ) = delete;
};

#endif /* __image_h */
