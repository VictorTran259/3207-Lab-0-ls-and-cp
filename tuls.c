#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

void recursive_dir(int indent, char * pathname);

int main(int argc, char **argv) {
    char *whatDir;

    //no arguments or too many arguments (more than 1 argument): only working with the current directory.
    if(argc == 1 || argc > 2) {
        whatDir = ".";
        recursive_dir(0, whatDir);
    }
    //1 argument: working with the user specified directory.
    else {
        whatDir = argv[1];
        recursive_dir(0, whatDir);
        recursive_dir(0, ".");
    }

    return 0;
}

void recursive_dir(int indent, char *pathname) {
    //Open the directory
    DIR *d = opendir(pathname);

    //Case: not a valid directory - throw error and exit status 1.
    if(d == NULL) {
        printf("tuls: cannot open directory\n");
        exit(1);
    }

    struct dirent *dir;

    //Read the files in the directory
    while((dir = readdir(d)) != NULL) {
        if(strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0 && strcmp(dir->d_name, ".git") != 0) {
            for(int i = 0; i < indent; i++) {
                printf(" ");
            }

            //case: 1 argument and directory - print the directory with the brackets
            //add the indent, and recursively call the function to print out the contents of the directory
            if(strcmp(pathname, ".") != 0 && dir->d_type == DT_DIR) {
                char newPath[4096] = { 0 };
                strcat(newPath, pathname);
                strcat(newPath, "/");
                strcat(newPath, dir->d_name);

                printf("[[%s]]\n", dir->d_name);

                recursive_dir(indent+5, newPath);
            }
            //Case: no arguments or too many arguments - Recursion is not needed. if it's a directory, print it with the brackets.
            else if(strcmp(pathname, ".") == 0 && dir->d_type == DT_DIR){
                printf("[[%s]]\n", dir->d_name);
            }
            //All cases: not a directory so print it normally
            else {
                printf("%s\n", dir->d_name);
            }
        }
    }

    closedir(d);
}