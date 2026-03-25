#ifndef VIEW_H
#define VIEW_H

typedef struct header
{
    FILE* fptr_mp3;
    char frame_Id[4]; /**< Version of the ID3 tag */

    char *title;   /**< Title of the song */
    int title_size;
    char *artist;  /**< Artist of the song */
    int artist_size;
    char *album;   /**< Album name */
    int album_size;
    char *year;    /**< Year of release */
    int year_size;
    char *comment; /**< Comment */
    int comment_size;
    char *genre;   /**< Genre */
    int genre_size;
} Taginfo;

int read_and_validate(char*[], Taginfo* mp3tagInfo);

int view_tag(char*[], Taginfo* mp3tagInfo);

int get_and_display_data (const char* str_frame, const char* str_Id, char *frame_Id, int* tag_size, char* tag, FILE* fptr);

#endif