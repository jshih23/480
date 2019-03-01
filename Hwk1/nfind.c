// Dir.c                                         Stan Eisenstat (10/19/07)
//
// An example program that uses opendir(), readdir(), and closedir()
// to  list the files in the current working directory and their types

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <fnmatch.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>


// Write message to stderr using format FORMAT
#define WARN(format,...) fprintf (stderr, "nfind: " format "\n", __VA_ARGS__)

// Write message to stderr using format FORMAT and exit.
#define DIE(format,...)  WARN(format,__VA_ARGS__), exit (EXIT_FAILURE)

int p_find_dir(char * name){
    DIR *dp;
    struct dirent *entry;
    struct stat buf;
    printf("before opendire name = %s\n", name);
    if ((dp = opendir (name)) == NULL)                // Open the directory
        DIE ("cannot opendir(%s)\n", name);

    while ((entry = readdir (dp))) {                  // Read & print filenames
        // skipping . and ..
        if (strcmp(entry->d_name,".") == 0 || strcmp(entry->d_name, "..") == 0){
            // skip . and ..
        }
        // process a single dir entry 
        else {
            // allocate memory for 
            char *full_path_name = malloc(sizeof(char)*(strlen(name) + strlen(entry->d_name) +1));

            strcpy(full_path_name, name);
            strcat(full_path_name, entry->d_name);
            if (lstat (full_path_name, &buf) < 0)
                DIE ("lstat(%s) failed\n", full_path_name);
            if (S_ISDIR(buf.st_mode))
                printf ("%s/\n", full_path_name);       //  Add / to directory
            else if (S_ISLNK(buf.st_mode))
                printf ("%s@\n", full_path_name);       //  Add @ to symbolic link
            else
                printf ("%s\n", full_path_name);

            free(full_path_name);
        }
    }

    closedir (dp);  // Close the directory
}

int main (int argc, char *argv[])
{   
    // -P and -F  flag setting
    int follow_symbolic_links = 0; // default behavior = don't follow symlinks

    int arg_index = 1;
    while(arg_index < argc){
        if (strcmp("-P",argv[arg_index]) == 0){
            follow_symbolic_links = 0;
            arg_index++;
        }
        else if (strcmp("-L",argv[arg_index]) == 0){
            follow_symbolic_links = 1;
            arg_index++;
        }
        else{
            break;
        }
    }


    // if next doesn't exist or looks like an expression, then default to . as filename
    // char *name;
    // if (arg_index >= argc){
    //     char *name = malloc(sizeof(char)*(1+1+1)); // +1 each for "." "/" and "\0"
    //     strcpy(name, ".");
    //     strcat(name, "/");

    // }
    // else{


    // }
    char *name = NULL;

    if (arg_index >= argc){
        printf("defaulting to dirname = \".\"\n");
        name = malloc(sizeof(char)*3);
        strcpy(name, "./");
    }
    else {
        name = malloc(sizeof(char)*(strlen(argv[arg_index])+1+1)); // +1 each for "/" and "\0"
        strcpy(name, argv[arg_index]);
        strcat(name, "/");
    }
    
    if (follow_symbolic_links == 0){
        printf("name = %s\n", name);
        printf("strlen(name) = %d\n", strlen(name));
        p_find_dir(name);
        free (name);
    }
    // else if (l_flag ==1 ){
    //     printf("running with p_flag = 1");
    //     char name[100];
    //     if (argc == 2){
    //         printf("success %s", argv[1]);
    //         strcpy(name, argv[1]);
    //     }
    //     else{
    //         strcpy(name, ".\0");
    //     }
    //     DIR *dp;
    //     struct dirent *entry;
    //     struct stat buf;

    //     if ((dp = opendir (name)) == NULL)                // Open the directory
    //         DIE ("cannot opendir(%s)\n", name);

    //     while ((entry = readdir (dp))) {                  // Read & print filenames
    //         if (stat (entry->d_name, &buf) < 0)
    //             DIE ("lstat(%s) failed\n", entry->d_name);
    //         if (S_ISDIR(buf.st_mode))
    //             printf ("%s/\n", entry->d_name);          //  Add / to directory
    //         else if (S_ISLNK(buf.st_mode))
    //             printf ("%s@\n", entry->d_name);          //  Add @ to symbolic link
    //         else
    //             printf ("%s\n", entry->d_name);
    //     }

    //     closedir (dp);                                    // Close the directory
    // }


    return EXIT_SUCCESS;
}
