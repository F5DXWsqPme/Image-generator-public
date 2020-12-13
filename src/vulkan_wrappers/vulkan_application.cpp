#include <cassert>
#include <iostream>

#include "vulkan_application.h"
#include "vulkan_validation.h"
#include "utils/error.h"

/**
 * \brief Class constructor
 */
vulkan_application::vulkan_application( VOID )
{
}

/**
 * \brief Initialize vulkan application structure
 * \param[in] SelectedPhysicalDeviceId
 */
VOID vulkan_application::InitApplication( UINT SelectedPhysicalDeviceId )
{
  Init();

  SelectedPhysicalDevice = SelectedPhysicalDeviceId;

  vkGetPhysicalDeviceMemoryProperties(PhysicalDevices[*SelectedPhysicalDevice], &DeviceMemoryProperties);
  //SelectMemoryTypes();
  SelectQueueIndices();
  CreateLogicalDevice();
}

/**
 * \brief Class destructor
 */
vulkan_application::~vulkan_application( VOID )
{
  if (Device != VK_NULL_HANDLE)
    vkDestroyDevice(Device, nullptr);
  if (Instance != VK_NULL_HANDLE)
    vkDestroyInstance(Instance, nullptr);
}

VkDevice vulkan_application::GetDeviceId( VOID ) const
{
  return Device;
}

/**
 * \brief Vulkan application initialization function
 * \return VK_SUCCESS-if success
 */
VOID vulkan_application::Init( VOID )
{
  vulkan_validation::Check(
    volkInitialize(),
    "vulkan loading failed (volk initialization)");

  // Main application information
  VkApplicationInfo AppInfo = {};

  AppInfo.sType =  VK_STRUCTURE_TYPE_APPLICATION_INFO;
  AppInfo.pApplicationName = "Vulkan_application";
  AppInfo.applicationVersion = 1;
  AppInfo.apiVersion = VK_MAKE_VERSION(1, 0, 0);

  // Instance creating
  VkInstanceCreateInfo InstanceCreateInfo = {};

  InstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  InstanceCreateInfo.pApplicationInfo = &AppInfo;

#ifdef ENABLE_VULKAN_VALIDATION_LAYER
  const CHAR *ValidationLayerName = "VK_LAYER_KHRONOS_validation";

  InstanceCreateInfo.enabledLayerCount = 1;
  InstanceCreateInfo.ppEnabledLayerNames = &ValidationLayerName;
#endif

  vulkan_validation::Check(
    vkCreateInstance(&InstanceCreateInfo, nullptr, &Instance),
    "instance creation error");

  volkLoadInstanceOnly(Instance);

  // Fill physical devices
  UINT32 NumberOfPhysicalDevices = 0;

  vulkan_validation::Check(
    vkEnumeratePhysicalDevices(Instance, &NumberOfPhysicalDevices, nullptr),
    "physical devices enumeration failed");

  PhysicalDevices.resize(NumberOfPhysicalDevices);

  vulkan_validation::Check(
    vkEnumeratePhysicalDevices(Instance, &NumberOfPhysicalDevices, PhysicalDevices.data()),
    "physical devices enumeration failed");
}

/**
 * \brief Create logical device function
 */
VOID vulkan_application::CreateLogicalDevice( VOID )
{
  //VkPhysicalDeviceFeatures SupportedFeatures;
  //
  //vkGetPhysicalDeviceFeatures(PhysicalDevices[SelectedPhysicalDevice], &SupportedFeatures);

  VkPhysicalDeviceProperties Properties;
  
  vkGetPhysicalDeviceProperties(PhysicalDevices[*SelectedPhysicalDevice], &Properties);
  std::cout << "Selected device: " << Properties.deviceName << "\n";     \
  
  VkPhysicalDeviceFeatures RequiredFeatures = {};

  std::vector<VkDeviceQueueCreateInfo> DeviceQueueCreateInfosArray;

  FLT QueuePriorities[] = {1};

  if (ComputeQueueFamilyIndex == TransferQueueFamilyIndex && ComputeQueueFamilyIndex)
  {
    VkDeviceQueueCreateInfo DeviceQueueCreateInfo = {};

    DeviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    DeviceQueueCreateInfo.pNext = nullptr;
    DeviceQueueCreateInfo.flags = 0;
    DeviceQueueCreateInfo.queueFamilyIndex = *ComputeQueueFamilyIndex;
    DeviceQueueCreateInfo.queueCount = 1;
    DeviceQueueCreateInfo.pQueuePriorities = QueuePriorities;

    DeviceQueueCreateInfosArray.push_back(DeviceQueueCreateInfo);
  }
  else
  {
    VkDeviceQueueCreateInfo DeviceQueueCreateInfo = {};

    DeviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    DeviceQueueCreateInfo.pNext = nullptr;
    DeviceQueueCreateInfo.flags = 0;
    DeviceQueueCreateInfo.queueCount = 1;
    DeviceQueueCreateInfo.pQueuePriorities = QueuePriorities;

    if (ComputeQueueFamilyIndex)
    {
      DeviceQueueCreateInfo.queueFamilyIndex = *ComputeQueueFamilyIndex;
      DeviceQueueCreateInfosArray.push_back(DeviceQueueCreateInfo);
    }

    if (TransferQueueFamilyIndex)
    {
      DeviceQueueCreateInfo.queueFamilyIndex = *TransferQueueFamilyIndex;
      DeviceQueueCreateInfosArray.push_back(DeviceQueueCreateInfo);
    }
  }

  VkDeviceCreateInfo DeviceCreateInfo = {};

  DeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  DeviceCreateInfo.queueCreateInfoCount = DeviceQueueCreateInfosArray.size();
  DeviceCreateInfo.pQueueCreateInfos = DeviceQueueCreateInfosArray.data();
  DeviceCreateInfo.pEnabledFeatures = &RequiredFeatures;

#ifdef ENABLE_VULKAN_VALIDATION_LAYER
  //const CHAR *PrintfExtensionName = "VK_KHR_shader_non_semantic_info";
  //
  //DeviceCreateInfo.enabledExtensionCount = 1;
  //DeviceCreateInfo.ppEnabledExtensionNames = &PrintfExtensionName;
#endif

  vulkan_validation::Check(
    vkCreateDevice(PhysicalDevices[*SelectedPhysicalDevice], &DeviceCreateInfo, nullptr, &Device),
    "logical device creation failed");

  volkLoadDevice(Device);
}

/**
 * \brief Find memory type index with required memory property flags
 * \param[in] MemoryRequirements Memory requirements
 * \param[in] RequiredProperty Required memory property flags
 * \return Memory type index
 */
std::optional<UINT32> vulkan_application::FindMemoryTypeWithFlags( const VkMemoryRequirements MemoryRequirements,
                                                                   VkMemoryPropertyFlags RequiredProperty ) const
{
  for (UINT32 MemoryTypeId = 0; MemoryTypeId < DeviceMemoryProperties.memoryTypeCount; MemoryTypeId++)
  {
    if (MemoryRequirements.memoryTypeBits & (1 << MemoryTypeId))
    {
      const VkMemoryType &Type = DeviceMemoryProperties.memoryTypes[MemoryTypeId];
      const VkMemoryHeap &Heap = DeviceMemoryProperties.memoryHeaps[Type.heapIndex];

      if ((Type.propertyFlags & RequiredProperty) == RequiredProperty &&
          Heap.size >= MemoryRequirements.size)
      {
        return MemoryTypeId;
      }
    }
  }

  return std::nullopt;
}

/**
 * \brief Select memory types.
 */
/*VOID vulkan_application::SelectMemoryTypes( VOID )
{
  vkGetPhysicalDeviceMemoryProperties(PhysicalDevices[*SelectedPhysicalDevice], &DeviceMemoryProperties);

  std::optional<UINT32> PerfectMemory =
    FindMemoryTypeWithFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

  if (PerfectMemory)
  {
    DeviceLocalMemoryIndex = PerfectMemory;
    HostVisibleMemoryIndex = PerfectMemory;
    return;
  }

  HostVisibleMemoryIndex =
    FindMemoryTypeWithFlags(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

  if (!HostVisibleMemoryIndex)
    error("not found host visible memory type");

  DeviceLocalMemoryIndex =
    FindMemoryTypeWithFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  if (DeviceLocalMemoryIndex)
    NeedCopyBuffers = TRUE;
}*/

/**
 * \brief Find queue family index with required queue property flags
 * \param Required Required queue family property flags
 * \return Queue family index
 */
std::optional<UINT32> vulkan_application::FindQueueFamilyWithFlags( VkQueueFlags Required ) const
{
  for (UINT32 QueueFamily = 0; QueueFamily < QueueFamilyProperties.size(); QueueFamily++)
    if ((QueueFamilyProperties[QueueFamily].queueFlags & Required) == Required)
      return QueueFamily;

  return std::nullopt;
}

/**
 * \brief Select queue indices.
 */
VOID vulkan_application::SelectQueueIndices( VOID )
{
  UINT32 NumberOfQueueFamilies;
  
  vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevices[*SelectedPhysicalDevice],
                                           &NumberOfQueueFamilies, nullptr);

  QueueFamilyProperties.resize(NumberOfQueueFamilies);

  vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevices[*SelectedPhysicalDevice],
                                           &NumberOfQueueFamilies, QueueFamilyProperties.data());

  std::optional<UINT32> PerfectFamily =
    FindQueueFamilyWithFlags(VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT);

  if (PerfectFamily)
  {
    TransferQueueFamilyIndex = PerfectFamily;
    ComputeQueueFamilyIndex = PerfectFamily;
    return;
  }

  ComputeQueueFamilyIndex =
    FindQueueFamilyWithFlags(VK_QUEUE_COMPUTE_BIT);

  TransferQueueFamilyIndex =
    FindQueueFamilyWithFlags(VK_QUEUE_TRANSFER_BIT);
}
