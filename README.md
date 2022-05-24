# Linux-File-System
Implemented a tree like file system(with files and directories) that the user can manipulate using bash-like commands
#ABOUT
INTRODUCTION IN ASSEMBLY LANGUAGE AND DATA ORGANIZATION
NOVEMBER 2021
<https://ocw.cs.pub.ro/courses/iocla/teme/tema-1>
Student : Trandafir Laura Florina 323CA

#Implementation:
tema1.c:
		-verify_if_file_exists: in this function I check if a file
		 with this name already exists, if it does, it returns 1 and
		 returns to main;

		 -touch: in this function, i call the verify_if_file_exists
		 function, if a file with the specified name doesn't already
		 exists, I create one using the given structure(File), where I
		 store the file's name, parent, and file. I also verify if it's
		 the first one created and set the parent->head_children_files, but
		 if it's not we put it at the end of the list;

		 -mkdir: in this function, I call the verify_if_directory_exists
		 function, if a direCtory with thE specified name doesn't already 
		 exists, I create a new one using the given structure(Dir), where
		 I store the directory's name, parent, head_children_files and
		 head_children_dirs. I also verify if it'sthe first one created
		 and set the parent->head_children_files, but if it's not I put
		 it at the end of the list;

		 -ls: in this function, I print all the files and directories from
		 the current directory;

		 -rm: the function erases the file with the given name from the file
		  list. If the file doesn't exist the function prints "Could not find
		  the file\n", and moves on;

		  -remove_files: we use this function to delete the files
		   from inside of a directory;

		 -rmdir: the function erases the directory with the given name from
		 the directory list and all its files(using the remove_files function)
		 or directories(here we recursively call the rmdir function on the children
		 directories). If the directory doesn't exist the function prints "Could
		 not find the directory\n", and moves on;

		 -cd: I use this function to change the pointer of the current
		  directory to another directory, if the function is called with the ".."
		  the argument, we go a directory back into the hierarchy, on the other hand,
		  if the argument is a name we look through the directories and find the one
		  with the given name, if we don't find a directory the function prints
		  "No directories found!\n";

		 -pwd: I use this function to print the path from the home directory
		  to the current directory. At first, I created a linked list using the ll_create
		  function and then add the name of the current directory to the list. After that, 
		  I changed the directory with its parent, until the directory is home(i step back
		  into the hierarchy). Then, I go through the list putting into a string the names,
		  and then removing the list nodes until there is no list and the string contains
		  the path from home to the given directory, in the chosen format.

		  -tree: I use this function to print the directory files hierarchy, I call it recursively
		  first with the directory who's next on the hierarchy and increase the level. Depending
		  on the level I print " ", for the output to be in the format wanted;

		  -mv: I use this function in order to change the name of a directory or a file if the file/directory
		  the name already exists I print "File/Director already exists\n", and if the file/directory
		  does not exist I print "File/Director not found\n". At first, I look for the directory/file
		  which's a name I want to change and when I find it I erase it and create a new one that I put
		  at the end of the file/directory list and use pass_the_olddir_and_files function to pass to 
		  the new directory the old directory children files or directories;

		  -stop: this function is called when the program stops, at first I make sure to go at the home
		  directory and from there to erase all the files, using the free_files function and all the
		  directories with the free_dirs function.

		  -main: I take the input and call the functions, and free the memory;



		 
