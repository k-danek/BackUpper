# BackUpper CLI program for backing up files

## Installation notes
Running 'make' in the root directory should produce an executable binary in /bin directory called 'backupper'.
The code includes libraries that were included in c++20 standard library that is provided by the gcc 10+ compiler.
Compilation and execution was tested in Fedora 32, gcc 10.2.1.

## How to run

### General summary
User can set up two directories, hot directory and backup directory, and trigger a listening process. 
The process will then check for each file in the hot directory and makes copies of the files present to the backup directory and will copy again every time the files are modified until the process is terminated.
The program will also produce a logfile 'backupper\_log.txt' which will contain logs with a timestamp and action taken on a file (created, altered, backed up and deleted).
Apart from that the logfile contains names of the hot and backup directory. 
If the logfile is present at the time of running BackUpper, the user will be asked by a dialog to specify whether he wants to clean the logfile, read the hot/backup directories from it or browse the logs.

Further notes:
- Backed-up files have additional '.bak' extension.
- The logfile is produced (and expected to be) in the current directory not in the /bin directory.

### Deleting files
Even though it is possible to just delete the files in the hot directory, the recommended approach is to rename them to add 'delete\_' prefix which makes sure that the backup copies get deleted too.

### Browsing the logfile
If the logfile is present, BackUpper can display its filtered content. The user can filter by a regex to find all the logs for specific file(s) or use date in format DD/MM/YYYY.

