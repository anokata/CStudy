CFLAGS=-Wall -Wextra -std=gnu11 -g -D_GNU_SOURCE=1
CC=gcc
GLIB=$(shell pkg-config --cflags --libs glib-2.0)

glog:
	$(CC) $@.c $(GLIB) -o /tmp/$@ && valgrind /tmp/$@

build: 
	gcc solution.c -o solution
sdltest: 
	gcc sdltest.c -lSDL -lSDL_ttf -lSDL_gfx `pkg-config --cflags --libs glib-2.0` -o sdltest && ./sdltest
bf: 
	gcc bf.c -lcurses && ./a.out
sdltestdebug: 
	gcc sdltest.c -g -lSDL -lSDL_ttf -lSDL_gfx `pkg-config --cflags --libs glib-2.0` -o sdltest && ./sdltest

hashmap:
	gcc hashmap.c dlist.c && ./a.out

dlist:
	gcc dlist.c && ./a.out

dlistdebug:
	gcc -DDEBUG dlist.c && ./a.out
curwin: 
	gcc curwin.c -lncurses && ./a.out
