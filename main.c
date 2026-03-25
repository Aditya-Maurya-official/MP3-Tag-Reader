/*              MP3 TAG AND EDITOR PROJECT
Name : Aditya Maurya
Description : The MP3 Tag Reader project 
Sample input: ./a.out -v sample.mp3
sample output: Title:    sunny
                Artist:   honey singer
                Album:    Yaariyan
                Year:     2000
                Content:  Bollywood Music
                Comments: english_hindi
*/
#include<stdio.h>
#include "main.h"
#include<string.h>
#include "view.h"
#include "edit.h"

void display_help()// function to display the help memu
{
    printf("Help memu for MP3 Tag Reader and Editor:\n");
    printf("\nFor Viewing the tags-\t./a.out -v <.mp3 file>\n");
    printf ("\nFor Editing the Tags -> ./a.out -e <modifier> \"New_Name\" <file_name.mp3>\n");
    printf("\nModifier\tFunction\n");
    printf ("-t\t\tModify Title Tag\n-A\t\tModify Artist Tag\n-a\t\tModify Album Tag\n-y\t\tModify Year Tag\n-cn\t\tModify Content Type Tag\n-cm\t\tModify Comments Tag\n");
}

int main(int argc,char*argv[])// passing the command line arguments
{
    Taginfo mp3taginfo;
    tagdata mp3tagdata;

    if(argc < 2)//checking the count of arguments is lessthan 2 invalid:
    {
        printf ("ERROR: Incorrect format of Command Line Arguments.\n");
        printf("Usage: ./a.out -h for help Menu\n");
        
    }
    else
    {
        if(strcmp(argv[1],"-v") == 0)// comapring if v means view:
        {
            int res = read_and_validate(argv,&mp3taginfo);
            if(res == 1){
                printf("\n--------------------------------------------------\n");
                printf ("\tMP3 TAG READER AND EDITOR FOR ID3V2\n");
                printf("--------------------------------------------------\n");
                int res_2 = view_tag(argv,&mp3taginfo);
                if(res_2 == 1)
                {
                    printf("\n-------------DETAILS DISPLAYED SUCCESSFULLY--------\n");
                }
            }

        }
        else if(strcmp(argv[1],"-h")==0)// if -h means need to display the help memu;
        {
            display_help();
        }
        else if(strcmp(argv[1],"-e")==0)// if -e means edit the details based on the argument;
        {
            int r = read_validate(argv,&mp3tagdata);// validate the give sample mp3 file;
            if(r == 1)
            {
                char* file = argv[3];
                char* mp3 = argv[4];
               edit_tag(file,mp3);
               printf("\n---------------------------------------------\n");
            }
        }
    }
    
}