EXEFILE = ducks
CXXFLAGS = -c -Wall
SOURCES = duck.cc mallard_duck.cc decoy_duck.cc rubber_duck.cc quack_behavior.cc mute_quack.cc quack_quack.cc squeak_quack.cc main.cc
OBJECTS = $(SOURCES:.cc=.o)
INCLUDES = -Iheaders

$(EXEFILE): $(OBJECTS)
	g++ $^ -o $@

%.o: %.cc
	g++ $(CXXFLAGS) $(INCLUDES) $^ -o $@

clean:
	rm *.o $(EXEFILE)

.PHONY: clean


