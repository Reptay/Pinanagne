CXX=g++
CXXFLAGS=-Wall -Wextra -Werror -pedantic -std=c++11 -g3
FILES=src/main.cc src/filter/filters.cc src/detection/shape.cc src/detection/circle.cc src/ransac/ransac.cc src/detection/typePanneau.cc
OBJ=$(FILES:.cc=.o)
TARGET=pinanagne

all: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $^ `pkg-config opencv --cflags --libs`

check: all
	./pinanagne tests/img/panneau-70.jpg

checkfp : all #faux positifs
	./pinanagne tests/img/faux-positif/demi_tour.jpg
	./pinanagne tests/img/faux-positif/interdiction1.png
	./pinanagne tests/img/faux-positif/non_priorite.png
	./pinanagne tests/img/faux-positif/pieton1.jpg
	./pinanagne tests/img/faux-positif/stop.jpg
	./pinanagne tests/img/faux-positif/velo.jpg
	./pinanagne tests/img/faux-positif/tourne_droite.jpg
	./pinanagne tests/img/faux-positif/interdiction2.jpg
	./pinanagne tests/img/faux-positif/pieton2.jpg
	./pinanagne tests/img/faux-positif/stop2.jpg

clean:
	$(RM) $(OBJ) $(OBJ2) $(TARGET)
