/***************************************************************************
                          global.h  -  description
                             -------------------
    begin                : Fri Nov 22 2002
    copyright            : (C) 2002 by Tim-Philipp Müller
    email                : t.i.m@orange.net
    modification         : Thu Nov 28 2013
                           Sun Dec 27 2015
	                      (C) 2013,2015 by Gerardo Ospina
	                      ngospina@gmail.com
*************************************************************************** /

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _file_hash_global_h_included_
#define _file_hash_global_h_included_

#if (!defined(__MAC_OS_X__))

#if defined(__APPLE__) && defined(__GNUC__)
#  define __MAC_OS_X__
#elif defined(__MACOSX__)
#  define __MAC_OS_X__
#endif /* defined(__APPLE__) && defined(__GNUC__) */

#endif /* (!defined(__MAC_OS_X__)) */

/* include extra WIN32 stuff if required */

#if defined(_WIN32)	/* detect MS Visual Studio */

#  define WIN32
#  define __WIN32

#endif /* defined(_WIN32) */

#ifdef __WIN32__	/* detect Borland c++ Definition and add MS Visual c++ one (???) */

#ifndef WIN32
#define WIN32
#endif

#endif /* ifdef __WIN32__ */

#ifdef WIN32
// #  define STRICT                        /* XXX - Strict typing, please (do we want this?) */
#  include <windows.h>

#ifndef alloca
#include <stdlib.h>
#include <malloc.h>
#define alloca _alloca
#endif /* ifndef alloca */

#ifndef strdup
#include <string.h>
#define strdup _strdup
#endif /* ifndef strdup */

#ifndef snprintf
#include <stdio.h>
#define snprintf _snprintf
#endif /* ifndef snprintf */

#endif /* ifdef WIN32 */

#endif /* ifndef _file_hash_global_h_included_ */


