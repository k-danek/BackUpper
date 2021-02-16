#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

#include <chrono>
#include <iomanip>

namespace fs = std::filesystem;
namespace ch = std::chrono;

// Class responsible to creating and reading log files
class Logger
{
  public:
    Logger();
    ~Logger() = default;

    std::ofstream output_file;
    //std::ifstream inputFile("inputFileName");

    void write_event(const std::string& in_dir_file,
                     const std::string& out_dir_file,
                     const std::string& event_type,
                     fs::file_time_type event_time 
                    );

  private:

    const std::string _log_filename = "BackUpperLog.txt";
};

#endif
