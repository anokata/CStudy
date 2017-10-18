/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <alsa/asoundlib.h>
#include "audio.h"
#include <stdlib.h>
#include <math.h>

void audio_play_1(snd_pcm_t *handle, unsigned int period, char *buffer, int size, snd_pcm_uframes_t frames) {
    int rc = 0;
    long loops;
    loops = 1000000 / period;
    while (loops > 0) {
        loops--;
        rc = read(0, buffer, size);
        if (rc == 0) {
            fprintf(stderr, "end of file on input\n");
            break;
        } else if (rc != size) {
            fprintf(stderr, "short read: read %d bytes\n", rc);
        }
        rc = snd_pcm_writei(handle, buffer, frames);
        if (rc == -EPIPE) {
            /* EPIPE means underrun */
            fprintf(stderr, "underrun occurred\n");
            snd_pcm_prepare(handle);
        } else if (rc < 0) {
            fprintf(stderr, "error from writei: %s\n", snd_strerror(rc));
        }  else if (rc != (int)frames) {
            fprintf(stderr, "short write, write %d frames\n", rc);
            }
    }
}

void audio_play_2(snd_pcm_t *handle, unsigned int period, char *buffer, int size, snd_pcm_uframes_t frames) {
    int rc = 0;
    long loops;
    loops = 1000000 / period;
    float a = 0.0;
    while (loops > 0) {
        loops--;
        /* buffer, size); */
        for (int i = 0; i < size; i++) {
            /* buffer[i] = rand() * (sinf(a += 1.0) * 100); */
            buffer[i] = (sinf(a += 0.10) * 100) + (rand() % 50);
        }

        rc = snd_pcm_writei(handle, buffer, frames);
        if (rc == -EPIPE) {
            /* EPIPE means underrun */
            fprintf(stderr, "underrun occurred\n");
            snd_pcm_prepare(handle);
        } else if (rc < 0) {
            fprintf(stderr, "error from writei: %s\n", snd_strerror(rc));
        }  else if (rc != (int)frames) {
            fprintf(stderr, "short write, write %d frames\n", rc);
            }
    }
}


int main() {
    int rc = 0;
    int size = 0;
    snd_pcm_t *handle;
    snd_pcm_hw_params_t *params;
    unsigned int val = 0;
    int dir = 0;
    snd_pcm_uframes_t frames;
    char *buffer = NULL;

    handle = audio_get_handle();

    snd_pcm_hw_params_malloc(&params);
    snd_pcm_hw_params_any(handle, params);

    audio_conf(handle, params);

    snd_pcm_hw_params_get_period_size(params, &frames, &dir);
    size = frames * 2 * CHANNELS;
    buffer = (char *) malloc(size);

    snd_pcm_hw_params_get_period_time(params, &val, &dir);
    snd_pcm_hw_params_free(params);

    audio_play_2(handle, val, buffer, size, frames);

    snd_pcm_drain(handle);
    snd_pcm_close(handle);
    free(buffer);

    return 0;
}
