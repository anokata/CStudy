/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#define RATE 44100
#include <alsa/asoundlib.h>
#include "audio.h"
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

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


#define width 20
char buf[width * 2] = "                    ";
char *wave_char8(int8_t x) {
    int value = x - 128;
    value = (value * width) / 256;
    int i = -width;
    /* printf("%d %d\n", x, value); */
    for (; i < value; i++) {
        buf[i + width] = '*';
    }
    for (; i < width; i++) {
        buf[i + width] = ' ';
    }
    return buf;
}
char *wave_char16(int16_t x) {
    int value = x - 32765;
    value = (value * width) / 65535;
    int i = -width;
    for (; i < value; i++) {
        buf[i + width] = ' ';
    }
	buf[i + width - 1] = '*';
    for (; i < width; i++) {
        buf[i + width] = ' ';
    }
    return buf;
}

void audio_gen_sine16(uint16_t *buffer, int size) {
    printf("buf all size: %d\n", size);
    float a = 0.0;
    uint16_t fq = 100;
    float delta = 0.001 * fq;
    uint16_t noise = 1;
    uint16_t vol = 1;

    for (int i = 0; i < size; i++) {
        buffer[i] = round((sinf(a) * 32000 * vol));
        printf("%s\n", wave_char16(buffer[i]));
        a += delta;
        a = fmod(a, (M_PI * 2));
    }
    printf("\n");
    printf("end fq: %d\n", fq);
}


void audio_gen_sine8(uint8_t *buffer, int size) {
    printf("buf all size: %d\n", size);
    float a = 0.0;
    uint8_t fq = 100;
    float delta = 0.001 * fq;
    uint8_t noise = 1;
    uint8_t vol = 1;

    for (int i = 0; i < size; i++) {
        buffer[i] = round((sinf(a) * 127 * vol));// + (rand() % noise));
        /* printf("%d \n", buffer[i]); */
        printf("%s\n", wave_char8(buffer[i]));
        /* printf("%f ", sinf(a)); */
        /* printf("%d: %d %f. \t", i, buffer[i], a); */
        a += delta;
        a = fmod(a, (M_PI * 2));
    }
    printf("\n");
    printf("end fq: %d\n", fq);
}


void audio_gen_tri(u_int16_t *buffer, int size) {
    u_int16_t a = 0;
    u_int16_t delta = 1;
    u_int16_t treshold = 2000;

    for (int i = 0; i < size; i++) {
        buffer[i] = a * 1000;
        a += delta;
        a %= treshold;
    }
}

#define sint uint16_t
//#define sint uint8_t
void audio_play_2(snd_pcm_t *handle, unsigned int period, sint *buffer, int size, snd_pcm_uframes_t frames) {
    int rc = 0;
    long loops;
    loops = 1000000 / period;
    printf("loops: %ld size: %d period: %d\n", loops, size, period);

    int len = loops * size;
    sint *buf = malloc(len * 2);
    /* audio_gen_sine8(buf, len); */
    audio_gen_sine16(buf, len);
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
    sint *buffer = NULL;

    handle = audio_get_handle();

    snd_pcm_hw_params_malloc(&params);
    snd_pcm_hw_params_any(handle, params);

    audio_conf(handle, params);

    snd_pcm_hw_params_get_period_size(params, &frames, &dir);
    size = frames * 2 * CHANNELS;
    buffer = (sint *) malloc(size * 2);

    snd_pcm_hw_params_get_period_time(params, &val, &dir);
    snd_pcm_hw_params_free(params);

    audio_play_2(handle, val, buffer, size, frames);

    snd_pcm_drain(handle);
    snd_pcm_close(handle);
    free(buffer);

    return 0;
}
