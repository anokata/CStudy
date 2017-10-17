OBJECTS=audio.c
pcm: 
	cc $@.c $(OBJECTS) -g -lasound -o /tmp/play
alsa1: 
	cc $@.c $(OBJECTS) -g -lasound -o /tmp/$@
alsa2: 
	cc $@.c $(OBJECTS) -g -lasound -o /tmp/$@
#include <alsa/asoundlib.h>
