/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#define RATE 44100
#include <alsa/asoundlib.h>
#include "audio.h"
#include <stdlib.h>
#include <math.h>

void audio_play_1(snd_pcm_t *handle, unsigned int period, u_int16_t *buffer, int size, snd_pcm_uframes_t frames) {
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

void audio_gen_sine(u_int16_t *buffer, int size) {
    printf("buf all size: %d\n", size);
    float a = 0.0;
    int fq = 500;
    float delta = 0.000001 * (fq / 1);
    int noise = 1;
    int vol = 200000;

    for (int i = 0; i < size; i++) {
        buffer[i] = ((sinf(a) * vol));// + (rand() % noise));
        a += delta;
    }
    printf("end fq: %d\n", fq);
}

void audio_gen_tri(u_int16_t *buffer, int size) {
    int a = 0;
    int delta = 1;
    int treshold = 200000;

    for (int i = 0; i < size; i++) {
        buffer[i] = a * 1000;
        a += delta;
        a %= treshold;
    }
}

void audio_play_2(snd_pcm_t *handle, unsigned int period, u_int16_t *buffer, int size, snd_pcm_uframes_t frames) {
    int rc = 0;
    long loops;
    loops = 1000000 / period;
    printf("loops: %ld size: %d period: %d\n", loops, size, period);

    int len = loops * size;
    u_int16_t *buf = malloc(len * 2);
    audio_gen_sine(buf, len);
    /* audio_gen_tri(buf, len); */
    int i = 0;

    while (loops > 0) {
        loops--;
        memcpy(buffer, buf + i * size, size);
        i++;

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
    free(buf);
}


int main() {
    int rc = 0;
    int size = 0;
    snd_pcm_t *handle;
    snd_pcm_hw_params_t *params;
    unsigned int val = 0;
    int dir = 0;
    snd_pcm_uframes_t frames;
    u_int16_t *buffer = NULL;

    handle = audio_get_handle();

    snd_pcm_hw_params_malloc(&params);
    snd_pcm_hw_params_any(handle, params);

    audio_conf(handle, params);

    snd_pcm_hw_params_get_period_size(params, &frames, &dir);
    size = frames * 2 * CHANNELS;
    buffer = (u_int16_t *) malloc(size * 2);

    snd_pcm_hw_params_get_period_time(params, &val, &dir);
    snd_pcm_hw_params_free(params);

    audio_play_2(handle, val, buffer, size, frames);

    snd_pcm_drain(handle);
    snd_pcm_close(handle);
    free(buffer);

    return 0;
}
