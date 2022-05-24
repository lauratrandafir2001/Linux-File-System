#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "LinkedList.h"

#define MAX_INPUT_LINE_SIZE 300
#define MAX_PATH_SIZE 200
#define MAX_COMM_SIZE 10

struct Dir;
struct File;

typedef struct Dir
{
	char *name;
	struct Dir *parent;
	struct File *head_children_files;
	struct Dir *head_children_dirs;
	struct Dir *next;
} Dir;

typedef struct File
{
	char *name;
	struct Dir *parent;
	struct File *next;
} File;

int verify_if_file_exists(Dir *parent, char *name)
{
	if (parent->head_children_files)
	{
		File *curr = parent->head_children_files;
		while (curr->next)
		{
			if (strcmp(curr->name, name) != 0)
			{
				curr = curr->next;
			}
			else
			{
				printf("File already exist\n");
				return 1;
			}
		}
		if (strcmp(curr->name, name) == 0)
		{

			printf("File already exists\n");
			return 1;
		}
	}
	return 0;
}

void touch(Dir *parent, char *name)
{
	DIE(parent == NULL, "malloc failed");
	int var = verify_if_file_exists(parent, name);
	if (var == 1)
	{
		return;
	}
	File *new_file = malloc(sizeof(*new_file));
	DIE(new_file == NULL, "MALLOC FAILED");
	new_file->name = malloc(sizeof(name));
	DIE(new_file->name == NULL, "malloc failed");
	strcpy(new_file->name, name);
	new_file->parent = parent;
	new_file->next = NULL;

	if (parent->head_children_files == NULL)
	{
		parent->head_children_files = new_file;
		parent->head_children_files->next = NULL;
		return;
	}
	if (parent->head_children_files) //if it's not the first one we put it at the end
	{
		File *curr;
		curr = parent->head_children_files;
		while (curr && curr->next)
		{
			curr = curr->next;
		}
		curr->next = new_file;
		return;
	}
}

int verify_if_directory_exists(Dir *parent, char *name)
{ //verify if directory exists
	if (parent->head_children_dirs)
	{
		Dir *current = parent->head_children_dirs;
		while (current->next)
		{
			if (strcmp(current->name, name) != 0)
			{
				current = current->next;
			}
			else
			{
				printf("Directory already exists\n");
				return 1;
			}
		}
		if (strcmp(current->name, name) == 0)
		{
			printf("Directory already exists\n");
			return 1;
		}
	}
	return 0;
}

void mkdir(Dir *parent, char *name)
{
	DIE(parent == NULL, "malloc failed");
	int var = verify_if_directory_exists(parent, name);
	if (var == 1)
	{
		return;
	}
	Dir *new = malloc(sizeof(Dir));
	DIE(new == NULL, "MALLOC FAILED");
	new->name = malloc(sizeof(name));
	DIE(new->name == NULL, "malloc failed");
	strcpy((char *)new->name, name);
	new->next = NULL;
	new->parent = parent;
	new->head_children_dirs = NULL; //it doesn't have childrens_dirs
	new->head_children_files = NULL;

	if (parent->head_children_dirs == NULL)
	{
		parent->head_children_dirs = new; //it's parent has children_dirs
		return;
	}
	if (parent->head_children_dirs)
	{
		Dir *curr = parent->head_children_dirs;
		while (curr && curr->next)
		{
			curr = curr->next;
		}
		curr->next = new;
		return;
	}
}

void ls(Dir *parent)
{
	if (parent->head_children_dirs)
	{
		Dir *curr_dir = parent->head_children_dirs;
		while (curr_dir)
		{
			printf("%s", curr_dir->name);
			curr_dir = curr_dir->next;
		}
	}

	if (parent->head_children_files)
	{
		File *curr = parent->head_children_files;
		while (curr)
		{
			printf("%s", curr->name);
			curr = curr->next;
		}
	}
}

void rm(Dir *parent, char *name)
{
	int cnt = 0;
	if (parent->head_children_files)
	{
		if (strcmp(parent->head_children_files->name, name) == 0)
		{
			cnt++;
			File *temp = parent->head_children_files;
			parent->head_children_files = parent->head_children_files->next;
			free(temp->name);
			free(temp);
			return;
		}
		File *curr = parent->head_children_files;
		File *removed;
		while (curr->next && curr->next->next)
		{
			if (strcmp(curr->next->name, name) == 0)
			{
				cnt++;
				removed = curr->next;
				curr->next = removed->next;
				free(removed->name);
				free(removed);
			}

			curr = curr->next;
		}

		if (curr == NULL && cnt == 0)
		{
			printf("Could not find the file\n");
			return;
		}
		if (curr == NULL)
		{
			return;
		}
		if (strcmp(curr->name, name) == 0)
		{
			cnt++;
			removed = curr;
			curr->next = NULL;
			free(removed->name);
			free(removed);
		}
		if (curr->next == NULL && cnt == 0)
		{
			printf("Could not find the file\n");
			return;
		}
		if (curr->next == NULL)
		{
			return;
		}
		if (strcmp(curr->next->name, name) == 0)
		{
			cnt++;
			removed = curr->next;
			curr->next = NULL;
			free(removed->name);
			free(removed);
		}
		if (cnt == 0)
		{
			printf("Could not find the file\n");
			return;
		}
	}
	else
	{
		printf("Could not find the file\n");
		return;
	}
}

void remove_files(Dir *parent, char *name)
{
	if (parent->head_children_dirs->head_children_files)
	{
		while (parent->head_children_dirs->head_children_files) //erase the files from inside the the directory
		{
			rm(parent->head_children_dirs, parent->head_children_dirs->head_children_files->name);
		}
	}
}

void rmdir(Dir *parent, char *name)
{
	int cnt = 0; // to verify if we found any directories or not
	if (strcmp(parent->name, name) == 0)
	{ //if we are currently in the directory we want to erase
		if (parent->parent)
		{
			parent = parent->parent;
		}
	}
	if (parent->head_children_dirs)
	{
		if (strcmp(parent->head_children_dirs->name, name) == 0)
		{
			remove_files(parent, name);
			if (parent->head_children_dirs->head_children_dirs)
			{
				while (parent->head_children_dirs->head_children_dirs) //erase the directories from inside the directory
				{
					rmdir(parent->head_children_dirs, parent->head_children_dirs->head_children_dirs->name);
				}
			}

			cnt++;
			Dir *temp = parent->head_children_dirs;
			parent->head_children_dirs = parent->head_children_dirs->next;
			free(temp->name);
			free(temp);
			return;
		}
		Dir *curr = parent->head_children_dirs;
		Dir *removed;
		while (curr->next && curr->next->next)
		{
			if (strcmp(curr->next->name, name) == 0) //find the dir we want
			{
				cnt++;
				remove_files(parent, curr->next->name);
				if (parent->head_children_dirs->head_children_dirs)
				{
					while (parent->head_children_dirs->head_children_dirs) //erase the directories from inside the directory
					{
						rmdir(parent->head_children_dirs, parent->head_children_dirs->head_children_dirs->name);
					}
				}
				removed = curr->next;
				curr->next = removed->next;
				free(removed->name);
				free(removed);
			}
			curr = curr->next;
		}
		if (curr == NULL && cnt == 0)
		{
			printf("Could not find the dir\n");
			return;
		}
		if (curr == NULL)
		{
			return;
		}
		if (strcmp(curr->name, name) == 0)
		{
			cnt++;
			remove_files(parent, curr->name);
			if (parent->head_children_dirs->head_children_dirs)
			{
				while (parent->head_children_dirs->head_children_dirs) //erase the directories from inside the directory
				{
					rmdir(parent->head_children_dirs, parent->head_children_dirs->head_children_dirs->name);
				}
			}
			removed = curr;
			curr->next = NULL;
			free(removed->name);
			free(removed);
		}
		if (curr->next == NULL && cnt == 0)
		{
			printf("Could not find the dir\n");
			return;
		}
		if (curr->next == NULL)
		{
			return;
		}
		if (strcmp(curr->next->name, name) == 0)
		{
			cnt++;
			remove_files(parent, name);
			if (parent->head_children_dirs->head_children_dirs)
			{
				while (parent->head_children_dirs->head_children_dirs) //erase the directories from inside the directory
				{
					rmdir(parent->head_children_dirs, parent->head_children_dirs->head_children_dirs->name);
				}
			}
			removed = curr->next;
			curr->next = NULL;
			free(removed->name);
			free(removed);
		}
		if (cnt == 0)
		{
			printf("Could not find the dir\n");
			return;
		}
	}
	else
	{
		printf("Could not find the dir\n");
		return;
	}
}

void cd(Dir **target, char *name)
{
	if (strcmp("..\n", name) == 0) //we go a directory back
	{
		if ((*target)->parent)
		{
			*target = (*target)->parent;
			return;
		}
		return;
	}

	if ((*target)->head_children_dirs)
	{
		Dir *curr = (*target)->head_children_dirs;
		while (curr)
		{
			if (strcmp(curr->name, name) == 0)
			{
				*target = curr; //we change the directory
				return;
			}
			curr = curr->next;
		}
	}
	printf("No directories found!\n");
}

char *pwd(Dir *target)
{
	linked_list_t *list = ll_create(sizeof(char *)); //create linked list
	Dir *temp = target;

	char *string_1 = malloc(sizeof(string_1));
	strcpy(string_1, "");

	while (temp->parent)
	{
		strcpy(string_1, temp->name);
		string_1[strlen(string_1) - 1] = '\0';
		ll_add_nth_node(list, 0, string_1); //add in a list the name of the dir
		temp = temp->parent;
	}
	free(string_1);
	ll_node_t *curr = list->head;
	char *string = malloc(MAX_PATH_SIZE * sizeof(char));
	strcpy(string, "");
	strcat(string, "/home");
	while (list->size)
	{
		ll_node_t *removed = ll_remove_nth_node(list, 0);
		strcat(string, "/");
		strcat(string, (char *)removed->data); //put in a string the names from the list
		free(removed->data);
		free(removed); //remove the list node
	}
	ll_free(&list); //free list
	free(list);
	return string;
}

void tree(Dir *target, int level)
{

	while (target->head_children_dirs)
	{
		for (int i = 0; i < level; i++)
		{
			printf("    ");
		}
		printf("%s", target->head_children_dirs->name);
		level++;
		if (target->head_children_dirs)
		{
			tree(target->head_children_dirs, level);
		}
		if (target->head_children_files)
		{
			for (int i = 0; i < level - 1; i++)
			{
				printf("    ");
			}
			printf("%s", target->head_children_files->name);
		}
		if (target->head_children_dirs->next)
		{
			for (int i = 0; i < level - 1; i++)
			{
				printf("    ");
			}
			printf("%s", target->head_children_dirs->next->name);
		}
		return;
	}
}

void pass_the_olddir_and_files(Dir *parent, char *old_name, char *new_name)
{
	Dir *curr_for_new = parent;
	Dir *curr_for_old = parent;
	while (curr_for_old != NULL)
	{
		if (strcmp(curr_for_old->name, old_name) == 0)
		{
			break;
		}
		curr_for_old = curr_for_old->next;
	}
	while (curr_for_new != NULL)
	{
		if (strcmp(curr_for_new->name, new_name) == 0)
		{
			break;
		}
		curr_for_new = curr_for_new->next;
	}
	curr_for_new->head_children_dirs = curr_for_old->head_children_dirs;
	curr_for_new->head_children_files = curr_for_old->head_children_files;
}
void mv(Dir *parent, char *oldname, char *newname)
{

	strcat(oldname, "\n");
	File *curr = parent->head_children_files;
	Dir *curr_dir = parent->head_children_dirs;
	while (curr)
	{
		if (strcmp(curr->name, newname) == 0)
		{
			printf("File/Director already exists\n");
			return;
		}
		curr = curr->next;
	}
	while (curr_dir)
	{
		if (strcmp(curr_dir->name, newname) == 0)
		{
			printf("File/Director already exists\n");
			return;
		}
		curr_dir = curr_dir->next;
	}
	curr = parent->head_children_files;
	while (curr)
	{
		if (strcmp(curr->name, oldname) == 0)
		{
			rm(parent, curr->name);
			touch(parent, newname);
			return;
		}
		curr = curr->next;
	}
	if (parent->head_children_dirs)
	{
		curr_dir = parent->head_children_dirs;
		while (curr_dir)
		{
			if (strcmp(curr_dir->name, oldname) == 0)
			{
				mkdir(parent, newname);

				if (curr_dir->head_children_dirs || curr_dir->head_children_files)
				{
					pass_the_olddir_and_files(parent->head_children_dirs, oldname, newname); //moves the files/directories inside the directory into another
				}
				rmdir(parent, curr_dir->name);
				return;
			}
			curr_dir = curr_dir->next;
		}
	}
	printf("File/Director not found\n");
}
void free_dirs(Dir *target)
{
	if (target->head_children_dirs)
	{ //we erase the children directories from inside the directories
		if (target->head_children_dirs->head_children_dirs)
		{
			while (target->head_children_dirs->head_children_dirs)
			{
				rmdir(target->head_children_dirs, target->head_children_dirs->head_children_dirs->name);
			}
		}
		Dir *temp_dir = target->head_children_dirs;
		while (temp_dir->next)
		{
			target->head_children_dirs = target->head_children_dirs->next;
			free(temp_dir->name);
			free(temp_dir);
			temp_dir = target->head_children_dirs;
		}
	}
	if (target->head_children_dirs)
	{
		free(target->head_children_dirs->name);
		free(target->head_children_dirs);
	}
}

void free_files(Dir *target)
{
	if (target->head_children_dirs)
	{ //we erase the files inside the directory
		if (target->head_children_dirs->head_children_files)
		{
			while (target->head_children_dirs->head_children_files)
			{
				rm(target->head_children_dirs, target->head_children_dirs->head_children_files->name);
			}
		}
	}
	if (target->head_children_files)
	{

		if (target->head_children_files->next)
		{
			File *temp = target->head_children_files;
			while (temp->next)
			{
				target->head_children_files = target->head_children_files->next;
				free(temp->name);
				free(temp);
				temp = target->head_children_files;
			}
		}
	}

	if (target->head_children_files)
	{
		free(target->head_children_files->name);
		free(target->head_children_files);
	}
}

void stop(Dir *target)
{

	if (target->parent)
	{
		while (target->parent)
		{
			target = target->parent; // go to home directory
		}
	}
	free_files(target); //erase files
	free_dirs(target);	//erase directories
	if (target)
	{
		free(target->name); //free the home directory
		free(target);
	}
}

int main()
{
	char *command_malloc = malloc(MAX_COMM_SIZE * sizeof(char)); //for input
	char *command_line = malloc(MAX_INPUT_LINE_SIZE * sizeof(char));
	char *command;
	char *arg_name = malloc(sizeof(arg_name));
	char *mv_new_name = malloc(sizeof(mv_new_name));
	char *first_name = "home";
	char *string = malloc(sizeof(first_name));
	strcpy(string, first_name);
	Dir *first = malloc(sizeof(*first)); //create home directory
	first->name = malloc(sizeof(string));
	strcpy(first->name, first_name);
	first->head_children_dirs = NULL;
	first->head_children_files = NULL;
	first->next = NULL;
	first->parent = NULL;
	while (1)
	{
		/*
		Summary:
			Reads from stdin a string and breaks it down into command and in
			case it needs into a name.
		*/
		fgets(command_line, MAX_INPUT_LINE_SIZE, stdin);
		command = strtok(command_line, "\n ");
		strcpy(command_malloc, command);
		if (strcmp(command_malloc, "touch") == 0)
		{
			char *file_name = strtok(NULL, " ");
			strcpy(arg_name, file_name);
			touch(first, arg_name);
		}
		if (strcmp(command, "stop") == 0)
		{
			stop(first);
			break;
		}
		if (strcmp(command, "ls") == 0)
		{
			ls(first);
		}
		if (strcmp(command, "mkdir") == 0)
		{
			char *dir_name = strtok(NULL, " ");
			strcpy(arg_name, dir_name);
			mkdir(first, arg_name);
		}
		if (strcmp(command, "rm") == 0)
		{
			char *file_name = strtok(NULL, " ");
			strcpy(arg_name, file_name);
			rm(first, arg_name);
		}
		if (strcmp(command, "rmdir") == 0)
		{
			char *file_name = strtok(NULL, " ");
			strcpy(arg_name, file_name);
			rmdir(first, arg_name);
		}
		if (strcmp(command, "cd") == 0)
		{
			char *file_name = strtok(NULL, " ");
			strcpy(arg_name, file_name);
			cd(&first, arg_name);
		}
		if (strcmp(command, "mv") == 0)
		{
			char *file_name = strtok(NULL, " ");
			strcpy(arg_name, file_name);
			char *idk = strtok(NULL, " ");
			strcpy(mv_new_name, idk);
			mv(first, arg_name, mv_new_name);
		}
		if (strcmp(command, "pwd") == 0)
		{
			char *str = pwd(first);
			printf("%s", str);
			printf("\n");
			free(str);
		}
		if (strcmp(command, "tree") == 0)
		{
			tree(first, 0);
		}
	}
	free(command_malloc);
	free(command_line);
	free(arg_name);
	free(mv_new_name);
	free(string);
	return 0;
}
