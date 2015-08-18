#ifndef AUDIO_HH
#define AUDIO_HH
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

void playSound(std::string path);

#endif
