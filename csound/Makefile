OBJECTS=audio.c
pcm: 
	cc $@.c $(OBJECTS) -g -lasound -o /tmp/$@ && valgrind /tmp/$@ < /dev/urandom
alsa1: 
	cc $@.c $(OBJECTS) -g -lasound -o /tmp/$@ && valgrind /tmp/$@ 
alsa2: 
	cc $@.c $(OBJECTS) -g -lasound -o /tmp/$@ && valgrind /tmp/$@
#include <alsa/asoundlib.h>
