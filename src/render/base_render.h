#ifndef __base_render_h_
#define __base_render_h_

#include "utils/image.h"
#include "scene/scene.h"

class base_render
{
public:
  /**
   * \brief Render initialization function
   * \param[in] SelectedDeviceId Selected Vulkan device
   * \param[in] OneSeed One seed for all sample flag for Vulkan implementation
   */
  virtual VOID InitRender( UINT SelectedDeviceId, BOOL OneSeed ) = 0;

  /**
   * \brief Setup scene function
   * \param[in] NewScene Scene for render
   */
  //virtual VOID SetScene( scene &SceneForRender ) = 0;

  /**
   * \brief Setup camera function
   * \param[in] NewCamera Camera for render
   */
  //virtual VOID SetCamera( const cam &Camera ) = 0;

  /**
   * \brief Begin frame function
   * \param[in] W Frame width
   * \param[in] H Frame height
   */
  //virtual VOID StartFrame( const INT W, const INT H ) = 0;

  /**
   * \brief End frame function
   */
  //virtual VOID EndFrame( VOID ) = 0;

  /**
   * \brief Make sample function
   * \param[in, out] Im Image
   */
  //virtual VOID MakeSample( image *Im ) = 0;

  /**
   * \brief Generate sample image function
   * \param[in, out] Im Image
   */
  //virtual VOID ProcessHDR( image *Im ) = 0;

  /**
   * \brief Render frame function
   * \param[in, out] Im Image for render
   * \param[in] Camera Camera for render
   * \param[in, out] Scene Scene for render
   * \param[in] NumberOfSamples Number of samples
   */
  virtual VOID RenderFrame( image *Im, const cam &Camera, scene &Scene, INT NumberOfSamples ) = 0;

  /**
   * \brief Class virtual destructor
   */
  virtual ~base_render( VOID ) = default;
};

#endif /* __base_render_h_ */
