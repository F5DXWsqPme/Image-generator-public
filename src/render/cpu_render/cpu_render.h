#ifndef __cpu_render_h_
#define __cpu_render_h_

#include "utils/image.h"
#include "scene/scene.h"
#include "render/base_render.h"
#include "cpu_hdr.h"

/**
 * \brief CPU render
 */
class cpu_render : public base_render
{
private:
  /** Pointer to scene */
  scene *Scene = nullptr;
 
  /** Pointer to camera */
  const cam *Camera = nullptr;

  /** HDR correction class */
  cpu_hdr HDR;

  /**
   * \brief Setup scene function
   * \param[in] NewScene Scene for render
   */
  VOID SetScene( scene &NewScene );

  /**
   * \brief Setup camera function
   * \param[in] NewCamera Camera for render
   */
  VOID SetCamera( const cam &NewCamera );

  /**
   * \brief Apply HDR correction to image function
   * \param[in, out] Im Image
   */
  VOID ProcessHDR( image *Im );

  /**
   * \brief Generate sample image function
   * \param[in, out] Im Image
   */
  VOID MakeSample( image *Im );

  /**
   * \brief Begin frame function
   * \param[in] W Frame width
   * \param[in] H Frame height
   */
  VOID StartFrame( INT W, INT H );

  /**
   * \brief End frame function
   */
  VOID EndFrame( VOID );

public:
  /**
   * \brief Render initialization function
   * \param[in] SelectedDeviceId Selected vulkan device
   * \param[in] OneSeed One seed for all sample flag for Vulkan implementation
   */
  VOID InitRender( UINT SelectedDeviceId, BOOL OneSeed ) override;
  
  /**
   * \brief Render frame function
   * \param[in, out] Im Image for render
   * \param[in] Camera Camera for render
   * \param[in, out] Scene Scene for render
   * \param[in] NumberOfSamples Number of samples
   */
  VOID RenderFrame( image *Im, const cam &Camera, scene &Scene, INT NumberOfSamples ) override;
};

#endif /* __cpu_render_h_ */
