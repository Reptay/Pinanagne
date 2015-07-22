CXX=g++
CXXFLAGS=-Wall -Wextra -Werror -pedantic -std=c++11 -g3
FILES=src/main.cc src/filter/filters.cc src/detection/shape.cc src/detection/circle.cc src/ransac/ransac.cc src/detection/typePanneau.cc
OBJ=$(FILES:.cc=.o)
TARGET=pinanagne

all: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $^ `pkg-config opencv --cflags --libs`

check: all
	./pinanagne tests/img/panneau-70.jpg

clean:
	$(RM) $(OBJ) $(OBJ2) $(TARGET)
