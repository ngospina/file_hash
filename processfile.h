/***************************************************************************
                          processfile.h  -  description
                             -------------------
    begin                : Tue Sep 3 2002
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

#ifndef _file_hash_processfile_h_included_
#define _file_hash_processfile_h_included_

#define BLOCKSIZE	(9500*1024)

#ifdef __WIN32
# define SLASH_CHAR '\\'
#else
# define SLASH_CHAR	'/'
#endif /* ifdef __WIN32 */

struct _fileinfo
{
	char			*filepath;
	char			*basename;			/* the filename w/o path component	*/

	unsigned int	 blocks;
	unsigned char	*parthashes;		/* the partial hashes in a row		*/

	unsigned char	*file_hash;			/* the file_hash in binary form		*/
	char			*file_hash_str;		/* the file_hash in ASCII form		*/

	void			*user_data;			/* extra data GUIs might set		*/

	unsigned int	 size;				/* the filesize in bytes			*/
} ;

typedef struct _fileinfo fileinfo;


int		process_file (const char *fn, fileinfo *info);
void	process_file_free_info_structure_content(fileinfo *info);


#endif /* ifndef _file_hash_processfile_h_included_ */



