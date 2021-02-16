#include<listener.h>

void File_Listener::listen(long int milliseconds)
{

  const auto copy_options = fs::copy_options::overwrite_existing
                          | fs::copy_options::recursive;

  while(true)
  {
    // TODO: Load the state by logger

    //ch::time_point<ch::system_clock> start_of_loop = ch::system_clock::now();
    fs::file_time_type start_of_loop = fs::file_time_type::clock::now();
    
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
  	for(auto& entry: fs::recursive_directory_iterator(_dir_in_name))
    {
      // So far allowing for copying of regular files only
      if(fs::is_regular_file(entry))
      {
        fs::file_time_type mod_time = last_write_time(entry.path());

        // Type conversion, works for c++20
        //ch::time_point<ch::system_clock> mod_time = ch::system_clock::to_time_t(ch::file_clock::to_sys(mod_time_fs));
        
        fs::path backup_path = get_out_path(entry.path()); 

        if (start_of_loop < mod_time)
        {
          // Copy the file into bak directory 
          fs::copy(entry.path(), backup_path, copy_options);
          std::cout << "Backed file " << entry.path().filename() << " to " << _dir_out_name << "\n";
          logger.write_event(entry.path(), backup_path, "Modified", mod_time);
        }
      }
    }
  }
}

fs::path File_Listener::get_out_path(const fs::path& file)
{
  fs::path out_path(_dir_out_name
                    + "/" 
                    + std::string(file.filename())
                    + ".bak");
  return out_path;
}

bool File_Listener::dir_exists(const fs::path& p)
{
  // TODO: make sure that this works with a directory
  //       that has the same name as a file
  return (fs::exists(p) && fs::is_directory(p));
}

bool File_Listener::na_dir_create(const fs::path& p)
{
  if(dir_exists(p))
  {
    return true;
  }
  else
  {
    std::string yes_no_option; 
    
    std::cout << "Directory specified does not exist, would you like to create it? [Y/n]\n";
    std::cin >> yes_no_option;

    if(yes_no_option == ""    ||
       yes_no_option == "Y"   ||
       yes_no_option == "y"   ||
       yes_no_option == "yes"
      )
    {
      fs::create_directory(p); 
      return true;
    }
    else if(yes_no_option == "n" || yes_no_option == "N")
    {
      std::cout << "Directory not available. Exiting.\n";
      return false;  
    }
    else
    {
      std::cout << "Invalid option, please try again. Exiting.\n";
      return false;
    }
  
  }

  return false;
}

