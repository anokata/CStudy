OBJECTS=audio.c
pcm: 
	cc $@.c $(OBJECTS) -g -lasound -o /tmp/$@ && valgrind /tmp/$@ < /dev/urandom
alsa1: 
	cc $@.c $(OBJECTS) -g -lasound -o /tmp/$@ && valgrind /tmp/$@ 
alsa2: 
	cc $@.c $(OBJECTS) -g -lasound -o /tmp/$@ && valgrind /tmp/$@
alsa3: 
	cc $@.c $(OBJECTS) -g -lm -lasound -o /tmp/$@ && valgrind /tmp/$@
	#cc $@.c $(OBJECTS) -g -lasound -o /tmp/$@ && valgrind /tmp/$@ < /dev/urandom
aplay: 
	cc $@.c $(OBJECTS) -g -lasound -o /tmp/$@ && valgrind /tmp/$@ < /dev/urandom
	#cc $@.c $(OBJECTS) -g -lasound -o /tmp/$@ && valgrind --leak-check=full /tmp/$@ < /dev/urandom
#include <alsa/asoundlib.h>
