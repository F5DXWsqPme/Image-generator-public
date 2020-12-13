#include <string>

#include "vulkan_validation.h"
#include "utils/error.h"

using std::string_literals::operator""s;

/**
 * \brief Vulkan function result validation
 * \param[in] Result Vulkan function result
 * \param[in] Message Error message
 */
VOID vulkan_validation::Check( const VkResult Result, const std::string_view &Message )
{
#ifdef ENABLE_VULKAN_FUNCTION_RESULT_VALIDATION
  if (Result != VK_SUCCESS)
    error("Vulkan error: "s + Message.data());
#endif
}
