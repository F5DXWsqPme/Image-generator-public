#ifndef __parallel_for_h_
#define __parallel_for_h_

#include <functional>

#include "def.h"

/**
 * \brief Parallel for class
 */
class parallel_for
{
private:
  /** Number of threads */
  static INT NumberOfThreads;

public:
  parallel_for( VOID ) = delete;

  /**
   * \brief Run parallel for
   * \param[in] N Number of iterations
   * \param[in] Func Function for run
   */
  static VOID Run( INT N, const std::function<VOID (INT)> &Func );
};

#endif /* __parallel_for_h_ */
