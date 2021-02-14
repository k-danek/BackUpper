#include <iostream>
#include <filesystem>
#include <string>
#include <chrono>
#include <thread>

namespace fs = std::filesystem;
namespace ch = std::chrono;

class File_Listener
{
  public:
    // Constructor
    File_Listener(std::string dir_in_name,
                  std::string dir_out_name): _dir_in_name(dir_in_name),
                                             _dir_out_name(dir_out_name){};

    void listen(long int interval_miliseconds);

    fs::path get_out_path(const fs::path& file);

    static bool dir_exists(const fs::path& p);

    static bool na_dir_create(const fs::path& p);

  private:
    std::string _dir_in_name = "";
    std::string _dir_out_name = "";
};

// TODO: Am I interested in copying the symlinks?
