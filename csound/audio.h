#pragma once
#include <alsa/asoundlib.h>
#define ALSA_PCM_NEW_HW_PARAMS_API
#define PCM_DEVICE "default"
#define CHANNELS 1
#ifndef RATE
#define RATE 22050
#endif

typedef struct AudioOut {
    snd_pcm_t *handle;
    snd_pcm_uframes_t frames;
    char *buffer;
    int size;
    unsigned int period;
    long loops;
} AudioOut;

snd_pcm_t *audio_get_handle();
void audio_close(snd_pcm_t *pcm_handle);

AudioOut audio_make();
void audio_free(AudioOut au);
void audio_error_check(int rc);
/* char *audio_config(snd_pcm_t *handle); */
char *audio_config(snd_pcm_t *handle, long loops);
void audio_conf(snd_pcm_t *handle, snd_pcm_hw_params_t *params);
