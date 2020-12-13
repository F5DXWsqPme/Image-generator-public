#ifndef __vulkan_render_h_
#define __vulkan_render_h_

#include <random>

#include "render/base_render.h"
#include "vulkan_wrappers/vulkan_application.h"
#include "vulkan_wrappers/shader_module.h"
#include "vulkan_wrappers/compute_pipeline.h"
#include "vulkan_wrappers/pipeline_layout.h"
#include "vulkan_wrappers/descriptor_set_layout.h"
#include "vulkan_wrappers/buffer.h"
#include "vulkan_wrappers/descriptor_pool.h"
#include "vulkan_wrappers/command_buffer.h"
#include "vulkan_wrappers/command_pool.h"
#include "vulkan_wrappers/queue.h"
#include "vulkan_wrappers/semaphore.h"
#include "render/cpu_render/cpu_hdr.h"

/**
 * \brief Vulkan render
 */
class vulkan_render : public base_render
{
private:
  static constexpr UINT64 DataAlignment = 256;

  /** Vulkan application class */
  vulkan_application VkApp;

  /** Render shader module */
  shader_module RenderShader;

  /** Scene data descriptor set layout */
  descriptor_set_layout RenderSceneDescriptionSetLayout;

  /** Out image descriptor set layout */
  descriptor_set_layout ImageSetLayout;

  /** Render pipeline pipeline layout */
  pipeline_layout RenderPipelineLayout;

  /** Render pipeline */
  compute_pipeline RenderPipeline;

  /** Descriptor pool */
  descriptor_pool DescriptorPool;

  /** Scene data descriptor set */
  VkDescriptorSet RenderSceneDescriptionSet = VK_NULL_HANDLE;

  /** Image descriptor set */
  VkDescriptorSet ImageSet = VK_NULL_HANDLE;

  /** Device storage memory */
  memory DeviceStorageMemory;

  /** Host storage memory */
  memory HostStorageMemory;

  /** Device uniform memory */
  memory DeviceUniformMemory;

  /** Host uniform memory */
  memory HostUniformMemory;

  /** Device uniform buffer */
  buffer DeviceUniformBuffer;

  /** Host uniform buffer */
  buffer HostUniformBuffer;

  /** Device storage buffer */
  buffer DeviceStorageBuffer;

  /** Host storage buffer */
  buffer HostStorageBuffer;

  /** Need copy uniform buffer flag */
  BOOL NeedCopyUniform = FALSE;

  /** Need copy storage buffer flag */
  BOOL NeedCopyStorage = FALSE;

  /** Compute queue */
  queue ComputeQueue;

  /** Transfer queue */
  queue TransferQueue;

  /** Compute command pool */
  command_pool ComputeCommandPool;

  /** Transfer command pool */
  command_pool TransferCommandPool;

  /** Semaphore for waiting transfer render arguments */
  semaphore TransferRenderArgumentsSemaphore;

  /** Shader arguments offset */
  UINT64 ShaderArgumentsOffset;

  /** Scene data size */
  UINT64 SceneDataSize;

  /** Uniform buffer size */
  UINT64 UniformBufferSize;

  /** Random numbers offset */
  UINT64 RandomNumbersOffset;

  /** Random numbers size */
  UINT64 RandomNumbersSize;

  /** Materials offset */
  UINT64 MaterialsOffset;

  /** Environments offset */
  UINT64 EnvironmentsOffset;

  /** Triangles offset */
  UINT64 TrianglesOffset;

  /** Nodes offset */
  UINT64 NodesOffset;

  /** Image offset */
  UINT64 ImageOffset;

  /** Image size */
  UINT64 ImageSize;

  /** Storage buffer size */
  UINT64 StorageBufferSize;

  /** Kd-tree elements sizes */
  kd_tree::PACKED_SCENE_ELEMENTS_SIZES Sizes;

  /** Random numbers generator */
  std::mt19937 Gen;

  /** HDR correction class */
  cpu_hdr HDR;

  /**
   * \brief Create render pipeline layout function.
   */
  VOID CreatePipelineLayout( VOID );

  /**
   * \brief Create render pipeline
   */
  VOID CreateRenderPipeline( VOID );

  /**
   * \brief Create descriptor pool and allocate descriptor sets function.
   */
  VOID CreateDescriptorPoolAndAllocateSets( VOID );

  /**
   * \brief Write descriptor sets function.
   */
  VOID WriteDescriptorSets( VOID );

  /**
   * \brief Create buffers for shader function
   * \param[in] W Image width
   * \param[in] H Image height
   * \param[in] Camera Camera for render
   * \param[in] Scene Scene for render
   * \param[in] Sizes Packed scene elements sizes
   */
  //VOID CreateBuffers( INT W, INT H, const cam &Camera, const scene &Scene,
  //                    const kd_tree::PACKED_SCENE_ELEMENTS_SIZES &Sizes );

  /**
   * \brief Allocate memory and create buffers (and bind memory)
   * \param[in] W Image width
   * \param[in] H Image height
   * \param[in] Camera Camera for render
   * \param[in] Scene Scene for render
   */
  VOID CreateBuffersAndAllocateMemory( INT W, INT H, const cam &Camera, const scene &Scene );

  /**
   * \brief Find memory type for device buffer.
   * \param[in] MemoryRequirements Buffer memory requirements
   * \return Memory type
   */
  std::optional<UINT32> FindDeviceMemoryType( const VkMemoryRequirements &MemoryRequirements ) const;

  /**
   * \brief Evaluate offsets and sizes
   * \param[in] W Image width
   * \param[in] H Image height
   * \param[in] Camera Camera for render
   * \param[in] Scene Scene for render
   */
  VOID EvaluateOffsetsAndSizes( INT W, INT H, const cam &Camera, const scene &Scene );

  /**
   * \brief Copy parameters to GPU memory (asynchronous execution! the method does not wait for commands to complete)
   * \param[in] W Image width
   * \param[in] H Image height
   * \param[in] Camera Camera for render
   * \param[in] Scene Scene for render
   * \param[in] Tree Scene Kd-tree
   */
  VOID CopyParametersToGPUMemory( INT W, INT H, const cam &Camera, const scene &Scene, const kd_tree &Tree );

  /**
   * \brief Run shader function
   * \param[in] W Image width
   * \param[in] H Image height
   * \param[in] NumberOfSamples Number of samples
   */
  VOID RunRenderShader( INT W, INT H, INT NumberOfSamples );

  /**
   * \brief Copy image back  to CPU
   * \param Im Destination image
   */
  VOID CopyImageToCPU( image *Im ) const;

  /**
   * \brief Apply HDR correction to image function
   * \param[in, out] Im Image
   */
  VOID ProcessHDR( image *Im );

public:
  /**
   * \brief Default constructor.
   * \param[in] DeviceId Vulkan device identifier
   */
  vulkan_render( UINT DeviceId );

  /**
   * \brief Render frame function
   * \param[in, out] Im Image for render
   * \param[in] Camera Camera for render
   * \param[in, out] Scene Scene for render
   * \param[in] NumberOfSamples Number of samples
   */
  VOID RenderFrame( image *Im, const cam &Camera, scene &Scene, INT NumberOfSamples ) override;

  /**
   * \brief Render initialization function
   * \param[in] SelectedDeviceId Selected vulkan device
   * \param[in] OneSeed One seed for all sample flag for Vulkan implementation
   */
  VOID InitRender( UINT SelectedDeviceId, BOOL OneSeed ) override;
};

#endif /* __vulkan_render_h_ */
