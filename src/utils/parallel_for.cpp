#include <future>
#include <thread>
#include <vector>

#include "parallel_for.h"

/** Number of threads */
INT parallel_for::NumberOfThreads = std::thread::hardware_concurrency();

/**
 * \brief Run parallel for
 * \param[in] N Number of iterations
 * \param[in] Func Function for run
 */
VOID parallel_for::Run( INT N, const std::function<VOID (INT)> &Func )
{
  std::vector<std::future<VOID>> Tasks(NumberOfThreads);

  for (INT i = 0; i < NumberOfThreads; i++)
    Tasks[i] = std::async([=]( VOID )
      {
        for (INT j = i; j < N; j += NumberOfThreads)
          Func(j);
      });

  for (INT i = 0; i < NumberOfThreads; i++)
    Tasks[i].wait();
}