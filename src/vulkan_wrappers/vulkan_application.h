#ifndef __vulkan_application_h_
#define __vulkan_application_h_

#include "ext/volk/volk.h"
#include <vector>
#include <optional>

#include "def.h"

/**
 * \brief Vulkan application class
 */
class vulkan_application
{
public:
  /**
   * \brief Class constructor
   */
  vulkan_application( VOID );

  /**
   * \brief Initialize vulkan application structure
   * \param[in] SelectedPhysicalDeviceId
   */
  VOID InitApplication( UINT SelectedPhysicalDeviceId );

  /**
   * \brief Class destructor
   */
  ~vulkan_application( VOID );

  /**
   * \brief Get device identifier function
   * \return Device identifier
   */
  VkDevice GetDeviceId( VOID ) const;

  /**
   * \brief Find memory type index with required memory property flags
   * \param[in] MemoryRequirements Memory requirements
   * \param[in] RequiredProperty Required memory property flags
   * \return Memory type index
   */
  std::optional<UINT32> FindMemoryTypeWithFlags( const VkMemoryRequirements MemoryRequirements,
                                                 VkMemoryPropertyFlags RequiredProperty ) const;

  /** Selected physical device number */
  std::optional<UINT32> SelectedPhysicalDevice;

  /** Device local memory type */
  //std::optional<UINT32> DeviceLocalMemoryIndex;

  /** Host local memory type */
  //std::optional<UINT32> HostVisibleMemoryIndex;

  /** Need copy buffers flags (need to call vkCopyBuffer for transfer data to device) */
  //BOOL NeedCopyBuffers = FALSE;

  /** Device memory properties */
  VkPhysicalDeviceMemoryProperties DeviceMemoryProperties;

  /** Compute queue family index */
  std::optional<UINT32> ComputeQueueFamilyIndex;

  /** Transfer queue family index */
  std::optional<UINT32> TransferQueueFamilyIndex;

  /** Queue family properties structures */
  std::vector<VkQueueFamilyProperties> QueueFamilyProperties;

private:
  /**
   * \brief Find queue family index with required queue property flags
   * \param Required Required queue family property flags
   * \return Queue family index
   */
  std::optional<UINT32> FindQueueFamilyWithFlags( VkQueueFlags Required ) const;

  /**
   * \brief Select queue indices.
   */
  VOID SelectQueueIndices( VOID );

  /**
   * \brief Select memory types.
   */
  //VOID SelectMemoryTypes( VOID );

  /**
   * \brief Vulkan application initialization function
   */
  VOID Init( VOID );

  /**
   * \brief Create logical device function
   */
  VOID CreateLogicalDevice( VOID );

  /**
   * \brief Removed copy function
   * \param[in] VkApp Vulkan application
   * \return Reference to this
   */
  vulkan_application & operator=( const vulkan_application &VkApp ) = delete;

  /**
   * \brief Removed copy constructor
   * \param[in] VkApp Vulkan application
   */
  vulkan_application( const vulkan_application &VkApp ) = delete;

  /** Vulkan instance */
  VkInstance Instance = VK_NULL_HANDLE;

  /** Physical devices */
  std::vector<VkPhysicalDevice> PhysicalDevices; 

  /** Logical device */
  VkDevice Device = VK_NULL_HANDLE;
};

#endif // __vulkan_application_h_
