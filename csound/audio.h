#pragma once
#include <alsa/asoundlib.h>
#define ALSA_PCM_NEW_HW_PARAMS_API

snd_pcm_t *audio_get_handle();
