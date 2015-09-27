BBBBBCXX=g++
CXXFLAGS= -std=c++11 -g3 -pthread
FILES=src/main.cc src/filter/filters.cc src/detection/shape.cc src/detection/circle.cc src/ransac/ransac.cc src/detection/typePanneau.cc src/audio/audio.cc src/surf/surf.cc src/detection/rectangle.cc
OBJ=$(FILES:.cc=.o)
TARGET=pinanagne

all: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $^ `pkg-config opencv --cflags --libs` -lSDL2 -lSDL2_mixer

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

checkslow: all
	./pinanagne tests/video/vitesse_reduite/panneau_30_1.mp4
	./pinanagne tests/video/vitesse_reduite/faux_positif_1.mp4
	./pinanagne tests/video/vitesse_reduite/faux_positif_2.mp4
	./pinanagne tests/video/vitesse_reduite/faux_positif_3.mp4
	./pinanagne tests/video/vitesse_reduite/faux_positif_4.mp4
check50: all
	./pinanagne tests/video/vitesse_reel_inf50/panneau_30_1.mp4
	./pinanagne tests/video/vitesse_reel_inf50/ panneau_30_2.mp4
	./pinanagne tests/video/vitesse_reel_inf50/faux_positif_1.mp4
	./pinanagne tests/video/vitesse_reel_inf50/panneau_50_1.mp4
	./pinanagne tests/video/vitesse_reel_inf50/panneau_30_3.mp4
	./pinanagne tests/video/vitesse_reel_inf50/panneau_50_2.mp4
	./pinanagne tests/video/vitesse_reel_inf50/panneau_50_3.mp4

checkv1: all
	./pinanagne tests/video/nationale/1/panneau1.mp4

checkv2: all
	./pinanagne tests/video/nationale/1/panneau2.mp4


clean:
	$(RM) $(OBJ) $(OBJ2) $(TARGET)
