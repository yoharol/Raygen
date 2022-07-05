/**
 * @file timer.h
 *
 * example:
 *  Timer timer;
 *  timer.setTimer();
 *  timer.getTimer();
 *  timer.endTimer();
 */

#ifndef RAYGEN_TIMER_H_
#define RAYGEN_TIMER_H_

#include <chrono>
#include <iostream>
#include <sstream>
#include <memory>
#include <iomanip>
#include "Math/math.h"

namespace Raygen {

class Timer {
 public:
  void setTimer() {
    if (m_base_timer != nullptr) {
      std::cerr << "Warning! Overwriting existing timer\n";
      m_base_timer = nullptr;
    }
    m_base_timer = std::make_unique<BaseTimer>(
        BaseTimer{std::chrono::system_clock::now()});
  }

  void getTimer(std::string message = "") {
    if (m_base_timer == nullptr) {
      std::cerr << "Error! No timer initialized\n";
    } else {
      const auto time_in_microsec = get_elapsed_time();
      const auto time_in_millisec = time_in_microsec / 1000.0;
      const auto time_in_second = time_in_millisec / 1000.0;
      std::ostringstream sstream;
      sstream << "[ " << std::setw(max<int>(30, message.length())) << message
              << " :\t";
      if (time_in_second > 1.0)
        sstream << std::setw(8) << time_in_second << "s \t]" << std::endl;
      else
        sstream << std::setw(8) << time_in_millisec << "ms\t]" << std::endl;
      std::cout << sstream.str();
    }
  }

  void endTimer(std::string message = "") {
    getTimer(message);
    m_base_timer = nullptr;
  }

  int64_t get_elapsed_time() const {
    return std::chrono::duration_cast<std::chrono::microseconds>(
               std::chrono::system_clock::now() - m_base_timer->m_start_time)
        .count();
  }

 private:
  typedef struct {
    std::chrono::system_clock::time_point m_start_time;
  } BaseTimer;

  std::unique_ptr<BaseTimer> m_base_timer = nullptr;
};
}  // namespace Raygen

#endif  // RAYGEN_TIMER_H_
