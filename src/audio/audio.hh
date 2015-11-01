#ifndef AUDIO_HH
#define AUDIO_HH
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

void playSound(std::string path);

/**
 * limitation = "30", "50"...
 * on ne peut lancer qu'une limitaton à la fois
 */
void playLimitation(std::string limitation);

#endif
