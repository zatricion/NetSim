CC=g++
CFLAGS=-std=c++0x -g -Wall
LDFLAGS=-lm

OBJECTS = build/Test.o build/Packet.o build/Flow.o build/Event.o build/UnackEvent.o build/FlowEvent.o build/PacketEvent.o build/EventGenerator.o build/FlowGenerator.o build/Router.o build/Host.o build/Link.o build/Handler.o build/CongestionAlg.o build/deref_comp.o build/TCPReno.o
SOURCE = src/IO/Test.cpp

all: simulator

simulator: runtest # TODO this will compile and create the actual program.
                   # For now, we will just generate the test file.

# Individual Object dependencies
build/Test.o: src/IO/Test.cpp src/IO/Test.h src/EventHandling/Packet.h src/EventHandling/Event.h src/EventHandling/Handler.h
	$(CC) $(CFLAGS) -c $< -o build/Test.o 
    
build/Packet.o: src/EventHandling/Packet.cpp src/EventHandling/Packet.h
	$(CC) $(CFLAGS) -c $< -o build/Packet.o
    
build/Flow.o: src/EventGenerators/Flow.cpp src/EventGenerators/Flow.h src/EventHandling/Packet.h src/EventGenerators/CongestionAlg.h src/EventGenerators/Host.h
	$(CC) $(CFLAGS) -c $< -o build/Flow.o

build/Event.o: src/EventHandling/Event.cpp src/EventHandling/Event.h
	$(CC) $(CFLAGS) -c $< -o build/Event.o
    
build/UnackEvent.o: src/EventHandling/UnackEvent.cpp src/EventHandling/UnackEvent.h src/EventHandling/Packet.h src/EventHandling/Event.h
	$(CC) $(CFLAGS) -c $< -o build/UnackEvent.o

build/FlowEvent.o: src/EventHandling/FlowEvent.cpp src/EventHandling/FlowEvent.h src/EventHandling/Packet.h src/EventHandling/Event.h src/EventGenerators/Flow.h
	$(CC) $(CFLAGS) -c $< -o build/FlowEvent.o
    
build/PacketEvent.o: src/EventHandling/PacketEvent.cpp src/EventHandling/PacketEvent.h src/EventHandling/Packet.h src/EventGenerators/EventGenerator.h
	$(CC) $(CFLAGS) -c $< -o build/PacketEvent.o

build/EventGenerator.o: src/EventGenerators/EventGenerator.cpp src/EventGenerators/EventGenerator.h src/EventHandling/Event.h src/Tools/deref_comp.h
	$(CC) $(CFLAGS) -c $< -o build/EventGenerator.o

build/FlowGenerator.o: src/EventGenerators/FlowGenerator.cpp src/EventGenerators/FlowGenerator.h src/EventGenerators/EventGenerator.h src/EventGenerators/Flow.h
	$(CC) $(CFLAGS) -c $< -o build/FlowGenerator.o

build/Router.o: src/EventGenerators/Router.cpp src/EventGenerators/Router.h src/EventHandling/Packet.h src/EventHandling/PacketEvent.h
	$(CC) $(CFLAGS) -c $< -o build/Router.o

build/Host.o: src/EventGenerators/Host.cpp src/EventGenerators/Host.h src/EventGenerators/Link.h src/EventHandling/Packet.h src/EventHandling/FlowEvent.h src/EventGenerators/Flow.h src/EventHandling/UnackEvent.h
	$(CC) $(CFLAGS) -c $< -o build/Host.o
    
build/Link.o: src/EventGenerators/Link.cpp src/EventGenerators/Link.h src/EventGenerators/EventGenerator.h src/EventHandling/PacketEvent.h
	$(CC) $(CFLAGS) -c $< -o build/Link.o

build/Handler.o: src/EventHandling/Handler.cpp src/EventHandling/Handler.h src/EventGenerators/EventGenerator.h src/EventHandling/Event.h
	$(CC) $(CFLAGS) -c $< -o build/Handler.o

build/CongestionAlg.o: src/EventGenerators/CongestionAlg.cpp src/EventGenerators/CongestionAlg.h src/EventHandling/UnackEvent.h src/EventHandling/PacketEvent.h src/EventGenerators/Flow.h
	$(CC) $(CFLAGS) -c $< -o build/CongestionAlg.o
    
build/deref_comp.o: src/Tools/deref_comp.cpp src/Tools/deref_comp.h src/EventHandling/Event.h
	$(CC) $(CFLAGS) -c $< -o build/deref_comp.o

build/TCPReno.o: src/EventGenerators/TCPReno.cpp src/EventGenerators/TCPReno.h src/EventGenerators/CongestionAlg.h src/EventHandling/UnackEvent.h src/EventHandling/PacketEvent.h src/EventGenerators/Flow.h
	$(CC) $(CFLAGS) -c $< -o build/TCPReno.o


# An executable, prepared to test the objects.
runtest: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

# Tests the objects by running the runtest executable.
test: clean runtest
	./runtest

clean:
	rm build/*.o
