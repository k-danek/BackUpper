#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <regex>
#include <algorithm>
#include <locale>

namespace fs = std::filesystem;
namespace ch = std::chrono;

// Class responsible to creating and reading log files
class Logger
{
  public:
    Logger();
    ~Logger();

    std::ofstream output_file;
    //std::ifstream inputFile("inputFileName");

    // Writes a file event into the logfile.
    void write_event(const std::string& in_dir_file,
                     const std::string& out_dir_file,
                     const std::string& event_type,
                     fs::file_time_type event_time 
                    );

    // Closes and opens the file for append.
    void reopen();

    // Allows to view the logs and filter them according to filenames.
    // TODO: Make this into a class!
    void browse_logs();
    char browse_logs_dialog();
    void browse_logs_filter(std::string regexp, std::string line);

    // Deletes previous logs while opening a new.
    void clear_open_logs();

    // Reads the in/out directories from existing logfile.
    bool read_in_out_dirs(std::string& in_directory,
                          std::string& out_directory
                         );

    // Simple Getter for whether the logfile exists.
    bool log_exists();

    // Generates a header for the log file.
    void add_log_header(std::string& in_directory,
                        std::string& out_directory);

  private:

    const std::string _log_filename = "BackUpperLog.txt";
    bool _log_exists = false;
    
    // Fixed strings in the logfile
    const std::string hot_dir  = "HOT DIRECTORY: ";
    const std::string back_dir = "BACKUP DIRECTORY: "; 

};

#endif
