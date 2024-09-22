# CIS 3207 Lab 0 - ls and cp

## tuls

### SYNOPSIS
	void recursive_dir(int indent, char *pathname);

### DESCRIPTION
	The tuls command lists out all the files and directories in the current directory
	or in a specified directory and other directories located within that directory using
	recursion.

### RETURN VALUE
	The tuls command returns a list of directories and files

### ERRORS
	tuls: cannot open directory - The directory is invalid/doesn't exist

## tucp

### SYNOPSIS
	void fileToFile(char *sourceFile, char *destinationFile);
	void fileToDirectory(char *sourceFile, char *destinationDirectory);
	void multipleFilesToDirectory(int sourceFilesIndex, char **sourceFiles, char *destinationDirectory);

### DESCRIPTION
	Copies a source file into a destination file, copies a source file into a directory, or copies multiple source
	files into a directory. tucp overwrites the content of an existing file with the content of the source file without warning
	or it creates a new file with the contents of the source file if the file doesn't exist.

### RETURN VALUE
	The tucp command returns 0 if the source file was successfully copied. It returns 1 if an error occured.

### ERRORS
	tucp: not enough arguments - less than 2 arguments were entered
	tucp: source file does not exist - the source file that was inputted could not be found
	tucp: destination file does not exist - the destination file that was inputted could not be found
	tucp: failed to open source file - the source file was unable to be opened.
	tucp: source file is not a file - the source file that was inputted to be copied was not a file.
	tucp: destination is not a directory or file - The destination that was inputted was not a directory or file.
