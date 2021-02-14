#include <iostream>
#include <filesystem>
#include <string>
#include <chrono>
#include <thread>
#include <listener.h>


int main()
{
  // Logger should read the state from the last log
  // If previous entry found try to set up the previous configuration

  std::string dir_in_name = "";
  std::string dir_out_name = "";

  std::cout << "Please enter name of directory to be backed\n";
  std::cin >> dir_in_name;
  if(!File_Listener::na_dir_create(dir_in_name)) return 0;

  std::cout << "Please enter name of directory for the files to be backed into\n"; 
  std::cin >> dir_out_name;
  if(!File_Listener::na_dir_create(dir_out_name)) return 0;

  // Run the backup and the logger
  // Activate the File listener
  File_Listener file_listener(dir_in_name, dir_out_name);
  file_listener.listen(1000);
}

