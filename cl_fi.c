/***************************************************************************
cl_fi.c  -  description
-------------------
begin                : Thu Nov 28 2013
copyright            : (C) 2013 by Gerardo Ospina
modification         : Sun Dec 27 2015
					   Mon Apr 14 2025
                       (C) 2015,2025 by Gerardo Ospina
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

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "global.h"

#if defined(__UNIX)
# include <sys/stat.h>
#endif

#include "cl_ui.h"
#include "cl_fi.h"
#include "options.h"

static FILE *fp;

int	fi_init(void)
{
	fp = fopen("file_hash.csv", "w+");
	if (fp == NULL)
	{
		return 0;
	}
	fprintf(fp, "file,size,hash,date,time\n");
	return 1;	/* nothing else to do here for the command line client version */
}

int	fi_cleanup(void)
{
	fclose(fp);
	return 1;	/* nothing else to do here for the command line client version */
}

/* fi_print
*
* prints a normal message to the file, takes arguments like printf()
*
*/

int	fi_print(const char *format, ...)
{
	int		ret = 0;
	va_list args;

	va_start(args, format);
	ret = vfprintf(fp, format, args);
	va_end(args);
	return ret;
}

/* get_file_date
*
* get file date in DOS Date Format: 
*   bit 0-4 : Day (1-31)
*       5-8 : Month (1=January, 2=February, etc.)
*       9-15: Year offset from 1980 (add 1980 to get the actual year)
*
* *time: get file time in DOS Time format:
*   bit  0-4 : Second divided by 2
*        5-10: Minute (0-59)
        11-15: Hour (0-23 on a 24 hour clock)
*/

unsigned int
get_file_date(const char *fn, unsigned int *time)
{
	if (!fn)
		return (unsigned int)-1;

#if defined(__WIN32)
	WORD fdate, ftime;
	HANDLE hFile;
	FILETIME ftCreation, ftLastAccess, ftLastWrite;

	*time = 0;
	hFile = CreateFile(TEXT(fn), GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		ui_printerr("in CreateFile(%s)\n\n", fn);
		return (unsigned int)-1;
	}

	if (!GetFileTime(hFile, &ftCreation, &ftLastAccess, &ftLastWrite))
	{
		ui_printerr("in GetFileTime(%s)\n\n", fn);
		return (unsigned int)-1;
	}

	if (!FileTimeToDosDateTime(&ftLastWrite, &fdate, &ftime))
	{
		ui_printerr("in FileTimeToDosDateTime(%s)\n\n", fn);
		return (unsigned int)-1;
	}

	CloseHandle(hFile);

	*time = (unsigned int)ftime;
	
	return (unsigned int)fdate;
#elif defined(__UNIX)
  struct stat st;
  struct tm *tm_info;

  if (stat(fn, &st) != 0) {
	ui_printerr("Error getting file stats for %s\n", fn);
	return (unsigned int)-1;
  }

  tm_info = localtime(&st.st_mtime);
  if (!tm_info) {
   	ui_printerr("Error converting time for %s\n", fn);
	return (unsigned int)-1;
  }

  /* Convert to DOS date format */
  unsigned int date = (unsigned int)(((tm_info->tm_year - 80) << 9) |  /* Years since 1980 */
				   ((tm_info->tm_mon + 1) << 5) |                      /* Month (1-12) */
				   tm_info->tm_mday);                                  /* Day (1-31) */

  /* Convert to DOS time format */
  *time = (unsigned int)((tm_info->tm_hour << 11) |   /* Hours (0-23) */
		(tm_info->tm_min << 5) |                      /* Minutes (0-59) */
		(tm_info->tm_sec >> 1));                      /* Seconds/2 (0-29) */
		
  return date;
#else
	*time = 0;
	ui_print ("IMPLEMENT ME FOR THIS OPERATING SYSTEM/PLATFORM: %s\n", __FUNCTION__);

	return 0;
#endif
}
