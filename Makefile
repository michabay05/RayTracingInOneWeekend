CXX=g++
CXXFLAGS=-Wall -Wextra -pedantic -std=c++17

.PHONY: all compile run

all: compile

run:
	$(MAKE) compile
	./raytracing

compile:
	g++ $(CXXFLAGS) -o raytracing src/main.cpp -O2