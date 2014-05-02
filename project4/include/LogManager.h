///
/// The log manager
/// 

#ifndef __LOG_MANAGER_H__
#define __LOG_MANAGER_H__

#include <stdio.h>

#include "Manager.h"
#include "utility.h"

#define LOGFILE_NAME "dragonfly.log"

class LogManager : public Manager {

 private:
  LogManager();                      ///< Private since a singleton.
  LogManager (LogManager const&);    ///< Don't allow copy.
  void operator=(LogManager const&); ///< Don't allow assignment.
  bool do_flush;                     ///< True if fflush after each write.
  FILE *fp;                          ///< Pointer to log file.
  int log_level;                     ///< Logging level.
  bool log_time_string;              ///< True if prepend time.
  bool log_step_count;               ///< True if prepend step count.

 public:
  ~LogManager();

  /// Get the one and only instance of the LogManager.
  static LogManager &getInstance();

  /// Start up LogManager (open logfile "dragonfly.log").
  /// If flush is true, then call fflush() afer each write.
  int startUp(bool flush=false);

  /// Shut down LogManager (close logfile).
  void shutDown();

  /// Write to logfile.
  /// Supports printf() formatting of strings.
  /// Return bytes written (excluding prepends), -1 if error
  int writeLog(const char *fmt, ...);

  /// Write to logfile.
  /// Only write if indicated log level >= LogManager log level
  /// Supports printf() formatting of strings.
  /// Return bytes written (excluding prepends), -1 if error
  int writeLog(int log_level, const char *fmt, ...);

  // Set logging level.
  void setLogLevel(int log_level);

  // Get logging level.
  int getLogLevel();
  
  /// Set prepend time string to log messages.
  void setLogTimeString(int log_time_string);
  
  /// Set prepend step count to log messages.
  void setLogStepCount(bool log_step_count);
};

#endif // __LOG_MANAGER_H__
