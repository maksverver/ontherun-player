CXXFLAGS=-Wall -ansi -g -O2 -m32
SOURCES=Detectives.cpp Fugitive.cpp Map.cpp main.cpp common.cpp debug.cpp
OBJECTS=Detectives.o Fugitive.o Map.o main.o common.o debug.o  # NOTE: debug.o MUST come last!

all: random player

player: $(OBJECTS)
	g++ $(CXXFLAGS) -o player $(OBJECTS)

submission.cpp: $(SOURCES)
	./compile.pl $(SOURCES) > submission.cpp

clean:
	-rm random player $(OBJECTS) submission.cpp

