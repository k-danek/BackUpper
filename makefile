CC=g++
CFLAGS= -Wl,--no-undefined -O2 -g -std=c++20
INC_LISTENER=./src/Listener
INC_LOGGER=./src/Logger
BUILD_TARGET=./bin
INCLUDES= -I. -I$(INC_LISTENER) -I$(INC_LOGGER)

all: backupper

# Executables
backupper: main.o listener.o logger.o
	$(CC) $(CFLAGS) -o $(BUILD_TARGET)/backupper $(BUILD_TARGET)/main.o $(BUILD_TARGET)/listener.o $(BUILD_TARGET)/logger.o
	
# Object files
main.o: listener.o
	$(CC) -c $(INCLUDES) $(CFLAGS) -o $(BUILD_TARGET)/main.o ./src/main.cc

listener.o: $(INC_LISTENER)/listener.h logger.o
	$(CC) -c $(INCLUDES) $(CFLAGS) -o $(BUILD_TARGET)/listener.o $(INC_LISTENER)/listener.cc 

logger.o: $(INC_LOGGER)/logger.h
	$(CC) -c $(INCLUDES) $(CFLAGS) -o $(BUILD_TARGET)/logger.o $(INC_LOGGER)/logger.cc 

out_dir:
	if ! [ -d bin/ ]; then mkdir bin/; fi

-include $(INCLUDES)

clean:
	rm $(BUILD_TARGET)/*.o $(BUILD_TARGET)/backupper

