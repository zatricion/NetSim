CC=g++
CFLAGS=-std=c++0x -g -Wall
LDFLAGS=-lm -lboost_iostreams -lboost_system -lboost_filesystem
 
OBJECTS = build/Test.o build/Packet.o build/Path.o build/Flow.o build/Event.o build/UnackEvent.o build/FlowEvent.o build/PacketEvent.o build/EventGenerator.o build/FlowGenerator.o build/Router.o build/Host.o build/Link.o build/Handler.o build/deref_comp.o build/TCPRenoUpdateEvent.o build/TimeoutEvent.o build/Plotter.o build/TCPVegasUpdateEvent.o build/BFResendEvent.o build/VegasFlow.o build/TahoeFlow.o src/IO/parser.tab.o src/IO/lexer.yy.o src/IO/NetInput.o #build/parser.tab.o build/lexer.yy.o build/NetInput.o
SOURCE = src/IO/Test.cpp

all: simulator 

simulator: runtest # TODO this will compile and create the actual program.
                   # For now, we will just generate the test file.

# Individual Object dependencies
build/Test.o: src/IO/Test.cpp src/IO/Test.h src/EventHandling/Packet.h src/EventHandling/Event.h src/EventHandling/Handler.h src/EventGenerators/Path.h
	$(CC) $(CFLAGS) -c $< -o build/Test.o 

build/Path.o: src/EventGenerators/Path.cpp src/EventGenerators/Path.h  
	$(CC) $(CFLAGS) -c $< -o build/Path.o 

build/Packet.o: src/EventHandling/Packet.cpp src/EventHandling/Packet.h src/EventGenerators/Path.h
	$(CC) $(CFLAGS) -c $< -o build/Packet.o

build/Flow.o: src/EventGenerators/Flow.cpp src/EventGenerators/Flow.h src/EventHandling/Packet.h src/EventGenerators/Host.h
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

build/deref_comp.o: src/Tools/deref_comp.cpp src/Tools/deref_comp.h src/EventHandling/Event.h
	$(CC) $(CFLAGS) -c $< -o build/deref_comp.o

build/TCPRenoUpdateEvent.o: src/EventHandling/TCPRenoUpdateEvent.cpp src/EventHandling/TCPRenoUpdateEvent.h
	$(CC) $(CFLAGS) -c $< -o build/TCPRenoUpdateEvent.o

build/TimeoutEvent.o: src/EventHandling/TimeoutEvent.cpp src/EventHandling/TimeoutEvent.h
	$(CC) $(CFLAGS) -c $< -o build/TimeoutEvent.o

build/Plotter.o: src/IO/Plotter.cpp src/IO/Plotter.h src/IO/gnuplot-iostream.h
	$(CC) $(CFLAGS) -c $< -o build/Plotter.o

build/TCPVegasUpdateEvent.o: src/EventHandling/TCPVegasUpdateEvent.cpp src/EventHandling/TCPVegasUpdateEvent.h
	$(CC) $(CFLAGS) -c $< -o build/TCPVegasUpdateEvent.o

build/BFResendEvent.o: src/EventHandling/BFResendEvent.cpp src/EventHandling/BFResendEvent.h
	$(CC) $(CFLAGS) -c $< -o build/BFResendEvent.o

build/VegasFlow.o: src/EventGenerators/VegasFlow.cpp src/EventGenerators/VegasFlow.h
	$(CC) $(CFLAGS) -c $< -o build/VegasFlow.o

build/TahoeFlow.o: src/EventGenerators/TahoeFlow.cpp src/EventGenerators/TahoeFlow.h
	$(CC) $(CFLAGS) -c $< -o build/TahoeFlow.o

# put everything in src/IO at first, then move .o's to build

#src/IO/parser.tab.cpp src/IO/parser.tab.hpp: src/IO/parser.ypp
src/IO/parser.tab.cpp src/IO/parser.tab.hpp: src/IO/parser.ypp
	bison -d src/IO/parser.ypp
        # lolwat
	mv parser.tab.cpp src/IO/parser.tab.cpp
	mv parser.tab.hpp src/IO/parser.tab.hpp	

build/parser.tab.o: src/IO/parser.tab.cpp src/IO/parser.tab.hpp
src/IO/parser.tab.o: src/IO/parser.tab.cpp src/IO/parser.tab.hpp
	#$(CC) $(CFLAGS) -c $< -o src/IO/parser.tab.o
	$(CC) $(CFLAGS) -c $< -o src/IO/parser.tab.o
	#mv parser.tab.o build/parser.tab.o

src/IO/lexer.yy.cpp: src/IO/lexer.lex
	flex -+ -o src/IO/lexer.yy.cpp src/IO/lexer.lex

build/lexer.yy.o: src/IO/lexer.yy.cpp
src/IO/lexer.yy.o: src/IO/lexer.yy.cpp
	#$(CC) $(CFLAGS) -c $< -o src/IO/lexer.yy.o
	$(CC) $(CFLAGS) -c $< -o src/IO/lexer.yy.o
	# ew
	#mv src/IO/lexer.yy.o build/lexer.yy.o

build/NetInput.o: src/IO/NetInput.cpp src/IO/NetInput.h
src/IO/NetInput.o: src/IO/NetInput.cpp src/IO/NetInput.h
	$(CC) $(CFLAGS) -c $< -o src/IO/NetInput.o

# An executable, prepared to test the objects.
runtest: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

# Tests the objects by running the runtest executable.
test: clean runtest
	./runtest

clean:
	rm build/*.o src/IO/*.o src/IO/lexer.yy.cpp src/IO/parser.tab.cpp src/IO/parser.tab.hpp
