CC=g++
CFLAGS=-std=c++0x -Wall
LDFLAGS=-lm

OBJECTS = Test.o Packet.o Event.o
SOURCE = Test.cpp

all: simulator clean

simulator: runtest # TODO this will compile and create the actual program.
                   # For now, we will just generate the test file.

# Individual Object dependencies
Test.o: Test.cpp Packet.h Event.h
	$(CC) $(CFLAGS) -c $<
Event.o: Event.cpp Packet.h # Only.h files??
	$(CC) $(CFLAGS) -c $<
Packet.o: Packet.cpp
	$(CC) $(CFLAGS) -c $<
# TODO add more when the classes are filled out.


# An executable, prepared to test the objects.
runtest: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)


# Tests the objects by running the runtest executable.
test: runtest clean
	./runtest

clean:
	rm *.o
