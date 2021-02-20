#include<listener.h>

void File_Listener::listen(long int milliseconds)
{

  std::cout << "Listening... \n";

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

        fs::path backup_path = get_out_path(entry.path()); 

        if (start_of_loop < mod_time)
        {
          // Copy the file into bak directory 
          fs::copy(entry.path(), backup_path, copy_options);
          logger.write_event(entry.path(), backup_path, "Modified", mod_time);
        }
      }
    }
    // open and close the log file
    logger.reopen();
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

void File_Listener::log_file_found_dialog()
{

  std::string log_choice = "";

  std::cout << "Log file found. How would you like to proceed?\n"
            << "[c] Continue with previous setup.\n"
            << "[d] Delete previous logs and start with new setup.\n"
            << "[b] Browse throught the logs.\n\n"
            << "Please select: [C/d/b]";
  
  std::cin  >> log_choice;

  if(log_choice == ""  ||
     log_choice == "C" ||
     log_choice == "c" )
  {
      if(logger.read_in_out_dirs(_dir_in_name, _dir_out_name)) 
      { 
        // Start Listening.
        listen(1000);
      }
      else // Logger failed to read out the directories from the logfile.
      {
        std::cout << "In/out directories not found in the logfile.\n"
                  << "Do you want to specify new ones? [Y/n].\n";     
      
        std::string yn = "";
        std::cin >> yn;
        
        if(yn == ""  ||
           yn == "y" ||
           yn == "Y")
        {
          dir_entry_dialog();
        }
        else
        {
          std::cout << "Aborting the setup\n";
          exit(EXIT_SUCCESS);
        }
      }
  }
  else if(log_choice == "d" || log_choice == "D")
  {
    // Discards old logs, opens a new file.
    logger.clear_open_logs();
    // Starts a dialog for new in/out directories.
    dir_entry_dialog();
  }
  else if(log_choice == "b" || log_choice == "B")
  {
    logger.browse_logs();
  }
  else
  {
    std::cout << "Invalid option selected. Please, try again.\n";
    log_file_found_dialog();
  }
}

void File_Listener::dir_entry_dialog()
{
  std::string dir_in_name  = "";
  std::string dir_out_name = "";

  std::cout << "Please enter name of directory to be backed:\n";
  std::cin >> dir_in_name;
  while(!File_Listener::na_dir_create(dir_in_name));

  std::cout << "Please enter name of directory for the files to be backed into:\n"; 
  std::cin >> dir_out_name;
  while(!File_Listener::na_dir_create(dir_out_name));

  _dir_in_name  = dir_in_name;
  _dir_out_name = dir_out_name;

  logger.add_log_header(dir_in_name, dir_out_name);

  // Activate the File listener
  listen(1000);
}

void File_Listener::_load_files(std::string dir, std::unordered_set<std::string>& set)
{
  if(!dir.empty())
  {
    for(auto& entry: fs::recursive_directory_iterator(dir))
    {
      // Stores only regular files.
      if(fs::is_regular_file(entry))
      {
        // Whole parh is inserted instead of just filename.
        set.insert(std::string(entry.path()));
      }
    }

  }
}

