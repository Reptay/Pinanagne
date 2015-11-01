#include "audio.hh"

static bool isPlaying = false;

int playSound2(std::string path)
{
  /*
  if (SDL_Init(SDL_INIT_AUDIO) != 0) {
    fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
    return 1;
  }
  */
  int audio_rate = 22050;
  Uint16 audio_format = AUDIO_S16SYS;
  int audio_channels = 2;
  int audio_buffers = 4096;
  
  if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) !=
     0) {
    fprintf(stderr, "Unable to initialize audio: %s\n", Mix_GetError());
    exit(1);
  }
  
  Mix_Chunk *sound = NULL;

  sound = Mix_LoadWAV(path.c_str());
  if(sound == NULL) {
    fprintf(stderr, "Unable to load WAV file: %s\n", Mix_GetError());
  }
  int channel;

  channel = Mix_PlayChannel(-1, sound, 0);
  if(channel == -1) {
    fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
  }
  while(Mix_Playing(channel) != 0);

  Mix_FreeChunk(sound);

  //  Mix_CloseAudio();
  //  SDL_Quit();
  return 0;
}

void playSound(std::string path)
{
  std::thread th(playSound2, path);
  th.detach();
}

void playLimitation2(std::string limitation)
{
  playSound2("audio/la_vitesse_est_limite_a.wav");
  playSound2("audio/"+limitation+".wav");
  isPlaying=false;
}

void playLimitation(std::string limitation)
{
  if (!isPlaying)
    {
      isPlaying=true;
      std::thread th(playLimitation2, limitation);
      th.detach();
    }
}
