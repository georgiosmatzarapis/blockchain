// author: georgiosmatzarapis

#include <chrono>
#include <fstream>
#include <thread>

#include "Logger.hpp"

namespace utils {

static constexpr std::string kLogsDirectory{"../../logs/"};

Log::Log() = default;

Log::~Log() = default;

Log& Log::GetInstance() {
  static Log sInstance{};
  return sInstance;
}

// Public API

void Log::toConsole(const LogLevel& iLogLevel, const std::string& iMessage,
                    const std::string& iFunctionName) const {
  std::ostream& aOutputType{
      (iLogLevel == LogLevel::ERROR || iLogLevel == LogLevel::FATAL)
          ? std::cerr
          : std::clog};
  aOutputType << constructStream(iLogLevel, iMessage, iFunctionName);
}

void Log::toFile(const LogLevel& iLogLevel, const std::string& iMessage,
                 const std::string& iFunctionName) const {
  const std::string aFileName{kLogsDirectory + GetCurrentDate() + ".log"};

  std::ofstream aOutputFile{};
  aOutputFile.exceptions(std::ofstream::failbit | std::ofstream::badbit);
  try {
    aOutputFile.open(aFileName,
                     IsFilePresent(aFileName) ? std::ios::app : std::ios::out);
    if (aOutputFile.is_open()) {
      aOutputFile << constructStream(iLogLevel, iMessage, iFunctionName);
    }
  } catch (const std::ofstream::failure& iOutputException) {
    toConsole(LogLevel::ERROR,
              "Exception while interacting with log file: " +
                  std::string{iOutputException.what()},
              __PRETTY_FUNCTION__);
  }
}

// Private API

std::string Log::GetCurrentDate() {
  const std::time_t sCurrentDatetime{
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())};
  std::tm* const sLocalTime{std::localtime(&sCurrentDatetime)};

  const int sYear{sLocalTime->tm_year + 1900};
  const int sMonth{sLocalTime->tm_mon + 1};
  const int sDay{sLocalTime->tm_mday};

  std::string sMonthStr{(sMonth < 10) ? "0" + std::to_string(sMonth)
                                      : std::to_string(sMonth)};
  std::string sDayStr{(sDay < 10) ? "0" + std::to_string(sDay)
                                  : std::to_string(sDay)};
  return std::to_string(sYear) + sMonthStr + sDayStr;
}

bool Log::IsFilePresent(const std::string& iFileName) {
  return std::ifstream{iFileName} ? true : false;
}

std::string Log::constructStream(const LogLevel& iLogLevel,
                                 const std::string& iMessage,
                                 const std::string& iFunctionName) const {
  std::string aLogLevel{};
  switch (iLogLevel) {
    case LogLevel::INFO:
      aLogLevel = "INFO";
      break;
    case LogLevel::DEBUG:
      aLogLevel = "DEBUG";
      break;
    case LogLevel::WARNING:
      aLogLevel = "WARNING";
      break;
    case LogLevel::ERROR:
      aLogLevel = "ERROR";
      break;
    case LogLevel::FATAL:
      aLogLevel = "FATAL";
      break;
    default:
      aLogLevel = "UNKNOWN";
      break;
  }

  const std::time_t aUtcTimestamp{
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())};

  std::ostringstream aFinalStream{};
  aFinalStream << std::put_time(gmtime(&aUtcTimestamp), "%FT%TZ") << " | "
               << aLogLevel << " | " << std::this_thread::get_id() << " | "
               << iFunctionName << ": | " << iMessage << std::endl;

  return aFinalStream.str();
}
} // namespace utils