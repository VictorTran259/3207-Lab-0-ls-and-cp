#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

void fileToFile(char *sourceFile, char *destinationFile);
void fileToDirectory(char *sourceFile, char *destinationDirectory);
void multipleFilesToDirectory(int sourceFilesIndex, char **sourceFiles, char *destinationDirectory);

int main(int argc, char **argv) {
    //Check if there are enough arguments. Throw an error if not.
    if(argc < 3) {
        printf("tucp: not enough arguments\n");
        exit(1);
    }

    //Case: 1 source file to a destination.
    if(argc == 3) {
        struct stat sb1, sb2;

        char *sourceFile = argv[1];
        char *destination = argv[2];

        //Check if the source file exists. If it doesn't, throw an error.
        int doesSourceFileExist = stat(sourceFile, &sb1);
        if(doesSourceFileExist != 0) {
            printf("tucp: source file does not exist\n");
            exit(1);
        }

        //Check if the source file is a file. If it isn't, throw an error.
        if(!(S_ISREG(sb1.st_mode))) {
            printf("tucp: source file is not a file\n");
            exit(1);
        }

        //Check if the destination is a directory or file if it exists. If it isn't either, throw an error.
        int doesDestinationExist = stat(destination, &sb2);
        if(!(S_ISREG(sb2.st_mode)) && !(S_ISDIR(sb2.st_mode)) && doesDestinationExist == 0) {
            printf("tucp: destination is not a directory or file\n");
            exit(1);
        }
        
        //Check if the destination is a directory. If it is, copy the source file into the directory.
        if(S_ISDIR(sb2.st_mode)) {
            fileToDirectory(sourceFile, destination);
        }
        //If the destination isn't a directory, it's assumed that it's file to file.
        else {
            fileToFile(sourceFile, destination);
        }
    }

    //Case: multiple source files to a destination.
    if(argc > 3) {
        char *sourceFile;
        char *destination = argv[argc-1];

        struct stat sb1, sb2;

        int doesDestinationExist = stat(destination, &sb2);

        //Check if the destination exists. If it doesn't, throw an error.
        if(doesDestinationExist != 0) {
            printf("tucp: destination does not exist\n");
            exit(1);
        }

        //Check if the destination is a directory. If it isn't, throw an error.
        if(!(S_ISDIR(sb2.st_mode))) {
            printf("tucp: destination is not a directory\n");
            exit(1);
        }

        //Check if all the source files exist and are files.
        //Throws an error if a source file doesn't exist or isn't a file.
        for(int i = 1; i < argc - 1; i++) {
            sourceFile = argv[i];
            int doesThisSourceFileExist = stat(sourceFile, &sb1);

            if(doesThisSourceFileExist != 0) {
                printf("tucp: source file %s does not exist\n", sourceFile);
                exit(1);
            }

            if(!(S_ISREG(sb1.st_mode))) {
                printf("tucp: source file %s is not a file\n", sourceFile);
                exit(1);
            }
        }
        
        int sourceFilesIndex = argc - 1;
        char **sourceFiles = argv;

        //Copy the source files to the directory.
        multipleFilesToDirectory(sourceFilesIndex, sourceFiles, destination);
    }

    return(0);
}

//***FOR ALL CASES***
//Case 1: Overwrites the content of an existing file with the content
//of the source file. No warning is given.
//Case 2: Creates a new file with the contents of the source file if the
//file doesn't exist in the directory.

//Copy a source file into a destination file.
void fileToFile(char *sourceFile, char *destinationFile) {
    //Open the source file in read only mode. Throws an error if the source file isn't able to be opened. 
    int source = open(sourceFile, O_RDONLY);
    if(source == -1) {
        printf("tucp: failed to open source file\n");
        exit(1);
    }

    //Open the destination file in write only mode.
    //Creates the destination file if it doesn't already exist.
    int destination = open(destinationFile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    
    //Copy the contents of the source file into the destination file.
    //Overwrites the destination file with the source file if it already exists.
    char buf;
    int bytes;
    while((bytes = read(source, &buf, sizeof(buf)))) {
        write(destination, &buf, sizeof(buf));
    }

    //Close the files.
    close(source);
    close(destination);
}

//Copy a source file into a directory.
void fileToDirectory(char *sourceFile, char *destinationDirectory) {
    //Build a new path in order to copy the source file into the destination directory.
    char newPath[4096] = { 0 };

    strcat(newPath, destinationDirectory);
    strcat(newPath, "/");
    
    //Case: A path is entered as the source file. retrieve the name of the file at
    //the end of the path and use strcat to add it to the newPath.
    if(*sourceFile == '/') {
        char isItAFile[4096] = { 0 };

        char buf[] = "/";
        char *fileName = strtok(sourceFile, buf);

        strcat(isItAFile, "/");
        strcat(isItAFile, fileName);

        struct stat sb;
        stat(isItAFile, &sb);

        //Check if the file name has been reached.
        while(!(S_ISREG(sb.st_mode)) && fileName != NULL) {
            fileName = strtok(NULL, buf);
            strcat(isItAFile, "/");
            strcat(isItAFile, fileName);
            stat(isItAFile, &sb);
        }

        strcat(newPath, fileName);
        sourceFile = isItAFile;
    }
    //Case: A file in the directory is entered as the source file. Just use strcat to
    //add it to the new path.
    else {
        strcat(newPath, sourceFile);
    }

    //Using the newly built path, copy the source file into the
    //destination directory using the fileToFile function.
    fileToFile(sourceFile, newPath);
}

//Copy multiple source files into a directory.
void multipleFilesToDirectory(int sourceFilesIndex, char **sourceFiles, char *destinationDirectory) {
    //Uses the fileToDirectory function to copy each individual source file into the destination directory.
    for(int i = 1; i < sourceFilesIndex; i++) {
        fileToDirectory(sourceFiles[i], destinationDirectory);
    }
}