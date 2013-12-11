CC=g++

# Code requires C++11.
CFLAGS=-std=c++0x -g -Wall

# Boost library is used.
LDFLAGS=-lm -lboost_iostreams -lboost_system -lboost_filesystem
 
# List of objects to be built; stored in build/ directory.
OBJECTS = build/Simulator.o build/Packet.o build/Path.o build/Flow.o build/Event.o build/UnackEvent.o build/FlowEvent.o build/PacketEvent.o build/EventGenerator.o build/FlowGenerator.o build/Router.o build/Host.o build/Link.o build/Handler.o build/deref_comp.o build/Plotter.o build/TCPVegasUpdateEvent.o build/BFResendEvent.o build/VegasFlow.o build/TahoeFlow.o src/IO/parser.tab.o src/IO/lexer.yy.o src/IO/NetInput.o

# Source file
SOURCE = src/IO/Simulator.cpp

all: simulator 

# To build the simulator, build and link all objects.
simulator: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

# Individual object dependencies
build/Simulator.o: src/IO/Simulator.cpp src/IO/Simulator.h src/IO/Plotter.h src/IO/NetInput.h src/EventHandling/Handler.h
	$(CC) $(CFLAGS) -c $< -o build/Simulator.o 

build/Path.o: src/EventGenerators/Path.cpp src/EventGenerators/Path.h
	$(CC) $(CFLAGS) -c $< -o build/Path.o 

build/Packet.o: src/EventHandling/Packet.cpp src/EventHandling/Packet.h src/EventGenerators/Path.h
	$(CC) $(CFLAGS) -c $< -o build/Packet.o

build/Flow.o: src/EventGenerators/Flow.cpp src/EventGenerators/Flow.h src/EventHandling/Packet.h src/EventGenerators/Host.h src/Tools/Log.h
	$(CC) $(CFLAGS) -c $< -o build/Flow.o

build/Event.o: src/EventHandling/Event.cpp src/EventHandling/Event.h
	$(CC) $(CFLAGS) -c $< -o build/Event.o

build/UnackEvent.o: src/EventHandling/UnackEvent.cpp src/EventHandling/UnackEvent.h src/EventHandling/Packet.h src/EventHandling/Event.h
	$(CC) $(CFLAGS) -c $< -o build/UnackEvent.o

build/FlowEvent.o: src/EventHandling/FlowEvent.cpp src/EventHandling/FlowEvent.h src/EventHandling/Packet.h src/EventHandling/Event.h src/EventGenerators/Flow.h
	$(CC) $(CFLAGS) -c $< -o build/FlowEvent.o

build/PacketEvent.o: src/EventHandling/PacketEvent.cpp src/EventHandling/PacketEvent.h src/EventHandling/Packet.h src/EventGenerators/EventGenerator.h
	$(CC) $(CFLAGS) -c $< -o build/PacketEvent.o

build/EventGenerator.o: src/EventGenerators/EventGenerator.cpp src/EventGenerators/EventGenerator.h src/EventHandling/Event.h src/Tools/deref_comp.h src/Tools/Log.h
	$(CC) $(CFLAGS) -c $< -o build/EventGenerator.o

build/FlowGenerator.o: src/EventGenerators/FlowGenerator.cpp src/EventGenerators/FlowGenerator.h src/EventGenerators/EventGenerator.h src/EventGenerators/Flow.h src/EventHandling/FlowEvent.h
	$(CC) $(CFLAGS) -c $< -o build/FlowGenerator.o

build/Router.o: src/EventGenerators/Router.cpp src/EventGenerators/Router.h src/EventHandling/Packet.h src/EventHandling/PacketEvent.h src/EventGenerators/Host.h src/EventGenerators/Path.h src/EventHandling/BFResendEvent.h
	$(CC) $(CFLAGS) -c $< -o build/Router.o

build/Host.o: src/EventGenerators/Host.cpp src/EventGenerators/Host.h src/EventGenerators/Link.h src/EventGenerators/Flow.h src/EventGenerators/VegasFlow.h src/EventHandling/Packet.h src/EventHandling/FlowEvent.h src/EventHandling/UnackEvent.h src/EventHandling/TCPVegasUpdateEvent.h src/Tools/Log.h
	$(CC) $(CFLAGS) -c $< -o build/Host.o

build/Link.o: src/EventGenerators/Link.cpp src/EventGenerators/Link.h src/EventGenerators/EventGenerator.h src/EventHandling/PacketEvent.h src/IO/Plotter.h src/Tools/Log.h
	$(CC) $(CFLAGS) -c $< -o build/Link.o

build/Handler.o: src/EventHandling/Handler.cpp src/EventHandling/Handler.h src/EventGenerators/EventGenerator.h src/EventHandling/Event.h src/EventHandling/PacketEvent.h src/Tools/Log.h
	$(CC) $(CFLAGS) -c $< -o build/Handler.o

build/deref_comp.o: src/Tools/deref_comp.cpp src/Tools/deref_comp.h src/EventHandling/Event.h
	$(CC) $(CFLAGS) -c $< -o build/deref_comp.o

build/Plotter.o: src/IO/Plotter.cpp src/IO/Plotter.h src/IO/gnuplot-iostream.h
	$(CC) $(CFLAGS) -c $< -o build/Plotter.o

build/TCPVegasUpdateEvent.o: src/EventHandling/TCPVegasUpdateEvent.cpp src/EventHandling/TCPVegasUpdateEvent.h src/EventHandling/Event.h
	$(CC) $(CFLAGS) -c $< -o build/TCPVegasUpdateEvent.o

build/BFResendEvent.o: src/EventHandling/BFResendEvent.cpp src/EventHandling/BFResendEvent.h src/EventHandling/Event.h
	$(CC) $(CFLAGS) -c $< -o build/BFResendEvent.o

build/VegasFlow.o: src/EventGenerators/VegasFlow.cpp src/EventGenerators/VegasFlow.h src/EventGenerators/Flow.h src/EventGenerators/Host.h src/EventHandling/Packet.h src/Tools/Log.h
	$(CC) $(CFLAGS) -c $< -o build/VegasFlow.o

build/TahoeFlow.o: src/EventGenerators/TahoeFlow.cpp src/EventGenerators/TahoeFlow.h src/EventGenerators/Flow.h src/EventGenerators/Host.h src/EventHandling/Packet.h src/Tools/Log.h
	$(CC) $(CFLAGS) -c $< -o build/TahoeFlow.o


# Make parser.
src/IO/parser.tab.cpp src/IO/parser.tab.hpp: src/IO/parser.ypp
	bison -d src/IO/parser.ypp
	mv parser.tab.cpp src/IO/parser.tab.cpp
	mv parser.tab.hpp src/IO/parser.tab.hpp	

build/parser.tab.o: src/IO/parser.tab.cpp src/IO/parser.tab.hpp
src/IO/parser.tab.o: src/IO/parser.tab.cpp src/IO/parser.tab.hpp
	$(CC) $(CFLAGS) -c $< -o src/IO/parser.tab.o

src/IO/lexer.yy.cpp: src/IO/lexer.lex
	flex -+ -o src/IO/lexer.yy.cpp src/IO/lexer.lex

build/lexer.yy.o: src/IO/lexer.yy.cpp
src/IO/lexer.yy.o: src/IO/lexer.yy.cpp
	$(CC) $(CFLAGS) -c $< -o src/IO/lexer.yy.o

build/NetInput.o: src/IO/NetInput.cpp src/IO/NetInput.h

src/IO/NetInput.o: src/IO/NetInput.cpp src/IO/NetInput.h
	$(CC) $(CFLAGS) -c $< -o src/IO/NetInput.o

clean:
	rm build/*.o src/IO/*.o src/IO/lexer.yy.cpp src/IO/parser.tab.cpp src/IO/parser.tab.hpp simulator
