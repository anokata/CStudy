#include "audio.h"

snd_pcm_t *audio_get_handle() {
  int rc;
  snd_pcm_t *handle;
  /* Open PCM device for playback. */
  rc = snd_pcm_open(&handle, "default",
                    SND_PCM_STREAM_PLAYBACK, 0);
  if (rc < 0) {
    fprintf(stderr,
            "unable to open pcm device: %s\n",
            snd_strerror(rc));
    exit(1);
  }
  return handle;
}


