#include<listener.h>

void File_Listener::listen(unsigned long int milliseconds)
{
 
  init_dirs();

  std::cout << "Listening... \n";

  while(true)
  {
    //ch::time_point<ch::system_clock> start_of_loop = ch::system_clock::now();
    fs::file_time_type start_of_loop = fs::file_time_type::clock::now();
    
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));

    for(auto& entry: fs::recursive_directory_iterator(_dir_in_name))
    {
      // So far allowing for copying of regular files only
      if(fs::is_regular_file(entry))
      {
        fs::path backup_path = get_out_path(entry.path()); 
        fs::file_time_type mod_time = last_write_time(entry.path());
        size_t delete_pos = std::string(entry.path()).find(_del_prefix);

        if(delete_pos!=std::string::npos)
        {
          
          
          apply_change(entry.path(),
                       backup_path,
                       "deleted",
                       fs::file_time_type::clock::now());
          
          // Get the path of the file without detele_ prefix.
          std::string unprefixed_filename = std::string(
                entry.path()).erase(delete_pos, delete_pos+_del_prefix.length());

          // Delete the unprefixed file in the backup directory if it is not
          // present in the hot directory.
          // This makes it easier to delete files in both places as you can 
          // you can do that just by renaming the file in the hot directory.
          if(!fs::exists(unprefixed_filename))
          {
            // Trying to delete the original file should not have an effect.
            // TODO: Try to avoid the abundant delete attempt. 
            apply_change(entry.path(),
                         get_out_path(fs::path(unprefixed_filename)),
                         "deleted",
                         fs::file_time_type::clock::now()
                        );

          }
      

        }
        else if(_files_listened_to.contains(std::string(entry.path())))
        {
          if (start_of_loop < mod_time)
          {
            apply_change(entry.path(), backup_path, "altered", mod_time);
          }
        }
        else
        {
          apply_change(entry.path(), backup_path, "created", mod_time);
        }
      }
    }
    // open and close the log file
    logger.reopen();
  }
}

void File_Listener::init_dirs()
{
  _load_files(_dir_out_name, _files_backed);

  for(auto& entry: fs::recursive_directory_iterator(_dir_in_name))
  {
    // So far allowing for copying of regular files only
    if(fs::is_regular_file(entry))
    {
      _files_listened_to.insert(std::string(entry.path()));

      fs::path backup_path = get_out_path(entry.path()); 

      fs::file_time_type mod_time_in  = last_write_time(entry.path());

      if(_files_backed.contains(std::string(backup_path)))
      {
        fs::file_time_type mod_time_out = last_write_time(backup_path);
        if(mod_time_in > mod_time_out)
        {
          apply_change(entry.path(), backup_path, "altered", mod_time_in);
        }
      } 
      else
      {
        apply_change(entry.path(), backup_path, "created", mod_time_in);        
      }
    }
  }
}

void File_Listener::apply_change(const fs::path&    in_path,
                                 const fs::path&    out_path,
                                 std::string        change_type,
                                 fs::file_time_type mod_time)
{
  if(change_type!="deleted")
  {
    
    logger.write_event(in_path, out_path, change_type, mod_time);
    
    // Non-delete events trigger copying.
    fs::copy(in_path, out_path, _copy_options);
    logger.write_event(in_path, out_path, "backed up", fs::file_time_type::clock::now());
  }
  else // More complex delete logic.
  {
    // Remove the delete file.  
    if(fs::remove(in_path))
    {
      logger.write_event(in_path, out_path, change_type, fs::file_time_type::clock::now());
    }  
    if(fs::remove(out_path))
    {
      logger.write_event(in_path, out_path, change_type, fs::file_time_type::clock::now());
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

