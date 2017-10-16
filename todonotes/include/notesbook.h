#pragma once

#include <glib.h>
#include <stdio.h>
#include "note.h"

static const char *notebooks_path = "./nb/"; // ~/.notebooks/

typedef struct NotesBook {
    GList *notes;
    char *name;
} *NotesBook;

NotesBook nbook_new(char *name);
void nbook_free(NotesBook book);

void nbook_add_note(NotesBook book, Note note);
void nbook_print(NotesBook book);
char *nbook_dump(NotesBook book);
void nbook_save(NotesBook book);
NotesBook nbook_load(const char *filename);
void nbook_list();

void nbook_file_new(char *name);
void nbook_file_rm(char *bookname);
char *nbook_path(const char *bookname);
guint nbook_len(NotesBook book);
void nbook_note_remove(NotesBook book, int note_index);
