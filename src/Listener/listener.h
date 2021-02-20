#ifndef LISTENER_H
#define LISTENER_H

#include <iostream>
#include <filesystem>
#include <string>
#include <chrono>
#include <thread>
#include <unordered_set>
#include <logger.h>

namespace fs = std::filesystem;
namespace ch = std::chrono;

class File_Listener
{
  public:
    // Constructor with given in/out dirs
    File_Listener(std::string dir_in_name,
                  std::string dir_out_name): _dir_in_name(dir_in_name),
                                             _dir_out_name(dir_out_name){};

    // Default constructor
    File_Listener() = default;

    void listen(long int interval_miliseconds);

    fs::path get_out_path(const fs::path& file);

    static bool dir_exists(const fs::path& p);

    // Decide on whether to create a new directory.
    static bool na_dir_create(const fs::path& p);

    // Picks the names of the files and starts the listener.
    void dir_entry_dialog();

    // Decide what to do with existing logfile.
    void log_file_found_dialog();

    Logger logger;

  private:
    std::string _dir_in_name  = "";
    std::string _dir_out_name = "";

    // Makes sets of filenames from in/out directories.
    void _load_files(std::string dir, std::unordered_set<std::string>& set);

    std::unordered_set<std::string> _files_listened_to;
    std::unordered_set<std::string> _files_backed;
};

// TODO: Am I interested in copying the symlinks?

#endif

