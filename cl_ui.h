/***************************************************************************
                          cl_ui.h  -  description
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

#ifndef _file_hash_cl_ui_h_included_
#define _file_hash_cl_ui_h_included_

#define PRINTERR_PREFIX	"ERROR:"

#include "linkedlist.h"

int			ui_init (void);
int			ui_cleanup (void);
int			ui_setoptions (int argc, char *argv[]);
int			ui_print (const char *format, ...);
int			ui_printerr (const char *format, ...);
int			ui_run (SList *filelist);
int			ui_update (char *filepath, unsigned int size, unsigned int done);

#endif /* ifndef _file_hash_cl_ui_h_included_ */





