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

    if(event_type == "backed up")
    {
      // Second output for the copy.
      output_file << std::asctime(std::localtime(&event_time_c)) << "|";
      output_file << event_type << " " << in_dir_file << " to " << out_dir_file << "\n";
    }
    else
    {
      // Altered, creted or deleted case.
      output_file << std::asctime(std::localtime(&event_time_c)) << "|";
      output_file << event_type << " " << in_dir_file << "\n";
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
              << back_dir << out_directory << "\n";

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

//  std::string line;
//
//  std::fstream logfile;
//  logfile.open(_log_filename, std::ios::in);
//
//  if(logfile.is_open())
//  {
//    while(getline(logfile, line))
//    {
//    }
//
//  }

  return;
};



