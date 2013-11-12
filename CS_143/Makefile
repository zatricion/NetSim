CC=g++
CFLAGS=-std=c++0x -Wall
LDFLAGS=-lm

OBJECTS = Test.o Packet.o Flow.o Event.o UnackEvent.o FlowEvent.o PacketEvent.o EventGenerator.o FlowGenerator.o Router.o Host.o Link.o Handler.o CongestionAlg.o
SOURCE = Test.cpp

all: simulator clean

simulator: runtest # TODO this will compile and create the actual program.
                   # For now, we will just generate the test file.

# Individual Object dependencies
Test.o: Test.cpp Packet.h Event.h
	$(CC) $(CFLAGS) -c $<
    
Packet.o: Packet.cpp
	$(CC) $(CFLAGS) -c $<
    
Flow.o: Flow.cpp Packet.h
	$(CC) $(CFLAGS) -c $<

Event.o: Event.cpp
	$(CC) $(CFLAGS) -c $<
    
UnackEvent.o: UnackEvent.cpp Packet.h Event.h
	$(CC) $(CFLAGS) -c $<

FlowEvent.o: FlowEvent.cpp Packet.h Event.h Flow.h
	$(CC) $(CFLAGS) -c $<
    
PacketEvent.o: PacketEvent.cpp Packet.h EventGenerator.h
	$(CC) $(CFLAGS) -c $<

EventGenerator.o: EventGenerator.cpp make_unique.h Event.h
	$(CC) $(CFLAGS) -c $<

FlowGenerator.o: FlowGenerator.cpp EventGenerator.h Flow.h
	$(CC) $(CFLAGS) -c $<

Router.o: Router.cpp Packet.h PacketEvent.h
	$(CC) $(CFLAGS) -c $<

Host.o: Host.cpp Link.h Packet.h FlowEvent.h Flow.h UnackEvent.h
	$(CC) $(CFLAGS) -c $<
    
Link.o: Link.cpp EventGenerator.h PacketEvent.h
	$(CC) $(CFLAGS) -c $<

Handler.o: Handler.cpp EventGenerator.h Event.h
	$(CC) $(CFLAGS) -c $<

CongestionAlg.o: CongestionAlg.cpp UnackEvent.h PacketEvent.h Flow.h
	$(CC) $(CFLAGS) -c $<

# An executable, prepared to test the objects.
runtest: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

# Tests the objects by running the runtest executable.
test: runtest clean
	./runtest

clean:
	rm *.o
