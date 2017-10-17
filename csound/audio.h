#pragma once
#include <alsa/asoundlib.h>
#define ALSA_PCM_NEW_HW_PARAMS_API
#define PCM_DEVICE "default"
#define CHANNELS 1
#define RATE 22050

snd_pcm_t *audio_get_handle();
void audio_close(snd_pcm_t *pcm_handle);
