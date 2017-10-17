#define ALSA_PCM_NEW_HW_PARAMS_API

/* All of the ALSA library API is defined
 * in this header */
#include <alsa/asoundlib.h>
#include "audio.h"

snd_pcm_hw_params_t *setup(snd_pcm_t *handle) {
    int rc;
    snd_pcm_hw_params_t *params;

    snd_pcm_hw_params_alloca(&params);
    snd_pcm_hw_params_any(handle, params);

    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);

    rc = snd_pcm_hw_params(handle, params);
    if (rc < 0) {
    fprintf(stderr,
            "unable to set hw parameters: %s\n",
            snd_strerror(rc));
    exit(1);
    }
    return params;
}

void getup(snd_pcm_t *handle, snd_pcm_hw_params_t *params) {
    int rc;

    snd_pcm_hw_params_alloca(&params);
    snd_pcm_hw_params_any(handle, params);

    snd_pcm_format_t fval = 0;
    snd_pcm_hw_params_get_format(params, &fval);
    printf("format = '%s' (%s)\n\n",
      snd_pcm_format_name(fval),
      snd_pcm_format_description(fval));
}

void test() {
    int rc;
    snd_pcm_t *handle = audio_get_handle();
    snd_pcm_hw_params_t *params;

    params = setup(handle);
    getup(handle, params);

    audio_close(handle);
}

int main() {
    test();
  int rc;
  snd_pcm_t *handle;
  snd_pcm_hw_params_t *params;
  unsigned int val, val2;
  int dir = 0;
  snd_pcm_uframes_t frames;

  handle = audio_get_handle();

  /* Allocate a hardware parameters object. */
  snd_pcm_hw_params_alloca(&params);

  /* Fill it in with default values. */
  snd_pcm_hw_params_any(handle, params);

  /* Set the desired hardware parameters. */

  /* Interleaved mode */
  snd_pcm_hw_params_set_access(handle, params,
                      SND_PCM_ACCESS_RW_INTERLEAVED);

  /* Signed 16-bit little-endian format */
  snd_pcm_hw_params_set_format(handle, params,
                              SND_PCM_FORMAT_S16_LE);

  /* Two channels (stereo) */
  snd_pcm_hw_params_set_channels(handle, params, 2);

  /* 44100 bits/second sampling rate (CD quality) */
  val = 44100;
  snd_pcm_hw_params_set_rate_near(handle,
                                 params, &val, &dir);

  /* Write the parameters to the driver */
  rc = snd_pcm_hw_params(handle, params);
  if (rc < 0) {
    fprintf(stderr,
            "unable to set hw parameters: %s\n",
            snd_strerror(rc));
    exit(1);
  }

  /* Display information about the PCM interface */

  printf("PCM handle name = '%s'\n",
         snd_pcm_name(handle));

  printf("PCM state = %s\n",
         snd_pcm_state_name(snd_pcm_state(handle)));

  snd_pcm_hw_params_get_access(params,
                          (snd_pcm_access_t *) &val);
  printf("access type = %s\n",
         snd_pcm_access_name((snd_pcm_access_t)val));

  snd_pcm_format_t fval;
  snd_pcm_hw_params_get_format(params, &fval);
  printf("format = '%s' (%s)\n",
    snd_pcm_format_name(fval),
    snd_pcm_format_description(fval));

  snd_pcm_hw_params_get_subformat(params,
                        (snd_pcm_subformat_t *)&val);
  printf("subformat = '%s' (%s)\n",
    snd_pcm_subformat_name((snd_pcm_subformat_t)val),
    snd_pcm_subformat_description(
                          (snd_pcm_subformat_t)val));

  snd_pcm_hw_params_get_channels(params, &val);
  printf("channels = %d\n", val);

  snd_pcm_hw_params_get_rate(params, &val, &dir);
  printf("rate = %d bps\n", val);

  snd_pcm_hw_params_get_period_time(params,
                                    &val, &dir);
  printf("period time = %d us\n", val);

  snd_pcm_hw_params_get_period_size(params,
                                    &frames, &dir);
  printf("period size = %d frames\n", (int)frames);

  snd_pcm_hw_params_get_buffer_time(params,
                                    &val, &dir);
  printf("buffer time = %d us\n", val);

  snd_pcm_hw_params_get_buffer_size(params,
                         (snd_pcm_uframes_t *) &val);
  printf("buffer size = %d frames\n", val);

  snd_pcm_hw_params_get_periods(params, &val, &dir);
  printf("periods per buffer = %d frames\n", val);

  snd_pcm_hw_params_get_rate_numden(params,
                                    &val, &val2);
  printf("exact rate = %d/%d bps\n", val, val2);

  val = snd_pcm_hw_params_get_sbits(params);
  printf("significant bits = %d\n", val);

  val = snd_pcm_hw_params_is_batch(params);
  printf("is batch = %d\n", val);

  val = snd_pcm_hw_params_is_block_transfer(params);
  printf("is block transfer = %d\n", val);

  val = snd_pcm_hw_params_is_double(params);
  printf("is double = %d\n", val);

  val = snd_pcm_hw_params_is_half_duplex(params);
  printf("is half duplex = %d\n", val);

  val = snd_pcm_hw_params_is_joint_duplex(params);
  printf("is joint duplex = %d\n", val);

  val = snd_pcm_hw_params_can_overrange(params);
  printf("can overrange = %d\n", val);

  val = snd_pcm_hw_params_can_mmap_sample_resolution(params);
  printf("can mmap = %d\n", val);

  val = snd_pcm_hw_params_can_pause(params);
  printf("can pause = %d\n", val);

  val = snd_pcm_hw_params_can_resume(params);
  printf("can resume = %d\n", val);

  val = snd_pcm_hw_params_can_sync_start(params);
  printf("can sync start = %d\n", val);

  snd_pcm_close(handle);

  return 0;
}

