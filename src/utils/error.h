#ifndef __error_h_
#define __error_h_

#include <string>

#include "def.h"

/**
 * \brief Error structure
 */
class error : public std::exception
{
public:
  /**
   * \brief Error constructor
   * \param[in] Str Error message
   */
  explicit error( const std::string &Str );

  /**
   * \brief Get error message function
   * \return Error message string
   */
  const std::string & GetMsg( VOID ) const;

  /**
   * \brief Get error message function
   * \return Error message string
   */
  const CHAR * what( VOID ) const noexcept override;

private:
  /** Error message */
  std::string Msg;
};

#endif /* __error_h_ */

