/***************************************************************************
                          main.c  -  description
                             -------------------
    begin                : Mon Sep  2 19:30:41 BST 2002
    copyright            : (C) 2002 by Tim-Philipp Müller
    email                : t.i.m@orange.net
    modification         : Thu Nov 28 2013
                           Sun Dec 27 2015 
	                      (C) 2013,2015 by Gerardo Ospina
	                      ngospina@gmail.com
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/* file_hash
 *
 * This program is a fork of a tool for the eDonkey2000 and overnet file
 * sharing programs.
 *
 * It produces the 'file-hash' and 'file-link' of a file or a number of
 * files.
 *
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "global.h"

#if (defined(__linux__) || defined(__MAC_OS_X__) || defined(__FreeBSD__) || (defined(sun) && defined(__svr4__)))
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <dirent.h>
#endif

#ifdef __WIN32
# include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "global.h"

#include "linkedlist.h"
#include "options.h"
#include "ui.h"				/* user interface */
#include "fi.h"				/* file interface */

/* functions */

static int		expand_if_directory (char *fn, SList **p_list);
static int		is_directory (char *fn);
static int		read_directory (char *fn, SList **p_list);


/* main
 *
 */

int
main (int argc, char *argv[])
{
	int		 exit_value = EXIT_FAILURE;

	if (ui_init() && fi_init())
	{
		/* ui_setoptions() returns the argument number that is the first file/directory
		 * specified
		 */
		int argfiles = ui_setoptions(argc,argv);
		if (argfiles)
		{
			SList *files = NULL;
			SList *node = NULL;

			while (argfiles<argc)
			{
				if ((argv[argfiles]))
				{
					int ret = expand_if_directory (argv[argfiles], &files);
					/* if the argument is not a directory, assume it's a file and append it to the list */
					if (ret==0)
					{
						files = slist_append (files, (void*)strdup(argv[argfiles]));
					}
				}
				argfiles++;
			}

			if (ui_run (files))
				exit_value = EXIT_SUCCESS;

			/* free list with filenames */
			node = files;
			while (node)
			{
				if (node->data)
					free(node->data);
				node = (SList*)node->next;
			}

		}
	}

	if (!ui_cleanup() && !fi_cleanup())
		exit_value = EXIT_FAILURE;

	return exit_value;
}


/* expand_if_directory
 *
 * Takes a filename and the adress of a linked list pointer.
 * If the filename is a directory, it will read the directory and
 *   append all the files in it to the linked list.
 * If option_recursive is set, sub-directories will be searched
 *   as well.
 *
 * returns 0 if the filename is not a directory,
 * returns 1 if it is a directory
 * returns -1 in case of error
 *
 */

static int
expand_if_directory (char *fn, SList **p_list)
{
	int ret;

	if ((!fn)||(!p_list))
		return -1;

	ret = is_directory(fn);

	if (ret<0)		/* error - neiher file nor directory */
		return -1;

	if (ret==0)		/* not a directory */
		return 0;

	if (option_excludedirs)
		return 1;

	/* it is a directory => read in entries (unless option_excludedirs is set)*/
	return read_directory (fn, p_list);
}



/* is_directory
 *
 * checks whether the filename specified is a directory or not
 *
 * returns 0 if the filename is not a directory,
 * returns 1 if it is a directory
 * returns -1 in case of error
 *
 */

#if (defined(__linux__) || defined(__MAC_OS_X__) || defined(__FreeBSD__) || (defined(sun) && defined(__svr4__)))

static int
is_directory (char *fn)
{
	struct stat		filestat;

	if (!fn)
		return -1;

	if (stat (fn, &filestat)<0)
	{
		ui_printerr ("stat(%s) failed - %s\n\n", fn, strerror(errno));
		return -1;
	}

	/* is it a directory? */
	if (S_ISDIR(filestat.st_mode))
		return 1;

	/* is it a regular file or a symbolic link? */
	if ((S_ISREG(filestat.st_mode)) || (S_ISLNK(filestat.st_mode)))
		return 0;

	return -1;
}

#elif defined(__WIN32)

static int
is_directory (char *fn)
{
    WIN32_FIND_DATA data;
    HANDLE hFile = FindFirstFile(TEXT(fn), &data);
    if (hFile != INVALID_HANDLE_VALUE)
    {
       int ret = 0;
       if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
       {
          ret = 1;
       }

       CloseHandle(hFile);
       return ret;
    }

    return -1;
}

#else

static int
is_directory (char *fn)
{
	struct stat		filestat;

	if (!fn)
		return -1;

	ui_print ("IMPLEMENT ME FOR THIS OPERATING SYSTEM/PLATFORM: %s\n", __FUNCTION__);
	return 0;
}

#endif


/* read_directory
 *
 * reads in the files from the specified directory and appends them to the list given
 *
 */

#if (defined(__linux__) || defined(__MAC_OS_X__) || defined(__FreeBSD__) || (defined(sun) && defined(__svr4__)))

static int
read_directory (char *fn, SList **p_list)
{
	struct dirent	*entry;
	DIR				*d;

	if ((!fn)||(!p_list))
		return -1;


	d = opendir (fn);
	if (!d)
	{
		ui_printerr ("opendir() failed - %s\n", strerror(errno));
		return -1;
	}

	while ((entry=readdir(d)))
	{
		if ((entry->d_name) && entry->d_name[0]!='.')	/* skip hidden files and dirs */
		{
			int		 pathfnlen = strlen(fn)+1+strlen(entry->d_name)+2;
			char	*pathfn = (char*) malloc(pathfnlen);
			if (pathfn)
			{
				int isdir;
				snprintf (pathfn, pathfnlen, "%s%s%s", fn, (fn[strlen(fn)-1]=='/') ? "" : "/", entry->d_name);
				isdir = is_directory (pathfn);
				if (isdir==0)
				{
					*p_list = slist_append (*p_list, (void*) pathfn);
				} else {
					if ((isdir==1) && (option_recursive))
						expand_if_directory (pathfn, p_list);
					free(pathfn);	/* don't need to save this in the file list => free it */
				}
			}
		}
	}
	closedir(d);

	return 1;
}

#elif defined(__WIN32)

static int
read_directory (char *fn, SList **p_list)
{
    char *pat = (char*)malloc(strlen(fn) + 5);
    strcpy(pat, fn);
    int fullpathlen = strlen(pat);

    if (pat[strlen(pat) - 1] != '\\')
    {
       strcat(pat, "\\");
       ++fullpathlen;
    }
    strcat(pat, "*.*");

    WIN32_FIND_DATA data;
    HANDLE hFile = FindFirstFile(TEXT(pat), &data);
    if (hFile != INVALID_HANDLE_VALUE)
    {
       do
       {
          if ((data.cFileName[0] != '.'))  /* don't even know if it's meaningful under Windows :) */
          {
             char *fullname = (char*)malloc(fullpathlen + strlen(data.cFileName) + 1);
             sprintf(fullname, "%.*s%s", fullpathlen, pat, data.cFileName);

             /* could use is_directory, but it's more efficient this way */
             if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
             {
                *p_list = slist_append (*p_list, (void*)fullname);
             }
             else
             {
                if (option_recursive)
                {
                   expand_if_directory(fullname, p_list);
                }

                free(fullname); /* don't need to save directory name in file list => free it */
             }
          }
       }
       while (FindNextFile(hFile, &data));
       CloseHandle(hFile);
    }

    free(pat);

    /* no error case, as far as I know */
    return 1;
}

#else

static int
read_directory (char *fn, SList **p_list)
{
	ui_print ("IMPLEMENT ME FOR THIS OPERATING SYSTEM/PLATFORM: %s\n", __FUNCTION__);
	return 0;
}

#endif






