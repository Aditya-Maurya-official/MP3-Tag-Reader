#include<stdio.h>
#include "main.h"
#include<string.h>
#include "view.h"
#include<stdlib.h>

int read_and_validate(char* argv[], Taginfo *mp3taginfo)// read and validate:
{
    if(argv[2] == NULL)
    {
        printf("INFO: for Viewing the tags --> ./a.out -v <file_name>\n");
        display_help();
        return 0;
    }
    else
    {
        mp3taginfo->fptr_mp3 = fopen(argv[2],"r");// opening the file in read mode to read the data:
        if(mp3taginfo->fptr_mp3 == NULL)
        {
            printf ("ERROR: Unable to Open the %s file.\n", argv [2]);
            printf ("INFO: For Viewing the Tags -> ./a.out -v <file_name.mp3>\n");
            return 0;
        }
        else
        {
            char str[3];
            fread(str,1,3,mp3taginfo->fptr_mp3);// storing the ID3 info in str and compring with ID3:
            if((strncmp(str,"ID3",3))==0)
            {
                printf("\n--------------SELECTED VIEW DETAILS--------------\n");
            }
            else
            {
                printf ("ERROR: The file Signature is not matching with that of a '.mp3' file.\n");
                printf ("INFO: For Viewing the Tags -> ./mp3_tag_reader -v <file_name.mp3>\n");
                return 0;
            }
        }
        fclose(mp3taginfo->fptr_mp3);
    }
    return 1;
}

int view_tag(char *argv[], Taginfo *mp3taginfo)
{
    
    mp3taginfo->fptr_mp3 = fopen(argv[2],"r");// opening the mp3 data in read mode;
    if(mp3taginfo->fptr_mp3 == NULL)
    {
        printf("INFO: For viewing the tags--> ./a.out -e <modifier>\"new_val\" <file.mp3>\n");

    }
    fseek(mp3taginfo->fptr_mp3,10,SEEK_SET);// skiping the 10 bytes header;
    
    int res;
    // function call for the viewing the title;
    res = get_and_display_data ("Title: ", "TIT2", mp3taginfo->frame_Id, &mp3taginfo->title_size, mp3taginfo->title, mp3taginfo->fptr_mp3);
    if(res == 0) 
    {
        printf("ERROR: Title Frame ID Contents cannot be displayed.\n");
        return 0;
    }
    // function call for the viewing the Artist;
    res = get_and_display_data ("Artist: ", "TPE1", mp3taginfo->frame_Id, &mp3taginfo->artist_size, mp3taginfo->artist, mp3taginfo->fptr_mp3);
    if(res == 0)
    {
        printf("ERROR: Artist Frame ID Contents cannot be displayed.\n");
        return 0;
    }
    // function call for the viewing the Album:
    res = get_and_display_data ("Album: ", "TALB", mp3taginfo->frame_Id, &mp3taginfo->album_size, mp3taginfo->album, mp3taginfo->fptr_mp3);
    if(res == 0)
    {
        printf("ERROR: Album Frame ID Contents cannot be displayed.\n");
        return 0;
    }
    // function call for the viewing the year;
    res = get_and_display_data ("Year: ", "TYER", mp3taginfo->frame_Id, &mp3taginfo->year_size, mp3taginfo->year, mp3taginfo->fptr_mp3);    
    if(res == 0)
    {
        printf("ERROR: Year Frame ID Contents cannot be displayed.\n");
        return 0;
    }
    // function call for the viewing the comment;
    res = get_and_display_data ("Content: ", "TCON", mp3taginfo->frame_Id, &mp3taginfo->genre_size, mp3taginfo->genre, mp3taginfo->fptr_mp3);
    if(res == 0)
    {
        printf("ERROR: Gener Frame ID Contents cannot be displayed.\n");
        return 0;
    }
    // function call for the viewing the genre:
    res = get_and_display_data ("Comments: ", "COMM", mp3taginfo->frame_Id, &mp3taginfo->comment_size, mp3taginfo->comment, mp3taginfo->fptr_mp3);
    if(res == 0)
    {
        printf("ERROR: Comments Frame ID Contents cannot be displayed.\n");
        return 0;
    }
    printf("\n--------------------------------------------------\n");
    fclose(mp3taginfo->fptr_mp3);

    return 1;
}
int get_and_display_data(const char* str_frame, const char* str_Id, char* frame_Id, int* tag_size, char* tag, FILE* fptr)
{
    int count;
    // Read the 4-byte Frame ID
    count = fread(frame_Id, 1, 4, fptr); //TIT2   (4 bytes Frame ID)
    if(count < 4){
        printf("ERROR: Unable to read from the MP3 File.\n");
        return 0;
    }
    // Compare frame ID with the expected one
    if(strncmp(frame_Id, str_Id, 4) == 0)
    {
        // Read 4-byte size (big endian) 
        //MP3 stores size in Big Endian Your system uses Little Endian
    
        count = fread(tag_size, 4, 1, fptr);
        if(count < 1)
        {
            printf("ERROR: Unable to read from the MP3 file.\n");
            return 0;
        }
        //printf("The tag_size is %d\n",*tag_size);
        // Convert big endian to little endian
        char *p = (char *)tag_size;
        for(int i = 0; i < sizeof(int)/2; i++)
        {
            char t = p[i];
            p[i] = p[sizeof(int) - i - 1];
            p[sizeof(int) - i - 1] = t;
        }// p[0] ↔ p[3] & p[1] ↔ p[2]

        // Allocate memory for tag data
        tag = (char *)malloc((*tag_size) * sizeof(char));
        if(tag == NULL)
        {
            printf("ERROR: Memory allocation failed.\n");
            return 0;
        }

        // Skip 2-byte flags + 1-byte encoding // skiped 10 + 4 + 4 for header frame size 
        fseek(fptr, 3, SEEK_CUR);// now for flag;

        // Read the frame's text content
        count = fread(tag, 1, (*tag_size - 1), fptr);
        if(count < (*tag_size - 1))
        {
            printf("ERROR: Unable to read from the MP3 file.\n");
            free(tag);
            return 0;
        }

        tag[*tag_size - 1] = '\0'; // NULL-terminate string

        // Display the tag content
        printf("%-10s %s\n", str_frame, tag);
        //printf("%u",*tag_size);
        free(tag);
        tag = NULL;
        return 1;
    }
    else
    {
        printf("ERROR: Frame ID %s not found.\n", str_Id);
        return 0;
    }
}