// author: georgiosmatzarapis

#pragma once

#include <iostream>

namespace utils {

enum class LogLevel { INFO, DEBUG, WARNING, ERROR, FATAL };

class Log {
 public:
  Log(const Log&) = delete;
  Log& operator=(const Log&) = delete;
  Log(Log&&) noexcept = delete;
  Log& operator=(Log&&) noexcept = delete;

  static Log& GetInstance();

  void toConsole(const LogLevel& iLogLevel, const std::string& iMessage,
                 const std::string& iFunctionName) const;
  void toFile(const LogLevel& iLogLevel, const std::string& iMessage,
              const std::string& iFunctionName) const;

 private:
  Log();
  ~Log();

  /**
   * @brief Get current date.
   * @return Date in %Y%m%d format.
   */
  static std::string GetCurrentDate();
  static inline bool IsFilePresent(const std::string& iFileName);
  std::string constructStream(const LogLevel& iLogLevel,
                              const std::string& iMessage,
                              const std::string& iFunctionName) const;
};

} // namespace utils