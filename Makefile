BBBBBCXX=g++
CXXFLAGS=-Wall -Wextra -Werror -pedantic -std=c++11 -g3 -lSDL_mixer
FILES=src/main.cc src/filter/filters.cc src/detection/shape.cc src/detection/circle.cc src/ransac/ransac.cc src/detection/typePanneau.cc src/audio/audio.cc
OBJ=$(FILES:.cc=.o)
TARGET=pinanagne

all: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $^ `pkg-config opencv --cflags --libs`

check: all
	./pinanagne -i tests/img/panneau-70.jpg

checkfp : all #faux positifs
	./pinanagne -i tests/img/faux-positif/demi_tour.jpg
	./pinanagne -i tests/img/faux-positif/interdiction1.png
	./pinanagne -i tests/img/faux-positif/non_priorite.png
	./pinanagne -i tests/img/faux-positif/pieton1.jpg
	./pinanagne -i tests/img/faux-positif/stop.jpg
	./pinanagne -i tests/img/faux-positif/velo.jpg
	./pinanagne -i tests/img/faux-positif/tourne_droite.jpg
	./pinanagne -i tests/img/faux-positif/interdiction2.jpg
	./pinanagne -i tests/img/faux-positif/pieton2.jpg
	./pinanagne -i tests/img/faux-positif/stop2.jpg

checkstd : all #panneaux vitesse standard
	./pinanagne -i tests/img/panneau-vitesse-standard/panneau30.jpg
	./pinanagne -i tests/img/panneau-vitesse-standard/panneau50.jpg
	./pinanagne -i tests/img/panneau-vitesse-standard/panneau70.png
	./pinanagne -i tests/img/panneau-vitesse-standard/panneau90.jpg
	./pinanagne -i tests/img/panneau-vitesse-standard/panneau110.jpg   
	./pinanagne -i tests/img/panneau-vitesse-standard/panneau130.png  

checkv1: all
	./pinanagne tests/video/nationale/1/panneau1.mp4

checkv2: all
	./pinanagne tests/video/nationale/1/panneau2.mp4


clean:
	$(RM) $(OBJ) $(OBJ2) $(TARGET)
