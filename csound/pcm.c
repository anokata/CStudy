#include <alsa/asoundlib.h>
#include <stdio.h>
#include "audio.h"

#define CHANNELS 1
#define RATE 22050
#define seconds 1

struct audio_config {
    int size;
    unsigned int period;
	snd_pcm_uframes_t frames;
};

struct audio_config snd_setup(snd_pcm_t *pcm_handle) {
	int rate = RATE;
	int buff_size;
    int channels = CHANNELS;
	unsigned int rc;
	unsigned int period;
    int dir = 0;
	snd_pcm_uframes_t frames;
	snd_pcm_hw_params_t *params;
	/* Allocate parameters object and fill it with default values*/
	snd_pcm_hw_params_alloca(&params);
	snd_pcm_hw_params_any(pcm_handle, params);

	/* Set parameters */

	/* Write parameters */
    rc = snd_pcm_hw_params(pcm_handle, params);
    if (rc < 0) {
        fprintf(stderr, "unable to set hw parameters: %s\n",
                snd_strerror(rc));
        exit(1);
    }

    snd_pcm_hw_params_get_period_size(params, &frames, &dir);
	snd_pcm_hw_params_get_period_time(params, &period, NULL);
	buff_size = frames * channels * 2 /* 2 -> sample size */;
    struct audio_config cfg;
    cfg.size = buff_size;
    cfg.period = period;
    cfg.frames = frames;
    return cfg;
}

int main(int argc, char **argv) {
	char *buff;
	unsigned int pcm, dir;
	snd_pcm_uframes_t frames;
	int buff_size, loops;
    int channels = CHANNELS;
    snd_pcm_t *pcm_handle = audio_get_handle();
    struct audio_config config = snd_setup(pcm_handle);
	buff_size = config.size;
    unsigned int period = config.period;
    frames = config.frames;

	buff = (char *) malloc(buff_size);

    /* printf("pcm: %u\nframes %d buf:%d\n params: %p %lx\n", pcm_handle, frames, buff_size, params, params); */

	for (loops = (seconds * 1000000) / period; loops > 0; loops--) {
		if (pcm = read(0, buff, buff_size) == 1) {
			printf("Early end of file.\n");
			return 0;
		}

		if (pcm = snd_pcm_writei(pcm_handle, buff, frames) == -EPIPE) {
			printf("XRUN.\n");
			snd_pcm_prepare(pcm_handle);
		} else if (pcm < 0) {
			printf("ERROR. Can't write to PCM device. %s\n", snd_strerror(pcm));
		}
	}

    audio_close(pcm_handle);
	free(buff);
	return 0;
}
