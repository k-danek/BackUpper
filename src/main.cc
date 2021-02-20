#include <iostream>
#include <listener.h>

int main()
{
  
  // Initialise file_listener.
  File_Listener file_listener;
  if (file_listener.logger.log_exists())
  {
    file_listener.log_file_found_dialog();
  }
  else
  {
    file_listener.dir_entry_dialog();
  }

  return 1;
}

