CC=g++
CFLAGS=-std=c++0x -g -Wall
LDFLAGS=-lm

OBJECTS = Test.o Packet.o Flow.o Event.o UnackEvent.o FlowEvent.o PacketEvent.o EventGenerator.o FlowGenerator.o Router.o Host.o Link.o Handler.o CongestionAlg.o deref_comp.o
SOURCE = Test.cpp

all: simulator

simulator: runtest # TODO this will compile and create the actual program.
                   # For now, we will just generate the test file.

# Individual Object dependencies
Test.o: Test.cpp Test.h Packet.h Event.h Handler.h
	$(CC) $(CFLAGS) -c $<
    
Packet.o: Packet.cpp Packet.h
	$(CC) $(CFLAGS) -c $<
    
Flow.o: EventGenerators/Flow.cpp EventGenerators/Flow.h Packet.h EventGenerators/CongestionAlg.h EventGenerators/Host.h
	$(CC) $(CFLAGS) -c $<

Event.o: Event.cpp Event.h
	$(CC) $(CFLAGS) -c $<
    
UnackEvent.o: UnackEvent.cpp UnackEvent.h Packet.h Event.h
	$(CC) $(CFLAGS) -c $<

FlowEvent.o: FlowEvent.cpp FlowEvent.h Packet.h Event.h EventGenerators/Flow.h
	$(CC) $(CFLAGS) -c $<
    
PacketEvent.o: PacketEvent.cpp PacketEvent.h Packet.h EventGenerators/EventGenerator.h
	$(CC) $(CFLAGS) -c $<

EventGenerator.o: EventGenerators/EventGenerator.cpp EventGenerators/EventGenerator.h Event.h deref_comp.h
	$(CC) $(CFLAGS) -c $<

FlowGenerator.o: EventGenerators/FlowGenerator.cpp EventGenerators/FlowGenerator.h EventGenerators/EventGenerator.h EventGenerators/Flow.h
	$(CC) $(CFLAGS) -c $<

Router.o: EventGenerators/Router.cpp EventGenerators/Router.h Packet.h PacketEvent.h
	$(CC) $(CFLAGS) -c $<

Host.o: EventGenerators/Host.cpp EventGenerators/Host.h EventGenerators/Link.h Packet.h FlowEvent.h EventGenerators/Flow.h UnackEvent.h
	$(CC) $(CFLAGS) -c $<
    
Link.o: EventGenerators/Link.cpp EventGenerators/Link.h EventGenerators/EventGenerator.h PacketEvent.h
	$(CC) $(CFLAGS) -c $<

Handler.o: Handler.cpp Handler.h EventGenerators/EventGenerator.h Event.h
	$(CC) $(CFLAGS) -c $<

CongestionAlg.o: EventGenerators/CongestionAlg.cpp EventGenerators/CongestionAlg.h UnackEvent.h PacketEvent.h EventGenerators/Flow.h
	$(CC) $(CFLAGS) -c $<
    
deref_comp.o: deref_comp.cpp deref_comp.h Event.h
	$(CC) $(CFLAGS) -c $<

# An executable, prepared to test the objects.
runtest: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

# Tests the objects by running the runtest executable.
test: runtest
	./runtest

clean:
	rm *.o
