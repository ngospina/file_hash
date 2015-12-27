/***************************************************************************
cl_fi.h  -  description
-------------------
begin                : Thu Nov 28 2013
copyright            : (C) 2013 by Gerardo Ospina
email                : ngospina@gmail.com
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef _file_hash_cl_fi_h_included_
#define _file_hash_cl_fi_h_included_

int	fi_init(void);
int	fi_cleanup(void);
int	fi_print(const char *format, ...);
unsigned int get_file_date(const char *fn, unsigned int *time);

#endif /* ifndef _file_hash_cl_fi_h_included_ */