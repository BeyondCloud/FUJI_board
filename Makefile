LIBS  = -lwinmm
CFLAGS = -Wall

# Should be equivalent to your list of C files, if you don't build selectively
SRC=$(wildcard *.cpp)
all: $(SRC)
	g++ -o $@ $^ $(CFLAGS) $(LIBS) -g
