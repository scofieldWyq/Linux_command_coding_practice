/* Linux_pwd.c
 * 	this pogram like Linux command "pwd"- print the path of current path.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>


ino_t get_inode( char *);			/* get thhe inode number */
void path_up_to_father( ino_t);			/* path up to father until '/' */
void inode_to_file_name( char *, ino_t, int);	/* convert inode to name string */

int main()
{
	path_up_to_father( get_inode("."));
	printf("\n");
	return 0;
}

void path_up_to_father( ino_t current_inode)
{

	ino_t this_inode;			/* this is used for next directory */

	char this_dir_name[BUFSIZ];		/* set a string to save cuurrent directory name */

	if( get_inode("..") != current_inode)	/* if this direcory inode is the same as father inode, it is get to the root directory */
	{
		chdir("..");			/* cause you can't know the directory name by itself, you should change directory to father, and compare with the inode to find the directory name */

		inode_to_file_name( this_dir_name, current_inode, BUFSIZ);
						/* get directory by inode */
		this_inode = get_inode(".");
		path_up_to_father( this_inode ); /* resursively */

		printf("/%s", this_dir_name);	/* prints directory name */
	}
}

void inode_to_file_name( char * current_file_name, ino_t current_inode, int str_len)
/*
 * change inode into curect file name.
 */
{
	DIR *dir_ptr;
	struct dirent *direntry_ptr;

	dir_ptr = opendir( ".");	/* open dir */

	if( dir_ptr == NULL )
	{
		perror(".");
		exit(1);
	}

	while( ( direntry_ptr = readdir(dir_ptr)) != NULL ) /* read directory until got the curect inode or not */
	{
		if( get_inode(direntry_ptr->d_name) == current_inode )
		{
			strncpy( current_file_name, direntry_ptr->d_name, str_len); /* copy directory name to current file name */
			closedir(dir_ptr); /* close dir */
			return ;
		}
	}
}

ino_t get_inode( char * fname)
/*
 * return inode number
 */

{
	struct stat f_info;	/* file property structure */

	if (stat( fname, &f_info) == -1 )
	/* return -1 means that failed. */
	{
		fprintf(stderr, "Cannot stat!\n");
		perror(fname);
		exit(1);
	}

	return f_info.st_ino;	/* return inode */
}




