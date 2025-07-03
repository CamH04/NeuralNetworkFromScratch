CXX = g++
AS  = as
LD  = g++

WIN_CXX = x86_64-w64-mingw32-g++
WIN_LD  = x86_64-w64-mingw32-g++

CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude
ASFLAGS  =
LDFLAGS  =

CPP_SOURCES := $(shell find src -name '*.cpp')

objects := $(CPP_SOURCES:src/%.cpp=obj/%.o)
win_objects := $(CPP_SOURCES:src/%.cpp=obj-win/%.o)

main: $(objects)
	$(LD) $(LDFLAGS) -o $@ $(objects)

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

win: main.exe

main.exe: $(win_objects)
	$(WIN_LD) $(LDFLAGS) -o $@ $(win_objects) -static

obj-win/%.o: src/%.cpp
	mkdir -p $(@D)
	$(WIN_CXX) $(CXXFLAGS) -c -o $@ $<

run: main
	./main

.PHONY: clean
clean:
	rm -rf obj obj-win main main.exe
