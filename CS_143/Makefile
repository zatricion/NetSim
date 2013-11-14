CC=g++-4.7
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
    
Flow.o: Flow.cpp Flow.h Packet.h CongestionAlg.h Host.h
	$(CC) $(CFLAGS) -c $<

Event.o: Event.cpp Event.h
	$(CC) $(CFLAGS) -c $<
    
UnackEvent.o: UnackEvent.cpp UnackEvent.h Packet.h Event.h
	$(CC) $(CFLAGS) -c $<

FlowEvent.o: FlowEvent.cpp FlowEvent.h Packet.h Event.h Flow.h
	$(CC) $(CFLAGS) -c $<
    
PacketEvent.o: PacketEvent.cpp PacketEvent.h Packet.h EventGenerator.h
	$(CC) $(CFLAGS) -c $<

EventGenerator.o: EventGenerator.cpp EventGenerator.h Event.h deref_comp.h
	$(CC) $(CFLAGS) -c $<

FlowGenerator.o: FlowGenerator.cpp FlowGenerator.h EventGenerator.h Flow.h
	$(CC) $(CFLAGS) -c $<

Router.o: Router.cpp Router.h Packet.h PacketEvent.h
	$(CC) $(CFLAGS) -c $<

Host.o: Host.cpp Host.h Link.h Packet.h FlowEvent.h Flow.h UnackEvent.h
	$(CC) $(CFLAGS) -c $<
    
Link.o: Link.cpp Link.h EventGenerator.h PacketEvent.h
	$(CC) $(CFLAGS) -c $<

Handler.o: Handler.cpp Handler.h EventGenerator.h Event.h
	$(CC) $(CFLAGS) -c $<

CongestionAlg.o: CongestionAlg.cpp CongestionAlg.h UnackEvent.h PacketEvent.h Flow.h
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
