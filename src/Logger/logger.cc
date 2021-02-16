#include<logger.h>


Logger::Logger()
{
  output_file.open(_log_filename, std::ios::app);
  output_file << "Logger initiated\n";
  output_file << std::string(80,'*');
  output_file.close();
};

//Logger::~Logger()
//{
//  output_file.close();
//};
//
void Logger::write_event(const std::string& in_dir_file,
                         const std::string& out_dir_file,
                         const std::string& event_type,
                         fs::file_time_type event_time 
                        )
{

  output_file.open(_log_filename, std::ios::app);

  if (output_file.is_open())
  {
    // do a (horrible) cast to more convenient type
    auto event_time_c = ch::system_clock::to_time_t(ch::file_clock::to_sys(event_time));

    output_file << "\n" << std::asctime(std::localtime(&event_time_c)) << " : ";
    output_file << event_type << " : " << in_dir_file << "\n";
    output_file << "Backed up File: " << out_dir_file << "\n";
    output_file << std::string(80,'*');
    output_file.close();
  }
  else
  {
    std::cout << "WARNING: Log file is not open for writing!";
  }
};


