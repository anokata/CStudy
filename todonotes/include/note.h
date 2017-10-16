#pragma once

#include <time.h>
#include <stdlib.h>
#include <string.h>

typedef char *string;
typedef struct Note {
    string text;
    string title;
    time_t date;
} *Note;

void note_free(Note note);
Note note_new(string title, string text);
char *note_show(Note note);
char *note_dump(Note note);
Note note_load(char *dump);
void _print_note(Note note);
