#ifndef __render_h_
#define __render_h_

#include "cpu_render/cpu_render.h"
#include "vulkan_render/vulkan_render.h"

/**
 * \brief Renders handler
 */
class render
{
private:
  /** Render CPU implementation */
  cpu_render RndCPU;

  /** Render vulkan compute shaders implementation */
  vulkan_render RndVulkan;

  /** Pointer to selected render */
  base_render *RndPtr = &RndCPU;
  
public:
  /**
   * \brief Render constructor
   * \param[in] DeviceId Vulkan device identifier
   */
  render( UINT DeviceId = 0 );
  
  /**
   * \brief Render mode enumeration
   */
  enum class MODE
  {
    /** CPU implementation */
    CPU,

    /** Vulkan compute implementation */
    VULKAN,

    /** Vulkan compute implementation for ont seed for all sample */
    VULKAN_ONE_SEED,
  };
  
  /**
   * \brief Setup render mode function
   * \param[in] RndMode New render mode
   * \param[in] DeviceId Vulkan device identifier
   */
  VOID SetRenderMode( MODE RndMode, UINT DeviceId = 0 );
  
  /**
   * \brief Make one frame function
   * \param[in, out] Img Image for render
   * \param[in] Camera Camera for render
   * \param[in] Scene Scene for render
   * \param[in] W Image width
   * \param[in] H Image heigth
   * \param[in] NumOfSamples Number of samples
   */
  VOID MakeFrame( image *Img, const cam &Camera, scene &Scn, const INT W, const INT H,
                  const INT NumOfSamples = 100 ) const;
};

#endif /* __render_h_ */

