#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "edit.h"
char tag[5];// for storing the modifier tag;
void endianess(int *ptr, int l) // endian coverting big to little
{
    char *str = (char*)ptr;
    for (int i = 0; i < l / 2; i++) 
    {
        char t = str[i];
        str[i] = str[l - i - 1];
        str[l - i - 1] = t;
    }
}

int read_validate(char *argv[], tagdata *mp3tagdata) // used to check and validate the mp3 
{
    if (argv[4] == NULL) 
    {
        printf("ERROR: File.mp3 name is mandatory\n");
        return 0;
    }

    mp3tagdata->fptr = fopen(argv[4], "r");// opening the mp3 file in read mode;
    if (mp3tagdata->fptr == NULL) 
    {
        printf("ERROR: Opening file %s\n", argv[4]);
        return 0;
    }

    char str[3];// taking the 3 bytes for ID3 signature checking:
    fread(str, 1, 3, mp3tagdata->fptr);// reading the mp3 file and stroing in into str
    if (strncmp(str, "ID3", 3) == 0) // ckecking the ID3
    {
        printf("\tversion ID V2.3\n");
    } 
    else 
    {
        printf("ERROR: Not a valid ID3v2 tag\n");
        fclose(mp3tagdata->fptr);// closing the file;
        return 0;
    }
    fclose(mp3tagdata->fptr);


    if (argv[3] == NULL) 
    {
        printf("ERROR: No value provided for frame ID %s\n", mp3tagdata->frame_id);
        return 0;
    } 
    else 
    {
        mp3tagdata->frame_id_size = strlen(argv[3]) + 1;// adding the null charater
        if (mp3tagdata->frame_id_size < 100) 
        {
            strncpy(mp3tagdata->new_frame_id, argv[3], mp3tagdata->frame_id_size);// copying the new frame value
        } 
        else 
        {
            printf("ERROR: Input too long\n");
            return 0;
        }
    }
    
    if (argv[2] == NULL) 
    {
        printf("\nModifier\tFunction\n");
        printf ("-t\t\tModify Title Tag\n-A\t\tModify Artist Tag\n-a\t\tModify Album Tag\n-y\t\tModify Year Tag\n-cn\t\tModify Content Type Tag\n-cm\t\tModify Comments Tag\n");
        return 0;
        // checking the arguments passing through the CLA: if matches storing in tag 
    } 
    else 
    {
        if (strcasecmp(argv[2], "-t") == 0)
        {
            strcpy(tag,"TIT2");
            return 1;
        }
        else if (strcasecmp(argv[2], "-A") == 0)
        {
            strcpy(tag,"TPE1");
            return 1;
        }  
        else if (strcasecmp(argv[2], "-a") == 0)
        {
            strcpy(tag,"TALB");
            return 1;
        }
        else if (strcasecmp(argv[2], "-y") == 0)
        {
            strcpy(tag,"TYER");
            return 1;
        }   
        else if (strcasecmp(argv[2], "-cm") == 0)
        {
            strcpy(tag,"COMM");
            return 1;
        }
        else if (strcasecmp(argv[2], "-cn") == 0)
        {
            strcpy(tag,"TCON");
            return 1;
        } 
        else 
        {
            printf("ERROR: Unsupported Modifier.\n");
            return 0;
        }
    }
}
// this function is used to edit the mp2 tag data and storing in new mp3 file:
//Afther storing in new mp3 file removing it and renameing into original file; 
int edit_tag(char *new_file, char *mp3) 
{
    FILE *orig = fopen(mp3, "rb");// opening the file in read mode in rb means binary file
    FILE *temp = fopen("temp.mp3", "wb");// opening the file in write mode in rb means binary file

    if (!orig || !temp) 
    {
        printf("ERROR: Could not open files\n");
        fclose(orig);
        fclose(temp);
        return 0;
    }

    // Copy ID3v2 header
    char header[10];
    fread(header, 1, 10, orig);
    fwrite(header, 1, 10, temp);

    while (1) 
    {
        char frame_id[5] = {0};
        if (fread(frame_id, 1, 4, orig) != 4 || frame_id[0] == 0)
        break;

        int size;// size of the frame
        fread(&size, 1, 4, orig);
        endianess(&size, sizeof(int));  // Convert  endieness big to little and vice versa

        char flags[2];// for flag data
        fread(flags, 1, 2, orig);

        char *data = malloc(size);// allocating the memory for the  new data to update or edit:
        if (!data) 
        {
            printf("ERROR: Memory Allocation\n");
            fclose(orig); fclose(temp);
            return 0;
        }
        fread(data, 1, size, orig);

        if (strcmp(frame_id, tag) == 0) 
        {
            // Write new frame
            int new_size = strlen(new_file) + 1;
            int be_size = new_size;
            endianess(&be_size, sizeof(int));

            fwrite(frame_id, 1, 4, temp);
            fwrite(&be_size, 1, 4, temp);
            fwrite(flags, 1, 2, temp);
            fputc(0x00, temp);  // Encoding byte for ISO-8859-1
            fwrite(new_file, 1, new_size - 1, temp);
        } 
        else 
        {
            // Write original frame
            int be_orig_size = size;
            endianess(&be_orig_size, sizeof(int));

            fwrite(frame_id, 1, 4, temp);
            fwrite(&be_orig_size, 1, 4, temp);
            fwrite(flags, 1, 2, temp);
            fwrite(data, 1, size, temp);
        }

        free(data);
    }
    printf("\n----------EDITING TAG DATA ----------\n");
    // Copy rest of the file (audio data)
    char ch;
    while ((ch = fgetc(orig)) != EOF)
        fputc(ch, temp);

    fclose(orig);
    fclose(temp);
    remove(mp3);
    rename("temp.mp3", mp3);
    printf("\n\t  Edited the %s\n",tag);
    return 1;
}