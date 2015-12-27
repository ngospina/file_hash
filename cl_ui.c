/***************************************************************************
                          cl_ui.c  -  description
                             -------------------
    begin                : Mon Sep 2 2002
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

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "global.h"

#include "cl_ui.h"
#include "options.h"
#include "processfile.h"

/* ui_init
 *
 */

int
ui_init (void)
{
	return 1;	/* nothing to do here for the command line client version */
}


/* ui_cleanup
 *
 */

int
ui_cleanup (void)
{
	if (option_waitwhenfinito)
	{
		fprintf (stderr, "<press enter>\n");
		(void) fgetc(stdin);
	}

	return 1;	/* nothing else to do here for the command line client version */
}


/* ui_print
 *
 * displays a normal message to the user, takes arguments like printf()
 *
 */

int
ui_print (const char *format, ...)
{
	int		ret = 0;
	va_list args;

	if (!option_quiet)
	{
		va_start(args,format);
		ret = vprintf(format,args);
		va_end(args);
	}
	return ret;
}


/* ui_printerr
 *
 * displays an error message to the user, takes arguments like printf()
 *
 */

int
ui_printerr (const char *format, ...)
{
	va_list  args;
	char	*errformat;
	int		 ret;

	if (option_silent)
		return 0;

	if (!format)
		return -1;

#ifdef __WIN32
        /* 14/02/2003 JL: alloca doesn't seem to be this secure on Win2k :) */
        errformat = (char*)malloc(strlen(PRINTERR_PREFIX)+strlen(format)+2);
#else
	errformat = alloca(strlen(PRINTERR_PREFIX)+strlen(format)+2);
#endif
	if (!errformat)
		return -1;

	snprintf (errformat, strlen(PRINTERR_PREFIX)+strlen(format)+1, "%s%s",
				PRINTERR_PREFIX, format);

	va_start(args,format);
	ret = vfprintf(stderr,errformat,args);
	va_end(args);

#ifdef __WIN32
        free(errformat);
#endif

	return ret;
}


/* ui_setoptions
 *
 * parses the options given on the command line
 *
 * returns 0 on failure
 * returns the first argument number that is presumably a filename or directory name
 *
 */

int
ui_setoptions (int argc, char *argv[])
{
	int i=0;
	if ((argc>1) && (argv))
	{
		i = 1;	/* the first argument is by convention the program name - skip that */
		while ((i<argc) && (argv[i]) && (*(argv[i])=='-'))
		{
			options_process_cl_argument (argv, &i);
			i++;
		}
	} else options_print_help_screen_and_exit();

	return i;
}


/* ui_run
 *
 * is called with a list of files passed via the command line and processes each
 * of those files. We do it this way so the ui stuff can be callback mechanisms as
 * used for GUI stuff as well (so files could be selected via GUI instead of CL)
 *
 * returns 0 on error, otherwise 1
 *
 */

int
ui_run (SList *filelist)
{
	SList *node = filelist;

	/* no files specified on the command line? show help and exit */
	if (!filelist)
	{
		ui_printerr ("You haven't specified any file(s) or directory to hash, have you?\n");
		options_print_help_screen_and_exit ();
	}

	while (node)
	{
		if (node->data)
		{
			fileinfo	info;

			if ((option_debug)||(option_verbose))
				ui_print ("hashing file          : %s\n", (char*)node->data);

			/* note: if process_file() returns 0, that might mean that an error
				has occured (e.g. filesize==0 or I/O error), _OR_ that ui_update()
				has requested us to stop processing. You will need to deal with
				that via some global variabel or something here in order to
				distinguish those two events for a GUI
			*/
			memset (&info, 0x00, sizeof(info));
			if (process_file ((char*)node->data, &info))
			{
				if (!option_htmllink)
				{
#if defined(__WIN32)
					ui_print ("ed2k://|file|%s|%lld|%s|\n",
						info.basename,
						info.size.QuadPart,
						info.file_hash_str);
#else
					ui_print("ed2k://|file|%s|%u|%s|\n",
								info.basename,
								info.size,
								info.file_hash_str);
#endif
				}
				else {
#if defined(__WIN32)
					ui_print("<a href=\"ed2k://|file|%s|%lld|%s|\">%s</a>\n",
						info.basename,
						info.size.QuadPart,
						info.file_hash_str,
						(option_htmlfull) ? info.filepath : info.basename);
#else
					ui_print("<a href=\"ed2k://|file|%s|%u|%s|\">%s</a>\n",
								info.basename,
								info.size,
								info.file_hash_str,
								(option_htmlfull) ? info.filepath : info.basename);
#endif
				}
			} else ui_printerr ("while processing file '%s'\n", (char*)node->data);
			process_file_free_info_structure_content(&info);
		}
		node = node->next;
	}

	return 1;
}


/* ui_update
 *
 * called regularly during hashing (so GUI clients can update the window and process
 * messages etc.).
 *
 * returns 0 if hashing should stop, otherwise 1 (implement this in ui_run() via a global variable)
 *
 */

int
ui_update (char *filepath, unsigned int size, unsigned int done)
{
	return 1;	/* nothing to do for us here */
}


