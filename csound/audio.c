#include "audio.h"

void audio_error_check(int rc) {
    if (rc < 0) {
        fprintf(stderr, "unable to open pcm device: %s\n", snd_strerror(rc));
        exit(1);
    }
}

snd_pcm_t *audio_get_handle() {
    int rc;
    snd_pcm_t *handle;
    rc = snd_pcm_open(&handle, PCM_DEVICE, SND_PCM_STREAM_PLAYBACK, 0);
    audio_error_check(rc);
    return handle;
}

void audio_close(snd_pcm_t *pcm_handle) {
    int rc;
	/* rc = snd_pcm_drain(pcm_handle); */
    /* audio_error_check(rc); */
	rc = snd_pcm_close(pcm_handle);
    audio_error_check(rc);
}

AudioOut audio_make() {
    AudioOut au;
    au.handle = audio_get_handle();
    au.buffer = NULL;
    return au;
}

void audio_free(AudioOut au) {
    audio_close(au.handle);
    if (au.buffer)
        free(au.buffer);
}


char *audio_config(snd_pcm_t *handle, long loops) {
    unsigned int val = 0;
    int dir = 0;
    int size;
    int rc;
    char *buffer = NULL;
    snd_pcm_hw_params_t *params;
    snd_pcm_uframes_t frames;

    snd_pcm_hw_params_malloc(&params);
    snd_pcm_hw_params_any(handle, params);

    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
    /* snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S8); */
    snd_pcm_hw_params_set_channels(handle, params, CHANNELS);

    val = RATE;
    snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);

    /* Set period size to 32 frames. */
    frames = 32;
    snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);

    /* Write the parameters to the driver */
    rc = snd_pcm_hw_params(handle, params);
    audio_error_check(rc);

    snd_pcm_hw_params_get_period_size(params, &frames, &dir);
    size = frames * 4; /* 2 bytes/sample, 2 channels */
    buffer = (char *) malloc(size);
    memset(buffer, 0, size);

    snd_pcm_hw_params_get_period_time(params, &val, &dir);
    /* au.period = val; */
    snd_pcm_hw_params_free(params);
    return buffer;
}

void audio_conf(snd_pcm_t *handle, snd_pcm_hw_params_t *params) {
    unsigned int val = 0;
    int dir = 0;
    snd_pcm_uframes_t frames;

    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(handle, params, CHANNELS);

    val = RATE;
    snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);

    frames = 32;
    snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);

    int rc = 0;
    rc = snd_pcm_hw_params(handle, params);
    audio_error_check(rc);
}
