#include <cassert>

#include "vulkan_validation.h"
#include "fence.h"

/**
 * \brief Fence constructor
 * \param[in] Device Device identifier
 * \parram[in] IsSignaled Fence signaled flag.
 */
fence::fence( VkDevice Device, BOOL IsSignaled ) :
  DeviceId(Device)
{
  VkFenceCreateInfo CreateInfo = {};

  CreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  CreateInfo.pNext = nullptr;
  if (IsSignaled)
    CreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  vulkan_validation::Check(
    vkCreateFence(Device, &CreateInfo, nullptr, &FenceId),
    "fence creation failed");
}

/**
 * \brief Fence destructor
 */
fence::~fence( VOID )
{
  if (FenceId != VK_NULL_HANDLE)
    vkDestroyFence(DeviceId, FenceId, nullptr);
}

/**
 * \brief Get fence identifier function
 * \return Fence identifier
 */
VkFence fence::GetFenceId( VOID ) const
{
  return FenceId;
}

/**
 * \brief Move function
 * \param[in] Fence Vulkan fence
 * \return Reference to this
 */
fence & fence::operator=( fence &&Fence )
{
  //assert(FenceId == VK_NULL_HANDLE);

  std::swap(FenceId, Fence.FenceId);
  std::swap(DeviceId, Fence.DeviceId);

  return *this;
}

/**
 * \brief Move constructor
 * \param[in] Fence Vulkan fence
 * \return Reference to this
 */
fence::fence( fence &&Fence )
{
  //assert(FenceId == VK_NULL_HANDLE);

  std::swap(FenceId, Fence.FenceId);
  std::swap(DeviceId, Fence.DeviceId);
}

/**
 * \brief Wait for fence signaled
 * \param[in] Timeout Timeout for exit from waiting.
 * return Result of waiting
 */
VkResult fence::Wait( UINT64 Timeout )
{
  return vkWaitForFences(DeviceId, 1, &FenceId, VK_TRUE, Timeout);
}
