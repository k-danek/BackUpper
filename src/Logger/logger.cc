#include<logger.h>


Logger::Logger()
{
  //output_file.open(_log_filename, std::ios::app | std::fstream::out);
  _log_exists = fs::exists(_log_filename);
};

Logger::~Logger()
{
  output_file.close();
};

void Logger::write_event(const std::string& in_dir_file,
                         const std::string& out_dir_file,
                         const std::string& event_type,
                         fs::file_time_type event_time 
                        )
{
  if (output_file.is_open())
  {
    // do a (horrible) cast to more convenient type
    auto event_time_c = ch::system_clock::to_time_t(
                                          ch::file_clock::to_sys(event_time));

    // Time conversion and formatting    
    char buffer[30];
    std::strftime(buffer, 30, "%d/%m/%Y %H:%M:%S", std::localtime(&event_time_c));
    std::string time_str = std::string(buffer); 

    if(event_type == "backed up")
    {
      // Second output for the copy.
      output_file << time_str << "|" << event_type << " \'" << in_dir_file 
                  << "\' to \'" << out_dir_file << "\'\n";
    }
    else
    {
      // Altered, creted or deleted case.
      output_file << time_str << "|" << event_type << " \'" << in_dir_file
                  << "\'\n";
    }  
   
    output_file.flush();    
  }
  else
  {
    std::cout << "WARNING: Log file is not open for writing!";
  }
};

void Logger::clear_open_logs()
{
  output_file.open(_log_filename, std::ofstream::out | std::ofstream::trunc);
};

void Logger::reopen()
{
  output_file.close();
  output_file.open(_log_filename, std::ios::app);
};

bool Logger::log_exists()
{
  return _log_exists;
};

void Logger::add_log_header(std::string& in_directory,
                            std::string& out_directory)
{
  output_file << "This is a log of the BackUpper CLI app.\n"
              << hot_dir  << in_directory  << "\n"
              << back_dir << out_directory << "\n"
              << std::string(80, '*') << "\n";

  output_file.flush();
  reopen();
};

bool Logger::read_in_out_dirs(std::string& in_directory,
                              std::string& out_directory)
{
  bool got_the_hot = false;
  bool got_the_back = false;

  std::string line;

  std::fstream logfile;
  logfile.open(_log_filename, std::ios::in);

  if(logfile.is_open())
  {
    while(!(got_the_hot && got_the_back) && getline(logfile, line))
    {
      std::size_t hot_pos  = line.find(hot_dir);
      std::size_t back_pos = line.find(back_dir);
      
      if (hot_pos != std::string::npos)
      {
         line = line.substr(hot_pos + hot_dir.size());
         if(!line.empty())
         {
           in_directory = line;
           got_the_hot = true;
         }
      } 

      if (back_pos != std::string::npos)
      {
         line = line.substr(back_pos + back_dir.size());
         if(!line.empty())
         {
           out_directory = line;
           got_the_back = true;
         }
      } 
    }
  }

  return (got_the_hot && got_the_back);
};

void Logger::browse_logs()
{
  std::string line;

  std::fstream logfile;

  if(logfile.is_open())
  {
    logfile.close();
  }

  logfile.open(_log_filename, std::ios::in);

  if(logfile.is_open())
  {

    char choice = browse_logs_dialog();

    // Create a regular expession for the filtering of the logfile.
    std::string regex_string;
    switch(choice)
    {
      case 'w':
      {  
        // Only the log entries have pipes in them.
        //regex = std::regex("\\\\|");
        regex_string = ".*\\|.*";
        break;
      }
      case 'f':
      {
        std::cout << "\nPlease enter a regular expession (without quotation marks): ";
        //std::string regex_str;
        std::cin >> regex_string;
        //regex = std::regex(regex_str);
        break;
      }
      case 'd':
      {
        std::string time_input;
        std::cout << "\nPlease enter a date in format" 
                  << "\"DD/MM/YYYY\", e.g., \"30/12/2021\": ";
        std::cin >> time_input;
        regex_string = time_input+".*";
        break;
      }
    }

    std::regex regex_line(regex_string);

    // Looping through the lines of the file. 
    while(getline(logfile, line))
    {
      if(std::regex_match(line, regex_line))
      {
        std::cout << line << "\n"; 
      }
    }
  }

  return;
};

char Logger::browse_logs_dialog()
{
    // Start of the dialog
    std::cout << "Log file is opened, what would you like to see?\n"
              << "[w] Whole log file.\n"
              << "[f] Lines satisfying a regual expression.\n"
              << "[d] Insert the time of the log.\n"
              << "Please enter your choice: [w/f/d]";

    char input_letter;
    std::cin >> input_letter;
    if(input_letter != 'w' &&
       input_letter != 'f' &&
       input_letter != 'd')
    {
      std::cout << "Invalid choice. Please try again. \n";
      return browse_logs_dialog();
    }
    else
    {
      return input_letter;
    }
}


