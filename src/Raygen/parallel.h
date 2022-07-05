/**
 * @file parallel.h
 * @brief Parallel for loop
 * Mostly accepted from https://github.com/yuki-koyama/parallel-util
 */

#ifndef RAYGEN_PARALLEL_H_
#define RAYGEN_PARALLEL_H_

#include <functional>
#include <thread>
#include <mutex>
#include <vector>
#ifdef RAYGEN_PARALLEL_LOG_INFO_
#include <iostream>
#endif

namespace Raygen {

template <typename Callable>
void parallel_for(int n, Callable func, int target_concurrency = 0) {
  // hint of max threads
  if (n == 0) return;
  const int hint = (target_concurrency) == 0
                       ? std::thread::hardware_concurrency()
                       : target_concurrency;
  // how many threads in total
  const int n_threads = std::min(n, (hint == 0) ? 4 : hint);
  // how many tasks in each thread
  const int n_max_tasks_per_thread =
      (n / n_threads) + (n % n_threads == 0 ? 0 : 1);
  // The n^th thread will lack how many tasks
  const int n_lacking_tasks = n_max_tasks_per_thread * n_threads - n;

#ifdef RAYGEN_PARALLEL_LOG_INFO_
  std::mutex io_mutex;
  std::cout << "Target maximun threads: " << hint << std::endl;
  std::cout << "Total threads: " << n_threads << std::endl;
  std::cout << "max tasks per thread: " << n_max_tasks_per_thread << std::endl;
#endif  // RAYGEN_PARALLEL_LOG_INFO_

  auto inner_loop = [&](const int thread_index) {
    //
    const int n_lacking_tasks_so_far =
        std::max(thread_index - n_threads + n_lacking_tasks, 0);
    const int inclusive_start_index =
        thread_index * n_max_tasks_per_thread - n_lacking_tasks_so_far;
    const int exclusive_end_index =
        inclusive_start_index + n_max_tasks_per_thread -
        (thread_index - n_threads + n_lacking_tasks >= 0 ? 1 : 0);
    for (int k = inclusive_start_index; k < exclusive_end_index; k++) {
#ifdef RAYGEN_PARALLEL_LOG_INFO_
      io_mutex.lock();
      std::cout << "parallel-util ... Threads " << thread_index << ": "
                << k - inclusive_start_index + 1 << "/"
                << exclusive_end_index - inclusive_start_index
                << " position: " << k << std::endl;
      io_mutex.unlock();
#endif  // RAYGEN_PARALLEL_LOG_INFO_
      func(k);
    }

#ifdef RAYGEN_PARALLEL_LOG_INFO_
    io_mutex.lock();
    std::cout << "parallel-util ... Thread " << thread_index << ": done"
              << std::endl;
    io_mutex.unlock();
#endif  // RAYGEN_PARALLEL_LOG_INFO_
  };
  std::vector<std::thread> threads;
  for (int j = 0; j < n_threads; j++)
    threads.emplace_back(std::thread(inner_loop, j));
  for (auto& t : threads) t.join();
}

template <typename Callable>
void parallel_for_2d(int width, int height, Callable func,
                     int target_concurrency = 0) {
  const int hint = (target_concurrency == 0)
                       ? std::thread::hardware_concurrency()
                       : target_concurrency;
  const int n_threads = std::min(width * height, (hint == 0) ? 4 : hint);

#ifdef RAYGEN_PARALLEL_LOG_INFO_
  std::mutex io_mutex;
  std::cout << "Target maximun threads: " << hint << std::endl;
  std::cout << "Total threads: " << n_threads << std::endl;
#endif  // RAYGEN_PARALLEL_LOG_INFO_

  auto inner_loop = [&](const int thread_index) {
    const int n = width * height;
    const int start_index = thread_index * (n / n_threads);
    const int end_index = (thread_index + 1 == n_threads)
                              ? n
                              : (thread_index + 1) * (n / n_threads);
    for (int k = start_index; k < end_index; k++) {
#ifdef RAYGEN_PARALLEL_LOG_INFO_
      io_mutex.lock();
      std::cout << "parallel-util ... Threads " << thread_index << ": "
                << k - start_index + 1 << "/" << end_index - start_index
                << " position: (" << k % width << ", " << k / width << ")"
                << std::endl;
      io_mutex.unlock();
#endif  // RAYGEN_PARALLEL_LOG_INFO_
      func(k % width, k / width);
    }

#ifdef RAYGEN_PARALLEL_LOG_INFO_
    io_mutex.lock();
    std::cout << "parallel-util ... Thread " << thread_index << ": done"
              << std::endl;
    io_mutex.unlock();
#endif  // RAYGEN_PARALLEL_LOG_INFO_
  };
  std::vector<std::thread> threads;
  for (int j = 0; j < n_threads; ++j)
    threads.push_back(std::thread(inner_loop, j));
  for (auto& t : threads) t.join();
}

}  // namespace Raygen

#endif  // RAYGEN_PARALLEL_H_
