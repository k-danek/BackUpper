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

    void listen(unsigned long int interval_miliseconds);

    fs::path get_out_path(const fs::path& file);

    static bool dir_exists(const fs::path& p);

    // Decide on whether to create a new directory.
    static bool na_dir_create(const fs::path& p);

    // Picks the names of the files and starts the listener.
    void dir_entry_dialog();

    // Decide what to do with existing logfile.
    void log_file_found_dialog();

    // Checks in/out dirs for copying of files.
    void init_dirs();

    // Simple wrapper for logging and backing up/deleting action.
    void apply_change(const fs::path&    in_path,
                      const fs::path&    out_path,
                      std::string        change_type,
                      fs::file_time_type time);

    Logger logger;

  private:
    std::string _dir_in_name  = "";
    std::string _dir_out_name = "";

    std::unordered_set<std::string> _files_listened_to;
    std::unordered_set<std::string> _files_listened_to_temp;
    std::unordered_set<std::string> _files_backed;
    
    // Option set-up for copying the files.
    static const auto _copy_options = fs::copy_options::overwrite_existing
                                    | fs::copy_options::recursive;

    // Makes sets of filenames from in/out directories.
    void _load_files(std::string dir, std::unordered_set<std::string>& set);

    // Prefix for deletion of a file.
    const std::string _del_prefix = "delete_";
};

#endif

