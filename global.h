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
#endif

#endif /* (!defined(__MAC_OS_X__)) */

/* include extra WIN32 stuff if required
 *
 * NOTE: ALL WIN32 STUFF IS NOT TESTED, I JUST WROTE IT HERE AS I THINK MIGHT BE RIGHT
 *
 */

#if defined(_WIN32)	/* detect MS Visual Studio */

#  define WIN32
#  define __WIN32

#endif /* defined(_WIN32) */

#ifdef __WIN32__	/* detect Borland c++ Definition and add MS Visual c++ one (???) */
#ifndef WIN32
#define WIN32
#endif
#endif

#ifdef WIN32

// #  define STRICT                        /* XXX - Strict typing, please (do we want this?) */
#  include <windows.h>
#  include <direct.h>
#  include <errno.h>
#  include <ctype.h>
#  ifdef _MSC_VER
#    include <io.h>
#  endif /* _MSC_VER */

#ifndef alloca
#include <stdlib.h>
#include <malloc.h>
#define alloca _alloca
#endif

#ifndef strdup
#include <string.h>
#define strdup _strdup
#endif

#ifndef snprintf
#include <stdio.h>
#define snprintf _snprintf
#endif

#endif /* ifdef WIN32 */

#endif


