CXXFLAGS = -Wall -pedantic -std=c++14

HEADERS = $(wildcard src/*.h)
SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:src/%.cpp=build/%.o)

build/main: $(OBJECTS)
	mkdir -p $(@D)
	g++ $^ -o $@

build/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ $(CXXFLAGS) $< -c -o $@

debug/test_%.o: tests/%.test.cpp
	mkdir -p $(@D)
	g++ $(CXXFLAGS) $< -I src/ -c -o $@

doc: Doxyfile $(HEADERS)
	doxygen Doxyfile

.PHONY: clean
clean:
	rm -rf username build/ debug/ doc/ 2>/dev/null

.PHONY: test_all
test_all: debug/test_CCoord
	debug/test_CCoord
