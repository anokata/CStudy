#include "audio.h"

void _error_check(int rc) {
    if (rc < 0) {
        fprintf(stderr, "unable to open pcm device: %s\n", snd_strerror(rc));
        exit(1);
    }
}

snd_pcm_t *audio_get_handle() {
    int rc;
    snd_pcm_t *handle;
    /* Open PCM device for playback. */
    rc = snd_pcm_open(&handle, PCM_DEVICE, SND_PCM_STREAM_PLAYBACK, 0);
    _error_check(rc);
    return handle;
}

void audio_close(snd_pcm_t *pcm_handle) {
    int rc;
	/* rc = snd_pcm_drain(pcm_handle); */
    /* _error_check(rc); */
	rc = snd_pcm_close(pcm_handle);
    _error_check(rc);
}


