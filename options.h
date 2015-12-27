/***************************************************************************
                          options.h  -  description
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


#ifndef _file_hash_options_h_included_
#define _file_hash_options_h_included_

#ifndef HAVE_CONFIG_H		/* e.g. on windows plaform */
# define PACKAGE	"file_hash"
# define VERSION	"1.0.1"
#else
# include <config.h>
#endif /* ifndef HAVE_CONFIG_H */

extern int			option_htmllink;	/* wrap html link around file-link */
extern int			option_htmlfull;	/* print path component as well in html link */
extern int			option_verbose;		/* be verbose */
extern int			option_quiet;		/* only output error messages */
extern int			option_silent;		/* do not even show error messages! */
extern int			option_debug;		/* show debug messages */
extern int			option_recursive;	/* process all subdirectories of directories as well */
extern int			option_excludedirs;	/* exclude directories */
extern int			option_waitwhenfinito; /* wait for keystroke when finished */

/* functions */

int		options_process_cl_argument (char *argv[], int *i);
void	options_print_help_screen_and_exit (void);

#endif /* ifndef _file_hash_options_h_included_ */



