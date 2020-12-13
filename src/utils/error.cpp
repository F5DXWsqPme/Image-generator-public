#include "error.h"

/**
 * \brief Error constructor
 * \param[in] Str Error message
 */
error::error( const std::string &Str ) : Msg(Str)
{
  throw *this;
}

/**
 * \brief Get error message function
 * \return Error message string
 */
const std::string & error::GetMsg( VOID ) const
{
  return Msg;
}

/**
 * \brief Get error message function
 * \return Error message string
 */
const CHAR * error::what( VOID ) const noexcept
{
  return Msg.c_str();
}