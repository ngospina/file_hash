/***************************************************************************
                          options.c  -  description
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

#include "global.h"

#include "options.h"
#include "ui.h"

int			option_htmllink = 0;	/* wrap html link around file-link */
int			option_htmlfull = 0;	/* print path component as well in html link */
int			option_verbose = 0;		/* be verbose */
int			option_quiet = 0;		/* only show error messages and the results */
int			option_silent = 0;		/* do not even show error messages! */
int			option_debug = 0;		/* show debug messages */
int			option_recursive = 0;	/* process all subdirectories of directories as well */
int			option_excludedirs = 0;	/* exclude directories */
int			option_waitwhenfinito = 0; /* wait for keystroke when finished */

/* functions */
static int		options_process_cl_option (char option, char *argv[], int *i);


/* options_process_cl_argument
 *
 * processes the command line argument given
 * and increases the argument counter if there are extra args to an option (and only then!)
 *
 * returns 0 on error
 *
 */

int
options_process_cl_argument (char *argv[], int *i)
{
	int		 compound_args = 0;			/* compound arguments? (e.g. '-ds') */
	char	*pos;

	if ((!argv)||(!i))
		return 0;

	if (!argv[*i])					/* this case shouldn't happen */
		return 0;

	if (*(argv[*i])!='-')			/* dito */
		return 0;

	if (strlen(argv[*i])>2)			/* assuming all options are one one letter options! */
		compound_args = 1;

	pos = argv[*i]+1;	/* skip the initial '-' */
	while (*pos)
	{
		if (!options_process_cl_option(*pos, argv, i))
		{
			ui_printerr("unknown command line option '-%c'\n", *pos);
			return 0;
		}
		pos++;
	}

	return *i;
}


/* options_process_cl_option
 *
 * processes one single option
 * increases the argument counter i if - and only if - the
 *  option has extra arguments
 *
 * returns 0 if something went wrong
 *
 */

static int
options_process_cl_option (char option, char *argv[], int *i)
{
/* XXX */
/*	ui_print ("command line option '-%c'\n", option); */

	switch (option)
	{
		case 'a':
			option_htmllink = 1;
		break;
		case 'p':
			option_htmlfull = 1;
		break;
		case 'h':
			options_print_help_screen_and_exit();
		break;
		case 'q':
			option_quiet = 1;		/* only output error messages */
		break;
		case 'v':
			option_verbose = 1;		/* be verbose */
			option_quiet = 0;
			option_silent = 0;
		break;
		case 's':
			option_silent = 1;		/* do not even show error messages! */
			option_quiet = 1;
		break;
		case 'd':
			option_debug = 1;		/* show debug messages */
		break;
		case 'r':
			option_recursive = 1;	/* process all subdirectories of directories as well */
		break;
		case 'x':
			option_excludedirs = 1;	/* exclude directories */
		break;
		case 'w':
			option_waitwhenfinito = 1;	/* wait for keystroke when finished */
		break;
		default:	/* unknown option */
		return 0;
	}

	return 1;
}



/* options_print_help_screen
 *
 * prints help screen and exits
 *
 */

void
options_print_help_screen_and_exit (void)
{
	ui_print ("\n%s v%s (build %s)\n\n", PACKAGE, VERSION, __DATE__);
	ui_print ("Copyright (C) 2001-2002 by Tim-Philipp Müller\n");

#ifndef HAVE_OPENSSL
	ui_print ("Copyright (C) 1990-2, RSA Data Security, Inc. (MD4 hashing functions only)\n");
#endif

	ui_print ("\n");
	ui_print ("This program is free software. See the source for copying conditions.\n");
	ui_print ("This program is distributed in the hope that it will be useful for someone,\n");
	ui_print ("but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
	ui_print ("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\n");
	ui_print ("GNU General Public License for more details.\n\n");

	ui_print ("Usage: file_hash [OPTIONS] <FILES or DIRECTORIES>\n\n");
	ui_print ("Creates the file hash (eDonkey2000 compatible) for the specified files.\n\n");
	ui_print ("Options:\n");

	ui_print ("\t-a\twrap html link around file-link (<a href=\"...\"></a>)\n");
	ui_print ("\t-p\tin html link print basename _with_ path component\n");
	ui_print ("\t-h\tshow this helpscreen\n");
	ui_print ("\t-v\tbe verbose (show extra information)\n");
	ui_print ("\t-r\trecursively descend into directories\n");
	ui_print ("\t-x\tdo not descend into directories\n");

/* these two options don't really make much sense, so why show them? */
/*	ui_print ("\t-q\tbe quiet (only show errors)\n"); */
/*	ui_print ("\t-s\tbe silent, show not even errors (pretty useless really)\n"); */

	ui_print ("\t-w\twait for keystroke when finished\n");
	ui_print ("\n");

	if (!ui_cleanup())
		exit(EXIT_FAILURE);

	exit(EXIT_SUCCESS);
}



