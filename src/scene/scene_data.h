#ifndef __gpu_scene_h_
#define __gpu_scene_h_

#include <vector>

#include "aabb.h"
#include "params.h"
#include "environment.h"
#include "cam.h"

#pragma pack(push, 4)

/**
 * \brief Scene structure data
 */
struct SCENE_DATA
{
  /**
   * \brief Compile-time alignment test.
   */
  static VOID AlignmentTestMethod( VOID )
  {
    static_assert(sizeof(SCENE_DATA) == sizeof(cam_data) + sizeof(RENDER_PARAMS) + sizeof(environment) + 16);
    static_assert(std::is_trivial<SCENE_DATA>::value && std::is_standard_layout<SCENE_DATA>::value);
  }

public:
  /** Camera structure */
  cam_data Cam;

  /** Render parameters */
  RENDER_PARAMS Par;

  /** Default environment */
  environment AirEnvi;

  /** Image width */
  UINT32 Width;

  /** Image height */
  UINT32 Height;

  /** Not used padding */
  UINT32 _Padding[2];
};

#pragma pack(pop)

#endif /* __gpu_scene_h_ */
