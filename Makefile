CPPFLAGS = -I/usr/include/irrlicht
CXXFLAGS = -Wall -Wextra -O2 -g -std=c++11 -Wno-unused-parameter

irrlicht-exemple4: main.o events.o collision_callback.o
	g++ $^ -o $@ -lIrrlicht

events.o collision_callback.o main.o: events.h collision_callback.h

clean:
	@rm -f irrlicht-exemple4 *.o
