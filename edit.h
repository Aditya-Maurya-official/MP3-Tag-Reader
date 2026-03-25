#ifndef EDIT_H
#define EDIT_H

typedef struct edit
{
    FILE* fptr;
    char frame_id [5];
    char new_frame_id [100];
    int frame_id_size;
}tagdata;

int read_validate(char *argv[],tagdata* mp3tagdata);

int edit_tag(char*,char*mp3);

#endif 