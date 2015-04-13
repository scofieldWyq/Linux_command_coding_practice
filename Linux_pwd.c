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

	ino_t this_inode;

	char this_dir_name[BUFSIZ];

	if( get_inode("..") != current_inode)
	{
		chdir("..");
		inode_to_file_name( this_dir_name, current_inode, BUFSIZ);
		this_inode = get_inode(".");
		path_up_to_father( this_inode );
		printf("/%s", this_dir_name);
	}
}

void inode_to_file_name( char * current_file_name, ino_t current_inode, int str_len)
{
	DIR *dir_ptr;
	struct dirent *direntry_ptr;

	dir_ptr = opendir( ".");

	if( dir_ptr == NULL )
	{
		perror(".");
		exit(1);
	}

	while( ( direntry_ptr = readdir(dir_ptr)) != NULL )
	{
		if( get_inode(direntry_ptr->d_name) == current_inode )
		{
			strncpy( current_file_name, direntry_ptr->d_name, str_len);
			closedir(dir_ptr);
			return ;
		}
	}
}

ino_t get_inode( char * fname)
{
	struct stat f_info;

	if (stat( fname, &f_info) == -1 )
	{
		fprintf(stderr, "Cannot stat!\n");
		perror(fname);
		exit(1);
	}

	return f_info.st_ino;
}




