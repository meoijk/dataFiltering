# *****************************************************
# Variables to control Makefile operation
 
CC = g++
CFLAGS = -Wall -g
 
# ****************************************************
# Targets needed to bring the executable up to date
 
TARGET = filteringEyeTracking 

main: main.o 
	$(CC) $(CFLAGS) -o filteringEyeTracking main.o 
 
# The main.o target can be written more simply
 
main.o: main.cpp 
	$(CC) $(CFLAGS) -c main.cpp

clean:
	$(RM) $(TARGET) *.o