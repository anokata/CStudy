#define ALSA_PCM_NEW_HW_PARAMS_API

#include <alsa/asoundlib.h>
#include "audio.h"

int main() {
    long loops;
    int rc;
    int dir;
    char *buffer;

    snd_pcm_t *handle;
    rc = snd_pcm_open(&handle, PCM_DEVICE, SND_PCM_STREAM_PLAYBACK, 0);
    audio_error_check(rc);
    buffer = audio_config(handle, 1000000);

  /* /1* 1 seconds in microseconds divided by * period time *1/ */
  /* loops = 1000000 / au.period; */

  /* while (loops > 0) { */
  /*   loops--; */
  /*   rc = read(0, au.buffer, au.size); */
  /*   if (rc == 0) { */
  /*     fprintf(stderr, "end of file on input\n"); */
  /*     break; */
  /*   } else if (rc != au.size) { */
  /*     fprintf(stderr, */
  /*             "short read: read %d bytes\n", rc); */
  /*   } */
  /*   rc = snd_pcm_writei(au.handle, au.buffer, au.frames); */
  /*   if (rc == -EPIPE) { */
  /*     /1* EPIPE means underrun *1/ */
  /*     fprintf(stderr, "underrun occurred\n"); */
  /*     snd_pcm_prepare(au.handle); */
  /*   } else if (rc < 0) { */
  /*     fprintf(stderr, */
  /*             "error from writei: %s\n", */
  /*             snd_strerror(rc)); */
  /*   }  else if (rc != (int)au.frames) { */
  /*     fprintf(stderr, */
  /*             "short write, write %d frames\n", rc); */
  /*   } */
  /* } */

  /* snd_pcm_drain(au.handle); */
	rc = snd_pcm_close(handle);
    audio_error_check(rc);
    free(buffer);

  return 0;
}
