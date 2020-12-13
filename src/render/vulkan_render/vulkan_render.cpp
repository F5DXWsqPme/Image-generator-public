#include <iostream>
#include <cstdio>
#include <ctime>
#include <cstring>

#include "vulkan_render.h"
#include "utils/error.h"
#include "scene/material.h"
#include "scene/environment.h"
#include "vulkan_wrappers/fence.h"
#include "vulkan_wrappers/vulkan_validation.h"
#include "vulkan_wrappers/event.h"

/**
 * \brief Default constructor.
 * \param[in] DeviceId Vulkan device identifier
 */
vulkan_render::vulkan_render( UINT DeviceId )
{
}

/**
 * \brief Create render pipeline layout function.
 */
VOID vulkan_render::CreatePipelineLayout( VOID )
{
  VkDescriptorSetLayoutBinding SceneDescriptionLayoutBindings[6] = {};

  SceneDescriptionLayoutBindings[0].binding = 0;
  SceneDescriptionLayoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  SceneDescriptionLayoutBindings[0].descriptorCount = 1;
  SceneDescriptionLayoutBindings[0].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
  SceneDescriptionLayoutBindings[0].pImmutableSamplers = nullptr;

  SceneDescriptionLayoutBindings[1].binding = 1;
  SceneDescriptionLayoutBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  SceneDescriptionLayoutBindings[1].descriptorCount = 1;
  SceneDescriptionLayoutBindings[1].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
  SceneDescriptionLayoutBindings[1].pImmutableSamplers = nullptr;

  SceneDescriptionLayoutBindings[2].binding = 2;
  SceneDescriptionLayoutBindings[2].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  SceneDescriptionLayoutBindings[2].descriptorCount = 1;
  SceneDescriptionLayoutBindings[2].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
  SceneDescriptionLayoutBindings[2].pImmutableSamplers = nullptr;

  SceneDescriptionLayoutBindings[3].binding = 3;
  SceneDescriptionLayoutBindings[3].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  SceneDescriptionLayoutBindings[3].descriptorCount = 1;
  SceneDescriptionLayoutBindings[3].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
  SceneDescriptionLayoutBindings[3].pImmutableSamplers = nullptr;

  SceneDescriptionLayoutBindings[4].binding = 4;
  SceneDescriptionLayoutBindings[4].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  SceneDescriptionLayoutBindings[4].descriptorCount = 1;
  SceneDescriptionLayoutBindings[4].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
  SceneDescriptionLayoutBindings[4].pImmutableSamplers = nullptr;

  SceneDescriptionLayoutBindings[5].binding = 5;
  SceneDescriptionLayoutBindings[5].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  SceneDescriptionLayoutBindings[5].descriptorCount = 1;
  SceneDescriptionLayoutBindings[5].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
  SceneDescriptionLayoutBindings[5].pImmutableSamplers = nullptr;

  RenderSceneDescriptionSetLayout =
    descriptor_set_layout(VkApp.GetDeviceId(), 6, SceneDescriptionLayoutBindings );

  VkDescriptorSetLayoutBinding ImageLayoutBindings[1] = {};

  ImageLayoutBindings[0].binding = 0;
  ImageLayoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  ImageLayoutBindings[0].descriptorCount = 1;
  ImageLayoutBindings[0].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
  ImageLayoutBindings[0].pImmutableSamplers = nullptr;

  ImageSetLayout =
    descriptor_set_layout(VkApp.GetDeviceId(), 1, ImageLayoutBindings);

  VkDescriptorSetLayout VulkanDescriptorSetLayoutsId[2] =
  {
    RenderSceneDescriptionSetLayout.GetSetLayoutId(),
    ImageSetLayout.GetSetLayoutId()
  };

  VkPushConstantRange PushConstantRange = {};

  PushConstantRange.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
  PushConstantRange.offset = 0;
  PushConstantRange.size = 2 * sizeof(UINT32);

  RenderPipelineLayout =
    pipeline_layout(VkApp.GetDeviceId(), 2, VulkanDescriptorSetLayoutsId,
                    1, &PushConstantRange);
}

/**
   * \brief Create render pipeline
   */
VOID vulkan_render::CreateRenderPipeline( VOID )
{
  pipeline_cache EmptyCache(VkApp.GetDeviceId());

  RenderPipeline =
    compute_pipeline(VkApp.GetDeviceId(), RenderPipelineLayout.GetPipelineLayoutId(),
                     RenderShader, "main", EmptyCache);
}

/**
 * \brief Create descriptor pool and allocate descriptor sets function.
 */
VOID vulkan_render::CreateDescriptorPoolAndAllocateSets( VOID )
{
  VkDescriptorPoolSize DescriptorPoolSizes[2];

  DescriptorPoolSizes[0].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  DescriptorPoolSizes[0].descriptorCount = 6;

  DescriptorPoolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  DescriptorPoolSizes[1].descriptorCount = 1;

  DescriptorPool =
    descriptor_pool(VkApp.GetDeviceId(), 0, 2, 2, DescriptorPoolSizes);

  VkDescriptorSetLayout DescriptorSetsLayout[2] =
  {
    RenderSceneDescriptionSetLayout.GetSetLayoutId(),
    ImageSetLayout.GetSetLayoutId()
  };

  VkDescriptorSet DescriptorSets[2] = {};

  DescriptorPool.AllocateSets(DescriptorSets, 2, DescriptorSetsLayout);

  RenderSceneDescriptionSet = DescriptorSets[0];
  ImageSet = DescriptorSets[1];
}

/**
 * \brief Write descriptor sets function.
 */
VOID vulkan_render::WriteDescriptorSets( VOID )
{
  VkWriteDescriptorSet WriteDescriptorSetStructures[7] = {};
  VkDescriptorBufferInfo BufferInfoArray[7] = {};

  BufferInfoArray[0].buffer = DeviceUniformBuffer.GetBufferId();
  BufferInfoArray[0].offset = ShaderArgumentsOffset;
  BufferInfoArray[0].range = SceneDataSize;

  WriteDescriptorSetStructures[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  WriteDescriptorSetStructures[0].pNext = nullptr;
  WriteDescriptorSetStructures[0].dstSet = RenderSceneDescriptionSet;
  WriteDescriptorSetStructures[0].dstBinding = 0;
  WriteDescriptorSetStructures[0].dstArrayElement = 0;
  WriteDescriptorSetStructures[0].descriptorCount = 1;
  WriteDescriptorSetStructures[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  WriteDescriptorSetStructures[0].pImageInfo = nullptr;
  WriteDescriptorSetStructures[0].pBufferInfo = &BufferInfoArray[0];
  WriteDescriptorSetStructures[0].pTexelBufferView = nullptr;

  BufferInfoArray[1].buffer = DeviceStorageBuffer.GetBufferId();
  BufferInfoArray[1].offset = RandomNumbersOffset;
  BufferInfoArray[1].range = RandomNumbersSize;

  WriteDescriptorSetStructures[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  WriteDescriptorSetStructures[1].pNext = nullptr;
  WriteDescriptorSetStructures[1].dstSet = RenderSceneDescriptionSet;
  WriteDescriptorSetStructures[1].dstBinding = 1;
  WriteDescriptorSetStructures[1].dstArrayElement = 0;
  WriteDescriptorSetStructures[1].descriptorCount = 1;
  WriteDescriptorSetStructures[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  WriteDescriptorSetStructures[1].pImageInfo = nullptr;
  WriteDescriptorSetStructures[1].pBufferInfo = &BufferInfoArray[1];
  WriteDescriptorSetStructures[1].pTexelBufferView = nullptr;

  BufferInfoArray[2].buffer = DeviceStorageBuffer.GetBufferId();
  BufferInfoArray[2].offset = MaterialsOffset;
  BufferInfoArray[2].range = Sizes.MaterialsSize;

  WriteDescriptorSetStructures[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  WriteDescriptorSetStructures[2].pNext = nullptr;
  WriteDescriptorSetStructures[2].dstSet = RenderSceneDescriptionSet;
  WriteDescriptorSetStructures[2].dstBinding = 2;
  WriteDescriptorSetStructures[2].dstArrayElement = 0;
  WriteDescriptorSetStructures[2].descriptorCount = 1;
  WriteDescriptorSetStructures[2].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  WriteDescriptorSetStructures[2].pImageInfo = nullptr;
  WriteDescriptorSetStructures[2].pBufferInfo = &BufferInfoArray[2];
  WriteDescriptorSetStructures[2].pTexelBufferView = nullptr;

  BufferInfoArray[3].buffer = DeviceStorageBuffer.GetBufferId();
  BufferInfoArray[3].offset = EnvironmentsOffset;
  BufferInfoArray[3].range = Sizes.EnvironmentsSize;

  WriteDescriptorSetStructures[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  WriteDescriptorSetStructures[3].pNext = nullptr;
  WriteDescriptorSetStructures[3].dstSet = RenderSceneDescriptionSet;
  WriteDescriptorSetStructures[3].dstBinding = 3;
  WriteDescriptorSetStructures[3].dstArrayElement = 0;
  WriteDescriptorSetStructures[3].descriptorCount = 1;
  WriteDescriptorSetStructures[3].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  WriteDescriptorSetStructures[3].pImageInfo = nullptr;
  WriteDescriptorSetStructures[3].pBufferInfo = &BufferInfoArray[3];
  WriteDescriptorSetStructures[3].pTexelBufferView = nullptr;

  BufferInfoArray[4].buffer = DeviceStorageBuffer.GetBufferId();
  BufferInfoArray[4].offset = TrianglesOffset;
  BufferInfoArray[4].range = Sizes.TrianglesSize;

  WriteDescriptorSetStructures[4].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  WriteDescriptorSetStructures[4].pNext = nullptr;
  WriteDescriptorSetStructures[4].dstSet = RenderSceneDescriptionSet;
  WriteDescriptorSetStructures[4].dstBinding = 4;
  WriteDescriptorSetStructures[4].dstArrayElement = 0;
  WriteDescriptorSetStructures[4].descriptorCount = 1;
  WriteDescriptorSetStructures[4].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  WriteDescriptorSetStructures[4].pImageInfo = nullptr;
  WriteDescriptorSetStructures[4].pBufferInfo = &BufferInfoArray[4];
  WriteDescriptorSetStructures[4].pTexelBufferView = nullptr;

  BufferInfoArray[5].buffer = DeviceStorageBuffer.GetBufferId();
  BufferInfoArray[5].offset = NodesOffset;
  BufferInfoArray[5].range = Sizes.NodesSize;

  WriteDescriptorSetStructures[5].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  WriteDescriptorSetStructures[5].pNext = nullptr;
  WriteDescriptorSetStructures[5].dstSet = RenderSceneDescriptionSet;
  WriteDescriptorSetStructures[5].dstBinding = 5;
  WriteDescriptorSetStructures[5].dstArrayElement = 0;
  WriteDescriptorSetStructures[5].descriptorCount = 1;
  WriteDescriptorSetStructures[5].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  WriteDescriptorSetStructures[5].pImageInfo = nullptr;
  WriteDescriptorSetStructures[5].pBufferInfo = &BufferInfoArray[5];
  WriteDescriptorSetStructures[5].pTexelBufferView = nullptr;

  BufferInfoArray[6].buffer = DeviceStorageBuffer.GetBufferId();
  BufferInfoArray[6].offset = ImageOffset;
  BufferInfoArray[6].range = ImageSize;

  WriteDescriptorSetStructures[6].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  WriteDescriptorSetStructures[6].pNext = nullptr;
  WriteDescriptorSetStructures[6].dstSet = ImageSet;
  WriteDescriptorSetStructures[6].dstBinding = 0;
  WriteDescriptorSetStructures[6].dstArrayElement = 0;
  WriteDescriptorSetStructures[6].descriptorCount = 1;
  WriteDescriptorSetStructures[6].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  WriteDescriptorSetStructures[6].pImageInfo = nullptr;
  WriteDescriptorSetStructures[6].pBufferInfo = &BufferInfoArray[6];
  WriteDescriptorSetStructures[6].pTexelBufferView = nullptr;

  vkUpdateDescriptorSets(VkApp.GetDeviceId(), 7, WriteDescriptorSetStructures, 0, nullptr);
}

/**
 * \brief Create buffers for shader function
 * \param[in] W Image width
 * \param[in] H Image height
 * \param[in] Camera Camera for render
 * \param[in] Scene Scene for render
 * \param[in] Sizes Packed scene elements sizes
 */
//VOID vulkan_render::CreateBuffers( INT W, INT H, const cam &Camera, const scene &Scene,
//                                   const kd_tree::PACKED_SCENE_ELEMENTS_SIZES &Sizes )
//{
//  UINT32 NumberOfFamilyIndices = 0;
//  UINT32 FamilyIndices[2] = {};
//  VkBufferUsageFlags AdditionalDeviceBufferUsage = 0;
//  VkBufferUsageFlags AdditionalHostBufferUsage = 0;
//  VkSharingMode BufferSharingMode = VK_SHARING_MODE_EXCLUSIVE;
//
//  if (VkApp.NeedCopyBuffers && VkApp.TransferQueueFamilyIndex != VkApp.ComputeQueueFamilyIndex)
//  {
//    NumberOfFamilyIndices = 2;
//    FamilyIndices[0] = *VkApp.TransferQueueFamilyIndex;
//    FamilyIndices[1] = *VkApp.ComputeQueueFamilyIndex;
//    AdditionalDeviceBufferUsage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
//    AdditionalHostBufferUsage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
//    BufferSharingMode = VK_SHARING_MODE_CONCURRENT;
//  }
//
//  DeviceShaderArgumentsBuffer =
//    buffer(VkApp.GetDeviceId(), sizeof(SCENE_DATA),
//           VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | AdditionalDeviceBufferUsage,
//           0, BufferSharingMode, NumberOfFamilyIndices, FamilyIndices);
//
//
//  DeviceRandomNumbersBuffer =
//    buffer(VkApp.GetDeviceId(), W * H * sizeof(FLT) * Scene.RenderPar.MaxDepthRender * 3,
//           VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | AdditionalDeviceBufferUsage,
//           0, BufferSharingMode, NumberOfFamilyIndices, FamilyIndices);
//
//  DeviceMaterialsBuffer =
//    buffer(VkApp.GetDeviceId(), Sizes.MaterialsSize,
//           VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | AdditionalDeviceBufferUsage,
//           0, BufferSharingMode, NumberOfFamilyIndices, FamilyIndices);
//
//  DeviceEnvironmentsBuffer =
//    buffer(VkApp.GetDeviceId(), Sizes.EnvironmentsSize,
//           VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | AdditionalDeviceBufferUsage,
//           0, BufferSharingMode, NumberOfFamilyIndices, FamilyIndices);
//
//
//  DeviceTrianglesBuffer =
//    buffer(VkApp.GetDeviceId(), Sizes.TrianglesSize,
//           VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | AdditionalDeviceBufferUsage,
//           0, BufferSharingMode, NumberOfFamilyIndices, FamilyIndices);
//
//  DeviceNodesBuffer =
//      buffer(VkApp.GetDeviceId(), Sizes.NodesSize,
//             VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | AdditionalDeviceBufferUsage,
//             0, BufferSharingMode, NumberOfFamilyIndices, FamilyIndices);
//
//  DeviceImageBuffer =
//        buffer(VkApp.GetDeviceId(), W * H * sizeof(vec),
//               VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | AdditionalDeviceBufferUsage,
//               0, BufferSharingMode, NumberOfFamilyIndices, FamilyIndices);
//
//  if (VkApp.NeedCopyBuffers)
//  {
//    HostShaderArgumentsBuffer =
//      buffer(VkApp.GetDeviceId(), sizeof(SCENE_DATA),
//             VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | AdditionalHostBufferUsage,
//             0, BufferSharingMode, NumberOfFamilyIndices, FamilyIndices);
//
//
//    HostRandomNumbersBuffer =
//      buffer(VkApp.GetDeviceId(), W * H * sizeof(FLT) * Scene.RenderPar.MaxDepthRender * 3,
//             VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | AdditionalHostBufferUsage,
//             0, BufferSharingMode, NumberOfFamilyIndices, FamilyIndices);
//
//    HostMaterialsBuffer =
//      buffer(VkApp.GetDeviceId(), Sizes.MaterialsSize,
//             VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | AdditionalHostBufferUsage,
//             0, BufferSharingMode, NumberOfFamilyIndices, FamilyIndices);
//
//    HostEnvironmentsBuffer =
//      buffer(VkApp.GetDeviceId(), Sizes.EnvironmentsSize,
//             VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | AdditionalHostBufferUsage,
//             0, BufferSharingMode, NumberOfFamilyIndices, FamilyIndices);
//
//
//    HostTrianglesBuffer =
//      buffer(VkApp.GetDeviceId(), Sizes.TrianglesSize,
//             VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | AdditionalHostBufferUsage,
//             0, BufferSharingMode, NumberOfFamilyIndices, FamilyIndices);
//
//    HostNodesBuffer =
//      buffer(VkApp.GetDeviceId(), Sizes.NodesSize,
//             VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | AdditionalHostBufferUsage,
//             0, BufferSharingMode, NumberOfFamilyIndices, FamilyIndices);
//
//    HostImageBuffer =
//      buffer(VkApp.GetDeviceId(), W * H * sizeof(vec),
//             VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | AdditionalHostBufferUsage,
//             0, BufferSharingMode, NumberOfFamilyIndices, FamilyIndices);
//  }
//}

/**
 * \brief Find memory type for device buffer.
 * \param[in] MemoryRequirements Buffer memory requirements
 * \return Memory type
 */
std::optional<UINT32> vulkan_render::FindDeviceMemoryType( const VkMemoryRequirements &MemoryRequirements ) const
{
  std::optional<UINT32> DeviceMemoryType =
    VkApp.FindMemoryTypeWithFlags(MemoryRequirements,
                                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

  if (!DeviceMemoryType)
  {
    DeviceMemoryType =
      VkApp.FindMemoryTypeWithFlags(MemoryRequirements,
                                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (!DeviceMemoryType)
    {
      DeviceMemoryType =
        VkApp.FindMemoryTypeWithFlags(MemoryRequirements,
                                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

      if (!DeviceMemoryType)
      {
        DeviceMemoryType =
          VkApp.FindMemoryTypeWithFlags(MemoryRequirements,
                                        0);
      }
    }
  }

  return DeviceMemoryType;
}

/**
 * \brief Evaluate offsets and sizes
 * \param[in] W Image width
 * \param[in] H Image height
 * \param[in] Camera Camera for render
 * \param[in] Scene Scene for render
 */
VOID vulkan_render::EvaluateOffsetsAndSizes( INT W, INT H, const cam &Camera, const scene &Scene )
{
  UINT64 Offset = 0;

  ShaderArgumentsOffset = 0;
  SceneDataSize = sizeof(SCENE_DATA);

  Offset += DataAlignment * ((SceneDataSize + DataAlignment - 1) / DataAlignment);

  UniformBufferSize = Offset;

  Offset = 0;

  RandomNumbersOffset = Offset;
  RandomNumbersSize = W * H * sizeof(UINT32);

  Offset += DataAlignment * ((RandomNumbersSize + DataAlignment - 1) / DataAlignment);

  MaterialsOffset = Offset;

  Offset += Sizes.MaterialsSizeAlignment;

  EnvironmentsOffset = Offset;

  Offset += Sizes.EnvironmentsSizeAlignment;

  TrianglesOffset = Offset;

  Offset += Sizes.TrianglesSizeAlignment;

  NodesOffset = Offset;

  Offset += Sizes.NodesSizeAlignment;

  ImageOffset = Offset;
  ImageSize = W * H * sizeof(vec);

  Offset += DataAlignment * ((ImageSize + DataAlignment - 1) / DataAlignment);

  StorageBufferSize = Offset;
}

/**
 * \brief Allocate memory and create buffers (and bind memory)
 * \param[in] W Image width
 * \param[in] H Image height
 * \param[in] Camera Camera for render
 * \param[in] Scene Scene for render
 */
VOID vulkan_render::CreateBuffersAndAllocateMemory( INT W, INT H, const cam &Camera, const scene &Scene )
{
  EvaluateOffsetsAndSizes(W, H, Camera, Scene);

  DeviceUniformBuffer =
    buffer(VkApp.GetDeviceId(), UniformBufferSize,
           VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
           0, VK_SHARING_MODE_EXCLUSIVE, 0, nullptr);

  DeviceStorageBuffer =
    buffer(VkApp.GetDeviceId(), StorageBufferSize,
           VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
           0, VK_SHARING_MODE_EXCLUSIVE, 0, nullptr);

  std::optional<UINT32> DeviceUniformMemoryType =
    FindDeviceMemoryType(DeviceUniformBuffer.GetMemoryRequirements());

  if (!DeviceUniformMemoryType)
    error("don't found uniform memory");

  if ((VkApp.DeviceMemoryProperties.memoryTypes[*DeviceUniformMemoryType].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) == 0)
    NeedCopyUniform = TRUE;

  std::optional<UINT32> DeviceStorageMemoryType =
    FindDeviceMemoryType(DeviceStorageBuffer.GetMemoryRequirements());

  if (!DeviceStorageMemoryType)
    error("don't found storage memory");

  if ((VkApp.DeviceMemoryProperties.memoryTypes[*DeviceStorageMemoryType].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) == 0)
    NeedCopyStorage = TRUE;

  if ((NeedCopyUniform || NeedCopyStorage) && !VkApp.TransferQueueFamilyIndex)
    error("don't found transfer queue");
  else if (NeedCopyUniform || NeedCopyStorage)
  {
    TransferQueue = queue(VkApp.GetDeviceId(), *VkApp.ComputeQueueFamilyIndex, 0);
    TransferCommandPool =
      command_pool(VkApp.GetDeviceId(), *VkApp.ComputeQueueFamilyIndex, VK_COMMAND_POOL_CREATE_TRANSIENT_BIT);
  }

  DeviceUniformMemory = memory(VkApp.GetDeviceId(), UniformBufferSize, *DeviceUniformMemoryType);
  DeviceStorageMemory = memory(VkApp.GetDeviceId(), StorageBufferSize, *DeviceStorageMemoryType);
  DeviceUniformBuffer.BindMemory(DeviceUniformMemory, 0);
  DeviceStorageBuffer.BindMemory(DeviceStorageMemory, 0);

  if (NeedCopyUniform)
  {
    HostUniformBuffer =
      buffer(VkApp.GetDeviceId(), UniformBufferSize,
             VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
             0, VK_SHARING_MODE_EXCLUSIVE, 0, nullptr);

    std::optional<UINT32> HostUniformMemoryType =
      VkApp.FindMemoryTypeWithFlags(HostUniformBuffer.GetMemoryRequirements(),
                                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

    if (!HostUniformMemoryType)
      error("don't found host visible memory for uniform buffer");

    HostUniformMemory = memory(VkApp.GetDeviceId(), UniformBufferSize, *HostUniformMemoryType);
    HostUniformBuffer.BindMemory(HostUniformMemory, 0);
  }

  if (NeedCopyStorage)
  {
    HostStorageBuffer =
      buffer(VkApp.GetDeviceId(), StorageBufferSize,
             VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
             0, VK_SHARING_MODE_EXCLUSIVE, 0, nullptr);

    std::optional<UINT32> HostStorageMemoryType =
      VkApp.FindMemoryTypeWithFlags(HostStorageBuffer.GetMemoryRequirements(),
                                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

    if (!HostStorageMemoryType)
      error("don't found host visible memory for storage buffer");

    HostStorageMemory = memory(VkApp.GetDeviceId(), StorageBufferSize, *HostStorageMemoryType);
    HostStorageBuffer.BindMemory(HostStorageMemory, 0);
  }
}

/**
 * \brief Copy parameters to GPU memory (asynchronous execution! the method does not wait for commands to complete.)
 * \param[in] W Image width
 * \param[in] H Image height
 * \param[in] Camera Camera for render
 * \param[in] Scene Scene for render
 * \param[in] Tree Scene Kd-tree
 */
VOID vulkan_render::CopyParametersToGPUMemory( INT W, INT H, const cam &Camera, const scene &Scene, const kd_tree &Tree )
{
  BYTE *Data;
  memory *CurMemory = &DeviceUniformMemory;

  if (NeedCopyUniform)
    CurMemory = &HostUniformMemory;

  CurMemory->MapMemory(0, VK_WHOLE_SIZE, reinterpret_cast<VOID **>(&Data));

  reinterpret_cast<SCENE_DATA *>(Data)->AirEnvi = Scene.AirEnvi;
  Camera.FillCamData(&reinterpret_cast<SCENE_DATA *>(Data)->Cam);
  reinterpret_cast<SCENE_DATA *>(Data)->Par = Scene.RenderPar;
  reinterpret_cast<SCENE_DATA *>(Data)->Height = H;
  reinterpret_cast<SCENE_DATA *>(Data)->Width = W;

  if ((VkApp.DeviceMemoryProperties.memoryTypes[CurMemory->MemoryType].propertyFlags &
       VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == 0)
  {
    CurMemory->FlushAllRange(0);
  }

  CurMemory->UnmapMemory();

  std::vector<VkCommandBuffer> CopyCommandBuffers;

  if (NeedCopyUniform)
  {
    VkCommandBuffer VulkanCopyCommandBuffer;

    TransferCommandPool.AllocateCommandBuffers(&VulkanCopyCommandBuffer);

    command_buffer CommandBuffer(VulkanCopyCommandBuffer);

    CommandBuffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    VkBufferMemoryBarrier BufferBarrier = {};

    BufferBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    BufferBarrier.pNext = nullptr;
    BufferBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
    BufferBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    BufferBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    BufferBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    BufferBarrier.buffer = HostUniformBuffer.GetBufferId();
    BufferBarrier.offset = 0;
    BufferBarrier.size = VK_WHOLE_SIZE;

    vkCmdPipelineBarrier(VulkanCopyCommandBuffer, VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                         0, 0, nullptr,
                         1, &BufferBarrier,
                         0, nullptr);

    VkBufferCopy CopyRegion = {};

    CopyRegion.srcOffset = 0;
    CopyRegion.dstOffset = 0;
    CopyRegion.size = UniformBufferSize;
    
    vkCmdCopyBuffer(VulkanCopyCommandBuffer, HostUniformBuffer.GetBufferId(), DeviceUniformBuffer.GetBufferId(),
                    1, &CopyRegion);

    CommandBuffer.End();

    CopyCommandBuffers.push_back(VulkanCopyCommandBuffer);
  }

  CurMemory = &DeviceStorageMemory;

  if (NeedCopyStorage)
    CurMemory = &HostStorageMemory;

  CurMemory->MapMemory(0, VK_WHOLE_SIZE, reinterpret_cast<VOID **>(&Data));

  for (INT i = 0; i < W * H; i++)
    reinterpret_cast<UINT *>(Data + RandomNumbersOffset)[i] = rand();

  memset(Data + ImageOffset, 0, ImageSize);
    
  Tree.FillSceneData(Data + MaterialsOffset, DataAlignment);

  if ((VkApp.DeviceMemoryProperties.memoryTypes[CurMemory->MemoryType].propertyFlags &
       VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == 0)
  {
    CurMemory->FlushAllRange(0);
  }

  CurMemory->UnmapMemory();

  if (NeedCopyStorage)
  {
    VkCommandBuffer VulkanCopyCommandBuffer;

    TransferCommandPool.AllocateCommandBuffers(&VulkanCopyCommandBuffer);

    command_buffer CommandBuffer(VulkanCopyCommandBuffer);

    CommandBuffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    VkBufferMemoryBarrier BufferBarrier = {};

    BufferBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    BufferBarrier.pNext = nullptr;
    BufferBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
    BufferBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    BufferBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    BufferBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    BufferBarrier.buffer = HostStorageBuffer.GetBufferId();
    BufferBarrier.offset = 0;
    BufferBarrier.size = VK_WHOLE_SIZE;

    vkCmdPipelineBarrier(VulkanCopyCommandBuffer, VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                         0, 0, nullptr,
                         1, &BufferBarrier,
                         0, nullptr);

    VkBufferCopy CopyRegion = {};

    CopyRegion.srcOffset = 0;
    CopyRegion.dstOffset = 0;
    CopyRegion.size = StorageBufferSize;

    vkCmdCopyBuffer(VulkanCopyCommandBuffer, HostStorageBuffer.GetBufferId(), DeviceStorageBuffer.GetBufferId(),
                    1, &CopyRegion);

    CommandBuffer.End();

    CopyCommandBuffers.push_back(VulkanCopyCommandBuffer);
  }

  if (!CopyCommandBuffers.empty())
  {
    TransferRenderArgumentsSemaphore = semaphore(VkApp.GetDeviceId());

    VkSemaphore VulkanSemaphore = TransferRenderArgumentsSemaphore.GetSemaphoreId();
    VkSubmitInfo SubmitInfo = {};

    SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    SubmitInfo.pNext = nullptr;
    SubmitInfo.waitSemaphoreCount = 0;
    SubmitInfo.pWaitSemaphores = nullptr;
    SubmitInfo.pWaitDstStageMask = nullptr;
    SubmitInfo.commandBufferCount = CopyCommandBuffers.size();
    SubmitInfo.pCommandBuffers = CopyCommandBuffers.data();
    SubmitInfo.signalSemaphoreCount = 1;
    SubmitInfo.pSignalSemaphores = &VulkanSemaphore;

    TransferQueue.Submit(&SubmitInfo);
  }
}

/**
 * \brief Apply HDR correction to image function
 * \param[in, out] Im Image
 */
VOID vulkan_render::ProcessHDR( image * Im )
{
  HDR.SetBrighnesLimit(1.5);
  HDR.SetExposure(2);
  HDR.SetNumberOfBloomIterations(100);
  HDR.Process(Im);
}

/**
 * \brief Run shader function
 * \param[in] W Image width
 * \param[in] H Image height
 * \param[in] NumberOfSamples Number of samples
 */
VOID vulkan_render::RunRenderShader(  INT W, INT H, INT NumberOfSamples )
{
  std::uniform_int_distribution<UINT32> Distr;
  INT64 t = clock();
  VkCommandBuffer VulkanCommandBuffer;

  ComputeCommandPool.AllocateCommandBuffers(&VulkanCommandBuffer, 1);

  command_buffer FirstCommandBuffer(VulkanCommandBuffer);

  FirstCommandBuffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

  if (NeedCopyStorage || NeedCopyUniform)
  {
    std::vector<VkBufferMemoryBarrier> BarriersArray;

    if (NeedCopyStorage)
    {
      VkBufferMemoryBarrier BufferBarrier = {};

      BufferBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
      BufferBarrier.pNext = nullptr;
      BufferBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      BufferBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

      if (VkApp.TransferQueueFamilyIndex != VkApp.ComputeQueueFamilyIndex)
      {
        BufferBarrier.srcQueueFamilyIndex = *VkApp.TransferQueueFamilyIndex;
        BufferBarrier.dstQueueFamilyIndex = *VkApp.ComputeQueueFamilyIndex;
      } else
      {
        BufferBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        BufferBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      }

      BufferBarrier.buffer = DeviceStorageBuffer.GetBufferId();
      BufferBarrier.offset = 0;
      BufferBarrier.size = VK_WHOLE_SIZE;

      BarriersArray.push_back(BufferBarrier);
    }

    if (NeedCopyUniform)
    {
      VkBufferMemoryBarrier BufferBarrier = {};

      BufferBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
      BufferBarrier.pNext = nullptr;
      BufferBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      BufferBarrier.dstAccessMask = VK_ACCESS_UNIFORM_READ_BIT;

      if (VkApp.TransferQueueFamilyIndex != VkApp.ComputeQueueFamilyIndex)
      {
        BufferBarrier.srcQueueFamilyIndex = *VkApp.TransferQueueFamilyIndex;
        BufferBarrier.dstQueueFamilyIndex = *VkApp.ComputeQueueFamilyIndex;
      }
      else
      {
        BufferBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        BufferBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      }

      BufferBarrier.buffer = DeviceUniformBuffer.GetBufferId();
      BufferBarrier.offset = 0;
      BufferBarrier.size = VK_WHOLE_SIZE;

      BarriersArray.push_back(BufferBarrier);
    }

    vkCmdPipelineBarrier(VulkanCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                         0, 0, nullptr,
                         BarriersArray.size(), BarriersArray.data(),
                         0, nullptr);
  }
  if (!NeedCopyStorage || !NeedCopyUniform)
  {
    std::vector<VkBufferMemoryBarrier> BarriersArray;

    if (!NeedCopyStorage)
    {
      VkBufferMemoryBarrier BufferBarrier = {};

      BufferBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
      BufferBarrier.pNext = nullptr;
      BufferBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
      BufferBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
      BufferBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      BufferBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      BufferBarrier.buffer = DeviceStorageBuffer.GetBufferId();
      BufferBarrier.offset = 0;
      BufferBarrier.size = VK_WHOLE_SIZE;

      BarriersArray.push_back(BufferBarrier);
    }

    if (!NeedCopyUniform)
    {
      VkBufferMemoryBarrier BufferBarrier = {};

      BufferBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
      BufferBarrier.pNext = nullptr;
      BufferBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
      BufferBarrier.dstAccessMask = VK_ACCESS_UNIFORM_READ_BIT;
      BufferBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      BufferBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      BufferBarrier.buffer = DeviceUniformBuffer.GetBufferId();
      BufferBarrier.offset = 0;
      BufferBarrier.size = VK_WHOLE_SIZE;

      BarriersArray.push_back(BufferBarrier);
    }

    vkCmdPipelineBarrier(VulkanCommandBuffer, VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                         0, 0, nullptr,
                         BarriersArray.size(), BarriersArray.data(),
                         0, nullptr);
  }

  FirstCommandBuffer.CmdBindComputePipeline(RenderPipeline);

  VkDescriptorSet VulkanDescriptorSets[2] =
  {
    RenderSceneDescriptionSet,
    ImageSet
  };

  vkCmdBindDescriptorSets(VulkanCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
                          RenderPipelineLayout.GetPipelineLayoutId(), 0, 2, VulkanDescriptorSets, 0, nullptr);

  {
    UINT32 Seeds[] =
      {
        Distr(Gen),
        Distr(Gen)
      };

    vkCmdPushConstants(VulkanCommandBuffer, RenderPipelineLayout.GetPipelineLayoutId(), VK_SHADER_STAGE_COMPUTE_BIT, 0,
                       sizeof(UINT32) * 2, Seeds);
  }

  vkCmdDispatch(VulkanCommandBuffer, (W + 8 - 1) / 8, (H + 8 - 1) / 8, 1);

  std::cout << (clock() - t) / (DBL)CLOCKS_PER_SEC << " before cycle\n";

  VkBufferMemoryBarrier MiddleBufferBarrier = {};

  MiddleBufferBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
  MiddleBufferBarrier.pNext = nullptr;
  MiddleBufferBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
  MiddleBufferBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
  MiddleBufferBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  MiddleBufferBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  MiddleBufferBarrier.buffer = DeviceStorageBuffer.GetBufferId();
  MiddleBufferBarrier.offset = ImageOffset;
  MiddleBufferBarrier.size = ImageSize;

  for (INT i = 0; i < NumberOfSamples - 1; i++)
  {

    //vkCmdPipelineBarrier(VulkanCommandBuffer, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
    //                     0, 0, nullptr,
    //                     1, &MiddleBufferBarrier,
    //                     0, nullptr);

    vkCmdPipelineBarrier(VulkanCommandBuffer, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                         0, 0, nullptr,
                         1, &MiddleBufferBarrier,
                         0, nullptr);

    UINT32 Seeds[] =
      {
        Distr(Gen),
        Distr(Gen)
      };

    vkCmdPushConstants(VulkanCommandBuffer, RenderPipelineLayout.GetPipelineLayoutId(),
                       VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(UINT32) * 2, Seeds);

    vkCmdDispatch(VulkanCommandBuffer, (W + 8 - 1) / 8, (H + 8 - 1) / 8, 1);
  }

  std::cout << (clock() - t) / (DBL)CLOCKS_PER_SEC << " before end\n";

  FirstCommandBuffer.End();

  std::cout << (clock() - t) / (DBL)CLOCKS_PER_SEC << " after end\n";

  fence Fence(VkApp.GetDeviceId());

  std::cout << (clock() - t) / (DBL)CLOCKS_PER_SEC << " after fence creation\n";

  VkSemaphore VulkanSemaphore = TransferRenderArgumentsSemaphore.GetSemaphoreId();
  VkSubmitInfo SubmitInfo = {};
  VkPipelineStageFlags DstStageMask = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;

  SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  SubmitInfo.pNext = nullptr;

  if (NeedCopyUniform || NeedCopyStorage)
  {
    SubmitInfo.waitSemaphoreCount = 1;
    SubmitInfo.pWaitSemaphores = &VulkanSemaphore;
  }
  else
  {
    SubmitInfo.waitSemaphoreCount = 0;
    SubmitInfo.pWaitSemaphores = nullptr;
  }

  SubmitInfo.pWaitDstStageMask = &DstStageMask;
  SubmitInfo.commandBufferCount = 1;
  SubmitInfo.pCommandBuffers = &VulkanCommandBuffer;
  SubmitInfo.signalSemaphoreCount = 0;
  SubmitInfo.pSignalSemaphores = nullptr;

  std::cout << (clock() - t) / (DBL)CLOCKS_PER_SEC << " before submit\n";

  ComputeQueue.Submit(&SubmitInfo, 1, Fence.GetFenceId());

  std::cout << (clock() - t) / (DBL)CLOCKS_PER_SEC << " after submit\n";

  vulkan_validation::Check(
    Fence.Wait(),
    "Wait fence error");

  std::cout << (clock() - t) / (DBL)CLOCKS_PER_SEC << " after fence wait\n";
}

/**
 * \brief Copy image back  to CPU
 * \param Im Destination image
 */
VOID vulkan_render::CopyImageToCPU( image *Im ) const
{
  const memory *CurMemory = &DeviceStorageMemory;

  if (NeedCopyStorage)
  {
    CurMemory = &HostStorageMemory;

    VkCommandBuffer VulkanCopyCommandBuffer;

    TransferCommandPool.AllocateCommandBuffers(&VulkanCopyCommandBuffer);

    command_buffer CommandBuffer(VulkanCopyCommandBuffer);

    CommandBuffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    VkBufferMemoryBarrier BufferBarrier = {};

    BufferBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    BufferBarrier.pNext = nullptr;
    BufferBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
    BufferBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

    if (VkApp.TransferQueueFamilyIndex != VkApp.ComputeQueueFamilyIndex)
    {
      BufferBarrier.srcQueueFamilyIndex = *VkApp.ComputeQueueFamilyIndex;
      BufferBarrier.dstQueueFamilyIndex = *VkApp.TransferQueueFamilyIndex;
    }
    else
    {
      BufferBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      BufferBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    }
    
    BufferBarrier.buffer = DeviceStorageBuffer.GetBufferId();
    BufferBarrier.offset = ImageOffset;
    BufferBarrier.size = ImageSize;

    vkCmdPipelineBarrier(VulkanCopyCommandBuffer, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                         0, 0, nullptr,
                         1, &BufferBarrier,
                         0, nullptr);

    VkBufferCopy CopyRegion = {};

    CopyRegion.srcOffset = ImageOffset;
    CopyRegion.dstOffset = ImageOffset;
    CopyRegion.size = ImageSize;

    vkCmdCopyBuffer(VulkanCopyCommandBuffer, DeviceStorageBuffer.GetBufferId(), HostStorageBuffer.GetBufferId(),
                    1, &CopyRegion);

    if ((VkApp.DeviceMemoryProperties.memoryTypes[HostStorageMemory.MemoryType].propertyFlags &
         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == 0)
    {
      BufferBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
      BufferBarrier.pNext = nullptr;
      BufferBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      BufferBarrier.dstAccessMask = VK_ACCESS_HOST_READ_BIT;
      BufferBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      BufferBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      BufferBarrier.buffer = HostStorageBuffer.GetBufferId();
      BufferBarrier.offset = ImageOffset;
      BufferBarrier.size = ImageSize;

      vkCmdPipelineBarrier(VulkanCopyCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                           VK_PIPELINE_STAGE_HOST_BIT, 0, 0, nullptr, 1, &BufferBarrier, 0, nullptr);
    }

    CommandBuffer.End();

    fence Fence(VkApp.GetDeviceId());

    VkSubmitInfo SubmitInfo = {};

    SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    SubmitInfo.pNext = nullptr;
    SubmitInfo.waitSemaphoreCount = 0;
    SubmitInfo.pWaitSemaphores = nullptr;
    SubmitInfo.pWaitDstStageMask = nullptr;
    SubmitInfo.commandBufferCount = 1;
    SubmitInfo.pCommandBuffers = &VulkanCopyCommandBuffer;
    SubmitInfo.signalSemaphoreCount = 0;
    SubmitInfo.pSignalSemaphores = nullptr;

    //ComputeQueue.Submit(&SubmitInfo, 1, Fence.GetFenceId());

    TransferQueue.Submit(&SubmitInfo, 1, Fence.GetFenceId());

    vulkan_validation::Check(
      Fence.Wait(),
      "Wait fence error");
  }

  vec *Data;

  CurMemory->MapMemory(ImageOffset, ImageSize, reinterpret_cast<VOID **>(&Data));

  if ((VkApp.DeviceMemoryProperties.memoryTypes[CurMemory->MemoryType].propertyFlags &
       VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == 0)
  {
    CurMemory->InvalidateAllRange(ImageOffset);
  }

  std::cout << "Read memory\n";

  for (INT y = 0; y < Im->FrameH; y++)
    for (INT x = 0; x < Im->FrameW; x++)
    {
      const vec &Pixel = Data[Im->FrameW * y + x];
      Im->SetPixel(x, y, image_vec(Pixel.X, Pixel.Y, Pixel.Z));
    }

  CurMemory->UnmapMemory();
}

/**
 * \brief Render frame function
 * \param Im Image for render
 * \param Camera Camera for render
 * \param Scene Scene for render
 * \param[in] NumberOfSamples Number of samples
 */
VOID vulkan_render::RenderFrame( image *Im, const cam &Camera, scene &Scene, INT NumberOfSamples )
{
  const kd_tree &Tree = Scene.GetTree();

  Sizes = Tree.GetSizesOfPackedSceneElements(DataAlignment);

  CreateBuffersAndAllocateMemory(Im->FrameW, Im->FrameH, Camera, Scene);
  CopyParametersToGPUMemory(Im->FrameW, Im->FrameH, Camera, Scene, Tree);
  WriteDescriptorSets();

  RunRenderShader(Im->FrameW, Im->FrameH, NumberOfSamples);

  CopyImageToCPU(Im);
  *Im /= NumberOfSamples;

  HDR.SetSize(Im->FrameW, Im->FrameH);
  ProcessHDR(Im);
}

/**
 * \brief Render initialization function
 * \param[in] SelectedDeviceId Selected vulkan device
 * \param[in] OneSeed One seed for all sample flag for Vulkan implementation
 */
VOID vulkan_render::InitRender( UINT SelectedDeviceId, BOOL OneSeed )
{
  VkApp.InitApplication(SelectedDeviceId);
  if (OneSeed)
    RenderShader = shader_module(VkApp.GetDeviceId(), "shaders-build/trace_one_seed.comp.spv");
  else
    RenderShader = shader_module(VkApp.GetDeviceId(), "shaders-build/trace.comp.spv");

  if (!VkApp.ComputeQueueFamilyIndex)
    error("Compute queue family dont found");

  ComputeQueue = queue(VkApp.GetDeviceId(), *VkApp.ComputeQueueFamilyIndex, 0);
  ComputeCommandPool = command_pool(VkApp.GetDeviceId(), *VkApp.ComputeQueueFamilyIndex, 0);

  //if (VkApp.NeedCopyBuffers && !VkApp.TransferQueueFamilyIndex)
  //  error("Transfer queue family dont found");

  CreatePipelineLayout();
  CreateRenderPipeline();
  CreateDescriptorPoolAndAllocateSets();
}
