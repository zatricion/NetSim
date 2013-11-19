CC=g++
CFLAGS=-std=c++0x -g -Wall
LDFLAGS=-lm

OBJECTS = Test.o Packet.o Flow.o Event.o UnackEvent.o FlowEvent.o PacketEvent.o EventGenerator.o FlowGenerator.o Router.o Host.o Link.o Handler.o CongestionAlg.o deref_comp.o
SOURCE = Test.cpp

all: simulator

simulator: runtest # TODO this will compile and create the actual program.
                   # For now, we will just generate the test file.

# Individual Object dependencies
Test.o: IO/Test.cpp IO/Test.h EventHandling/Packet.h EventHandling/Event.h EventHandling/Handler.h
	$(CC) $(CFLAGS) -c $<
    
Packet.o: EventHandling/Packet.cpp EventHandling/Packet.h
	$(CC) $(CFLAGS) -c $<
    
Flow.o: EventGenerators/Flow.cpp EventGenerators/Flow.h EventHandling/Packet.h EventGenerators/CongestionAlg.h EventGenerators/Host.h
	$(CC) $(CFLAGS) -c $<

Event.o: EventHandling/Event.cpp EventHandling/Event.h
	$(CC) $(CFLAGS) -c $<
    
UnackEvent.o: EventHandling/UnackEvent.cpp EventHandling/UnackEvent.h EventHandling/Packet.h EventHandling/Event.h
	$(CC) $(CFLAGS) -c $<

FlowEvent.o: EventHandling/FlowEvent.cpp EventHandling/FlowEvent.h EventHandling/Packet.h EventHandling/Event.h EventGenerators/Flow.h
	$(CC) $(CFLAGS) -c $<
    
PacketEvent.o: EventHandling/PacketEvent.cpp EventHandling/PacketEvent.h EventHandling/Packet.h EventGenerators/EventGenerator.h
	$(CC) $(CFLAGS) -c $<

EventGenerator.o: EventGenerators/EventGenerator.cpp EventGenerators/EventGenerator.h EventHandling/Event.h Tools/deref_comp.h
	$(CC) $(CFLAGS) -c $<

FlowGenerator.o: EventGenerators/FlowGenerator.cpp EventGenerators/FlowGenerator.h EventGenerators/EventGenerator.h EventGenerators/Flow.h
	$(CC) $(CFLAGS) -c $<

Router.o: EventGenerators/Router.cpp EventGenerators/Router.h EventHandling/Packet.h EventHandling/PacketEvent.h
	$(CC) $(CFLAGS) -c $<

Host.o: EventGenerators/Host.cpp EventGenerators/Host.h EventGenerators/Link.h EventHandling/Packet.h EventHandling/FlowEvent.h EventGenerators/Flow.h EventHandling/UnackEvent.h
	$(CC) $(CFLAGS) -c $<
    
Link.o: EventGenerators/Link.cpp EventGenerators/Link.h EventGenerators/EventGenerator.h EventHandling/PacketEvent.h
	$(CC) $(CFLAGS) -c $<

Handler.o: EventHandling/Handler.cpp EventHandling/Handler.h EventGenerators/EventGenerator.h EventHandling/Event.h
	$(CC) $(CFLAGS) -c $<

CongestionAlg.o: EventGenerators/CongestionAlg.cpp EventGenerators/CongestionAlg.h EventHandling/UnackEvent.h EventHandling/PacketEvent.h EventGenerators/Flow.h
	$(CC) $(CFLAGS) -c $<
    
deref_comp.o: Tools/deref_comp.cpp Tools/deref_comp.h EventHandling/Event.h
	$(CC) $(CFLAGS) -c $<

# An executable, prepared to test the objects.
runtest: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

# Tests the objects by running the runtest executable.
test: runtest
	./runtest

clean:
	rm *.o
